# Phantom Executor: Deep Implementation Guide

## Phase 1: Luau VM Hooking Architecture

### 1.1 VM State Acquisition

The Luau VM state is the gateway to script execution. There are three primary methods to acquire a valid state:

**Method 1: Script Context Extraction (Roblox-Specific)**
The Roblox client maintains a global script context object that holds references to all active Luau states. By pattern-scanning for this object and extracting the state pointer, we can gain direct access without triggering hooks.

**Method 2: VM Creation Interception**
Hook `lua_newstate` to capture newly created states. This method is reliable but requires active monitoring of state creation events.

**Method 3: Luau C API Hooking**
Hook standard Luau functions like `lua_gettop`, `lua_pcall`, or `lua_load` to steal the state from function arguments. This is the most universal approach and works across different Roblox versions.

### 1.2 Critical Luau C API Functions to Hook

| Function | Purpose | Hook Priority |
|----------|---------|----------------|
| `lua_pcall` | Protected call - executes Lua code | CRITICAL |
| `lua_load` | Loads Lua bytecode/source | CRITICAL |
| `lua_newstate` | Creates new VM instance | HIGH |
| `lua_close` | Destroys VM instance | HIGH |
| `lua_getglobal` | Retrieves global variable | HIGH |
| `lua_setglobal` | Sets global variable | HIGH |
| `lua_pushstring` | Pushes string onto stack | MEDIUM |
| `lua_tolstring` | Converts stack value to string | MEDIUM |
| `lua_call` | Calls Lua function | MEDIUM |
| `lua_gettop` | Gets stack top | LOW |

### 1.3 Trampoline Hook Implementation

A trampoline hook works by:
1. Backing up the first N bytes of the target function
2. Writing a JMP instruction to redirect to our hook function
3. In our hook, calling the original function via the backed-up bytes
4. Returning to the caller with modified behavior

**x86-64 Assembly Pattern:**
```asm
; Original function start (backed up)
mov rax, [rsp + 8]
mov rcx, [rax + 0x20]
...

; Replaced with JMP to our hook
jmp [rip + rel_offset_to_hook]

; Our hook function:
; 1. Save registers
; 2. Call original via trampoline
; 3. Modify behavior
; 4. Restore registers
; 5. Return
```

### 1.4 Function Resolution Strategy

Roblox updates frequently, changing function addresses. Dynamic resolution is essential:

**Pattern Scanning Approach:**
- Scan for unique byte sequences (signatures) within the Roblox executable
- Signatures are more stable across versions than absolute addresses
- Example: `lua_pcall` often has a distinctive prologue pattern

**Export Table Parsing:**
- Some Luau functions may be exported from DLLs
- Parse PE headers to extract export tables
- Less reliable but faster when available

**Relative Offset Calculation:**
- Once a base function is found, calculate offsets to related functions
- Luau functions often cluster together in memory
- Offset-based discovery is more version-resistant

## Phase 2: Byfron/Hyperion Bypass Strategy

### 2.1 Hyperion Detection Mechanisms

