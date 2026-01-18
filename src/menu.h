//
// Created by lax on 1/17/26.
//

#ifndef TERMINALUTILITY_MENU_H
#define TERMINALUTILITY_MENU_H


#include <iostream>
#include <string>
#include <vector>
#include "terminal.h"

namespace laxterm {

    class Menu {
    public:
        explicit Menu(std::vector<std::string> &menuItems, int startMenuIndex = 0);
        std::string MenuRun();

    private:
        bool isMenu = true;
        int menuIndex = 0;
        std::vector<std::string>* menuItemsPtr;

        void printMenu() const;
        void increaseMenuIndex();
        void decreaseMenuIndex();
    };
}

#endif //TERMINALUTILITY_MENU_H