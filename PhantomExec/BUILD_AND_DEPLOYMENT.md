# Phantom Executor: Build and Deployment Guide

## Project Structure

```
PhantomExec/
├── PhantomCore.h/cpp              # Core executor logic
├── LuauHooker.h/cpp               # Luau VM hooking and state management
├── UNCValidator.h/cpp             # UNC/sUNC/Myriad compliance validation
├── ByfronBypass.h/cpp             # Byfron/Hyperion bypass mechanisms
├── IPCServer.h/cpp                # Named pipe IPC communication
├── dllmain.cpp                    # DLL entry point
├── Phantom_Executor_Design.md     # Architectural design document
├── DEEP_IMPLEMENTATION_GUIDE.md   # Technical implementation details
├── COMPREHENSIVE_GUIDE.md         # User and developer guide
├── BUILD_AND_DEPLOYMENT.md        # This file
├── CMakeLists.txt                 # Build configuration
└── PhantomExecutor.sln            # Visual Studio solution
```

## Prerequisites

### System Requirements
- **OS:** Windows 10 or later (x64)
- **Compiler:** Visual Studio 2022 with C++20 support
- **SDK:** Windows 10/11 SDK
- **RAM:** 8GB minimum
- **Disk:** 2GB for build artifacts

### Development Tools
- Visual Studio 2022 Community/Professional/Enterprise
- CMake 3.20 or later
- Git for version control
- Optional: Detours library for advanced hooking

### Dependencies
- Luau source code (for headers and linking)
- Windows API headers (included in SDK)
- Optional: nlohmann/json for JSON parsing

## Build Instructions

### Step 1: Clone and Setup

```bash
# Clone the repository
git clone https://github.com/phantom-executor/phantom.git
cd phantom

# Create build directory
mkdir build
cd build
```

### Step 2: Configure with CMake

```bash
# Configure for Release build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_GENERATOR="Visual Studio 17 2022"

# Or configure for Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_GENERATOR="Visual Studio 17 2022"
```

### Step 3: Compile

```bash
# Build the project
cmake --build . --config Release

# Or use Visual Studio directly
msbuild PhantomExecutor.sln /p:Configuration=Release /p:Platform=x64
```

### Step 4: Output

Build artifacts are located in:
- **Release DLL:** `build/Release/Phantom.dll`
- **Debug DLL:** `build/Debug/Phantom.dll`
- **Injector:** `build/Release/PhantomInjector.exe`

## Configuration

### CMakeLists.txt Example

```cmake
cmake_minimum_required(VERSION 3.20)
project(PhantomExecutor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
add_library(Phantom SHARED
    PhantomCore.cpp
    LuauHooker.cpp
    UNCValidator.cpp
    ByfronBypass.cpp
    IPCServer.cpp
    dllmain.cpp
)

# Include directories
target_include_directories(Phantom PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LUAU_INCLUDE_DIR}
)

# Link libraries
target_link_libraries(Phantom PRIVATE
    kernel32
    user32
    ws2_32
    ntdll
)

# Compiler flags
if(MSVC)
    target_compile_options(Phantom PRIVATE
        /W4
        /WX
        /O2
        /Oy-
    )
endif()
```

## Deployment

### Method 1: Manual Injection

```bash
# 1. Start Roblox client
# 2. Run injector
PhantomInjector.exe

# 3. Select Roblox process from list
# 4. Click "Inject"
# 5. Wait for success message
```

### Method 2: Programmatic Injection

```cpp
#include "PhantomCore.h"

int main() {
    // Find Roblox process
    DWORD roblox_pid = FindProcessByName("RobloxPlayerBeta.exe");
    
    // Open process
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, roblox_pid);
    
    // Inject DLL
    InjectDLL(process, "Phantom.dll");
    
    // Wait for initialization
    Sleep(2000);
    
    // Connect to IPC server
    Phantom::IPCClient& client = Phantom::IPCClient::GetInstance();
    client.Connect();
    
    // Send test message
    Phantom::IPCMessage msg;
    msg.type = "status";
    msg.id = 1;
    client.SendMessage(msg);
    
    return 0;
}
```

### Method 3: Manual Mapping

```cpp
#include "ByfronBypass.h"

int main() {
    // Find Roblox process
    DWORD roblox_pid = FindProcessByName("RobloxPlayerBeta.exe");
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, roblox_pid);
    
    // Use manual mapper for stealth injection
    Phantom::ManualMapper& mapper = Phantom::ManualMapper::GetInstance();
    void* base = mapper.MapDLL("Phantom.dll", process);
    
    if (base) {
        std::cout << "DLL mapped at: 0x" << std::hex << (uintptr_t)base << std::endl;
    }
    
    return 0;
}
```

## Testing

### Unit Tests

```bash
# Run unit tests
ctest --output-on-failure

# Or run specific test
ctest -R TestLuauHooking -V
```

### Integration Tests

```cpp
// Test Luau hooking
Phantom::LuauHooker& hooker = Phantom::LuauHooker::GetInstance();
assert(hooker.HookAllLuauFunctions());

// Test UNC validation
Phantom::UNCValidator& validator = Phantom::UNCValidator::GetInstance();
Phantom::ComplianceResult result = validator.ValidateAll();
assert(result.unc_compliant);
assert(result.sunc_compliant);
assert(result.myriad_valid);

// Test IPC communication
Phantom::IPCServer& server = Phantom::IPCServer::GetInstance();
assert(server.Start());
```

## Troubleshooting

### Issue: DLL fails to inject
**Solution:** 
- Ensure Roblox is running before injection
- Check that Phantom.dll is in the correct directory
- Verify Windows Defender/antivirus is not blocking injection

### Issue: Script execution fails
**Solution:**
- Verify Luau VM was successfully hooked
- Check IPC server is running and accessible
- Review execution logs for error messages

### Issue: Compliance validation fails
**Solution:**
- Ensure all required UNC functions are exposed
- Verify sUNC function behavior matches specification
- Check Myriad test results for specific failures

### Issue: Byfron detection
**Solution:**
- Enable Byfron bypass mechanisms
- Use manual mapping instead of standard injection
- Verify anti-cheat evasion is active

## Performance Optimization

### Memory Usage
- Target: <100MB including VM and caches
- Optimize: Reduce hook overhead, minimize state copies

### Execution Speed
- Target: <50ms script execution time
- Optimize: Inline critical functions, reduce IPC latency

### Detection Resistance
- Use manual mapping for injection
- Implement self-healing code
- Randomize execution timing

## Security Hardening

1. **Code Obfuscation:** Apply obfuscation to compiled DLL
2. **Anti-Debugging:** Implement anti-debug checks
3. **Integrity Verification:** Add code signing to DLL
4. **Memory Protection:** Use encrypted strings and data

## Release Checklist

- [ ] All unit tests passing
- [ ] Integration tests successful
- [ ] Code review completed
- [ ] Security audit passed
- [ ] Performance benchmarks met
- [ ] Documentation updated
- [ ] DLL signed and timestamped
- [ ] Release notes prepared

## Deployment Checklist

- [ ] Phantom.dll compiled and tested
- [ ] PhantomInjector.exe ready
- [ ] UI checkpoint created and tested
- [ ] IPC communication verified
- [ ] Compliance validation working
- [ ] Anti-cheat evasion active
- [ ] Performance acceptable
- [ ] User documentation prepared

## Support and Updates

For build issues, feature requests, or bug reports:
- Visit: https://github.com/phantom-executor/phantom/issues
- Email: support@phantom-executor.dev
- Discord: [Community Server Link]

---

**Version:** 1.0.0  
**Last Updated:** July 2026  
**Status:** Production Ready
