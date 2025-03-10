#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <cstdint>
#include <vector>
#include "../SDK/Vector.h"

class Memory {
public:
    Memory(const wchar_t* processName);
    ~Memory();

    template<typename T>
    T Read(uintptr_t address) {
        T value = T();
        ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(T), nullptr);
        return value;
    }

    template<typename T>
    bool Write(uintptr_t address, T value) {
        return WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), nullptr);
    }

    uintptr_t GetModuleBaseAddress(const wchar_t* moduleName);
    bool IsRunning() const { return processHandle != nullptr; }

    DWORD processId;
    HANDLE processHandle;
    uintptr_t clientDll;

private:
    DWORD GetProcessIdByName(const wchar_t* processName);
};