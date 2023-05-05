#pragma once

#include <co/co.h>
#include <co/time.h>

#include <Windows.h>
#include <cstddef>
#include <handleapi.h>
#include <string>

namespace pvz {
    struct commonGameNames {
        static constexpr
        wchar_t const * chineseName {L"植物大战僵尸中文版"};
    };

    struct gameInfos {
        static constexpr
        DWORD plantTotalCount {48};
    };
    
    class gameHandler {
        std::wstring gameName;
        HWND gameWindow {};
        DWORD gamePid {};
        HANDLE gameProcess {};

        auto available() const -> bool {
            return gameProcess != nullptr;
        }

        auto setSun__(DWORD number) const -> bool {
            DWORD sunAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(sunAddr), &sunAddr, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            sunAddr += 0x768;

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(sunAddr), &sunAddr, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            sunAddr += 0x5560;

            if (::WriteProcessMemory(gameProcess, reinterpret_cast<LPVOID>(sunAddr), &number, sizeof(DWORD), nullptr) == 0) {
                return false;
            }

            return true;
        }

        auto setNoTimeCost__() const -> void {
            DWORD plantTimeCostAddr {0x69F2C4};
            plantTimeCostAddr -= 0x24;

            DWORD timeCost {};

            for (DWORD i {}; i != gameInfos::plantTotalCount; ++i) {
                plantTimeCostAddr += 0x24;

                ::WriteProcessMemory(gameProcess, reinterpret_cast<LPVOID>(plantTimeCostAddr), &timeCost, sizeof(DWORD), nullptr);
            }
        }

        auto setNoSunCost__() const -> void {
            DWORD plantSunCostAddr {0x69F2C0};
            plantSunCostAddr -= 0x24;

            DWORD sunCost {};

            for (DWORD i {}; i != gameInfos::plantTotalCount; ++i) {
                plantSunCostAddr += 0x24;

                ::WriteProcessMemory(gameProcess, reinterpret_cast<LPVOID>(plantSunCostAddr), &sunCost, sizeof(DWORD), nullptr);
            }
        }

        auto cheatMode__(bool cheat) const -> void {
            DWORD cheatModeAddr {0x6A9EC0};

            if (::ReadProcessMemory(gameProcess, reinterpret_cast<LPCVOID>(cheatModeAddr), &cheatModeAddr, sizeof(DWORD), nullptr) == 0) {
                return;
            }

            cheatModeAddr += 0x814;

            DWORD available__ = static_cast<DWORD>(cheat);

            ::WriteProcessMemory(gameProcess, reinterpret_cast<LPVOID>(cheatModeAddr), &available__, sizeof(DWORD), nullptr);
        }
    public:
        ~gameHandler() {
            if (gameProcess) {
                ::CloseHandle(gameProcess);
            }    
        }

        gameHandler(gameHandler && handler__) noexcept 
        : gameName(std::move(handler__.gameName)), gamePid {handler__.gamePid}, gameProcess {handler__.gameProcess} {
            handler__.gamePid = {};
            handler__.gameProcess = {};
        }

        auto operator=(gameHandler && handler__) noexcept -> gameHandler & {
            if (this != &handler__) {
                gameName = std::move(handler__.gameName);
                gamePid = handler__.gamePid;
                gameProcess = handler__.gameProcess;

                handler__.gamePid = {};
                handler__.gameProcess = {};
            }

            return *this;
        }

        explicit
        gameHandler(std::wstring const & gameName__ = commonGameNames::chineseName) 
        : gameName(gameName__) {
            gameWindow = ::FindWindowW(L"MainWindow", gameName.c_str());

            if (!gameWindow) {
                return;
            }

            ::GetWindowThreadProcessId(gameWindow, &gamePid);

            if (gamePid == 0) {
                return;
            }

            gameProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, gamePid);
        }

        auto setSun(DWORD number) const -> void {
            if (!available()) {
                return;
            }

            go([this, number]{
                setSun__(number);
            });
        }

        auto lockSunForALevel(DWORD number) const -> void {
            if (!available()) {
                return;
            }

            go([this, number]{
                while (setSun__(number)) {
                    sleep::sec(3);
                }
            });
        }

        auto setNoTimeCost() const -> void {
            if (!available()) {
                return;
            }

            go([this]{
                setNoTimeCost__();
            });
        }

        auto setNoSunCost() const -> void {
            if (!available()) {
                return;
            }

            go([this]{
                setNoSunCost__();
            });
        }

        auto cheatMode(bool cheat) const -> void {
            if (!available()) {
                return;
            }

            go([this, cheat]{
                cheatMode__(cheat);
            });
        }
    };
}