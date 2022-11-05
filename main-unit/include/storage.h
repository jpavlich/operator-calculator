#pragma once
#include "config.h"
#include <SPI.h>
#define USE_SPI_LIB
#include <SD.h>

class Storage
{
private:
    Sd2Card card;
    SdVolume volume;
    SdFile root;

public:
    void setup()
    {
        if (card.init(SPI_HALF_SPEED, SDCARD_SS_PIN))
        {
            Serial.println("SD Card Ok");
        }
        else
        {
            Serial.println("SD Card Fail");
            while (true)
                ;
        }

        // print the type of card
        Serial.println();
        Serial.print("Card type:         ");
        switch (card.type())
        {
        case SD_CARD_TYPE_SD1:
            Serial.println("SD1");
            break;
        case SD_CARD_TYPE_SD2:
            Serial.println("SD2");
            break;
        case SD_CARD_TYPE_SDHC:
            Serial.println("SDHC");
            break;
        default:
            Serial.println("Unknown");
        }

        if (!volume.init(card))
        {
            Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
            while (true)
                ;
        }
    }

    void ls()
    {
        // FIXME
        // dac.active = false;
        // display.active = false;
        Serial.println("\nFiles found on the card (name, date and size in bytes): ");
        root.openRoot(volume);

        // list all files in the card with date and size
        root.ls(LS_DATE | LS_SIZE);

        // dac.active = true;
        // display.active = true;
    }
};