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
    uint8_t size;
    //char name[] = "AAA";
    char RecordName1[] = "AAA";
    char RecordName2[] = "AAA";
    char RecordName3[] = "AAA";

    Configuration()
    {
        // считать настройки и рекорды из энергонезависимой памяти
        speed = EEPROM.read(0);
        size = EEPROM.read(16);

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

    void SetNewSize(const uint8_t newSize)
    {
        // устанавливаем новый масштаб игры
        // сохраняем в энергонезависимую память

        size = newSize;
        EEPROM.write(16, newSize);
    };



    void TrySetNewRecord(const uint8_t newRecord)
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


            // for (int i = 4; i < 7; ++i)
                //EEPROM.write(i, name[i - 4]);
        }
        else if (newRecord > records[1])
        {
            records[2] = records[1];
            records[1] = newRecord;
            for (int i = 1; i < 3; ++i)
                EEPROM.write(i + 1, records[i]);
        }
        else if (newRecord > records[2])
        {
            records[2] = newRecord;
            EEPROM.write(3, records[2]);
        }
    }
};