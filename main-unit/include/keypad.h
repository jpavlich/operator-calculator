#pragma once
#include <Arduino.h>
#include <functional>

static const char NUL = 0x00;
static const char STX = 0x02; // Start of text
static const char US = 0x1F;  // Unit separator

class Keypad
{
public:
    const uint16_t update_freq;
    enum class EventType : uint8_t
    {
        READY = STX,
        PRESS = 0x20,
        HOLD = 0x21,
        RELEASE = 0x22,
        IDLE = 0x23,
    };

    struct Event
    {
        EventType type;
        uint8_t data;

        uint8_t key_index()
        {
            return data - 0x20; 
        }
    };

private:
    Event event;

    typedef std::function<void(const Event &)> KeyEventCallback;

    uint8_t evt_index(EventType type)
    {
        return (uint8_t)type - 0x20; // FIXME EventType::READY genera un código incorrecto (STX - 0x20)
    }
    KeyEventCallback callbacks[5];

public:
    Keypad(const uint16_t update_freq) : update_freq(update_freq) {}

    void setup()
    {
        for (int i = 0; i < 5; i++)
        {
            callbacks[i] = [i](const Event &evt)
            {
                Serial.print(i);
                Serial.print(" ");
                Serial.println(evt.data, HEX);
            };
        }
    }

    void update()
    {
        if (Serial.available() > 0)
        {
            if (Serial.readBytesUntil(US, (uint8_t *)&event, 2) == 2)
            {
                callbacks[evt_index(event.type)](event);
            }
        }
    }

    void on(EventType eventType, KeyEventCallback callback)
    {
        auto i = evt_index(eventType);
        callbacks[i] = callback;
    }
};
