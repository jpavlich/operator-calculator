#pragma once
#include <cstdint>
#include <cstdlib>
#include <Arduino.h>

const auto DAC_BCK_PIN = PB13;
const auto DAC_WS_PIN = PB12;
const auto DAC_DIN_PIN = PB15;

const auto SDCARD_SS_PIN = A4;
const auto SDCARD_SCK_PIN = A5;
const auto SDCARD_MISO_PIN = A6;
const auto SDCARD_MOSI_PIN = A7;

const size_t NUM_SAMPLES = 2048;
const size_t BIT_DEPTH = 0xFFFF;

const uint16_t DAC_UPDATE_FREQ = 56320;
const uint16_t DISPLAY_UPDATE_FREQ = 65535;
const uint16_t KEYPAD_UPDATE_FREQ = 128;

#define KEYPAD_TIMER TIM3
#define DISPLAY_TIMER TIM4
#define DAC_TIMER TIM5
