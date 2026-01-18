#include <iostream>
#include <string>
#include <vector>
#include "terminal.h"
#include "menu.h"
#include "input.h"

int main() {

    laxterm::RawMode raw_mode;
    laxterm::clearScreen();
    laxterm::hideCursor();

    std::vector<std::string> mainMenuItems = {"Option 1", "Option 2"};

    laxterm::Menu menu(mainMenuItems);
    std::string menuReturn = menu.MenuRun();

    laxterm::clearScreen();

    std::string input = laxterm::getInput();

    return 0;
}