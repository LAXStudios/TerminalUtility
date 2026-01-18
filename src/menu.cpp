//
// Created by lax on 1/17/26.
//

#include "menu.h"

namespace laxterm {
    Menu::Menu(std::vector<std::string> &menuItems, const int startMenuIndex) {
        menuItemsPtr = &menuItems;
        menuIndex = startMenuIndex;
    }

    std::string Menu::MenuRun() {
        printMenu();

        // Main Menu loop
        while (isMenu) {

            laxterm::KeyResult keyResult = laxterm::readKey();

            if (keyResult.character == 'q') {
                isMenu = false;
                return std::string{};
            }

            if (keyResult.character == '\n') {
                return menuItemsPtr->at(menuIndex);
            }

            switch (keyResult.keyType) {
                case laxterm::Key::ArrowUp:
                    decreaseMenuIndex();
                    break;
                case laxterm::Key::ArrowDown:
                    increaseMenuIndex();
                    break;
                default:
                    // Nothing
                    break;
            }
            laxterm::clearScreen();
            printMenu();
        }
        return std::string{};
    }

    void Menu::increaseMenuIndex() {
        if (menuIndex < menuItemsPtr->size() - 1) {
            menuIndex++;
        } else {
            menuIndex = 0;
        }
    }

    void Menu::decreaseMenuIndex() {
        if (menuIndex > 0 && menuIndex < menuItemsPtr->size()) {
            menuIndex--;
        } else {
            menuIndex = menuItemsPtr->size() - 1;
        }
    }

    void Menu::printMenu() const {
        for (int i = 0; i < menuItemsPtr->size(); i++) {
            if (i == menuIndex) {
                std::cout << "> " << menuItemsPtr->at(i) << std::endl;
            } else {
                std::cout << "  " << menuItemsPtr->at(i) << std::endl;
            }
        }
    }
}