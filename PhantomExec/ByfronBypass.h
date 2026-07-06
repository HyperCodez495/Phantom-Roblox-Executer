#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <vector>

namespace Phantom {

    /**
     * Byfron/Hyperion Anti-Cheat Bypass Module
     * Implements detection evasion and integrity check circumvention
     */
    class ByfronBypass {
    public:
        static ByfronBypass& GetInstance() {
            static ByfronBypass instance;
            return instance;
        }

        /**
         * Initialize bypass mechanisms
         * Should be called early in DLL initialization
         */
        bool Initialize();

        /**
         * Detect Hyperion presence and capabilities
         */
        bool DetectHyperion();

        /**
         * Bypass CFG (Control Flow Guard) checks
         */
        bool BypassCFG();

        /**
         * Evade integrity checks
         */
        bool EvadeIntegrityChecks();

        /**
         * Evade instrumentation detection
         */
        bool EvadeInstrumentation();

        /**
         * Perform stealth injection setup
         */
        bool SetupStealthInjection();

    private:
        ByfronBypass() : hyperion_detected(false), cfg_bypassed(false) {}
        ~ByfronBypass() {}

        bool hyperion_detected;
        bool cfg_bypassed;

        /**
         * Detect if Hyperion is active
         * Checks for known Hyperion signatures and behaviors
         */
        bool IsHyperionActive();

        /**
         * Disable or spoof Hyperion detection functions
         */
        bool SpoofHyperionChecks();

        /**
         * Manipulate CFG bitmap to allow unauthorized jumps
         */
        bool ManipulateCFGBitmap();

        /**
         * Randomize timing to avoid detection
         */
        void RandomizeExecutionTiming();

        /**
         * Self-healing code that repairs modifications after execution
         */
        bool ImplementSelfHealing();
    };

    /**
     * Manual DLL Mapping for stealth injection
     * Avoids LoadLibrary and PEB entry
     */
    class ManualMapper {
    public:
        static ManualMapper& GetInstance() {
            static ManualMapper instance;
            return instance;
        }

        /**
         * Manually map DLL into target process
         * @param dll_path Path to DLL file
         * @param target_process Handle to target process
         * @return Base address of mapped DLL, or nullptr on failure
         */
        void* MapDLL(const std::string& dll_path, HANDLE target_process);

        /**
         * Manually map DLL into current process
         */
        void* MapDLLLocal(const std::string& dll_path);

    private:
        ManualMapper() {}
        ~ManualMapper() {}

        /**
         * Load DLL file into memory
         */
        void* LoadDLLFile(const std::string& dll_path);

        /**
         * Parse PE headers
         */
        bool ParsePEHeaders(void* dll_data);

        /**
         * Allocate memory in target process
         */
        void* AllocateTargetMemory(HANDLE process, size_t size);

        /**
         * Fix import address table (IAT)
         */
        bool FixIAT(void* base_addr);

        /**
         * Fix base relocations
         */
        bool FixRelocations(void* base_addr, uintptr_t new_base);

        /**
         * Call DLL entry point
         */
        bool CallDLLEntry(void* base_addr);
    };

    /**
     * Reflective DLL Injection
     * DLL contains its own loader code
     */
    class ReflectiveInjector {
    public:
        static ReflectiveInjector& GetInstance() {
            static ReflectiveInjector instance;
            return instance;
        }

        /**
         * Inject reflective DLL into target process
         */
        bool InjectReflective(const std::string& dll_path, HANDLE target_process);

    private:
        ReflectiveInjector() {}
        ~ReflectiveInjector() {}

        /**
         * Find reflective loader export in DLL
         */
        uintptr_t FindReflectiveLoader(void* dll_data);
    };

}
