#pragma once
#include "config.h"
#include <Arduino.h>
#include <SPI.h>
#include <cmath>
#include "wavetable.h"

class DAC
{
private:
    SPIClass spi;

    uint16_t hData, lData;
    uint16_t x;
    uint16_t dx = 1;
    uint16_t sine_table[NUM_SAMPLES];
    const uint16_t num_samples;
    const uint32_t bck_pin;
    const uint32_t ws_pin;
    const uint32_t din_pin;
    const uint16_t scaler_bits;
    const uint16_t N;

public:
    const uint16_t update_freq;
    volatile bool active = true;
    DAC(
        const uint16_t num_samples,
        const uint16_t update_freq,
        const uint32_t bck_pin,
        const uint32_t ws_pin,
        const uint32_t din_pin,
        const uint16_t scaler_bits = 4) :

                                          spi(SPIClass(din_pin, PB14, bck_pin)),
                                          num_samples(num_samples),
                                          update_freq(update_freq),
                                          bck_pin(bck_pin),
                                          ws_pin(ws_pin),
                                          din_pin(din_pin),
                                          N(num_samples << scaler_bits),
                                          scaler_bits(scaler_bits)

    {
    }
    void setup()
    {
        gen_table((int16_t *)sine_table, &sinf,
                  0, 2 * PI);

        delay(100);
        spi.begin();                // Initialize the SPI_1 port.
        spi.setBitOrder(MSBFIRST);  // Set the SPI_1 bit order
        spi.setDataMode(SPI_MODE0); // Set the SPI_2 data mode 0
        spi.setClockDivider(1);     // Slow speed (72 / 16 = 4.5 MHz SPI_1 speed)
        pinMode(ws_pin, OUTPUT);    // Set the Word Select pin (ws_pin) as output.
        Serial.println("ok");
    }

    // TODO DMA
    void update()
    {
        if (active)
        {
            digitalWrite(ws_pin, LOW);                       // Select RIGHT Audio channel
            spi.transfer(sine_table[x >> scaler_bits] >> 8); // Data bits 15-8
            spi.transfer(sine_table[x >> scaler_bits]);      // Data bits 7-0

            digitalWrite(ws_pin, HIGH);                      // Select LEFT Audio channel
            spi.transfer(sine_table[x >> scaler_bits] >> 8); // Data bits 15-8
            spi.transfer(sine_table[x >> scaler_bits]);      // Data bits 7-0
            x = (x + dx) % N;
        }
    }
    void set_frequency(float freq)
    {
        dx = (freq * (float)N / (float)update_freq);
    }

    void set_note(int note)
    {
        // First note is A0 = 27.5 Hz
        float freq = 27.5 * pow(2, note / 12.0);
        set_frequency(freq);
    }
};