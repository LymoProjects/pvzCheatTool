#include <complex>
#include <iostream>
#include <string>

#include <ftxui/component/component.hpp>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/screen_interactive.hpp>

#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/event.hpp"

#include "ftxui/dom/node.hpp"
#include "ftxui/screen/color.hpp"
#include "gameHandler.hpp"

auto test(int argc, char const ** argv) -> int {
    if (argc < 2) {
        std::cout << "you need to input operation.\n";

        return 1;
    }

    pvz::gameHandler game;

    std::string operation(argv[1]);

    if (operation == "setSun") {
        if (argc < 3) {
            std::cout << "you need to input sun count!\n";

            return 1;
        }

        game.setSun(std::stoi(std::string(argv[2])));
    } else if (operation == "cheatMode") {
        if (argc < 3) {
            std::cout << "you need to input cheat mode avtive boolean!\n";

            return 1;
        }
        
        game.cheatMode(std::stoi(std::string(argv[2])));
    } else if (operation == "toLastAttack") {
        game.toLastAttack();
    } else if (operation == "passLevel") {
        game.passLevel();
    } else if (operation == "setMoney") {
        if (argc < 3) {
            std::cout << "you need to input money!\n";

            return 1;
        }
        
        game.setMoney(std::stoi(std::string(argv[2])));
    } else {
        std::cout << "operation is not working!\n";

        return 1;
    }

    sleep::ms(500);

    return 0;
}

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

    auto cheatModeTogglePart__ {ftxui::Toggle(&cheatModeOptions, &cheatModeSelected)};

    auto cheatModeTogglePart {ftxui::Renderer(cheatModeTogglePart__, [&]{
        return ftxui::hbox({
            ftxui::text("无消耗无冷却种植 : "),
            cheatModeTogglePart__->Render(),
        });
    })};

    auto cheatModeButton {ftxui::Button("应用", [&]{
        game.cheatMode(std::abs(1 - cheatModeSelected));
    })};

    auto toLastAttackButton {ftxui::Button("直接到达最后一波", [&]{
        game.toLastAttack();
    })};
    auto passLevelButton {ftxui::Button("直接通过本关", [&]{
        game.passLevel();
    })};

    auto levelButtons {ftxui::Container::Horizontal({
        toLastAttackButton,
        passLevelButton
    })};

    auto comps {ftxui::Container::Vertical({
        moneyInputPart,
        moneyButtonPart,
        sunInputPart,
        sunButtonPart,
        cheatModeTogglePart,
        cheatModeButton,
        levelButtons
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
                ftxui::text("请输入金钱数 : "),
                ftxui::filler(),
                moneyInputPart->Render(),
                ftxui::filler()
            }),
            ftxui::hbox({
                ftxui::filler(),
                moneyButtonPart->Render(),
                ftxui::filler()
            }),
            ftxui::separatorEmpty(),
            ftxui::hbox({
                ftxui::text("请输入阳光数 : "),
                ftxui::filler(),
                sunInputPart->Render(),
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
                cheatModeTogglePart->Render(),
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
                levelButtons->Render(),
                ftxui::filler()
            })
        }));
    })};
    
    auto mainWindowScreen {ftxui::ScreenInteractive::TerminalOutput()};

    mainWindowScreen.Loop(mainWindow);

    return 0;
}