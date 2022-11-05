#include <Arduino.h>
#include <STM32TimerInterrupt.h>

#include "config.h"
#include "ti_display.h"
#include "dac.h"
#include "keypad.h"
#include "storage.h"

STM32Timer timer_dac(DAC_TIMER);
STM32Timer timer_display(DISPLAY_TIMER);
STM32Timer timer_keypad(KEYPAD_TIMER);

TiDisplay display(DISPLAY_UPDATE_FREQ);
DAC dac(NUM_SAMPLES, DAC_UPDATE_FREQ, DAC_BCK_PIN, DAC_WS_PIN, DAC_DIN_PIN);
Keypad keypad(KEYPAD_UPDATE_FREQ);
Storage storage;

void dac_handler()
{
  dac.update();
}

void display_handler()
{
  display.update();
}

void keypad_handler()
{
  keypad.update();
}

void setup_storage()
{
  storage.setup();
  storage.ls();
}

void setup_dac()
{
  dac.setup();
  if (
      timer_dac.attachInterrupt(dac.update_freq, dac_handler))
  {
    Serial.println("DAC Ok");
  }
  else
  {
    Serial.println("DAC Fail");
  }
}
void setup_display()
{
  display.setup();
  if (
      timer_display.attachInterrupt(display.update_freq, display_handler))
  {
    Serial.println("Display Ok");
  }
  else
  {
    Serial.println("Display Fail");
  }
  display.show("READY       ");
}

void setup_keypad()
{
  keypad.setup();
  keypad.on(Keypad::EventType::PRESS,
            [&](const Keypad::Event &evt)
            {
              Serial.println(evt.data, HEX);
              dac.set_note(evt.data - 0x20 + 24);
              display.show(evt.data - 0x20);
              // display[1] = evt.data + 0x0F;
            });

  if (
      timer_keypad.attachInterrupt(keypad.update_freq, keypad_handler))
  {
    Serial.println("Keypad Ok");
  }
  else
  {
    Serial.println("Keypad Fail");
  }
}

void setup()
{

  delay(500);
  Serial.begin(115200);

  ///////////////////////////
  setup_storage();
  delay(100);
  setup_dac();
  delay(100);
  setup_display();
  delay(100);
  setup_keypad();
}

void loop()
{
  // keypad.update();
}