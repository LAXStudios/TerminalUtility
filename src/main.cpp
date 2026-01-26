#include <iostream>
#include <string>
#include <vector>
#include "terminal.h"

int main() {

    // Testing Sandbox
    laxterm::RawMode raw_mode;
    laxterm::clearScreen();
    laxterm::hideCursor();

    std::vector<std::string> mainMenuItems = { laxterm::setColor("Blue", laxterm::TerminalColor::BLUE), "Option 2"};

    laxterm::Menu menu(mainMenuItems);
    std::string menuReturn = menu.MenuRun();

    laxterm::writeToTerminal(menuReturn + '\n');

    return 0;
}