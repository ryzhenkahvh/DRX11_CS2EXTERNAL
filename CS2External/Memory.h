#pragma once
#include <Windows.h>
#include <string>
#include <TlHelp32.h>

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
    void ReadArray(uintptr_t address, T* buffer, size_t size) {
        ReadProcessMemory(processHandle, (LPCVOID)address, buffer, size, nullptr);
    }

    HANDLE processHandle;
    uintptr_t clientDll;
};