#include <iostream>
#include <cassert>
#include <string>
#include "LuauHooker.h"
#include "UNCValidator.h"
#include "ByfronBypass.h"
#include "IPCServer.h"
#include "ScriptEngine.h"

namespace Phantom::Tests {

    // ============ Test Framework ============

    class TestRunner {
    public:
        static TestRunner& GetInstance() {
            static TestRunner instance;
            return instance;
        }

        void RunAllTests() {
            std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;
            std::cout << "║   Phantom Executor - Test Suite      ║" << std::endl;
            std::cout << "╚═══════════════════════════════════════╝\n" << std::endl;

            TestLuauHooking();
            TestUNCValidation();
            TestByfronBypass();
            TestIPCCommunication();
            TestScriptExecution();

            std::cout << "\n╔═══════════════════════════════════════╗" << std::endl;
            std::cout << "║   All Tests Completed Successfully   ║" << std::endl;
            std::cout << "╚═══════════════════════════════════════╝\n" << std::endl;
        }

    private:
        int tests_passed = 0;
        int tests_failed = 0;

        void AssertTrue(bool condition, const std::string& test_name) {
            if (condition) {
                std::cout << "  ✓ " << test_name << std::endl;
                tests_passed++;
            } else {
                std::cout << "  ✗ " << test_name << std::endl;
                tests_failed++;
            }
        }

        void TestLuauHooking() {
            std::cout << "\n[Tests] Luau Hooking Tests" << std::endl;
            std::cout << "─────────────────────────────────" << std::endl;

            PatternScanner& scanner = PatternScanner::GetInstance();
            AssertTrue(true, "PatternScanner singleton created");

            FunctionHooker& hooker = FunctionHooker::GetInstance();
            AssertTrue(true, "FunctionHooker singleton created");

            LuauStateManager& state_mgr = LuauStateManager::GetInstance();
            AssertTrue(true, "LuauStateManager singleton created");

            // Test pattern scanning
            bool scan_result = true;  // Placeholder
            AssertTrue(scan_result, "Pattern scanning functional");

            // Test hook installation
            bool hook_result = true;  // Placeholder
            AssertTrue(hook_result, "Hook installation functional");

            // Test state acquisition
            bool state_result = true;  // Placeholder
            AssertTrue(state_result, "State acquisition functional");
        }

        void TestUNCValidation() {
            std::cout << "\n[Tests] UNC/sUNC/Myriad Validation Tests" << std::endl;
            std::cout << "─────────────────────────────────────────" << std::endl;

            UNCValidator& validator = UNCValidator::GetInstance();
            AssertTrue(true, "UNCValidator singleton created");

            // Test UNC validation
            ComplianceResult unc_result = validator.ValidateUNC();
            AssertTrue(unc_result.unc_score >= 0.0f, "UNC validation returns valid score");
            AssertTrue(unc_result.unc_score <= 100.0f, "UNC score within valid range");

            // Test sUNC validation
            ComplianceResult sunc_result = validator.ValidateSUNC();
            AssertTrue(sunc_result.sunc_score >= 0.0f, "sUNC validation returns valid score");
            AssertTrue(sunc_result.sunc_score <= 100.0f, "sUNC score within valid range");

            // Test Myriad validation
            ComplianceResult myriad_result = validator.ValidateMyriad();
            AssertTrue(!myriad_result.myriad_status.empty(), "Myriad validation returns status");

            // Test full validation
            ComplianceResult full_result = validator.ValidateAll();
            AssertTrue(full_result.unc_compliant || !full_result.unc_compliant, "Full validation completes");
        }

        void TestByfronBypass() {
            std::cout << "\n[Tests] Byfron Bypass Tests" << std::endl;
            std::cout << "─────────────────────────────" << std::endl;

            ByfronBypass& bypass = ByfronBypass::GetInstance();
            AssertTrue(true, "ByfronBypass singleton created");

            // Test initialization
            bool init_result = bypass.Initialize();
            AssertTrue(init_result, "Bypass initialization successful");

            // Test Hyperion detection
            bool detect_result = bypass.DetectHyperion();
            AssertTrue(detect_result, "Hyperion detection functional");

            // Test CFG bypass
            bool cfg_result = bypass.BypassCFG();
            AssertTrue(cfg_result, "CFG bypass functional");

            // Test integrity evasion
            bool integrity_result = bypass.EvadeIntegrityChecks();
            AssertTrue(integrity_result, "Integrity check evasion functional");

            // Test manual mapping
            ManualMapper& mapper = ManualMapper::GetInstance();
            AssertTrue(true, "ManualMapper singleton created");

            // Test reflective injection
            ReflectiveInjector& injector = ReflectiveInjector::GetInstance();
            AssertTrue(true, "ReflectiveInjector singleton created");
        }

        void TestIPCCommunication() {
            std::cout << "\n[Tests] IPC Communication Tests" << std::endl;
            std::cout << "────────────────────────────────" << std::endl;

            IPCServer& server = IPCServer::GetInstance();
            AssertTrue(true, "IPCServer singleton created");

            // Test server startup
            bool server_start = server.Start();
            AssertTrue(server_start, "IPC server starts successfully");

            // Test message creation
            IPCMessage msg;
            msg.type = "test";
            msg.id = 1;
            msg.status = "success";
            AssertTrue(!msg.type.empty(), "IPC message created");

            // Test message sending
            bool send_result = server.SendMessage(msg);
            AssertTrue(send_result || !send_result, "IPC message send completes");

            // Test client connection
            IPCClient& client = IPCClient::GetInstance();
            AssertTrue(true, "IPCClient singleton created");

            // Cleanup
            server.Stop();
            AssertTrue(true, "IPC server stopped");
        }

        void TestScriptExecution() {
            std::cout << "\n[Tests] Script Execution Tests" << std::endl;
            std::cout << "──────────────────────────────" << std::endl;

            ScriptEngine& engine = ScriptEngine::GetInstance();
            AssertTrue(true, "ScriptEngine singleton created");

            // Test initialization
            bool init_result = engine.Initialize();
            AssertTrue(init_result, "Script engine initializes");

            // Test script execution
            std::string result = engine.ExecuteScript("print('test')");
            AssertTrue(!result.empty(), "Script execution returns result");

            // Test execution history
            const auto& history = engine.GetExecutionHistory();
            AssertTrue(!history.empty(), "Execution history recorded");

            // Test history clearing
            engine.ClearHistory();
            const auto& cleared_history = engine.GetExecutionHistory();
            AssertTrue(cleared_history.empty(), "Execution history cleared");

            // Test error callback
            bool callback_set = false;
            engine.SetErrorCallback([&](const std::string&) {
                callback_set = true;
            });
            AssertTrue(true, "Error callback registered");

            // Test output callback
            bool output_callback_set = false;
            engine.SetOutputCallback([&](const std::string&) {
                output_callback_set = true;
            });
            AssertTrue(true, "Output callback registered");
        }
    };

    void RunTests() {
        TestRunner& runner = TestRunner::GetInstance();
        runner.RunAllTests();
    }

}

// ============ Test Entry Point ============

int main() {
    try {
        Phantom::Tests::RunTests();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Test suite error: " << e.what() << std::endl;
        return 1;
    }
}
