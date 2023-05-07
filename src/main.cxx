#include <iostream>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "gameHandler.hpp"

auto main() -> int {
    pvz::gameHandler game;

    std::string moneyInputValue;

    auto moneyInputPart {ftxui::Input(&moneyInputValue, "金钱")};
    auto moneyButtonPart {ftxui::Button("修改金钱", [&]{
        if (moneyInputValue.empty()) {
            return;
        }

        game.setMoney(std::stoi(moneyInputValue));
    })};

    std::string sunInputValue;

    auto sunInputPart {ftxui::Input(&sunInputValue, "阳光")};
    auto sunButtonPart {ftxui::Button("修改阳光", [&]{
        if (sunInputValue.empty()) {
            return;
        }

        game.setSun(std::stoi(sunInputValue));
    })};

    std::vector<std::string> cheatModeOptions {"开", "关"};
    int cheatModeSelected {1};

    auto cheatModeTogglePart {ftxui::Toggle(&cheatModeOptions, &cheatModeSelected)};

    auto cheatModeButton {ftxui::Button("应用", [&]{
        game.cheatMode(std::abs(1 - cheatModeSelected));
    })};

    auto toLastAttackButton {ftxui::Button("直接迎来最后一波", [&]{
        game.toLastAttack();
    })};

    auto comps {ftxui::Container::Vertical({
        moneyInputPart,
        moneyButtonPart,
        sunInputPart,
        sunButtonPart,
        cheatModeTogglePart,
        cheatModeButton,
        toLastAttackButton
    })};

    std::wstring windowTitleString(
        game.available() ?
        L"游戏已启动(" + game.getGameName() + L")" : L"请启动游戏后再打开修改器"
    );

    auto windowTitle {ftxui::text(windowTitleString)};

    windowTitle |= ftxui::color(
        game.available() ?
        ftxui::Color::Green : ftxui::Color::Red
    );

    auto mainWindow {ftxui::Renderer(comps, [&]{
        return ftxui::window(windowTitle, ftxui::vbox({
            ftxui::hbox({
                ftxui::filler(),
                ftxui::window(ftxui::text("请输入金钱数"), moneyInputPart->Render()),
                ftxui::filler()
            }),
            ftxui::hbox({
                ftxui::filler(),
                moneyButtonPart->Render(),
                ftxui::filler()
            }),
            ftxui::separatorEmpty(),
            ftxui::hbox({
                ftxui::filler(),
                ftxui::window(ftxui::text("请输入阳光数"), sunInputPart->Render()),
                ftxui::filler()
            }),
            ftxui::hbox({
                ftxui::filler(),
                sunButtonPart->Render(),
                ftxui::filler()
            }),
            ftxui::separatorEmpty(),
            ftxui::hbox({
                ftxui::filler(),
                ftxui::window(ftxui::text("无消耗无冷却种植"), cheatModeTogglePart->Render()),
                ftxui::filler()
            }),
            ftxui::hbox({
                ftxui::filler(),
                cheatModeButton->Render(),
                ftxui::filler()
            }), 
            ftxui::separatorEmpty(),
            ftxui::hbox({
                ftxui::filler(),
                toLastAttackButton->Render(),
                ftxui::filler()
            })
        }));
    })};
    
    auto mainWindowScreen {ftxui::ScreenInteractive::TerminalOutput()};

    mainWindowScreen.Loop(mainWindow);

    return 0;
}