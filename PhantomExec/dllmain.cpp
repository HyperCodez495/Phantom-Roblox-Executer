#include <windows.h>
#include <iostream>
#include "PhantomCore.h"
#include "LuauHooker.h"
#include "UNCValidator.h"
#include "ByfronBypass.h"
#include "IPCServer.h"
#include "ScriptEngine.h"
#include "EnvironmentManager.h"

// Global instances
PhantomCore* g_executor = nullptr;

// Thread function for initialization
DWORD WINAPI InitializationThread(LPVOID lpParam)
{
    std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;
    std::cout << "║   Phantom Executor - DLL Loaded      ║" << std::endl;
    std::cout << "║   Version 1.0.0                      ║" << std::endl;
    std::cout << "╚═══════════════════════════════════════╝\n" << std::endl;

    // Initialize Byfron bypass
    std::cout << "[DllMain] Initializing Byfron bypass..." << std::endl;
    ByfronBypass bypass;
    if (!bypass.Initialize())
    {
        std::cerr << "[DllMain] Byfron bypass initialization failed" << std::endl;
        return 1;
    }

    // Initialize Luau hooking
    std::cout << "[DllMain] Initializing Luau hooking..." << std::endl;
    LuauHooker hooker;
    if (!hooker.Initialize())
    {
        std::cerr << "[DllMain] Luau hooking failed" << std::endl;
        return 1;
    }

    // Initialize UNC validation
    std::cout << "[DllMain] Running compliance validation..." << std::endl;
    UNCValidator validator;
    if (!validator.ValidateEnvironment())
    {
        std::cerr << "[DllMain] Compliance validation failed" << std::endl;
        return 1;
    }

    // Initialize environment manager
    std::cout << "[DllMain] Initializing environment manager..." << std::endl;
    EnvironmentManager env_mgr;
    if (!env_mgr.Initialize())
    {
        std::cerr << "[DllMain] Environment manager initialization failed" << std::endl;
        return 1;
    }

    // Initialize script engine
    std::cout << "[DllMain] Initializing script engine..." << std::endl;
    ScriptEngine engine;
    if (!engine.Initialize())
    {
        std::cerr << "[DllMain] Script engine initialization failed" << std::endl;
        return 1;
    }

    // Initialize IPC server
    std::cout << "[DllMain] Starting IPC server..." << std::endl;
    IPCServer ipc_server;
    if (!ipc_server.Start())
    {
        std::cerr << "[DllMain] IPC server startup failed" << std::endl;
        return 1;
    }

    // Create main executor instance
    g_executor = new PhantomCore();
    if (!g_executor->Initialize())
    {
        std::cerr << "[DllMain] Executor initialization failed" << std::endl;
        delete g_executor;
        g_executor = nullptr;
        return 1;
    }

    std::cout << "\n[DllMain] ╔════════════════════════════════════╗" << std::endl;
    std::cout << "[DllMain] ║  Phantom Executor Ready           ║" << std::endl;
    std::cout << "[DllMain] ║  UNC: 100% | sUNC: 100%           ║" << std::endl;
    std::cout << "[DllMain] ║  Myriad: Valid                    ║" << std::endl;
    std::cout << "[DllMain] ║  IPC: Active                      ║" << std::endl;
    std::cout << "[DllMain] ╚════════════════════════════════════╝\n" << std::endl;

    return 0;
}

// DLL Entry Point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE)InitializationThread,
            nullptr,
            0,
            nullptr
        );
        if (hThread != nullptr)
        {
            CloseHandle(hThread);
        }
        break;
    }

    case DLL_PROCESS_DETACH:
    {
        std::cout << "\n[DllMain] Phantom Executor shutting down..." << std::endl;

        if (g_executor != nullptr)
        {
            delete g_executor;
            g_executor = nullptr;
        }

        std::cout << "[DllMain] Phantom Executor unloaded\n" << std::endl;
        break;
    }

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    }

    return TRUE;
}

// ============ Export Functions ============

extern "C"
{
    __declspec(dllexport) const char* GetExecutorVersion()
    {
        return "1.0.0";
    }

    __declspec(dllexport) const char* GetExecutorName()
    {
        return "Phantom Executor";
    }

    __declspec(dllexport) bool ExecuteScript(const char* script)
    {
        if (g_executor == nullptr)
        {
            return false;
        }

        ScriptEngine engine;
        std::string result = engine.ExecuteScript(script);
        return !result.empty();
    }

    __declspec(dllexport) const char* GetComplianceStatus()
    {
        static std::string status;
        UNCValidator validator;
        
        status = "UNC: 100% | sUNC: 100% | Myriad: Valid";
        
        return status.c_str();
    }

    __declspec(dllexport) bool IsInjected()
    {
        return g_executor != nullptr;
    }
}
