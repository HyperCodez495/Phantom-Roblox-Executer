# Phantom Executor - Quick Start Guide

## Installation (30 seconds)

1. **Download** entire `/mnt/desktop/PhantomExec/` folder to Windows
2. **Extract** to a location (e.g., `C:\PhantomExec`)
3. **Done!** No installation required

## Running Phantom Executor

### Option 1: Simple Batch Interface (Recommended - No Dependencies)

1. **Double-click** `PhantomExecutor-Simple.bat`
2. **Menu appears** with options
3. **Select option 1** to scan for Roblox
4. **Select option 2** to inject DLL
5. **Select option 3** to execute scripts

This is a pure batch script - works on any Windows machine with no dependencies.

### Option 2: VBScript GUI (Windows Forms)

1. **Double-click** `PhantomExecutor.vbs`
2. **GUI window opens** with cyberpunk theme
3. **Use tabs** to navigate features
4. **Click buttons** to perform actions

### Option 3: Full Build from Source (Advanced)

If you have .NET 6.0 SDK installed:

1. **Open Command Prompt** in folder
2. **Run:** `BUILD.bat`
3. **Wait** for compilation
4. **Run:** `Release\PhantomExecutor.exe`

## Basic Workflow

### Step 1: Prepare Roblox
```
1. Start Roblox client
2. Join any game
3. Wait for full load
```

### Step 2: Launch Phantom
```
1. Double-click PhantomExecutor-Simple.bat
2. Menu appears
```

### Step 3: Scan for Roblox
```
1. Press [1] - Scan for Roblox Processes
2. Roblox process will be detected
3. PID displayed
```

### Step 4: Inject DLL
```
1. Press [2] - Inject DLL
2. Injection sequence runs
3. Success message appears
```

### Step 5: Execute Scripts
```
1. Press [3] - Execute Script
2. Enter Luau code
3. Type END to execute
4. Results displayed
```

## File Structure

```
PhantomExec/
├── PhantomExecutor-Simple.bat    ← START HERE (No dependencies)
├── PhantomExecutor.vbs           ← Alternative (GUI)
├── BUILD.bat                     ← Advanced (requires .NET SDK)
├── Phantom.dll                   ← Main executor
├── PhantomInjector.exe           ← Standalone injector
└── [Documentation files]
```

## Troubleshooting

### "Phantom.dll not found"
- Ensure `Phantom.dll` is in same folder as batch file
- Check file permissions

### "No Roblox processes found"
- Start Roblox first
- Join a game
- Wait for full load
- Try scanning again

### "Injection failed"
- Run as Administrator (right-click → Run as administrator)
- Ensure Roblox is running
- Check Windows Defender isn't blocking

### "Script won't execute"
- Verify DLL was injected (status should show Connected)
- Try a simple test: `print("test")`
- Check logs for errors

## System Requirements

- Windows 10/11 (64-bit)
- Roblox client installed
- Administrator access (for injection)
- No additional software required

## Features

✓ **Automatic Process Detection** - Finds Roblox automatically  
✓ **DLL Injection** - Injects Phantom.dll into Roblox  
✓ **Script Execution** - Run Luau scripts in-game  
✓ **Compliance Monitoring** - Real-time UNC/sUNC/Myriad status  
✓ **Execution Logging** - Track all operations  
✓ **Zero Dependencies** - Works on any Windows machine  

## Quick Commands

| Command | Action |
|---------|--------|
| `PhantomExecutor-Simple.bat` | Launch batch interface |
| `PhantomExecutor.vbs` | Launch VBScript GUI |
| `BUILD.bat` | Build from source (requires .NET SDK) |
| `PhantomInjector.exe` | Standalone injector tool |

## Example Scripts

### Hello World
```lua
print("Hello from Phantom!")
```

### Get Player Info
```lua
local players = game:GetService("Players")
local player = players.LocalPlayer
print("Player: " .. player.Name)
```

### Teleport Player
```lua
local player = game.Players.LocalPlayer
local character = player.Character
if character then
    character:MoveTo(Vector3.new(0, 50, 0))
end
```

## Support

For help:
1. Check **Logs** section in application
2. Review error messages
3. Consult documentation files
4. Try restarting Roblox and re-injecting

## Version Info

- **Version**: 1.0.0
- **Release**: July 2026
- **Status**: Production Ready
- **Platform**: Windows 10/11 (64-bit)

## Legal Notice

Phantom Executor is for educational purposes only. Unauthorized use to exploit Roblox or violate its Terms of Service is prohibited. Users assume full responsibility for their actions.

---

**Ready to start?** Double-click `PhantomExecutor-Simple.bat` now!
