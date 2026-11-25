// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyFirstHorrorGameCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "HorrorInteractable.h"
#include "InteractionHUD.h"
#include "Inventory/HorrorInventoryComponent.h"
#include "Inventory/HorrorInventoryMenu.h"
#include "TimerManager.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SpotLightComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "MyFirstHorrorGame.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/Soundbase.h"


AMyFirstHorrorGameCharacter::AMyFirstHorrorGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 260.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 55.0f, 55.0f);
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetupAttachment(FollowCamera);
	Flashlight->SetIntensityUnits(ELightUnits::Candelas);
	Flashlight->SetIntensity(900.0f);
	Flashlight->SetAttenuationRadius(800.0f);
	Flashlight->SetInnerConeAngle(9.0f);
	Flashlight->SetOuterConeAngle(18.0f);
	Flashlight->SetVisibility(false);

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AMyFirstHorrorGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	ResetWalkSpeed();
}

void AMyFirstHorrorGameCharacter::StartSprint()
{
	if (!InventoryMenu)
	{
		GetWorldTimerManager().ClearTimer(WalkTransitionTimer);
		GetCharacterMovement()->MaxWalkSpeed = FMath::Max(WalkSpeed, SprintSpeed);
	}
}

void AMyFirstHorrorGameCharacter::ReleaseSprint()
{
	ReleaseStartSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (InventoryMenu || SprintReleaseDuration <= 0.0f || ReleaseStartSpeed <= WalkSpeed)
	{
		ResetWalkSpeed();
		return;
	}

	SprintReleaseTime = GetWorld()->GetTimeSeconds();
	GetWorldTimerManager().SetTimer(WalkTransitionTimer, this,
		&AMyFirstHorrorGameCharacter::UpdateWalkTransition, 1.0f / 60.0f, true);
}

void AMyFirstHorrorGameCharacter::UpdateWalkTransition()
{
	const double Elapsed = GetWorld()->GetTimeSeconds() - SprintReleaseTime;
	if (Elapsed >= SprintReleaseDuration)
	{
		ResetWalkSpeed();
		return;
	}

	const float Alpha = static_cast<float>(Elapsed / SprintReleaseDuration);
	GetCharacterMovement()->MaxWalkSpeed = FMath::Lerp(ReleaseStartSpeed, WalkSpeed, Alpha);
}

