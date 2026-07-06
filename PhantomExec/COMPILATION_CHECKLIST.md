# Phantom Executor - Compilation Checklist

## Pre-Compilation

- [ ] Visual Studio 2022 installed
- [ ] C++ Desktop Development workload installed
- [ ] Windows 10/11 SDK installed
- [ ] All source files copied to Windows
- [ ] Project folder at: `C:\PhantomExec\`

## Step 1: Open Project

- [ ] Launch Visual Studio 2022
- [ ] **File** → **Open** → **Project/Solution**
- [ ] Navigate to: `C:\PhantomExec\`
- [ ] Select: `Phantom.sln`
- [ ] Click **Open**

## Step 2: Verify Project Structure

In **Solution Explorer** (right panel), verify you see:

**Header Files:**
- [ ] PhantomCore.h
- [ ] LuauHooker.h
- [ ] UNCValidator.h
- [ ] ByfronBypass.h
- [ ] IPCServer.h
- [ ] ScriptEngine.h
- [ ] EnvironmentManager.h

**Source Files:**
- [ ] PhantomCore.cpp
- [ ] LuauHooker.cpp
- [ ] UNCValidator.cpp
- [ ] ByfronBypass.cpp
- [ ] IPCServer.cpp
- [ ] ScriptEngine.cpp
- [ ] EnvironmentManager.cpp
- [ ] dllmain.cpp

## Step 3: Configure Build Settings

- [ ] **Build** → **Configuration Manager**
- [ ] Set **Active solution configuration** to: **Release**
- [ ] Set **Active solution platform** to: **x64**
- [ ] Click **Close**

## Step 4: Build Project

- [ ] **Build** → **Build Solution** (Ctrl+Shift+B)
- [ ] Wait for compilation to complete
- [ ] Check **Output** window for errors

## Step 5: Verify Build Success

In **Output** window, you should see:
```
========== Build: 1 succeeded, 0 failed ==========
```

- [ ] Build succeeded message appears
- [ ] No errors reported
- [ ] No critical warnings

## Step 6: Locate DLL

- [ ] Navigate to: `C:\PhantomExec\Release\`
- [ ] Verify `Phantom.dll` exists
- [ ] Check file size: 2-3 MB
- [ ] Check modified date: Today

## Step 7: Deploy DLL

- [ ] Copy `Phantom.dll` from `Release\` folder
- [ ] Paste into: `C:\PhantomExec\` (root folder)
- [ ] Verify file is in same folder as `PhantomExecutor-Simple.bat`

## Step 8: Test Execution

- [ ] Start Roblox client
- [ ] Join a game
- [ ] Double-click `PhantomExecutor-Simple.bat`
- [ ] Menu appears (no "DLL not found" error)
- [ ] Select **Option 1** - Scan for Roblox
- [ ] Roblox process detected
- [ ] Select **Option 2** - Inject DLL
- [ ] Injection completes successfully

## Troubleshooting Checklist

### If Build Fails:

- [ ] Check **Output** window for specific error
- [ ] Verify all `.cpp` and `.h` files are in project
- [ ] Check **Additional Include Directories** includes project folder
- [ ] Right-click project → **Rebuild**
- [ ] If still failing, try **Clean Solution** first

### If DLL Not Found:

- [ ] Verify `Phantom.dll` is in `C:\PhantomExec\` (not in Release subfolder)
- [ ] Check file permissions (should be readable)
- [ ] Verify file size is 2-3 MB (not corrupted)
- [ ] Try copying again from Release folder

### If Injection Fails:

- [ ] Run `PhantomExecutor-Simple.bat` as Administrator
- [ ] Ensure Roblox is running and game is loaded
- [ ] Check Windows Defender isn't blocking DLL
- [ ] Try restarting Roblox and re-injecting

## Build Configuration Details

| Setting | Value |
|---------|-------|
| Configuration | Release |
| Platform | x64 |
| Output Type | Dynamic Library (DLL) |
| Runtime Library | Multi-threaded DLL |
| Optimization | Maximize Speed (/O2) |
| C++ Standard | C++20 |

## Expected Output Files

After successful build:

```
C:\PhantomExec\Release\
├── Phantom.dll           ← Main DLL (copy to root)
├── Phantom.lib           ← Import library
├── Phantom.exp           ← Export file
└── Phantom.pdb           ← Debug symbols
```

## Quick Build Command

If you prefer command line:

```batch
REM Open Command Prompt in C:\PhantomExec\
cd C:\PhantomExec

REM Build Release configuration
msbuild Phantom.sln /p:Configuration=Release /p:Platform=x64

REM DLL will be at: Release\Phantom.dll
```

## Common Issues & Solutions

| Issue | Solution |
|-------|----------|
| "Missing header files" | Ensure all .h files are in project |
| "Unresolved external symbol" | Check all .cpp files are included |
| "Cannot open file" | Close Phantom, delete build folder, rebuild |
| "DLL not found" | Copy from Release\ to root folder |
| "Access denied" | Run as Administrator |

## Final Verification

After deployment:

- [ ] `C:\PhantomExec\Phantom.dll` exists
- [ ] File size: 2-3 MB
- [ ] Modified date: Today
- [ ] `PhantomExecutor-Simple.bat` runs without errors
- [ ] Roblox process detected
- [ ] DLL injection completes successfully

## Success Indicators

✓ Build completed with 0 errors  
✓ `Phantom.dll` created in Release folder  
✓ DLL copied to `C:\PhantomExec\`  
✓ Batch script runs without "DLL not found"  
✓ Roblox process detected  
✓ Injection completes successfully  

---

**Next Step:** Once all items are checked, proceed to `QUICKSTART.md` for usage instructions.
