# Phantom Executor - Windows Setup Guide

## Quick Start

### Method 1: Automated Installation (Recommended)

1. **Download the installer package** from `/mnt/desktop/PhantomExec/`
2. **Right-click** `Install-PhantomExecutor.ps1`
3. **Select** "Run with PowerShell"
4. **Accept** administrator prompt
5. **Wait** for installation to complete
6. **Phantom Executor** will launch automatically

The installer will:
- Check and install Python if needed
- Install required dependencies
- Create desktop and Start Menu shortcuts
- Launch the application

### Method 2: Manual Setup

1. **Ensure Python 3.9+ is installed**
   ```
   python --version
   ```

2. **Navigate to PhantomExec directory**
   ```
   cd C:\Users\[YourUsername]\Downloads\PhantomExec
   ```

3. **Run the batch launcher**
   ```
   PhantomExecutor.bat
   ```

4. **Or run Python directly**
   ```
   python phantom_launcher.py
   ```

## Application Interface

### Main Window

The Phantom Executor application features a cyberpunk-themed interface with four main tabs:

#### 1. Injection Tab
- **Process Selection**: Auto-detects running Roblox instances
- **DLL Path**: Configure path to Phantom.dll
- **Inject Button**: Injects the DLL into Roblox process
- **Status Log**: Real-time injection progress and status

**Steps:**
1. Click "Refresh" to scan for Roblox processes
2. Select your Roblox process from the dropdown
3. Verify DLL path is correct
4. Click "▶ INJECT DLL"
5. Wait for success confirmation

#### 2. Executor Tab
- **Script Editor**: Write Luau scripts
- **Execute Button**: Run the script
- **Output Panel**: View execution results

**Steps:**
1. Write your Luau script in the editor
2. Click "▶ EXECUTE"
3. View results in the output panel

#### 3. Compliance Tab
- **UNC Status**: Shows UNC compliance percentage
- **sUNC Status**: Shows sUNC compliance percentage
- **Myriad Valid**: Myriad validation status
- **Anti-Cheat**: Anti-cheat evasion status

All metrics should show green when Phantom is properly injected.

#### 4. Logs Tab
- **Real-time Logs**: All system events and messages
- **Clear Button**: Clear log history

## Troubleshooting

### Issue: "No Roblox processes found"

**Solution:**
1. Start Roblox first
2. Wait for the game to fully load
3. Click "Refresh" in the Injection tab
4. If still not found, try selecting "RobloxPlayerBeta.exe (PID: Auto-detect)"

### Issue: "DLL not found"

**Solution:**
1. Verify Phantom.dll exists in the specified path
2. Use "Browse" button to locate the file
3. Ensure you have read permissions on the file

### Issue: "Injection failed"

**Solution:**
1. Run Phantom Executor as Administrator
   - Right-click the application
   - Select "Run as administrator"
2. Ensure Roblox is running
3. Try restarting Roblox and re-injecting
4. Check Windows Defender isn't blocking the DLL

### Issue: "Script execution not working"

**Solution:**
1. Verify DLL was injected successfully (green indicator)
2. Try a simple test script: `print("test")`
3. Check the logs for error messages
4. Ensure you're connected to the game

## File Structure

```
PhantomExec/
├── phantom_launcher.py          # Main application (Python)
├── PhantomExecutor.bat          # Windows batch launcher
├── Install-PhantomExecutor.ps1  # Automated installer
├── Phantom.dll                  # Main executor DLL
├── PhantomInjector.exe          # Standalone injector
├── WINDOWS_SETUP.md             # This file
└── [Other documentation files]
```

## System Requirements

- **OS**: Windows 10/11 (64-bit)
- **Python**: 3.9 or higher
- **RAM**: 2GB minimum
- **Disk Space**: 500MB
- **Administrator Access**: Required for injection
- **Roblox**: Latest version

## Installation Locations

After installation, you can find Phantom Executor at:

- **Desktop**: `Phantom Executor` shortcut
- **Start Menu**: `Phantom Executor` folder
- **Program Files**: `C:\Program Files\Phantom Executor\` (if installed via MSI)
- **Current Directory**: `phantom_launcher.py`

## Advanced Usage

### Command Line Arguments

```bash
# Run with specific DLL path
python phantom_launcher.py --dll "C:\path\to\Phantom.dll"

# Run with auto-injection
python phantom_launcher.py --auto-inject

# Run in headless mode (no GUI)
python phantom_launcher.py --headless
```

### Configuration File

Create `phantom_config.json` in the same directory:

```json
{
  "dll_path": "C:\\path\\to\\Phantom.dll",
  "auto_inject": false,
  "theme": "dark",
  "font_size": 10,
  "window_width": 1200,
  "window_height": 700
}
```

## Security Notes

1. **Administrator Privileges**: Phantom requires admin access for DLL injection
2. **Antivirus**: Some antivirus software may flag the DLL - add to exceptions if needed
3. **Firewall**: No internet connection required - all operations are local
4. **Data Privacy**: No data is sent to external servers

## Uninstallation

### Method 1: Windows Settings
1. Open **Settings** → **Apps** → **Apps & features**
2. Find **Phantom Executor**
3. Click and select **Uninstall**

### Method 2: Manual Removal
1. Delete the installation directory
2. Remove desktop shortcut
3. Remove Start Menu folder

## Support & Troubleshooting

For additional help:
1. Check the **Logs** tab for detailed error messages
2. Review `FINAL_README.md` for technical details
3. Consult `DEEP_IMPLEMENTATION_GUIDE.md` for architecture info

## Version Information

- **Version**: 1.0.0
- **Release Date**: July 2026
- **Status**: Production Ready
- **Platform**: Windows 10/11 (64-bit)

## Legal Disclaimer

Phantom Executor is provided for educational and research purposes only. Users are responsible for ensuring their use complies with all applicable laws and terms of service. Unauthorized use to exploit Roblox or violate its Terms of Service is prohibited.

---

**Last Updated**: July 2026  
**Maintained By**: Phantom Development Team
