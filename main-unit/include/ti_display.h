#pragma once
#include <Arduino.h>
#include <algorithm>
#include <string>
#include <cctype>
// #include <sstream>
#include <stdio.h>

#define DIGIT_PORT GPIOA->ODR
#define SEGMENT_PORT GPIOB->ODR

class TiDisplay
{
private:
    const size_t SCREEN_LEN = 11;

    const uint32_t C = 0b100000000;
    const uint32_t DOT = 0b10000000;
    const uint32_t DIGIT_PORT_MASK = 0b1111;
    const uint32_t SEGMENT_PORT_MASK = 0x1FF;

    uint32_t character_table[256];

    const uint32_t UL_BAR = '`';
    const uint32_t UR_BAR = '\'';
    const uint32_t LL_BAR = 0xA;
    const uint32_t LR_BAR = 0xB;
    const uint32_t U_BAR = '^';
    const uint32_t L_BAR = '_';
    const uint32_t M_BAR = '-';
    const uint32_t L_C = 'c';
    const uint32_t L_C_INV = 0xC;
    const uint32_t U_C = 0xD;
    const uint32_t U_C_INV = 0xE;
    const uint32_t L_U = 'u';
    const uint32_t U_U = 0xF;
    const uint32_t L_N = 0x10;
    const uint32_t U_N = 0x11;

    const uint32_t L_L1 = 0x12;
    const uint32_t L_L2 = 0x13;
    const uint32_t L_L3 = 0x14;
    const uint32_t L_L4 = 0x15;
    const uint32_t U_L1 = 0x16;
    const uint32_t U_L2 = 0x17;
    const uint32_t U_L3 = 0x18;
    const uint32_t U_L4 = 0x19;
    const uint32_t U_O = 0x20;
    const uint32_t L_O = 'o';
    const uint32_t U_P_H = 0x21;
    const uint32_t U_P_V = '"';
    const uint32_t L_P_H = '=';
    const uint32_t L_P_V = 0x22;

    volatile uint32_t j = 0;
    char screen_buffer[12];

    void set_sign() {}

public:
    const uint16_t update_freq;
    std::string text;
    std::string dots;
    volatile bool active = true;
    TiDisplay(const uint16_t update_freq) : update_freq(update_freq) {}

    void show(const std::string &t)
    {
        text = "";
        dots = "";
        size_t i = 0;
        if (t[i] != '-' && t[i] != ' ')
        {
            text.push_back(' ');
        }

        for (; i < t.size() - 1; i++)
        {
            if (t[i] == '.')
            {
                dots.push_back(t[i]);
                if (t[i + 1] == '.')
                {
                    text.push_back(' ');
                }
            }
            else
            {
                text.push_back(t[i]);
                if (t[i + 1] != '.')
                {
                    dots.push_back(' ');
                }
            }
        }
        if (t[i] == '.')
        {
            dots.push_back(t[i]);
        }
        else
        {
            text.push_back(t[i]);
        }
    }

    void show(int num)
    {
        sprintf(screen_buffer, "%d", num);
        show(screen_buffer);
    }

    char &operator[](int index)
    {
        return text[index];
    }

    void update()
    {
        if (active)
        {

            DIGIT_PORT = (DIGIT_PORT & ~DIGIT_PORT_MASK) | (SCREEN_LEN - j);
            // DIGIT_PORT = (DIGIT_PORT & ~DIGIT_PORT_MASK) | j;
            SEGMENT_PORT = (SEGMENT_PORT & ~SEGMENT_PORT_MASK) | character_table[size_t(text[j])];
            if (dots[j] == '.')
            {
                SEGMENT_PORT |= DOT;
            }
            j = (j + 1) % text.size();
        }
    }

    void setup()
    {
        std::fill(character_table, character_table + 256, 0);

        character_table[' '] = 0;

        // TODO Corregir
        character_table[M_BAR] = 0b000000010;
        character_table[L_BAR] = 0b000001000;
        character_table[U_BAR] = 0b000001000;  // Upper score
        character_table[LL_BAR] = 0b000000010; // lower left bar
        character_table[LR_BAR] = 0b000000010; // lower right bar
        character_table['['] = 0b001001110;
        character_table[']'] = 0b001111000;

        character_table['|'] = 0b000000110;

        character_table[L_C] = 0b000001101;
        character_table[L_C_INV] = 0b000011001;
        character_table[U_C] = 0b001000011;
        character_table[U_C_INV] = 0b001100001;

        character_table[L_U] = 0b000011100;
        character_table[U_U] = 0b000100011;

        character_table[L_N] = 0b000010101;
        character_table[U_N] = 0b001100010;

        character_table[L_L1] = 0b000001100;
        character_table[L_L2] = 0b000011000;
        character_table[L_L3] = 0b000010001;
        character_table[L_L4] = 0b000000101;

        character_table[U_L1] = 0b000000011;
        character_table[U_L2] = 0b000100001;
        character_table[U_L3] = 0b001100000;
        character_table[U_L4] = 0b001000010;

        character_table[U_O] = 0b001100011;
        character_table[L_O] = 0b000011101;

        character_table[U_P_H] = 0b001000001;
        character_table[U_P_V] = 0b000100010;
        character_table[L_P_H] = 0b000010100;
        character_table[L_P_V] = 0b000001001;

        character_table['A'] = 0b001110111;
        character_table['B'] = 0b000011111;
        character_table['C'] = 0b001001110;
        character_table['D'] = 0b000111101;
        character_table['E'] = 0b001001111;
        character_table['F'] = 0b001000111;
        character_table['G'] = 0b001011110;
        character_table['H'] = 0b000110111;
        character_table['I'] = 0b000110111;
        character_table['J'] = 0b001111100;
        character_table['K'] = 0b000001111;
        character_table['L'] = 0b000001110;
        character_table['M'] = 0b000000000;
        character_table['N'] = 0b000010101;
        character_table['O'] = 0b001111101;
        character_table['P'] = 0b001100111;
        character_table['Q'] = 0b001110011;
        character_table['R'] = 0b000000101;
        character_table['S'] = 0b001011110;
        character_table['T'] = 0b000111001;
        character_table['U'] = 0b000111001;
        character_table['V'] = 0b000011100;
        character_table['W'] = 0b000000000;
        character_table['X'] = 0b000000000;
        character_table['Y'] = 0b000110011;
        character_table['Z'] = 0b000101111;

        character_table['0'] = 0b001111101;
        character_table['1'] = 0b001000001;
        character_table['2'] = 0b000101111;
        character_table['3'] = 0b001001111;
        character_table['4'] = 0b001010011;
        character_table['5'] = 0b001011110;
        character_table['6'] = 0b001111110;
        character_table['7'] = 0b001001001;
        character_table['8'] = 0b001111111;
        character_table['9'] = 0b001011111;

        // Init DIGIT ports
        pinMode(LED_BUILTIN, OUTPUT);
        for (uint32_t i = PA0; i < (PA0 + 4); i++)
        {
            pinMode(i, OUTPUT);
        }

        // Init SEGMENT ports
        pinMode(PB0, OUTPUT);
        pinMode(PB1, OUTPUT);
        for (uint32_t i = PB2; i < (PB2 + 7); i++)
        {
            pinMode(i, OUTPUT);
        }
        show("            ");
    }
};