#pragma once

#include <co/co.h>
#include <co/co/mutex.h>
#include <co/time.h>
#include <co/tasked.h>

#include <Windows.h>

#include <string>
#include <unordered_set>

#include "memoryOperation.hpp"

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

        auto getGameProcess() const -> HANDLE {
            co::MutexGuard guard(gameMut);

            return gameProcess;
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

                writeLevelMemory(getGameProcess(), 0x5560, number);
            });
        }

        auto cheatMode(DWORD active) const -> void {
            go([this, active]{
                if (!available()) {
                    return;
                }

                writeGlobalMemory(getGameProcess(), 0x814, active);
            });
        }

        auto toLastAttack() const -> void {
            go([this]{
                if (!available()) {
                    return;
                }

                DWORD totalAttacks {readLevelMeMory(getGameProcess(), 0x5564)};

                if (totalAttacks == 0) {
                    return;
                }

                writeLevelMemory(getGameProcess(), 0x557C, totalAttacks - 1);
                writeLevelMemory(getGameProcess(), 0x559C,1);
            });
        }

        auto setMoney(DWORD money) const -> void {
            go([money {money / 10}, this]{
                if (!available()) {
                    return;
                }

                writeArchiveMemory(getGameProcess(), 0x28, money);
            });
        }
    };
}