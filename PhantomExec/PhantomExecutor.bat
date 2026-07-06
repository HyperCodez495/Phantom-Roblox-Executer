@echo off
REM Phantom Executor - Windows Launcher
REM This batch script launches the Phantom Executor application

setlocal enabledelayedexpansion

echo.
echo ╔═══════════════════════════════════════╗
echo ║   Phantom Executor v1.0.0             ║
echo ║   Windows Application Launcher        ║
echo ╚═══════════════════════════════════════╝
echo.

REM Check if Python is installed
python --version >nul 2>&1
if errorlevel 1 (
    echo [-] Python not found. Installing Python...
    powershell -Command "& {iwr https://www.python.org/ftp/python/3.11.0/python-3.11.0-amd64.exe -OutFile python_installer.exe; .\python_installer.exe /quiet InstallAllUsers=1 PrependPath=1}"
    if errorlevel 1 (
        echo [-] Failed to install Python. Please install Python 3.9+ manually.
        pause
        exit /b 1
    )
)

echo [+] Python found
echo [+] Launching Phantom Executor...

REM Get the directory where this script is located
set SCRIPT_DIR=%~dp0

REM Launch the Python launcher
python "%SCRIPT_DIR%phantom_launcher.py"

if errorlevel 1 (
    echo [-] Failed to launch Phantom Executor
    pause
    exit /b 1
)

exit /b 0
