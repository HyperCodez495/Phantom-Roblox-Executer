# Phantom Executor - Windows Installation Script
# Run this script with: powershell -ExecutionPolicy Bypass -File Install-PhantomExecutor.ps1

Write-Host "`n╔═══════════════════════════════════════╗" -ForegroundColor Cyan
Write-Host "║   Phantom Executor v1.0.0             ║" -ForegroundColor Cyan
Write-Host "║   Windows Installation Script         ║" -ForegroundColor Cyan
Write-Host "╚═══════════════════════════════════════╝`n" -ForegroundColor Cyan

# Check if running as administrator
$isAdmin = ([Security.Principal.WindowsPrincipal] [Security.Principal.WindowsIdentity]::GetCurrent()).IsInRole([Security.Principal.WindowsBuiltInRole] "Administrator")
if (-not $isAdmin) {
    Write-Host "[-] This script requires administrator privileges" -ForegroundColor Red
    Write-Host "[*] Restarting as administrator..." -ForegroundColor Yellow
    Start-Process powershell -ArgumentList "-ExecutionPolicy Bypass -File `"$PSCommandPath`"" -Verb RunAs
    exit
}

Write-Host "[+] Running with administrator privileges" -ForegroundColor Green

# Check Python installation
Write-Host "[*] Checking Python installation..." -ForegroundColor Yellow
$pythonPath = (Get-Command python -ErrorAction SilentlyContinue).Source

if (-not $pythonPath) {
    Write-Host "[-] Python not found. Installing Python 3.11..." -ForegroundColor Red
    
    $pythonInstaller = "$env:TEMP\python-3.11.0-amd64.exe"
    Write-Host "[*] Downloading Python installer..." -ForegroundColor Yellow
    
    try {
        Invoke-WebRequest -Uri "https://www.python.org/ftp/python/3.11.0/python-3.11.0-amd64.exe" -OutFile $pythonInstaller
        Write-Host "[+] Python installer downloaded" -ForegroundColor Green
        
        Write-Host "[*] Installing Python..." -ForegroundColor Yellow
        & $pythonInstaller /quiet InstallAllUsers=1 PrependPath=1
        
        if ($LASTEXITCODE -eq 0) {
            Write-Host "[+] Python installed successfully" -ForegroundColor Green
        } else {
            Write-Host "[-] Python installation failed" -ForegroundColor Red
            exit 1
        }
    } catch {
        Write-Host "[-] Failed to download Python: $_" -ForegroundColor Red
        exit 1
    }
} else {
    Write-Host "[+] Python found at: $pythonPath" -ForegroundColor Green
}

# Install required packages
Write-Host "[*] Installing required Python packages..." -ForegroundColor Yellow
python -m pip install --upgrade pip -q
python -m pip install tkinter -q 2>$null

Write-Host "[+] Python environment ready" -ForegroundColor Green

# Create desktop shortcut
Write-Host "[*] Creating desktop shortcut..." -ForegroundColor Yellow

$desktopPath = [Environment]::GetFolderPath("Desktop")
$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Definition
$launcherPath = Join-Path $scriptPath "phantom_launcher.py"
$shortcutPath = Join-Path $desktopPath "Phantom Executor.lnk"

if (Test-Path $launcherPath) {
    $shell = New-Object -ComObject WScript.Shell
    $shortcut = $shell.CreateShortcut($shortcutPath)
    $shortcut.TargetPath = "python"
    $shortcut.Arguments = "`"$launcherPath`""
    $shortcut.WorkingDirectory = $scriptPath
    $shortcut.IconLocation = "C:\Windows\System32\cmd.exe,0"
    $shortcut.Save()
    
    Write-Host "[+] Desktop shortcut created" -ForegroundColor Green
} else {
    Write-Host "[-] Launcher script not found at: $launcherPath" -ForegroundColor Red
}

# Create Start Menu shortcut
Write-Host "[*] Creating Start Menu shortcut..." -ForegroundColor Yellow

$startMenuPath = [Environment]::GetFolderPath("StartMenu")
$appFolder = Join-Path $startMenuPath "Programs\Phantom Executor"

if (-not (Test-Path $appFolder)) {
    New-Item -ItemType Directory -Path $appFolder -Force | Out-Null
}

$startMenuShortcut = Join-Path $appFolder "Phantom Executor.lnk"
$shell = New-Object -ComObject WScript.Shell
$shortcut = $shell.CreateShortcut($startMenuShortcut)
$shortcut.TargetPath = "python"
$shortcut.Arguments = "`"$launcherPath`""
$shortcut.WorkingDirectory = $scriptPath
$shortcut.Save()

Write-Host "[+] Start Menu shortcut created" -ForegroundColor Green

# Launch application
Write-Host "`n[+] Installation complete!" -ForegroundColor Green
Write-Host "[*] Launching Phantom Executor..." -ForegroundColor Yellow

Start-Process python -ArgumentList "`"$launcherPath`"" -WorkingDirectory $scriptPath

Write-Host "[+] Phantom Executor started" -ForegroundColor Green
Write-Host "`n[*] You can now access Phantom Executor from:" -ForegroundColor Cyan
Write-Host "    - Desktop shortcut: Phantom Executor" -ForegroundColor Cyan
Write-Host "    - Start Menu: Phantom Executor" -ForegroundColor Cyan
Write-Host "`n"
