#pragma once
#include "Arduino.h"

#define LEVEL_SIZE 16

class GameLogic
{
private:
    // 0 - на земле, 1,2 - в воздухе, 3 - в воздухе падает (в слудющем кадре упадет на землю)
    uint8_t _charlieState;
    uint8_t _score;
    uint8_t _level[LEVEL_SIZE];

    void GenerateNewObstacle()
    {
        // находим позицию крайнего препятсвия
        if(_level[15] != 0 || _level[14] != 0 || _level[13] != 0 || _level[12] != 0)
            return;

        // если есть место для нового препятсвия => пытаемся его сгенерить
        if (random(100) % 2 == 1)
            _level[LEVEL_SIZE - 1] = 1;
    };
    
    // проверка того что игра еще не закончена
    bool IsEnd()
    {
        return _charlieState == 0 && _level[2] == 1;
    };

public:
    GameLogic(){};
    // задает начальные значения для перемнных и обнуляет уровень
    void Start()
    {
        _charlieState = 0;
        _score = 0;

        for (int i = 0; i < LEVEL_SIZE; ++i)
            _level[i] = 0;

        GenerateNewObstacle();
    };

    // принимает данные о том, был ввод или нет
    // возвращает true если игра продолжается, false, если игра прекращена
    bool Update(bool input)
    {
        // если ввод был, то перемещаем чарли в воздух (если он уже не находится в воздухе)
        for (int i = 0; i < LEVEL_SIZE; ++i)
        {
            Serial.print(_level[i]);
            Serial.print(" ");
        }

        if (input)
            if (_charlieState == 0)
                _charlieState = 1;
            else
                _charlieState = (_charlieState + 1) % 4;
        else if (_charlieState != 0)
            _charlieState = (_charlieState + 1) % 4;

        for (int i = 1; i < LEVEL_SIZE; ++i)
            _level[i - 1] = _level[i];

        _level[LEVEL_SIZE - 1] = 0;

        if (_level[2] == 1 && _charlieState != 0)
            _score++;

        GenerateNewObstacle();

        return !IsEnd();
    };

    // возвращает состояние Чарли в данный момент
    uint8_t GetCharlieState()
    {
        return _charlieState;
    }
    // возвращает счет игрока в данный момент
    uint8_t GetPoints()
    {
        return _score;
    }
    // возвращает состояние уровня в данный момент (состоит из ноликов и единичек, 1 - препятствие, 0 - его отсутствие)
    uint8_t *GetLevel()
    {
        return _level;
    }
};