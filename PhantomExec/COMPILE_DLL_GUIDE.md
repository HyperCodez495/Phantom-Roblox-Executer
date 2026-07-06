# Phantom Executor - DLL Compilation Guide

## Overview

This guide walks you through compiling `Phantom.dll` from source code using Visual Studio 2022 on Windows.

## Prerequisites

### Required Software

1. **Visual Studio 2022** (Community Edition is free)
   - Download: https://visualstudio.microsoft.com/vs/
   - Must include: C++ Desktop Development workload

2. **Windows 10/11 SDK**
   - Included with Visual Studio
   - Or download separately: https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/

3. **CMake** (optional, for advanced builds)
   - Download: https://cmake.org/download/

### System Requirements

- Windows 10/11 (64-bit)
- 8GB RAM minimum
- 10GB free disk space
- Administrator access

## Step 1: Install Visual Studio 2022

### 1.1 Download Visual Studio

1. Go to: https://visualstudio.microsoft.com/vs/
2. Click **Download Visual Studio 2022 Community** (free)
3. Run the installer

### 1.2 Configure Installation

During installation, select these workloads:

- ✓ **Desktop development with C++**
- ✓ **Windows 10/11 SDK**
- ✓ **CMake tools for Windows**

Optional but recommended:
- ✓ **Git for Windows**
- ✓ **GitHub Desktop**

### 1.3 Complete Installation

1. Click **Install**
2. Wait for installation to complete (30-60 minutes)
3. Launch Visual Studio when finished

## Step 2: Prepare Source Files

### 2.1 Copy Project Files

