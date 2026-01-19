//
// Created by lax on 1/18/26.
//

// The More I know:
// Terminal entry cord is 1:1

#ifndef TERMINALUTILITY_INPUT_H
#define TERMINALUTILITY_INPUT_H

#pragma once

#include "terminal.h"


inline void renderInput(
        const int &row,
        const int &column,
        const std::string &prompt,
        const std::string &input,
        const std::string &inputSymbol) {

    if (prompt.empty()) {
        std::cout << inputSymbol << input << std::endl;
        laxterm::moveTo(row, column);
    } else {
        std::cout << prompt << std::endl;
        std::cout << inputSymbol<< input << std::endl;
        laxterm::moveTo(row + 1, column);
    }

}

namespace laxterm {
    inline std::string getInput(const std::string &prompt = "", const std::string &inputSymbol = "", const int &currentRow = 1) {
        std::string input;
        const int cursorEntryPoint = static_cast<int>(inputSymbol.length() + 1);

        showCursor();

        renderInput(currentRow, cursorEntryPoint, prompt, input, inputSymbol);

        while (true) {
            KeyResult kr = readKey();
            clearScreen();

            if (kr.character == '\n') {
                break;
            }

            if (kr.character == '\b' || kr.character == '\x7f') {
                if (!input.empty()) {
                    input.erase(input.length() - 1);
                    renderInput(currentRow, static_cast<int>(input.length() + cursorEntryPoint), prompt, input, inputSymbol);
                    continue;
                } else {
                    renderInput(currentRow, cursorEntryPoint, prompt, input, inputSymbol);
                    continue;
                }

            }

            if (kr.keyType == Key::Char) {
                input += kr.character;
            }

            renderInput(currentRow, static_cast<int>(input.length() + cursorEntryPoint), prompt, input, inputSymbol);
        }

        return input;
    }

    // IDEA: Input but with Context, so that everything that was above is rendered ontop
    /*
    inline std::string getInput(const std::string &inputSymbol, const std::string &previousContext) {
        std::string input;

        return input;

    }
    */
}

#endif //TERMINALUTILITY_INPUT_H