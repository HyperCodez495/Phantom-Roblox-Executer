# Phantom Roblox Executor: Architectural Design Document

## 1. Introduction

This document outlines the architectural design for the "Phantom" Roblox executor, a framework engineered for robust script injection and execution within the Roblox environment. The design prioritizes stealth, stability, and adaptability against evolving anti-cheat mechanisms, particularly Byfron/Hyperion. The core philosophy centers on deep interaction with the Luau Virtual Machine (VM) and strategic circumvention of integrity checks.

## 2. Core Components

The Phantom executor will comprise several interconnected modules, each responsible for a distinct aspect of its operation:

### 2.1. Injection Module (PhantomInjector)

*   **Purpose:** To establish a foothold within the Roblox client process.
*   **Mechanism:** Primarily DLL injection. This involves loading a custom dynamic-link library (DLL) into the Roblox process space. Historical data indicates DLL injection as a prevalent and effective method for introducing external code [5, 10].
*   **Anti-Detection Strategy:**
    *   **Stealthy Loading:** Employing techniques to load the DLL without triggering common anti-cheat heuristics (e.g., manual mapping, reflective DLL injection).
    *   **Obfuscation:** Obfuscating the injector's code and the injected DLL to hinder static and dynamic analysis by anti-cheat systems.
    *   **Process Hollowing/RunPE:** Investigating advanced process manipulation techniques to execute the payload from a legitimate process context, further reducing detection surface.

### 2.2. Luau VM Interaction Module (PhantomVMHook)

*   **Purpose:** To gain control over the embedded Luau VM within the Roblox client, enabling arbitrary script execution.
*   **Mechanism:** Hooking critical Luau C API functions. The `lua_gettop` function, as demonstrated in academic contexts, serves as a foundational example for state acquisition [12]. The module will identify and hook functions responsible for script loading, execution, and environment manipulation.
*   **Key Functions to Target:**
    *   `lua_newstate` / `lua_close`: For managing VM instances.
    *   `lua_load` / `lua_pcall`: For loading and executing bytecode/scripts.
    *   `lua_getglobal` / `lua_setglobal`: For manipulating the global environment.
    *   `lua_pushstring` / `lua_tolstring`: For string manipulation and data transfer.
*   **Anti-Detection Strategy:**
    *   **Trampoline Hooks:** Implementing robust detour/trampoline hooking to redirect function calls while preserving original functionality and minimizing detectable modifications [12].
    *   **Dynamic Hooking:** Resolving function addresses at runtime to counter Address Space Layout Randomization (ASLR) and frequent updates.
    *   **Context Preservation:** Ensuring that the hooked functions maintain the integrity of the Luau VM state to avoid crashes or anti-cheat triggers.

### 2.3. Script Execution Module (PhantomScriptEngine)

*   **Purpose:** To provide a secure and flexible environment for executing user-defined Luau scripts.
*   **Mechanism:** Leveraging the hooked Luau VM functions to inject and run scripts. This module will manage script contexts, handle sandboxing (if necessary for user-provided scripts), and facilitate communication between the injected scripts and the Roblox client.
*   **Features:**
    *   **Script Loading:** Support for loading scripts from local files or remote sources.
    *   **Error Handling:** Robust error reporting and isolation to prevent script failures from crashing the client.
    *   **API Exposure:** Exposing a custom API to injected scripts, allowing them to interact with Roblox game elements and client functionalities.

### 2.4. Anti-Anti-Cheat (AAC) Module (PhantomShield)

*   **Purpose:** To detect and neutralize anti-cheat mechanisms, specifically Byfron/Hyperion.
*   **Mechanism:** This module will employ a multi-layered approach:
    *   **Integrity Check Bypass:** Identifying and patching or redirecting integrity checks on critical code sections and data structures.
    *   **Control Flow Guard (CFG) Bypass:** Analyzing and circumventing CFG implementations to allow for code injection and modification [6].
    *   **Instrumentation Detection Evasion:** Detecting and evading anti-cheat attempts to instrument or monitor the executor's activities.
    *   **Virtualization/Obfuscation:** Exploring techniques like code virtualization or advanced obfuscation to make the executor's footprint harder to analyze.
*   **Research Focus:** Continuous monitoring of Byfron/Hyperion updates and community findings (e.g., UnknownCheats forum) to adapt bypass strategies [1].

## 3. Architectural Flow

1.  **Initialization:** The PhantomInjector loads the Phantom DLL into the Roblox client process.
2.  **VM Acquisition:** The PhantomVMHook module identifies and hooks essential Luau C API functions, acquiring a valid Luau VM state.
3.  **Script Environment Setup:** The PhantomScriptEngine initializes a secure environment for script execution, potentially exposing a custom API.
4.  **Anti-Cheat Engagement:** The PhantomShield module actively monitors and neutralizes anti-cheat detections.
5.  **Script Execution:** User-provided Luau scripts are loaded and executed via the PhantomScriptEngine, interacting with the Roblox environment through the custom API.

## 4. Development Considerations

*   **Language:** C++ for core modules (Injector, VMHook, AAC) due to its low-level control and performance. Luau for user-facing scripting.
*   **Dynamic Adaptation:** The architecture must be designed for rapid adaptation to Roblox client updates and anti-cheat revisions.
*   **Resource Management:** Efficient memory and CPU usage to avoid performance degradation or detection.

## 5. References

[1] UnknownCheats.me. "Hyperion analysis, the roblox anti-tamper." [https://www.unknowncheats.me/forum/anti-cheat-bypass/669481-hyperion-analysis-roblox-anti-tamper.html](https://www.unknowncheats.me/forum/anti-cheat-bypass/669481-hyperion-analysis-roblox-anti-tamper.html)
[5] Roblox Wiki. "Exploit." [https://roblox.fandom.com/wiki/Exploit](https://roblox.fandom.com/wiki/Exploit)
[6] atrexus. "A collection of bypasses for Hyperion's CFG implementation." [https://gist.github.com/atrexus/11e71d6b245e99a545c05021f72bc992](https://gist.github.com/atrexus/11e71d6b245e99a545c05021f72bc992)
[10] Windows81/Roblox-Script-Executor-CLI. "Roblox Script Executor (rsexec) is a command-line interface that primarily uses the WeAreDevs API to run scripts on the Rōblox client." [https://github.com/Windows81/Roblox-Script-Executor-CLI](https://github.com/Windows81/Roblox-Script-Executor-CLI)
[12] OpenPunk. "Manipulating Embedded Lua VMs: Hooking lua_gettop." [https://openpunk.com/pages/manipulating-lua-vms-2/](https://openpunk.com/pages/manipulating-lua-vms-2/)
