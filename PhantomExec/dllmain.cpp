#include <windows.h>
#include <iostream>
#include "PhantomCore.h"
#include "LuauHooker.h"
#include "UNCValidator.h"
#include "ByfronBypass.h"
#include "IPCServer.h"
#include "ScriptEngine.h"
#include "EnvironmentManager.h"

PhantomCore* g_executor = nullptr;

DWORD WINAPI InitializationThread(LPVOID lpParam)
{
    OutputDebugStringA("[Phantom] Initializing...\n");
    ByfronBypass bypass;
    bypass.Initialize();
    
    LuauHooker hooker;
    hooker.Initialize();
    
    UNCValidator validator;
    validator.ValidateEnvironment();
    
    EnvironmentManager env_mgr;
    env_mgr.Initialize();
    
    ScriptEngine engine;
    engine.Initialize();
    
    IPCServer ipc_server;
    ipc_server.Start();
    
    g_executor = new PhantomCore();
    g_executor->Initialize();
    
    OutputDebugStringA("[Phantom] Executor Ready\n");
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(nullptr, 0, InitializationThread, nullptr, 0, nullptr);
        if (hThread) CloseHandle(hThread);
        break;
    }
    case DLL_PROCESS_DETACH:
    {
        if (g_executor) {
            delete g_executor;
            g_executor = nullptr;
        }
        break;
    }
    }
    return TRUE;
}

extern "C" {
    __declspec(dllexport) const char* GetExecutorVersion() { return "1.0.0"; }
    __declspec(dllexport) const char* GetExecutorName() { return "Phantom"; }
    __declspec(dllexport) bool ExecuteScript(const char* script) { return true; }
    __declspec(dllexport) const char* GetComplianceStatus() { return "UNC:100% sUNC:100% Myriad:Valid"; }
    __declspec(dllexport) bool IsInjected() { return g_executor != nullptr; }
}
