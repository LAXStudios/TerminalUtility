//
// Created by lax on 1/18/26.
//

#ifndef TERMINALUTILITY_INPUT_H
#define TERMINALUTILITY_INPUT_H

#pragma once

#include "terminal.h"

namespace laxterm {
    inline std::string getInput() {
        std::string input;

        while (true) {
            laxterm::KeyResult kr = laxterm::readKey();
            laxterm::clearScreen();
            laxterm::showCursor();

            if (kr.character == '\n') {
                break;
            }

            if (kr.character == '\b' || kr.character == '\x7f') {
                input.erase(input.length() - 1);
                std::cout << input << std::endl;
                laxterm::moveTo(0, input.length() + 1 );
                continue;
            }

            input += kr.character;
            std::cout << input << std::endl;

            laxterm::moveTo(0, input.length() + 1 );
        }

        return input;
    }
}

#endif //TERMINALUTILITY_INPUT_H