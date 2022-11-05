#include <Arduino.h>
#include <Keypad.h>


static const char NUL = 0x00;
static const char STX = 0x02; // Start of text
static const char US = 0x1F;  // Unit separator


const byte ROWS = 9; // four rows
const byte COLS = 5; // three columns
char keys[ROWS][COLS];
//  = {
//     {'a', 'b', 'c', 'd', 'e'},
//     {'f', 'g', 'h', 'i', 'j'},
//     {'k', 'l', 'm', 'n', 'o'},
//     {'p', 'q', 'r', 's', 't'},
//     {'u', 'v', 'w', 'x', 'y'},
//     {'z', 'A', 'B', 'C', 'D'},
//     {'E', 'F', 'G', 'H', 'I'},
//     {'J', 'K', 'L', 'M', 'N'},
//     {'O', 'P', 'Q', 'R', 'S'},
// };

void init_key_table()
{
  char c = 0x20;
  for (size_t i = 0; i < ROWS; i++)
  {
    for (size_t j = 0; j < COLS; j++)
    {
      keys[i][j] = c;
      c++;
    }
  }
}

byte rowPins[ROWS] = {15, 7, 4, 10, 9, 12, 16, 3, 6}; // connect to the row pinouts of the kpd
byte colPins[COLS] = {2, 5, 8, 11, 14};               // connect to the column pinouts of the kpd

Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup()
{
  init_key_table();

  Serial.begin(115200);
  Serial.write(STX);
  Serial.write(NUL);
  Serial.write(US);
}

void loop()
{

  if (kpd.getKeys())
  {
    for (int i = 0; i < LIST_MAX; i++)
    {
      if (kpd.key[i].stateChanged)
      {
        switch (kpd.key[i].kstate)
        {
        case PRESSED:
          Serial.write(0x20);
          break;
        case HOLD:
          Serial.write(0x21);
          break;
        case RELEASED:
          Serial.write(0x22);
          break;
        case IDLE:
          Serial.write(0x23);
        }
        Serial.write(kpd.key[i].kchar);
        Serial.write(US);
      }
    }
  }
} 