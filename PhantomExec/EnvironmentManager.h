#pragma once
#include <string>
#include <map>
#include <vector>

namespace Phantom {

    /**
     * Environment Manager
     * Manages Roblox game environment access and object manipulation
     */
    class EnvironmentManager {
    public:
        static EnvironmentManager& GetInstance() {
            static EnvironmentManager instance;
            return instance;
        }

        /**
         * Initialize environment manager
         */
        bool Initialize();

        /**
         * Get global environment
         */
        void* GetGlobalEnvironment();

        /**
         * Get Roblox environment
         */
        void* GetRobloxEnvironment();

        /**
         * Get game object
         */
        void* GetGameObject();

        /**
         * Get workspace
         */
        void* GetWorkspace();

        /**
         * Find object by name
         */
        void* FindObjectByName(const std::string& name);

        /**
         * Get object property
         */
        std::string GetObjectProperty(void* obj, const std::string& property);

        /**
         * Set object property
         */
        bool SetObjectProperty(void* obj, const std::string& property, const std::string& value);

        /**
         * Call object method
         */
        std::string CallObjectMethod(void* obj, const std::string& method, const std::vector<std::string>& args);

        /**
         * Create object
         */
        void* CreateObject(const std::string& class_name);

        /**
         * Destroy object
         */
        bool DestroyObject(void* obj);

        /**
         * Get object children
         */
        std::vector<void*> GetObjectChildren(void* obj);

        /**
         * Get object parent
         */
        void* GetObjectParent(void* obj);

    private:
        EnvironmentManager() : global_env(nullptr), roblox_env(nullptr), game_obj(nullptr) {}
        ~EnvironmentManager() {}

        void* global_env;
        void* roblox_env;
        void* game_obj;

        /**
         * Acquire global environment reference
         */
        bool AcquireGlobalEnvironment();

        /**
         * Acquire Roblox-specific environment
         */
        bool AcquireRobloxEnvironment();

        /**
         * Setup environment hooks
         */
        bool SetupEnvironmentHooks();
    };

}
