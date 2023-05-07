#pragma once

#include <co/co.h>
#include <co/co/mutex.h>
#include <co/time.h>
#include <co/tasked.h>

#include <Windows.h>
#include <cstddef>
#include <handleapi.h>

#include <string>
#include <unordered_set>

namespace pvz {
    class gameHandler {
        static inline
        std::unordered_set<std::wstring> const gameNames {
            L"植物大战僵尸中文版",
            L"Plants vs. Zombies"
        };

        HANDLE gameProcess {};

        mutable
        co::Mutex gameMut;

        Tasked tsk;

        auto loadGame() -> bool {
            HWND gameWindow {};
            
            for (auto const & pvzName : gameNames) {
                gameWindow = ::FindWindowW(L"MainWindow", pvzName.c_str());

                if (gameWindow) {
                    break;
                }
            }

            if (!gameWindow) {
                co::MutexGuard guard(gameMut);

                gameProcess = {};

                return false;
            }

            DWORD gamePid {};

            ::GetWindowThreadProcessId(gameWindow, &gamePid);

            if (gamePid == 0) {
                co::MutexGuard guard(gameMut);

                gameProcess = {};

                return false;
            }

            auto gameProcess__ {::OpenProcess(PROCESS_ALL_ACCESS, false, gamePid)};

            co::MutexGuard guard(gameMut);

            if (gameProcess__ != gameProcess) {
                gameProcess = gameProcess__;
            }

            return gameProcess;
        }

        auto writeLevelMemory(DWORD offset, DWORD value) const -> bool {
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

        auto readLevelMeMory(DWORD offset) const -> DWORD {
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

        auto writeArchiveMemory(DWORD offset, DWORD value) const -> bool {
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

        auto readArchiveMemory(DWORD offset, DWORD value) const -> DWORD {
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

        auto writeGlobalMemory(DWORD offset, DWORD value) const -> bool {
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

        auto readGlobalMemory(DWORD offset) const -> DWORD {
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
    public:
        ~gameHandler() {
            tsk.stop();

            if (co::MutexGuard guard(gameMut); gameProcess) {
                ::CloseHandle(gameProcess);
            }    
        }

        gameHandler(gameHandler const &) = delete;
        auto operator=(gameHandler const &) -> gameHandler & = delete;

        explicit
        gameHandler() {
            loadGame();

            tsk.run_every([this]{
                go([this]{
                    loadGame();
                });
            }, 2);
        }

        auto available() const -> bool {
            co::MutexGuard guard(gameMut);

            return gameProcess;
        }

        auto setSun(DWORD number) const -> void {
            go([this, number]{
                if (!available()) {
                    return;
                }

                writeLevelMemory(0x5560, number);
            });
        }

        auto cheatMode(DWORD active) const -> void {
            go([this, active]{
                if (!available()) {
                    return;
                }

                writeGlobalMemory(0x814, active);
            });
        }

        auto toLastAttack() const -> void {
            go([this]{
                if (!available()) {
                    return;
                }

                DWORD totalAttacks {readLevelMeMory(0x5564)};

                if (totalAttacks == 0) {
                    return;
                }

                writeLevelMemory(0x557C, totalAttacks - 1);
                writeLevelMemory(0x559C,1);
            });
        }

        auto setMoney(DWORD money) const -> void {
            go([money {money / 10}, this]{
                if (!available()) {
                    return;
                }

                writeArchiveMemory(0x28, money);
            });
        }
    };
}