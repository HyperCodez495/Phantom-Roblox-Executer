#pragma once
#include <windows.h>
#include <cstdint>
#include <vector>
#include <string>
#include <map>

namespace Phantom {

    // Luau VM function signatures
    typedef int (*lua_pcall_t)(void* L, int nargs, int nresults, int errfunc);
    typedef int (*lua_load_t)(void* L, const char* chunk, size_t size, const char* chunkname);
    typedef void* (*lua_newstate_t)(void* (*f)(void*, void*, size_t), void* ud);
    typedef void (*lua_close_t)(void* L);
    typedef void (*lua_getglobal_t)(void* L, const char* name);
    typedef void (*lua_setglobal_t)(void* L, const char* name);
    typedef const char* (*lua_tolstring_t)(void* L, int idx, size_t* len);
    typedef void (*lua_pushstring_t)(void* L, const char* s);

    /**
     * Pattern-based function finder for Luau C API
     * Scans Roblox executable for function signatures
     */
    class PatternScanner {
    public:
        static PatternScanner& GetInstance() {
            static PatternScanner instance;
            return instance;
        }

        /**
         * Scan for function by signature
         * @param signature Byte pattern with wildcards (? = any byte)
         * @param mask Mask indicating which bytes to match
         * @param start_addr Starting address for scan
         * @param end_addr Ending address for scan
         * @return Address of first match, or nullptr if not found
         */
        uintptr_t FindPattern(const std::string& signature, const std::string& mask,
                             uintptr_t start_addr, uintptr_t end_addr);

        /**
         * Find Luau function by known signature
         */
        uintptr_t FindLuaPcall();
        uintptr_t FindLuaLoad();
        uintptr_t FindLuaNewstate();
        uintptr_t FindLuaGetglobal();
        uintptr_t FindLuaSetglobal();

    private:
        PatternScanner() {}
        ~PatternScanner() {}

        // Known signatures for Luau functions (may vary by Roblox version)
        const std::string LUA_PCALL_SIG = "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC";
        const std::string LUA_PCALL_MASK = "xxxx?xxxx?xxxx?xxxxxxxxxx";
    };

    /**
     * Trampoline-based function hooker
     * Implements x86-64 detour hooks with minimal overhead
     */
    class FunctionHooker {
    public:
        static FunctionHooker& GetInstance() {
            static FunctionHooker instance;
            return instance;
        }

        /**
         * Install a trampoline hook on a function
         * @param target_func Address of function to hook
         * @param hook_func Address of replacement function
         * @param original_func Pointer to store original function address
         * @return true if hook installed successfully
         */
        bool InstallHook(uintptr_t target_func, uintptr_t hook_func, uintptr_t* original_func);

        /**
         * Remove a previously installed hook
         */
        bool RemoveHook(uintptr_t target_func);

        /**
         * Hook all critical Luau functions
         */
        bool HookAllLuauFunctions();

    private:
        FunctionHooker() {}
        ~FunctionHooker() {}

        struct HookInfo {
            uintptr_t target;
            uintptr_t hook;
            uintptr_t trampoline;
            std::vector<uint8_t> original_bytes;
        };

        std::map<uintptr_t, HookInfo> active_hooks;

        /**
         * Create x86-64 trampoline
         * Backs up original function prologue and creates JMP to hook
         */
        uintptr_t CreateTrampoline(uintptr_t target_func, uintptr_t hook_func);

        /**
         * Write JMP instruction (x86-64 RIP-relative)
         */
        void WriteJmp(void* address, uintptr_t destination);
    };

    /**
     * Luau VM state manager
     * Maintains references to active VM states and provides access
     */
    class LuauStateManager {
    public:
        static LuauStateManager& GetInstance() {
            static LuauStateManager instance;
            return instance;
        }

        /**
         * Acquire a valid Luau VM state
         * Tries multiple methods: context extraction, state interception, API hooking
         */
        void* AcquireState();

        /**
         * Get the current active state
         */
        void* GetCurrentState();

        /**
         * Register a newly discovered state
         */
        void RegisterState(void* state);

        /**
         * Unregister a state (called on lua_close)
         */
        void UnregisterState(void* state);

    private:
        LuauStateManager() : current_state(nullptr) {}
        ~LuauStateManager() {}

        void* current_state;
        std::vector<void*> known_states;

        /**
         * Method 1: Extract state from Roblox script context
         */
        void* ExtractFromScriptContext();

        /**
         * Method 2: Wait for state creation via lua_newstate hook
         */
        void* WaitForStateCreation();

        /**
         * Method 3: Steal state from lua_pcall hook
         */
        void* StealFromPcallHook();
    };

}
