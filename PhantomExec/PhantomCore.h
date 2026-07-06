#pragma once
#include <windows.h>
#include <iostream>
#include <vector>

// Forward declarations for Luau VM structures (placeholders for actual Luau types)
struct lua_State;

namespace Phantom {

    class Executor {
    public:
        static Executor& GetInstance() {
            static Executor instance;
            return instance;
        }

        bool Initialize();
        bool InjectScript(const std::string& script);
        void Shutdown();

    private:
        Executor() : m_luaState(nullptr) {}
        ~Executor() { Shutdown(); }

        lua_State* m_luaState;
        bool m_isInitialized = false;

        // Internal methods for VM interaction
        bool FindLuauFunctions();
        bool AcquireState();
    };

}