Hyperion (Byfron's anti-tamper) employs multiple detection layers:

**Integrity Checks:**
- Monitors critical code sections for modifications
- Detects inline hooks and JMP instructions
- Validates function prologues against known signatures

**Control Flow Guard (CFG):**
- Validates indirect calls and jumps
- Maintains a bitmap of valid jump targets
- Detects unauthorized control flow transfers

**Instrumentation Detection:**
- Monitors for debuggers and instrumentation tools
- Detects breakpoints and single-stepping
- Identifies memory access patterns indicative of hooking

**Behavioral Analysis:**
- Monitors for suspicious API call patterns
- Detects unusual memory allocation sequences
- Analyzes timing anomalies

### 2.2 Bypass Techniques

**CFG Bypass via Indirect Call Manipulation:**
```cpp
// Instead of direct hook:
// jmp [rip + offset_to_hook]  // Detected by CFG

// Use indirect call through valid target:
mov rax, [rip + valid_function_pointer]
call rax  // CFG sees this as valid call
```

**Integrity Check Evasion:**
- Perform hooks during initialization before integrity checks activate
- Unhook before Hyperion scans
- Use self-modifying code that repairs itself after execution

**Instrumentation Detection Evasion:**
- Avoid common debugging APIs (IsDebuggerPresent, CheckRemoteDebuggerPresent)
- Use alternative detection methods
- Randomize timing to avoid pattern recognition

### 2.3 Stealth Injection Techniques

**Manual Mapping:**
- Load DLL into memory manually without using LoadLibrary
- Avoids DLL entry in PEB (Process Environment Block)
- Bypasses standard DLL detection mechanisms

**Reflective Injection:**
- DLL contains its own loader code
- Loads itself into target process
- Appears as code allocation rather than DLL load

**Process Hollowing:**
- Create suspended process
- Replace image with payload
- Resume execution from payload entry point

## Phase 3: Script Execution Pipeline

### 3.1 Environment Setup

Before executing user scripts, establish a sandboxed environment:

```cpp
// 1. Create isolated Luau state
lua_State* script_state = lua_newstate(malloc_wrapper, nullptr);

// 2. Load standard libraries (with restrictions)
luaL_openlibs(script_state);  // or selective library loading

// 3. Expose Phantom API
expose_phantom_api(script_state);

// 4. Set environment isolation
set_environment_restrictions(script_state);
```

### 3.2 Phantom API Exposure

Expose a comprehensive API for script interaction:

```lua
-- Available to user scripts
phantom = {
  -- Execution control
  execute = function(code) end,
  
  -- Environment queries
  getenv = function() end,
  getrenv = function() end,
  
  -- Object manipulation
  cloneref = function(obj) end,
  compareinstances = function(a, b) end,
  
  -- Function manipulation
  hookfunction = function(func, hook) end,
  clonefunction = function(func) end,
  
  -- Cache management
  cache = {
    invalidate = function(obj) end,
    iscached = function(obj) end,
    replace = function(obj, new) end,
  },
  
  -- Logging
  log = function(msg) end,
  warn = function(msg) end,
  error = function(msg) end,
}
```

### 3.3 Error Handling and Isolation

Ensure script errors don't crash the executor:

```cpp
// Wrap script execution in protected call
int result = lua_pcall(script_state, 0, LUA_MULTRET, 0);
if (result != LUA_OK) {
  const char* error_msg = lua_tostring(script_state, -1);
  log_error("[Script Error] %s", error_msg);
  lua_pop(script_state, 1);  // Clear error from stack
}
```

## Phase 4: IPC Communication Layer

### 4.1 Named Pipes for UI-Backend Communication

Use Windows Named Pipes for inter-process communication between the web UI and the injected DLL:

```cpp
// Server (DLL) side
HANDLE pipe = CreateNamedPipeA(
  "\\\\.\\pipe\\PhantomExecutor",
  PIPE_ACCESS_DUPLEX,
  PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE,
  1, 4096, 4096, 0, nullptr
);

// Client (UI) side
HANDLE pipe = CreateFileA(
  "\\\\.\\pipe\\PhantomExecutor",
  GENERIC_READ | GENERIC_WRITE,
  0, nullptr, OPEN_EXISTING, 0, nullptr
);
```

### 4.2 Message Protocol

Define a simple JSON-based protocol:

```json
// Execute script request
{
  "type": "execute",
  "script": "print('Hello from Phantom')",
  "id": 1
}

// Response
{
  "type": "execute_response",
  "id": 1,
  "status": "success",
  "output": "Hello from Phantom"
}

// Status query
{
  "type": "status",
  "id": 2
}

// Status response
{
  "type": "status_response",
  "id": 2,
  "unc_compliance": 100,
  "sunc_compliance": 100,
  "myriad_valid": true,
  "memory_usage": 52428800
}
```

## Phase 5: Build and Deployment

### 5.1 Compilation Requirements

- Visual Studio 2022 with C++20 support
- Windows 10/11 SDK
- Luau source code for headers
- Optional: Detours library for advanced hooking

### 5.2 Linking Strategy

```cmake
# Link against system libraries
target_link_libraries(PhantomExecutor
  PRIVATE
  kernel32
  user32
  ws2_32
  ntdll
)

# Optional: Link Detours for robust hooking
target_link_libraries(PhantomExecutor
  PRIVATE
  detours
)
```

### 5.3 Deployment Flow

1. Compile Phantom.dll (Release build with optimizations)
2. Sign DLL to avoid Windows SmartScreen warnings
3. Package with injector executable
4. User runs injector → selects Roblox process → injects DLL
5. DLL initializes and creates Named Pipe
6. Web UI connects to pipe and begins communication
7. User can now execute scripts through the UI

## Performance Considerations

- **Hook Overhead:** Minimize by hooking only essential functions
- **Memory Usage:** Target <100MB including VM and caches
- **Execution Latency:** Aim for <50ms script execution time
- **Detection Risk:** Balance performance with stealth

## Security & Legal Disclaimer

This guide is for educational and research purposes only. Unauthorized use of Phantom to exploit Roblox or violate Roblox's Terms of Service is prohibited. Users assume full responsibility for their actions and any consequences.

---

**Version:** 1.0.0  
**Last Updated:** July 2026  
**Status:** Technical Reference
