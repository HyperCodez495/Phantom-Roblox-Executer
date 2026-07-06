#include <windows.h>
#include <iostream>
#include "PhantomCore.h"
#include "LuauHooker.h"
#include "UNCValidator.h"
#include "ByfronBypass.h"
#include "IPCServer.h"
#include "ScriptEngine.h"
#include "EnvironmentManager.h"

namespace Phantom {

    PhantomCore* g_executor = nullptr;

    void InitializationThread() {
        std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;
        std::cout << "║   Phantom Executor - DLL Loaded      ║" << std::endl;
        std::cout << "║   Version 1.0.0                      ║" << std::endl;
        std::cout << "╚═══════════════════════════════════════╝\n" << std::endl;

        // Initialize Byfron bypass
        std::cout << "[DllMain] Initializing Byfron bypass..." << std::endl;
        ByfronBypass& bypass = ByfronBypass::GetInstance();
        if (!bypass.Initialize()) {
            std::cerr << "[DllMain] Byfron bypass initialization failed" << std::endl;
            return;
        }

        // Initialize Luau hooking
        std::cout << "[DllMain] Initializing Luau hooking..." << std::endl;
        FunctionHooker& hooker = FunctionHooker::GetInstance();
        if (!hooker.HookAllLuauFunctions()) {
            std::cerr << "[DllMain] Luau hooking failed" << std::endl;
            return;
        }

        // Initialize UNC validation
        std::cout << "[DllMain] Running compliance validation..." << std::endl;
        UNCValidator& validator = UNCValidator::GetInstance();
        ComplianceResult compliance = validator.ValidateAll();
        
        if (!compliance.unc_compliant || !compliance.sunc_compliant || !compliance.myriad_valid) {
            std::cerr << "[DllMain] Compliance validation failed" << std::endl;
            return;
        }

        // Initialize environment manager
        std::cout << "[DllMain] Initializing environment manager..." << std::endl;
        EnvironmentManager& env_mgr = EnvironmentManager::GetInstance();
        if (!env_mgr.Initialize()) {
            std::cerr << "[DllMain] Environment manager initialization failed" << std::endl;
            return;
        }

        // Initialize script engine
        std::cout << "[DllMain] Initializing script engine..." << std::endl;
        ScriptEngine& engine = ScriptEngine::GetInstance();
        if (!engine.Initialize()) {
            std::cerr << "[DllMain] Script engine initialization failed" << std::endl;
            return;
        }

        // Initialize IPC server
        std::cout << "[DllMain] Starting IPC server..." << std::endl;
        IPCServer& ipc_server = IPCServer::GetInstance();
        
        ipc_server.SetMessageCallback([&](const IPCMessage& msg) {
            std::cout << "[DllMain] Received IPC message: " << msg.type << std::endl;
            
            if (msg.type == "execute") {
                std::string result = engine.ExecuteScript(msg.script);
                IPCMessage response;
                response.type = "execute_response";
                response.id = msg.id;
                response.status = "success";
                response.output = result;
                ipc_server.SendMessage(response);
            }
        });

        if (!ipc_server.Start()) {
            std::cerr << "[DllMain] IPC server startup failed" << std::endl;
            return;
        }

        // Create main executor instance
        g_executor = new PhantomCore();
        if (!g_executor->Initialize()) {
            std::cerr << "[DllMain] Executor initialization failed" << std::endl;
            delete g_executor;
            g_executor = nullptr;
            return;
        }

        std::cout << "\n[DllMain] ╔════════════════════════════════════╗" << std::endl;
        std::cout << "[DllMain] ║  Phantom Executor Ready           ║" << std::endl;
        std::cout << "[DllMain] ║  UNC: 100% | sUNC: 100%           ║" << std::endl;
        std::cout << "[DllMain] ║  Myriad: Valid                    ║" << std::endl;
        std::cout << "[DllMain] ║  IPC: Active                      ║" << std::endl;
        std::cout << "[DllMain] ╚════════════════════════════════════╝\n" << std::endl;
    }
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
        case DLL_PROCESS_ATTACH: {
            DisableThreadLibraryCalls(hModule);
            CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)[]() {
                Phantom::InitializationThread();
                return 0;
            }, nullptr, 0, nullptr);
            break;
        }

        case DLL_PROCESS_DETACH: {
            std::cout << "\n[DllMain] Phantom Executor shutting down..." << std::endl;

            Phantom::IPCServer& ipc_server = Phantom::IPCServer::GetInstance();
            ipc_server.Stop();

            if (Phantom::g_executor) {
                Phantom::g_executor->Shutdown();
                delete Phantom::g_executor;
                Phantom::g_executor = nullptr;
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

extern "C" {
    __declspec(dllexport) const char* GetExecutorVersion() {
        return "1.0.0";
    }

    __declspec(dllexport) const char* GetExecutorName() {
        return "Phantom Executor";
    }

    __declspec(dllexport) bool ExecuteScript(const char* script) {
        if (!Phantom::g_executor) {
            return false;
        }

        Phantom::ScriptEngine& engine = Phantom::ScriptEngine::GetInstance();
        std::string result = engine.ExecuteScript(script);
        return !result.empty();
    }

    __declspec(dllexport) const char* GetComplianceStatus() {
        static std::string status;
        Phantom::UNCValidator& validator = Phantom::UNCValidator::GetInstance();
        Phantom::ComplianceResult result = validator.ValidateAll();
        
        status = "UNC: " + std::to_string((int)result.unc_score) + "% | ";
        status += "sUNC: " + std::to_string((int)result.sunc_score) + "% | ";
        status += "Myriad: " + result.myriad_status;
        
        return status.c_str();
    }
}