void AMyFirstHorrorGameCharacter::ResetWalkSpeed()
{
	GetWorldTimerManager().ClearTimer(WalkTransitionTimer);
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void AMyFirstHorrorGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		if (IsValid(SprintAction))
		{
			EnhancedInputComponent->BindAction(SprintAction.Get(), ETriggerEvent::Started, this, &AMyFirstHorrorGameCharacter::StartSprint);
			EnhancedInputComponent->BindAction(SprintAction.Get(), ETriggerEvent::Completed, this, &AMyFirstHorrorGameCharacter::ReleaseSprint);
			EnhancedInputComponent->BindAction(SprintAction.Get(), ETriggerEvent::Canceled, this, &AMyFirstHorrorGameCharacter::ReleaseSprint);
		}

		if (IsValid(FlashlightAction))
		{
			EnhancedInputComponent->BindAction(FlashlightAction.Get(), ETriggerEvent::Started, this, &AMyFirstHorrorGameCharacter::ToggleFlashlight);
		}

		if (IsValid(InteractAction))
		{
			EnhancedInputComponent->BindAction(InteractAction.Get(), ETriggerEvent::Started, this, &AMyFirstHorrorGameCharacter::TryInteract);
		}

		if (IsValid(ToggleInventoryAction))
		{
			EnhancedInputComponent->BindAction(ToggleInventoryAction.Get(), ETriggerEvent::Started, this,
				&AMyFirstHorrorGameCharacter::ToggleInventory);
		}

		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyFirstHorrorGameCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyFirstHorrorGameCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyFirstHorrorGameCharacter::Move);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AMyFirstHorrorGameCharacter::Look);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyFirstHorrorGameCharacter::Look);
	}
	else
	{
		UE_LOG(LogMyFirstHorrorGame, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AMyFirstHorrorGameCharacter::ToggleFlashlight()
{
	if (!InventoryMenu && IsValid(Flashlight))
	{
		Flashlight->SetVisibility(!Flashlight->IsVisible());
		if (IsValid(FlashlightToggleSound))
		{
			UGameplayStatics::PlaySound2D(this, FlashlightToggleSound.Get());
		}
	}
}

AActor* AMyFirstHorrorGameCharacter::FindInteractionTarget() const
{
	UWorld* World = GetWorld();
	if (!IsValid(World) || !IsValid(FollowCamera))
	{
		return nullptr;
	}

	const FVector Origin = GetActorLocation();
	const FVector Start = FollowCamera->GetComponentLocation();
	const double RayLength = FVector::Distance(Origin, Start) + InteractionReach;
	const FVector End = Start + FollowCamera->GetForwardVector() * RayLength;
	FCollisionQueryParams Query(SCENE_QUERY_STAT(HorrorInteraction), false, this);
	FHitResult Hit;
	if (!World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Query)
		|| FVector::Distance(Origin, Hit.ImpactPoint) > InteractionReach)
	{
		return nullptr;
	}

	AActor* Target = Hit.GetActor();
	return IsValid(Target) && Target->Implements<UHorrorInteractable>() ? Target : nullptr;
}

void AMyFirstHorrorGameCharacter::TryInteract()
{
	if (InventoryMenu)
	{
		return;
	}

	if (AActor* Target = FindInteractionTarget())
	{
		IHorrorInteractable::Execute_Interact(Target, this);
	}
	RefreshInteractionHUD();
}

void AMyFirstHorrorGameCharacter::ToggleInventory()
{
	if (InventoryMenu)
	{
		CloseInventory();
		return;
	}

	APlayerController* Player = Cast<APlayerController>(GetController());
	UHorrorInventoryComponent* Inventory = FindComponentByClass<UHorrorInventoryComponent>();
	if (!IsValid(Player) || !Player->IsLocalPlayerController() || !InventoryMenuClass || !IsValid(Inventory))
	{
		return;
	}

	UHorrorInventoryMenu* Menu = CreateWidget<UHorrorInventoryMenu>(Player, InventoryMenuClass);
	if (!IsValid(Menu) || !Menu->AddToPlayerScreen(10))
	{
		return;
	}
	if (!Menu->SetInventory(Inventory))
	{
		Menu->RemoveFromParent();
		return;
	}

	InventoryMenu = Menu;
	ResetWalkSpeed();
	InventoryMenuController = Player;
	bPreviousCursorVisible = Player->bShowMouseCursor;
	Player->SetIgnoreMoveInput(true);
	Player->SetIgnoreLookInput(true);
	GetCharacterMovement()->StopMovementImmediately();
	StopJumping();

	FInputModeGameAndUI Mode;
	Mode.SetHideCursorDuringCapture(false);
	Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	Player->SetInputMode(Mode);
	Player->bShowMouseCursor = true;
}

void AMyFirstHorrorGameCharacter::CloseInventory()
{
	if (IsValid(InventoryMenu))
	{
		InventoryMenu->RemoveFromParent();
	}
	InventoryMenu = nullptr;

	if (APlayerController* Player = InventoryMenuController.Get())
	{
		Player->SetIgnoreMoveInput(false);
		Player->SetIgnoreLookInput(false);
		Player->SetInputMode(FInputModeGameOnly());
		Player->bShowMouseCursor = bPreviousCursorVisible;
	}
	InventoryMenuController.Reset();
}

void AMyFirstHorrorGameCharacter::PawnClientRestart()
{
	CloseInventory();
	Super::PawnClientRestart();
	ResetWalkSpeed();
	StopInteractionHUD();
	APlayerController* Player = Cast<APlayerController>(GetController());
	if (!IsValid(Player) || !Player->IsLocalPlayerController() || !InteractionHUDClass)
	{
		return;
	}

	InteractionHUD = CreateWidget<UInteractionHUD>(Player, InteractionHUDClass);
	if (IsValid(InteractionHUD))
	{
		InteractionHUD->AddToPlayerScreen();
		RefreshInteractionHUD();
		GetWorldTimerManager().SetTimer(InteractionTimer, this,
			&AMyFirstHorrorGameCharacter::RefreshInteractionHUD, 0.1f, true);
	}
}

void AMyFirstHorrorGameCharacter::RefreshInteractionHUD()
{
	if (IsValid(InteractionHUD))
	{
		InteractionHUD->SetInteractionAvailable(FindInteractionTarget() != nullptr);
	}
}

void AMyFirstHorrorGameCharacter::StopInteractionHUD()
{
	GetWorldTimerManager().ClearTimer(InteractionTimer);
	if (IsValid(InteractionHUD))
	{
		InteractionHUD->RemoveFromParent();
	}
	InteractionHUD = nullptr;
}

void AMyFirstHorrorGameCharacter::UnPossessed()
{
	CloseInventory();
	StopInteractionHUD();
	ResetWalkSpeed();
	Super::UnPossessed();
}

void AMyFirstHorrorGameCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CloseInventory();
	StopInteractionHUD();
	GetWorldTimerManager().ClearTimer(WalkTransitionTimer);
	Super::EndPlay(EndPlayReason);
}

void AMyFirstHorrorGameCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	// route the input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AMyFirstHorrorGameCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// route the input
	DoLook(LookAxisVector.X, LookAxisVector.Y);
}

void AMyFirstHorrorGameCharacter::DoMove(float Right, float Forward)
{
	if (GetController() != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = GetController()->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, Forward);
		AddMovementInput(RightDirection, Right);
	}
}

void AMyFirstHorrorGameCharacter::DoLook(float Yaw, float Pitch)
{
	if (GetController() != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AMyFirstHorrorGameCharacter::DoJumpStart()
{
	if (!InventoryMenu)
	{
		Jump();
	}
}

void AMyFirstHorrorGameCharacter::DoJumpEnd()
{
	// signal the character to stop jumping
	StopJumping();
}
