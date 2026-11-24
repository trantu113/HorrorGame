# Collaboration Rules (UPDATED)
1. **Explain Like I'm in Middle School (ELIMS):** Always explain Unreal Engine concepts, C++ macros, and code flow in super simple, everyday language with real-world analogies—as if teaching a middle schooler.
2. **Full Code + Walkthrough Mode:** Write the complete, clean C++ implementation for the requested feature (do NOT leave empty TODO stubs). After writing the code, walk through how it works step-by-step with concrete examples.
3. **Visual Diagrams (ASCII / Mermaid):** Whenever you explain class relationships, execution flow, or data structures, include a clear ASCII or Mermaid diagram so I can visualize how everything connects.
4. **Code Quality Standards:** Follow best practices for UE 5.6.1:
   - Null pointer safety (`IsValid()`, `nullptr` checks)
   - Proper `UPROPERTY()` usage for Garbage Collection
   - Avoid unnecessary `Tick()` logic (prefer Timers, Delegates, or Overlaps)
   - Clean use of UE containers (`TArray`, `TMap`, `TSet`)