//
// Created by lax on 1/17/26.
//
// Version: 2
// Date: 25.01.2026

#pragma once

#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <sys/ioctl.h>

namespace laxterm {

    enum class TerminalColor8Bit;

    enum class TerminalColor {
        BLACK = 30,
        RED = 31,
        GREEN = 32,
        YELLOW = 33,
        BLUE = 34,
        MAGENTA = 35,
        CYAN = 36,
        WHITE = 37,
        // Bright Colors
        BRIGHT_BLACK = 90,
        BRIGHT_RED = 91,
        BRIGHT_GREEN = 92,
        BRIGHT_YELLOW = 93,
        BRIGHT_BLUE = 94,
        BRIGHT_MAGENTA = 95,
        BRIGHT_CYAN = 96,
        BRIGHT_WHITE = 97,
    };

    enum class Key {
        ArrowDown,
        ArrowUp,
        ArrowLeft,
        ArrowRight,
        Char,
        Unknown,
    };

    struct KeyResult {
        Key keyType;
        char character;
    };

    inline std::string setColor(const std::string &text, TerminalColor color) {
        return "\033[" + std::to_string(static_cast<int>(color)) + "m" + text + "\033[0m";
    }

    inline std::string set8BitColor(const std::string &text, TerminalColor8Bit color) {
        return "\033[38;5;" + std::to_string(static_cast<int>(color)) + "m" + text + "\033[0m";
    }

    inline void clearScreen() {
        std::cout << "\033[2J\033[1;1H" << std::flush;
    }

    inline void hideCursor() {
        write(STDOUT_FILENO, "\x1b[?25l", 6);
    }

    inline void showCursor() {
        write(STDOUT_FILENO, "\x1b[?25h", 6);
    }

    inline void moveCursorTo(const int row, const int col) {
        std::string seq = "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "H";
        write(STDOUT_FILENO, seq.c_str(), seq.size());
    }

    inline void writeToTerminal(const std::string &text) {
        write(STDOUT_FILENO, text.c_str(), text.size());
    }


    //
    // Read Key stuff
    //

    inline char readByte() {
        char byte{};
        while (read(STDIN_FILENO, &byte, 1) != 1) {}
        return byte;
    }

    inline KeyResult readKey() {
        if (const char firstCase = readByte(); firstCase != '\x1b')
            return {Key::Char, firstCase};

        if (const char secondCase = readByte(); secondCase != '[')
            return {Key::Unknown, 0};

        switch (readByte()) {
            case 'A': return {Key::ArrowUp, 0};
            case 'B': return {Key::ArrowDown, 0};
            case 'C': return {Key::ArrowRight, 0};
            case 'D': return {Key::ArrowLeft, 0};
            default: return {Key::Unknown, 0};
        }
    }

    // WARNING: Untested
    inline KeyResult readUninterruptingKey() {
        fd_set set;
        FD_ZERO(&set);
        FD_SET(STDIN_FILENO, &set);

        timeval tv{};
        tv.tv_sec  = 0;
        tv.tv_usec = 10'000; // 10ms

        int returnCode = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &tv);
        if (returnCode > 0 && FD_ISSET(STDIN_FILENO, &set)) {
            return readKey();
        }

        return {Key::Unknown};
    }

    //
    // RawMode
    //

    class RawMode {
    public:
        RawMode() {
            tcgetattr(STDIN_FILENO, &orig_);
            raw_ = orig_;
            raw_.c_lflag &= ~(ECHO | ICANON | ISIG);
            raw_.c_cc[VMIN]  = 0;
            raw_.c_cc[VTIME] = 1;
            tcsetattr(STDIN_FILENO, TCSANOW, &raw_);
            hideCursor();
        }
        ~RawMode() {
            tcsetattr(STDERR_FILENO, TCIFLUSH, &orig_);
            showCursor();
        }

        // Der spaß soll single bleiben, wie ich
        RawMode(const RawMode&) = delete;
        RawMode& operator=(const RawMode&) = delete;

    private:
        struct termios orig_{}, raw_{};
    };


    //
    // Menu
    //

    class Menu {
    public:
        explicit Menu(const std::vector<std::string> &menuItems, const int startMenuIndex = 0)
            : menuIndex(startMenuIndex), menuItems(menuItems) {}

        std::string MenuRun() {
            printMenu();

            // Main Menu loop
            while (isMenu) {

                auto [keyType, character] = readKey();

                if (character == 'q') {
                    isMenu = false;
                    return std::string{};
                }

                if (character == '\n') {
                    return menuItems.at(menuIndex);
                }

                switch (keyType) {
                    case Key::ArrowUp:
                        decreaseMenuIndex();
                        break;
                    case Key::ArrowDown:
                        increaseMenuIndex();
                        break;
                    default:
                        // Nothing
                        break;
                }
                clearScreen();
                printMenu();
            }
            return std::string{};
        }

    private:
        bool isMenu = true;
        int menuIndex = 0;
        std::vector<std::string> menuItems;

        void printMenu() const {
            for (int i = 0; i < menuItems.size(); i++) {
                if (i == menuIndex) {
                    std::cout << "> " << menuItems.at(i) << std::endl;
                } else {
                    std::cout << "  " << menuItems.at(i) << std::endl;
                }
            }
        }
        void increaseMenuIndex() {
            if (menuIndex < menuItems.size() - 1) {
                menuIndex++;
            } else {
                menuIndex = 0;
            }
        }
        void decreaseMenuIndex() {
            if (menuIndex > 0 && menuIndex < menuItems.size()) {
                menuIndex--;
            } else {
                menuIndex = static_cast<int>(menuItems.size()) - 1;
            }
        }
    };

    //
    // Terminal Window Info
    //

    struct TerminalSize {
        int height, width;
    };

    inline void getTerminalSize(TerminalSize &terminalSize) {
        struct winsize ws{};
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1) {
            terminalSize.width = ws.ws_col;
            terminalSize.height = ws.ws_row;
        }
    }

    // TODO: Fill in the codes with the rest of the 256 Colors
    enum class TerminalColor8Bit {
        // Standard
        BLACK = 0,
        RED = 1,
        GREEN = 2,
        YELLOW = 3,
        BLUE = 4,
        PURPLE = 5,
        CYAN = 6,
        GRAY = 7,
        WHITE = 15,
        // High Intensity
        H_Gray = 8,
        H_Red = 9,
        H_GREEN = 10,
        H_YELLOW = 11,
        H_BLUE = 12,
        H_PURPLE = 13,
        H_CYAN = 14,
    };
}

// Version Info:
// V.1
// Coloring; KeyInput; writeToTerminal; RawMode; TerminalSize; Cursor management
// V.2
// Added: 8Bit Colors; Rename: moveTo -> moveCursorTo; Added: readUninterruptingKey();
// Added Menu Class from Menu.h and Menu.cpp;