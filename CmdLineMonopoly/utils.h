#pragma once

#include <curses.h>
#include <string>
#include "BoardState.h"

#ifdef _MSC_VER
#include <intrin.h>

inline unsigned long ctz(unsigned long value) {
    unsigned long numTrailingZeroes = 0;
    if (_BitScanForward(&numTrailingZeroes, value)) {
        return numTrailingZeroes;
    } else {
        return 0; // an error ocurred
    }
}

inline unsigned long clz(unsigned long value) {
    unsigned long numLeadingZeroes = 0;
    if (_BitScanReverse(&numLeadingZeroes, value)) {
        // BitScanReverse returns the position of the bit found, so we have to do 7 - it to get number of leading zeroes
        return sizeof(unsigned long) * 8 - 1 - numLeadingZeroes;
    } else {
        return 0; // an error ocurred
    }
}

#else

inline unsigned long ctz(unsigned long value) {
    if (value == 0) {
        return 0;
    } else {
        return __builtin_ctz(value);
    }
}

inline unsigned long clz(unsigned long value) {
    if (value == 0) {
        return 0;
    } else {
        return __builtin_clz(value);
    }
}

#endif

#define SET_CCHAR_COLOR(cchar, color) cchar = cchar & ~A_COLOR | (color) << 24;

bool showYesNoPrompt(WINDOW* win, BoardState* boardState, std::string prompt, int x, int y);
