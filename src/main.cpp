#include <iostream>
#include <string>
#include <vector>
#include "terminal.h"
#include "menu.h"
#include "input.h"

int main() {

    // Testing Sandbox
    laxterm::RawMode raw_mode;
    laxterm::clearScreen();
    laxterm::hideCursor();

    std::vector<std::string> mainMenuItems = { laxterm::setColor("Blue", laxterm::TerminalColor::BLUE), "Option 2"};

    laxterm::Menu menu(mainMenuItems);
    std::string menuReturn = menu.MenuRun();

    laxterm::clearScreen();

    std::string input = laxterm::getInput();
    std::string input1 = laxterm::getInput("Hi", "> ");

    std::string input2 = laxterm::InputField::getInput("Hi from upper class", "> ");

    return 0;
}