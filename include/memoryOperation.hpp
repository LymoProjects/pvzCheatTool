#pragma once

#include <Windows.h>

namespace pvz {
    inline
    auto writeLevelMemory(HANDLE gameProcess, DWORD offset, DWORD value) -> bool {
            DWORD baseAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            baseAddr += 0x768;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            baseAddr += offset;

            if (::WriteProcessMemory(gameProcess, reinterpret_cast<LPVOID>(baseAddr), &value, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            return true;
        }

        inline
        auto readLevelMeMory(HANDLE gameProcess, DWORD offset) -> DWORD {
            DWORD baseAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            baseAddr += 0x768;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            baseAddr += offset;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            return baseAddr;
        }

        inline
        auto writeArchiveMemory(HANDLE gameProcess, DWORD offset, DWORD value) -> bool {
            DWORD baseAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            baseAddr += 0x82C;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            baseAddr += offset;

            if (::WriteProcessMemory(gameProcess, reinterpret_cast<LPVOID>(baseAddr), &value, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            return true;
        }

        inline
        auto readArchiveMemory(HANDLE gameProcess, DWORD offset, DWORD value) -> DWORD {
            DWORD baseAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            baseAddr += 0x82C;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            baseAddr += offset;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            return baseAddr;
        }

        inline
        auto writeGlobalMemory(HANDLE gameProcess, DWORD offset, DWORD value) -> bool {
            DWORD baseAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            baseAddr += offset;

            if (::WriteProcessMemory(gameProcess, reinterpret_cast<LPVOID>(baseAddr), &value, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            return true;
        }

        inline
        auto readGlobalMemory(HANDLE gameProcess, DWORD offset) -> DWORD {
            DWORD baseAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            baseAddr += offset;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(baseAddr), &baseAddr, sizeof(DWORD), nullptr) == 0) {
                return 0;
            }

            return baseAddr;
        }
}