#pragma once
#include "Arduino.h"

#define LEVEL_SIZE 16
//extern unsigned short charlie[368];
//extern const unsigned short charlie1[368];
extern int color;
class GameLogic
{
private:
    // 0 - на земле, 1,2 - в воздухе, 3 - в воздухе падает (в слудющем кадре упадет на землю)
    uint8_t _charlieState;
    uint8_t _score;
    uint8_t _lives;
    uint8_t _level[LEVEL_SIZE]; //hoop
    uint8_t _level2[LEVEL_SIZE]; //obstacle
    uint8_t _obstacles_state[LEVEL_SIZE]; 

    void GenerateNewHoop()
    {
        // находим позицию крайнего препятсвия
        if(_level[15] != 0 || _level[14] != 0 || _level[13] != 0 || _level[12] != 0 || _level[11] != 0)
            return;

        // если есть место для нового препятсвия => пытаемся его сгенерить
        if (random(100) % 2 == 1)
            _level[LEVEL_SIZE - 1] = 1;
    };

    void GenerateNewObstacle()
    {
        // находим позицию крайнего препятсвия
        if(_level2[15] != 0 || _level2[14] != 0 || _level2[13] != 0 || _level2[12] != 0 || _level2[11] != 0 || _level2[10] != 0
                                                                    || _level2[9] != 0|| _level2[8] != 0 )
            return;

        // если есть место для нового препятсвия => пытаемся его сгенерить
        if (random(100) % 2 == 1)
            _level2[LEVEL_SIZE - 1] = 1;
    };
    
    void ChangeObstaclesState()
    {
        for (int i= 0; i < 16; i++)
        {
            if (_obstacles_state[i] == 1)
            {
                _obstacles_state[i] = 0;
            }
            else
            {
                _obstacles_state[i] = 1;
            }
            
        }
    };

    // проверка того что игра еще не закончена
    // bool IsEnd(uint8_t lives)
    // {   
    //     // if (lives == 3 && lives == 2 && lives == 1)
    //     //return true;
    //     // if (lives == 0)
    //     //     return false;
    // };

public:
    GameLogic(){};
    // задает начальные значения для перемнных и обнуляет уровень
    void Start()
    {
        _charlieState = 0;
        _score = 0;
        _lives = 3;

        for (int i = 0; i < LEVEL_SIZE; ++i)
        {
            _level[i] = 0;
            _level2[i] = 0;
            _obstacles_state[i] = 0;
        }
        GenerateNewHoop();
        GenerateNewObstacle();
        
    };

    // принимает данные о том, был ввод или нет
    // возвращает true если игра продолжается, false, если игра прекращена
    bool Update(bool input)
    {
        // если ввод был, то перемещаем чарли в воздух (если он уже не находится в воздухе)
        if (input)
            if (_charlieState == 0)
                _charlieState = 1;
            else
                _charlieState = (_charlieState + 1) % 4;
        else if (_charlieState != 0)
            _charlieState = (_charlieState + 1) % 4;

        for (int i = 1; i < LEVEL_SIZE; ++i)
        {
            _level[i - 1] = _level[i];
            _level2[i - 1] = _level2[i];
        }
        _level[LEVEL_SIZE - 1] = 0;
        _level2[LEVEL_SIZE - 1] = 0;

        if (_level[2] == 1 && _charlieState != 0)
            _score++;
        if(_charlieState == 0 && _level2[2] == 1)
            _lives--;

        GenerateNewHoop();
        GenerateNewObstacle();
        ChangeObstaclesState();
        if (_lives)
            return true;
        else
            return false;
        
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
    uint8_t *GetHoop()
    {
        return _level;
    }
    uint8_t *GetLevel()
    {
        return _level2;
    }
    uint8_t *GetObstaclesState()
    {
        return _obstacles_state;
    }
    // возвращает количество жизней персонажа 3,2,1 или 0
    uint8_t GetLives()
    {
        return _lives;
    }
};