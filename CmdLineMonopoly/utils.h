#pragma once

#define PDC_WIDE
#include <curses.h>
#include <string>

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

#define rotr _rotr8
#define rotl _rotl8

#define SET_CCHAR_COLOR(cchar, color) cchar = static_cast<unsigned int>(cchar) & ~A_COLOR | (color) << 24;

#else
#include <wchar.h>
#include <x86intrin.h>

inline unsigned long ctz(unsigned long value) {
    if (value == 0) {
        return 0;
    } else {
        return __builtin_ctzl(value);
    }
}

inline unsigned long clz(unsigned long value) {
    if (value == 0) {
        return 0;
    } else {
        return __builtin_clzl(value);
    }
}

#define rotr __rorb
#define rotl __rolb

#define SET_CCHAR_COLOR(cchar, color) {\
    wchar_t wch;attr_t attrs;short color_pair;getcchar(&cchar, &wch, &attrs, &color_pair,NULL);\
    setcchar(&cchar, &wch, attrs, color, NULL);\
}

#endif

#include "BoardState.h"

bool showYesNoPrompt(WINDOW* win, BoardState* boardState, std::string prompt, int x, int y);
