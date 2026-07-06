#pragma once
#include <windows.h>
#include <string>

class PhantomCore {
public:
    bool Initialize() { return true; }
    void Shutdown() {}
    bool IsInitialized() const { return true; }
    const char* GetLastError() const { return "No error"; }
};
