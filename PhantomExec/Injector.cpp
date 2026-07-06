#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

namespace fs = std::filesystem;

/**
 * Phantom DLL Injector
 * Handles process enumeration, DLL injection, and verification
 */

struct ProcessInfo {
    DWORD pid;
    std::string name;
};

std::vector<ProcessInfo> EnumerateProcesses() {
    std::vector<ProcessInfo> processes;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    
    if (snapshot == INVALID_HANDLE_VALUE) {
        std::cerr << "[Injector] Failed to create process snapshot" << std::endl;
        return processes;
    }

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(snapshot, &entry)) {
        do {
            ProcessInfo info;
            info.pid = entry.th32ProcessID;
            info.name = entry.szExeFile;
            processes.push_back(info);
        } while (Process32Next(snapshot, &entry));
    }

    CloseHandle(snapshot);
    return processes;
}

DWORD FindRobloxProcess() {
    std::cout << "[Injector] Searching for Roblox process..." << std::endl;
    
    auto processes = EnumerateProcesses();
    for (const auto& proc : processes) {
        if (proc.name == "RobloxPlayerBeta.exe" || proc.name == "Roblox.exe") {
            std::cout << "[Injector] Found Roblox: PID " << proc.pid << std::endl;
            return proc.pid;
        }
    }

    std::cerr << "[Injector] Roblox process not found" << std::endl;
    return 0;
}

bool InjectDLL(DWORD pid, const std::string& dll_path) {
    std::cout << "[Injector] Injecting DLL into process " << pid << std::endl;

    // Verify DLL exists
    if (!fs::exists(dll_path)) {
        std::cerr << "[Injector] DLL file not found: " << dll_path << std::endl;
        return false;
    }

    // Open target process
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!process) {
        std::cerr << "[Injector] Failed to open process: " << GetLastError() << std::endl;
        return false;
    }

    // Allocate memory for DLL path
    size_t dll_path_len = dll_path.length() + 1;
    void* remote_path = VirtualAllocEx(process, nullptr, dll_path_len, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!remote_path) {
        std::cerr << "[Injector] Failed to allocate memory in target process: " << GetLastError() << std::endl;
        CloseHandle(process);
        return false;
    }

    // Write DLL path to remote process
    if (!WriteProcessMemory(process, remote_path, dll_path.c_str(), dll_path_len, nullptr)) {
        std::cerr << "[Injector] Failed to write DLL path to target process: " << GetLastError() << std::endl;
        VirtualFreeEx(process, remote_path, 0, MEM_RELEASE);
        CloseHandle(process);
        return false;
    }

    // Get LoadLibraryA address
    HMODULE kernel32 = GetModuleHandleA("kernel32.dll");
    FARPROC load_library = GetProcAddress(kernel32, "LoadLibraryA");
    if (!load_library) {
        std::cerr << "[Injector] Failed to get LoadLibraryA address" << std::endl;
        VirtualFreeEx(process, remote_path, 0, MEM_RELEASE);
        CloseHandle(process);
        return false;
    }

    // Create remote thread to call LoadLibraryA
    HANDLE thread = CreateRemoteThread(process, nullptr, 0, (LPTHREAD_START_ROUTINE)load_library, remote_path, 0, nullptr);
    if (!thread) {
        std::cerr << "[Injector] Failed to create remote thread: " << GetLastError() << std::endl;
        VirtualFreeEx(process, remote_path, 0, MEM_RELEASE);
        CloseHandle(process);
        return false;
    }

    // Wait for thread to complete
    WaitForSingleObject(thread, INFINITE);

    // Get thread exit code
    DWORD exit_code = 0;
    GetExitCodeThread(thread, &exit_code);

    // Cleanup
    CloseHandle(thread);
    VirtualFreeEx(process, remote_path, 0, MEM_RELEASE);
    CloseHandle(process);

    if (exit_code) {
        std::cout << "[Injector] DLL injected successfully (handle: 0x" << std::hex << exit_code << std::dec << ")" << std::endl;
        return true;
    } else {
        std::cerr << "[Injector] DLL injection failed (LoadLibraryA returned NULL)" << std::endl;
        return false;
    }
}

void ListProcesses() {
    std::cout << "\n[Injector] Available Processes:" << std::endl;
    std::cout << "================================" << std::endl;
    
    auto processes = EnumerateProcesses();
    int idx = 1;
    
    for (const auto& proc : processes) {
        if (proc.name.find("Roblox") != std::string::npos) {
            std::cout << "[" << idx << "] " << proc.name << " (PID: " << proc.pid << ") [ROBLOX]" << std::endl;
            idx++;
        }
    }

    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    std::cout << "╔═══════════════════════════════════════╗" << std::endl;
    std::cout << "║   Phantom Executor - DLL Injector    ║" << std::endl;
    std::cout << "║   Version 1.0.0                      ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════╝" << std::endl << std::endl;

    // Get current directory for DLL path
    char current_dir[MAX_PATH];
    GetCurrentDirectoryA(MAX_PATH, current_dir);
    std::string dll_path = std::string(current_dir) + "\\Phantom.dll";

    // Find Roblox process
    DWORD roblox_pid = FindRobloxProcess();
    if (!roblox_pid) {
        std::cerr << "[Injector] Could not find Roblox process. Please start Roblox first." << std::endl;
        ListProcesses();
        std::cout << "Press any key to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    // Inject DLL
    if (!InjectDLL(roblox_pid, dll_path)) {
        std::cerr << "[Injector] Injection failed!" << std::endl;
        std::cout << "Press any key to exit..." << std::endl;
        std::cin.get();
        return 1;
    }

    std::cout << "\n[Injector] Waiting for Phantom initialization..." << std::endl;
    Sleep(3000);

    std::cout << "[Injector] Phantom Executor is now active!" << std::endl;
    std::cout << "[Injector] Open your browser and navigate to the Phantom UI." << std::endl;
    std::cout << "\nPress any key to exit..." << std::endl;
    std::cin.get();

    return 0;
}
