#pragma once
#include "Arduino.h"
#include "EEPROM.h"

class Configuration
{
public:
    uint8_t records[3];
    uint8_t speed;

    Configuration()
    {
        // считать настройки и рекорды из энергонезависимой памяти
        speed = EEPROM.read(0);

        for (int i = 0; i < 3; ++i)
            records[i] = EEPROM.read(i + 1);
    };

    void SetNewSpeed(const uint8_t newSpeed)
    {
        // устанавливает новую скорость игры
        // сохраняем ее в эн память

        speed = newSpeed;
        EEPROM.write(0, newSpeed);
    };

    void TrySetNewRecord(const uint8_t newRecord)
    {
        // проверяем превосходит ли новый рекорд сохраненные
        // да => перезаписываем

        if (newRecord > records[0])
        {
            records[2] = records[1];
            records[1] = records[0];
            records[0] = newRecord;

            for (int i = 0; i < 3; ++i)
                EEPROM.write(i + 1, records[i]);
        }
    }
};