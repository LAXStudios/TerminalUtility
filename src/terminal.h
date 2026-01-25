//
// Created by lax on 1/17/26.
//
// Version: 1
// Date: 25.01.2026

#pragma once

#include <termios.h>
#include <unistd.h>
#include <iostream>
#include <sys/ioctl.h>

namespace laxterm {

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

    inline std::string setColor(std::string text, TerminalColor color) {
        return "\033[" + std::to_string(static_cast<int>(color)) + "m" + text + "\033[0m";
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

    inline void moveTo(const int row, const int col) {
        std::string seq = "\x1b[" + std::to_string(row) + ";" + std::to_string(col) + "H";
        write(STDOUT_FILENO, seq.c_str(), seq.size());
    }

    inline void writeToTerminal(const std::string &text) {
        write(STDOUT_FILENO, text.c_str(), text.size());
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

        // Der spaß soll single bleiben wie ich
        RawMode(const RawMode&) = delete;
        RawMode& operator=(const RawMode&) = delete;

    private:
        struct termios orig_{}, raw_{};
    };

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

        switch (char thirdCase = readByte()) {
            case 'A': return {Key::ArrowUp, 0};
            case 'B': return {Key::ArrowDown, 0};
            case 'C': return {Key::ArrowRight, 0};
            case 'D': return {Key::ArrowLeft, 0};
            default: return {Key::Unknown, 0};
        }
    }

    //
    // Terminal Window Info
    //

    struct TerminalSize {
        int height, width;
    };

    inline void getTerminalSize(TerminalSize &terminalSize) {

        struct winsize ws;
        if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) != -1) {
            terminalSize.width = ws.ws_col;
            terminalSize.height = ws.ws_row;
        }
    }
}


// Version Info:
// V.1
// Coloring, KeyInput, writeToTerminal, RawMode, TerminalSize, Cursor management