1. Copy entire `/mnt/desktop/PhantomExec/` folder to Windows
2. Extract to: `C:\PhantomExec\` (or any location)

### 2.2 Verify File Structure

```
C:\PhantomExec\
├── PhantomCore.h/cpp
├── LuauHooker.h/cpp
├── UNCValidator.h/cpp
├── ByfronBypass.h/cpp
├── IPCServer.h/cpp
├── ScriptEngine.h/cpp
├── EnvironmentManager.h/cpp
├── dllmain.cpp
├── CMakeLists.txt
└── [Other files]
```

## Step 3: Create Visual Studio Project

### Option A: Using CMake (Recommended)

#### 3A.1 Open CMake GUI

1. Launch **CMake GUI** (installed with Visual Studio)
2. Set **Source code** path: `C:\PhantomExec`
3. Set **Build binaries** path: `C:\PhantomExec\build`
4. Click **Configure**

#### 3A.2 Configure Generator

1. Select generator: **Visual Studio 17 2022**
2. Platform: **x64**
3. Click **Finish**

#### 3A.3 Generate Project

1. Click **Generate**
2. Wait for completion
3. Click **Open Project** (opens in Visual Studio)

### Option B: Manual Project Creation

#### 3B.1 Create New Project

1. Open Visual Studio 2022
2. **File** → **New** → **Project**
3. Search for: **Dynamic-Link Library (DLL)**
4. Select **Dynamic-Link Library (DLL) (C++)**
5. Click **Next**

#### 3B.2 Configure Project

1. **Project name**: `Phantom`
2. **Location**: `C:\PhantomExec`
3. Click **Create**

#### 3B.3 Add Source Files

1. **Solution Explorer** (right panel)
2. Right-click **Source Files** → **Add** → **Existing Item**
3. Select all `.cpp` files:
   - PhantomCore.cpp
   - LuauHooker.cpp
   - UNCValidator.cpp
   - ByfronBypass.cpp
   - IPCServer.cpp
   - ScriptEngine.cpp
   - EnvironmentManager.cpp
   - dllmain.cpp

#### 3B.4 Add Header Files

1. Right-click **Header Files** → **Add** → **Existing Item**
2. Select all `.h` files:
   - PhantomCore.h
   - LuauHooker.h
   - UNCValidator.h
   - ByfronBypass.h
   - IPCServer.h
   - ScriptEngine.h
   - EnvironmentManager.h

## Step 4: Configure Project Settings

### 4.1 Set Build Configuration

1. **Build** → **Configuration Manager**
2. Set **Active solution configuration** to: **Release**
3. Set **Active solution platform** to: **x64**

### 4.2 Project Properties

1. Right-click project → **Properties**

#### 4.2.1 General Settings

- **Configuration**: Release
- **Platform**: x64
- **Output Directory**: `$(SolutionDir)Release\`
- **Intermediate Directory**: `$(SolutionDir)build\`

#### 4.2.2 C++ Settings

**C/C++** → **General**:
- **Additional Include Directories**: `$(ProjectDir)`

**C/C++** → **Code Generation**:
- **Runtime Library**: Multi-threaded DLL (`/MD`)
- **Enable Function-Level Linking**: Yes

**C/C++** → **Optimization**:
- **Optimization**: Maximize Speed (`/O2`)
- **Inline Function Expansion**: Any Suitable (`/Ob2`)

#### 4.2.3 Linker Settings

**Linker** → **General**:
- **Output File**: `$(OutDir)Phantom.dll`

**Linker** → **Input**:
- **Additional Dependencies**:
  ```
  kernel32.lib
  user32.lib
  ws2_32.lib
  ntdll.lib
  advapi32.lib
  ```

**Linker** → **System**:
- **Subsystem**: Windows (`/SUBSYSTEM:WINDOWS`)

## Step 5: Build the DLL

### 5.1 Build Solution

1. **Build** → **Build Solution** (Ctrl+Shift+B)
2. Or right-click project → **Build**

### 5.2 Monitor Build Progress

- **Output** window shows compilation progress
- Watch for errors/warnings
- Build should complete in 1-2 minutes

### 5.3 Verify Build Success

```
========== Build: 1 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========
```

If successful, `Phantom.dll` created at:
```
C:\PhantomExec\Release\Phantom.dll
```

## Step 6: Verify DLL

### 6.1 Check File Properties

1. Navigate to: `C:\PhantomExec\Release\`
2. Right-click `Phantom.dll` → **Properties**
3. Verify:
   - **File size**: 2-3 MB
   - **File type**: Application extension
   - **Created date**: Today's date

### 6.2 Verify DLL Exports

Using **Dependency Walker** (optional):

1. Download: http://www.dependencywalker.com/
2. Open `Phantom.dll`
3. Verify exports:
   - `GetExecutorVersion`
   - `GetExecutorName`
   - `ExecuteScript`
   - `GetComplianceStatus`

## Step 7: Deploy DLL

### 7.1 Copy to PhantomExec Folder

1. Copy `Phantom.dll` from `Release\` folder
2. Paste into: `C:\PhantomExec\` (same folder as batch script)

### 7.2 Verify Deployment

```
C:\PhantomExec\
├── PhantomExecutor-Simple.bat
├── Phantom.dll  ← Should be here
├── PhantomInjector.exe
└── [Other files]
```

## Step 8: Test Execution

### 8.1 Run Phantom Executor

1. Double-click `PhantomExecutor-Simple.bat`
2. Should NOT show "DLL not found" error
3. Menu appears successfully

### 8.2 Test Injection

1. Start Roblox
2. Run Phantom Executor
3. Select **Option 1** - Scan for Roblox
4. Select **Option 2** - Inject DLL
5. Should complete without errors

## Troubleshooting

### Build Error: "Missing header files"

**Solution:**
1. Ensure all `.h` files are in project
2. Check **Additional Include Directories** includes project folder
3. Rebuild solution

### Build Error: "Unresolved external symbol"

**Solution:**
1. Verify all `.cpp` files are included
2. Check linker dependencies are correct
3. Ensure no duplicate definitions

### Build Error: "LNK1104: cannot open file"

**Solution:**
1. Close all running instances of Phantom
2. Delete `build\` folder
3. Rebuild solution

### DLL not loading

**Solution:**
1. Verify DLL is in correct folder
2. Check Windows Defender isn't blocking
3. Run as Administrator
4. Ensure 64-bit DLL (not 32-bit)

## Advanced: Command Line Build

If you prefer command line compilation:

```batch
REM Navigate to project folder
cd C:\PhantomExec

REM Create build directory
mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 17 2022" -A x64

REM Build Release configuration
cmake --build . --config Release

REM DLL will be at: Release\Phantom.dll
```

## Build Output

After successful compilation:

```
Release\
├── Phantom.dll           ← Main executor DLL
├── Phantom.lib           ← Import library
├── Phantom.exp           ← Export file
└── Phantom.pdb           ← Debug symbols (optional)
```

## Performance Optimization

For faster builds:

1. **Build** → **Configuration Manager**
2. Set **Active solution configuration** to: **Release**
3. Disable **Edit and Continue** (Debug → Options)
4. Use **Incremental Linking** (Linker → General)

## Next Steps

After successful compilation:

1. Copy `Phantom.dll` to `C:\PhantomExec\`
2. Run `PhantomExecutor-Simple.bat`
3. Test injection with Roblox
4. Execute scripts through UI

## Support

If compilation fails:

1. Check Visual Studio is fully installed
2. Verify C++ workload is installed
3. Ensure Windows SDK is present
4. Try cleaning and rebuilding
5. Check build output for specific errors

## Version Information

- **Visual Studio**: 2022 (17.0+)
- **C++ Standard**: C++20
- **Platform**: x64 (64-bit)
- **Configuration**: Release
- **Output**: Phantom.dll

---

**Next:** After compilation, proceed to `QUICKSTART.md` for usage instructions.
