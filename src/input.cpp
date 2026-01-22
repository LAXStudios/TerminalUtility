#include "input.h"
//
// Created by lax on 1/21/26.
//

#include "input.h"

namespace laxterm{
    std::string InputField::getInput(const std::string &prompt, const std::string &inputSymbol, const int &currentRow) {
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
}
