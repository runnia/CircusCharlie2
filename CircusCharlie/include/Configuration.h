#pragma once
#include "Arduino.h"
#include "EEPROM.h"
#include "GameRenderer.h"
#include "GameManager.h"

class Configuration
{
public:
    uint8_t records[3];
    uint8_t speed;
    uint8_t color; // 0 = blue 1 = green 2 =yellow
    // uint8_t size;
    //char name[] = "AAA";
    // char RecordName1[] = "AAA";
    // char RecordName2[] = "AAA";
    // char RecordName3[] = "AAA";

    Configuration()
    {
        // считать настройки и рекорды из энергонезависимой памяти
        speed = EEPROM.read(0);
        color = EEPROM.read(16);

        for (int i = 0; i < 3; ++i)
            records[i] = EEPROM.read(i + 1);
    };

    void SetNewSpeed(const uint8_t newSpeed)
    {
        // устанавливаем новую скорость игры
        // сохраняем в энергонезависимую память

        speed = newSpeed;
        EEPROM.write(0, newSpeed);
    };

    void SetNewColor(const uint8_t newColor)
    {
        // устанавливаем новый масштаб игры
        // сохраняем в энергонезависимую память

        color = newColor;
        EEPROM.write(16, newColor);
    };


    void TrySetNewRecord(const uint8_t newRecord, char* name)
    {
        // проверяем превосходит ли новый рекорд сохраненные
        // если да, то перезаписываем

        if (newRecord > records[0])
        {
            records[2] = records[1];
            records[1] = records[0];
            records[0] = newRecord;
            for (int i = 0; i < 3; ++i)
                EEPROM.write(i + 1, records[i]);

            // for (int i = 10; i < 13; ++i)
            //     EEPROM.write(i, EEPROM[i-3]);
            // for (int i = 7; i < 10; ++i)
            //     EEPROM.write(i, EEPROM[i-3]);    
            for (int i = 4; i < 7; ++i)
                EEPROM.write(i, name[i - 4]);
        }
        else if (newRecord > records[1])
        {
            records[2] = records[1];
            records[1] = newRecord;
            for (int i = 1; i < 3; ++i)
                EEPROM.write(i + 1, records[i]);
        //     for (int i = 10; i < 13; ++i)
        //         EEPROM.write(i, EEPROM[i-3]);
        //     for (int i = 7; i < 10; ++i)
        //         EEPROM.write(i, name[i - 4]);     
        }
        else if (newRecord > records[2])
        {
            records[2] = newRecord;
            EEPROM.write(3, records[2]);
            // for (int i = 10; i < 13; ++i)
            //     EEPROM.write(i, name[i - 4]);
        }
    }
};