#include "Memory.h"

Memory::Memory(const wchar_t* processName) : processHandle(nullptr), clientDll(0) {
    DWORD processId = 0;
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != INVALID_HANDLE_VALUE) {
        PROCESSENTRY32W processEntry;
        processEntry.dwSize = sizeof(processEntry);
        if (Process32FirstW(snapshot, &processEntry)) {
            do {
                if (_wcsicmp(processEntry.szExeFile, processName) == 0) {
                    processId = processEntry.th32ProcessID;
                    break;
                }
            } while (Process32NextW(snapshot, &processEntry));
        }
        CloseHandle(snapshot);
    }

    if (processId) {
        processHandle = OpenProcess(PROCESS_VM_READ | PROCESS_VM_WRITE, FALSE, processId);
        if (processHandle) {
            HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);
            if (snapshot != INVALID_HANDLE_VALUE) {
                MODULEENTRY32W moduleEntry;
                moduleEntry.dwSize = sizeof(moduleEntry);
                if (Module32FirstW(snapshot, &moduleEntry)) {
                    do {
                        if (_wcsicmp(moduleEntry.szModule, L"client.dll") == 0) {
                            clientDll = (uintptr_t)moduleEntry.modBaseAddr;
                            break;
                        }
                    } while (Module32NextW(snapshot, &moduleEntry));
                }
                CloseHandle(snapshot);
            }
        }
    }
}

Memory::~Memory() {
    if (processHandle) {
        CloseHandle(processHandle);
    }
}