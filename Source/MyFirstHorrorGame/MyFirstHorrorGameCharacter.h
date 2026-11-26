// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "MyFirstHorrorGameCharacter.generated.h"

class UHorrorHealthComponent;
class UHorrorGameOverWidget;
class UAudioComponent;
class UAnimMontage;
class USpringArmComponent;
class UCameraComponent;
class USpotLightComponent;
class USoundBase;
class UInputAction;
class UInteractionHUD;
class UHorrorInventoryMenu;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A simple player-controllable third person character
 *  Implements a controllable orbiting camera
 */
UCLASS(abstract)
class AMyFirstHorrorGameCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** A cone of light pointing along the player's view. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USpotLightComponent> Flashlight;

	UPROPERTY(Transient)
	TObjectPtr<class UPointLightComponent> FlashlightFillLight;
	
private:
	void StartSprint();
	void ReleaseSprint();
	void UpdateWalkTransition();
	void ResetWalkSpeed();
	FTimerHandle WalkTransitionTimer;
	double SprintReleaseTime = 0.0;
	float ReleaseStartSpeed = 0.0f;

	void ToggleInventory();
	void CloseInventory();

	UPROPERTY(Transient)
	TObjectPtr<UHorrorInventoryMenu> InventoryMenu;

	TWeakObjectPtr<APlayerController> InventoryMenuController;
	bool bPreviousCursorVisible = false;

	AActor* FindInteractionTarget() const;
	void RefreshInteractionHUD();
	void StopInteractionHUD();
	FTimerHandle InteractionTimer;

	UPROPERTY(Transient)
	TObjectPtr<UInteractionHUD> InteractionHUD;


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health")
	TObjectPtr<UHorrorHealthComponent> Health;

	UPROPERTY(EditDefaultsOnly, Category="Health|Feedback", meta=(ClampMin="0", ClampMax="2"))
	float InjurySwayDegrees = 1.8f;

	UPROPERTY(EditDefaultsOnly, Category="Health|Feedback", meta=(ClampMin="0", ClampMax="1"))
	float CriticalVignette = 0.95f;

	UPROPERTY(EditDefaultsOnly, Category="Health|Feedback", meta=(ClampMin="0", ClampMax="3"))
	float DesaturationStrength = 1.35f;

	UPROPERTY(EditDefaultsOnly, Category="Health|Feedback")
	FLinearColor CriticalTint = FLinearColor(0.88f, 1.12f, 1.04f);

	UPROPERTY(EditDefaultsOnly, Category="Health|Feedback", meta=(ClampMin="0", ClampMax="1"))
	float InjuryMotionBlur = 0.35f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Health|Audio")
	TObjectPtr<UAudioComponent> LowHealthBreathingAudio;

	UPROPERTY(EditDefaultsOnly, Category="Health|Audio", meta=(ClampMin="0.01", ClampMax="1"))
	float BreathingThreshold = 0.45f;

	UPROPERTY(EditDefaultsOnly, Category="Health|Death")
	TObjectPtr<USoundBase> DeathSound;

	UPROPERTY(EditDefaultsOnly, Category="Health|Audio")
	TObjectPtr<USoundBase> HurtSound;

	UPROPERTY(EditDefaultsOnly, Category="Health|Death")
	TObjectPtr<UAnimMontage> DeathMontage;

	UPROPERTY(EditDefaultsOnly, Category="Health|Death")
	TSubclassOf<UHorrorGameOverWidget> GameOverClass;

	UPROPERTY(EditDefaultsOnly, Category="Health|Death", meta=(ClampMin="0"))
	float GameOverDelay = 2.5f;

	UPROPERTY(Transient)
	TObjectPtr<UHorrorGameOverWidget> GameOverWidget;

	FTimerHandle GameOverTimer;
	void ShowGameOver();

	float InjuryStrength = 0.0f;

	UFUNCTION()
	void UpdateInjuryFeedback(float Current, float Maximum, float Ratio);

	UFUNCTION()
	void HandlePlayerDeath();

	virtual void CalcCamera(float DeltaTime, FMinimalViewInfo& OutResult) override;
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Sprint", meta=(ClampMin="0", Units="cm/s"))
	float WalkSpeed = 220.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Sprint", meta=(ClampMin="0", Units="cm/s"))
	float SprintSpeed = 500.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Movement|Sprint", meta=(ClampMin="0", Units="s"))
	float SprintReleaseDuration = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> SprintAction;

	virtual void PawnClientRestart() override;
	virtual void UnPossessed() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	TSubclassOf<UInteractionHUD> InteractionHUDClass;

	UPROPERTY(EditDefaultsOnly, Category="Inventory")
	TSubclassOf<UHorrorInventoryMenu> InventoryMenuClass;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> ToggleInventoryAction;

	/** Assign a digital action in the playable character Blueprint. */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> FlashlightAction;

	/** Digital action used to interact once per press. */
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> InteractAction;

	/** Maximum distance from the character to the point hit by the trace, in centimeters. */
	UPROPERTY(EditDefaultsOnly, Category="Interaction", meta=(ClampMin="0.0"))
	float InteractionReach = 200.0f;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MouseLookAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Flashlight|Audio")
	TObjectPtr<USoundBase> FlashlightToggleSound;

public:

	/** Constructor */
	AMyFirstHorrorGameCharacter();	

protected:

	/** Initialize input action bindings */
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Find and interact with the first reachable object in the center of the view. */
	void TryInteract();

public:

	/** Switch the light once per press, including presses from a UI button. */
	UFUNCTION(BlueprintCallable, Category="Flashlight")
	void ToggleFlashlight();

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles look inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoLook(float Yaw, float Pitch);

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump pressed inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

