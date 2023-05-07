#pragma once

#include <co/co.h>
#include <co/time.h>

#include <Windows.h>
#include <cstddef>
#include <handleapi.h>

#include <string>

namespace pvz {
    struct gameNames {
        static constexpr
        wchar_t const * chineseName {L"植物大战僵尸中文版"};

        static constexpr
        wchar_t const * englishName {L"Plants vs. Zombies"};
    };
    
    class gameHandler {
        std::wstring gameName {gameNames::chineseName};
        HWND gameWindow {};
        DWORD gamePid {};
        HANDLE gameProcess {};

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
            if (gameProcess) {
                ::CloseHandle(gameProcess);
            }    
        }

        gameHandler(gameHandler && handler__) noexcept 
        : gameName(std::move(handler__.gameName)), gameWindow {handler__.gameWindow}, gamePid {handler__.gamePid}, gameProcess {handler__.gameProcess} {
            handler__.gameWindow = {};
            handler__.gamePid = {};
            handler__.gameProcess = {};
        }

        auto operator=(gameHandler && handler__) noexcept -> gameHandler & {
            if (this != &handler__) {
                gameName = std::move(handler__.gameName);
                gameWindow = handler__.gameWindow;
                gamePid = handler__.gamePid;
                gameProcess = handler__.gameProcess;

                handler__.gameWindow = {};
                handler__.gamePid = {};
                handler__.gameProcess = {};
            }

            return *this;
        }

        explicit
        gameHandler() {
            gameWindow = ::FindWindowW(L"MainWindow", gameName.c_str());

            if (!gameWindow) {
                gameName = gameNames::englishName;

                gameWindow = ::FindWindowW(L"MainWindow", gameName.c_str());

                if (!gameWindow) {
                    return;
                }
            }

            ::GetWindowThreadProcessId(gameWindow, &gamePid);

            if (gamePid == 0) {
                return;
            }

            gameProcess = ::OpenProcess(PROCESS_ALL_ACCESS, false, gamePid);
        }

        auto available() const -> bool {
            return gameProcess != nullptr;
        }

        auto getGameName() const -> std::wstring const & {
            return gameName;
        }

        auto setSun(DWORD number) const -> void {
            if (!available()) {
                return;
            }

            go([this, number]{
                writeLevelMemory(0x5560, number);
            });
        }

        auto cheatMode(DWORD active) const -> void {
            if (!available()) {
                return;
            }

            go([this, active]{
                writeGlobalMemory(0x814, active);
            });
        }

        auto toLastAttack() const -> void {
            if (!available()) {
                return;
            }

            DWORD totalAttacks {readLevelMeMory(0x5564)};

            if (totalAttacks == 0) {
                return;
            }

            go([totalAttacks, this]{
                writeLevelMemory(0x557C, totalAttacks - 1);
                writeLevelMemory(0x559C,1);
            });
        }

        auto passLevel() const -> void {
            if (!available()) {
                return;
            }

            go([this]{
                writeLevelMemory(0x5600, 1);
            });
        }

        auto setMoney(DWORD money) const -> void {
            if (!available()) {
                return;
            }

            go([money {money / 10}, this]{
                writeArchiveMemory(0x28, money);
            });
        }
    };
}