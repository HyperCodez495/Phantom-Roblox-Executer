@echo off
REM Phantom Executor - Build Script
REM Compiles C# Windows Forms application to standalone EXE

setlocal enabledelayedexpansion

echo.
echo ╔═══════════════════════════════════════╗
echo ║   Phantom Executor - Build System    ║
echo ║   Version 1.0.0                      ║
echo ╚═══════════════════════════════════════╝
echo.

REM Check if .NET SDK is installed
dotnet --version >nul 2>&1
if errorlevel 1 (
    echo [-] .NET SDK not found
    echo [*] Installing .NET 6.0 SDK...
    powershell -Command "& {iwr https://dot.net/v1/dotnet-install.ps1 -OutFile dotnet-install.ps1; .\dotnet-install.ps1 -Channel 6.0}"
    if errorlevel 1 (
        echo [-] Failed to install .NET SDK
        echo [*] Please install .NET 6.0 SDK manually from https://dotnet.microsoft.com/download
        pause
        exit /b 1
    )
)

echo [+] .NET SDK found
echo [*] Building Phantom Executor...
echo.

REM Build the project
dotnet publish -c Release -r win-x64 --self-contained -p:PublishSingleFile=true -p:PublishTrimmed=false -o ".\Release" PhantomExecutor.csproj

if errorlevel 1 (
    echo.
    echo [-] Build failed
    pause
    exit /b 1
)

echo.
echo [+] Build completed successfully!
echo [+] Output: .\Release\PhantomExecutor.exe
echo.

REM Create shortcut on desktop
echo [*] Creating desktop shortcut...

powershell -Command "^
$DesktopPath = [Environment]::GetFolderPath('Desktop'); ^
$ShortcutPath = Join-Path $DesktopPath 'Phantom Executor.lnk'; ^
$Shell = New-Object -ComObject WScript.Shell; ^
$Shortcut = $Shell.CreateShortcut($ShortcutPath); ^
$Shortcut.TargetPath = (Resolve-Path '.\Release\PhantomExecutor.exe').Path; ^
$Shortcut.WorkingDirectory = (Get-Location).Path; ^
$Shortcut.Save(); ^
Write-Host '[+] Desktop shortcut created'
"

echo.
echo ╔═══════════════════════════════════════╗
echo ║   Build Complete                     ║
echo ║   Ready to run: Release\PhantomExecutor.exe ║
echo ╚═══════════════════════════════════════╝
echo.

pause
