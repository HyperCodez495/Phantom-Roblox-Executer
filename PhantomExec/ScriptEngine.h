#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace Phantom {

    /**
     * Phantom Script Execution Engine
     * Manages Luau script execution and environment
     */
    class ScriptEngine {
    public:
        static ScriptEngine& GetInstance() {
            static ScriptEngine instance;
            return instance;
        }

        /**
         * Initialize script engine
         */
        bool Initialize();

        /**
         * Execute a Luau script
         * @param script Script source code
         * @return Execution result (output/error)
         */
        std::string ExecuteScript(const std::string& script);

        /**
         * Execute script with timeout
         */
        std::string ExecuteScriptWithTimeout(const std::string& script, int timeout_ms);

        /**
         * Load script from file
         */
        std::string ExecuteScriptFile(const std::string& file_path);

        /**
         * Get execution history
         */
        const std::vector<std::string>& GetExecutionHistory() const;

        /**
         * Clear execution history
         */
        void ClearHistory();

        /**
         * Set error callback
         */
        void SetErrorCallback(std::function<void(const std::string&)> callback);

        /**
         * Set output callback
         */
        void SetOutputCallback(std::function<void(const std::string&)> callback);

    private:
        ScriptEngine() : luau_state(nullptr) {}
        ~ScriptEngine() { Shutdown(); }

        void* luau_state;
        std::vector<std::string> execution_history;
        std::function<void(const std::string&)> error_callback;
        std::function<void(const std::string&)> output_callback;

        /**
         * Create isolated Luau state
         */
        bool CreateState();

        /**
         * Expose Phantom API to script
         */
        bool ExposePhantomAPI();

        /**
         * Setup environment restrictions
         */
        bool SetupEnvironmentRestrictions();

        /**
         * Shutdown and cleanup
         */
        void Shutdown();

        /**
         * Expose individual API functions
         */
        void ExposeCache();
        void ExposeCloningFunctions();
        void ExposeHookingFunctions();
        void ExposeUtilityFunctions();
        void ExposeLoggingFunctions();
    };

}
