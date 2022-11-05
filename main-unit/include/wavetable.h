#pragma once
#include "config.h"
#include <Arduino.h>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <limits>
#include <algorithm>

typedef std::function<float(float)> WaveFun;

void gen_table(int16_t *table, WaveFun f, float x_min, float x_max, size_t num_samples = NUM_SAMPLES, uint16_t bit_depth = BIT_DEPTH)
{
    float x_step = (x_max - x_min) / num_samples;

    // float y_min = std::numeric_limits<float>::max();
    float y_max = std::numeric_limits<float>::min();

    // Finds y_min and y_max
    float x = x_min;
    for (size_t i = 0; i < num_samples; i++)
    {
        // y_min = std::min(y_min, f(x));
        y_max = std::max(y_max, std::abs(f(x)));
        x += x_step;
    }
    Serial.print("y min max");
    // Serial.println(y_min);
    Serial.println(y_max);

    x = x_min;
    float y_step = (float)(bit_depth >> 1) / y_max;
    Serial.println(y_step, 10);
    for (size_t i = 0; i < num_samples; i++)
    {
        table[i] = f(x) * y_step;
        // Serial.println(f(x) * y_step, 10);
        x += x_step;
    }
}