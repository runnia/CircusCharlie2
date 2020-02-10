#pragma once
#include "GameLogic.h"
#include "GameRenderer.h"
#include "Buttons.h"
#include "Configuration.h"

// если ввод имеет это значение, значит его не было
#define INVALID_INPUT 400

enum GameState
{
    InMenu,
    InSettings,
    InRecords,
    InGame,
    InEndGame,
};

class GameManager
{
private:
    GameState _state;       // состояние игры в данный момент
    GameLogic _logic;       // компонент игровой логики
    GameRenderer _renderer; // копонент рендера
    Configuration _config;  // настройки игры
    uint8_t _ticks;         // кол-во тиков прошедших с предыдущего обновления игры
    bool _input;

public:
    GameManager(UTFT &glcd) : _logic(), _renderer(glcd), _config(){};

    void InitGameManager()
    {
        // устанаваливаем начальное состояние игры
        _state = InMenu;
        // отрисовываем меню
        _renderer.RenderMenu();
 
    }

    void Tick(uint16_t &x, uint16_t &y)
    {
        // принимаемые параметры - координаты точки в которую мы нажали
        // если мы в меню или настройках нам нужно знать куда мы нажали, что обработать эту логику
        // если мы в игре, то счетчик нужен для того, чтобы обновить игру раз в 5 тиков
        // т.е. у нас происходит 2 кадра (обновления) в секунду, когда мы играем
        _ticks++;

        if (!(x == INVALID_INPUT && y == INVALID_INPUT))
        {

            switch (_state)
            {
            case InMenu:
                // проверка на нажатие кнопок игрового меню и выполнение соответствующих обновлений игры
                if (x > START_GAME_BUTTON_MIN_X && x < START_GAME_BUTTON_MAX_X && y > START_GAME_BUTTON_MIN_Y && y < START_GAME_BUTTON_MAX_Y)
                {
                    _state = InGame;
                    _logic.Start();
                    _renderer.PrerenderGame(_config.color);
                    //_renderer.RenderGame(_logic.GetCharlieState(), _logic.GetLevel(), _logic.GetPoints(),_config.size);
                    _renderer.RenderGame(_logic.GetCharlieState(), _logic.GetLevel(), _logic.GetPoints(),_config.color);
                }

                if (x > SETTINGS_BUTTON_MIN_X && x < SETTINGS_BUTTON_MAX_X && y > SETTINGS_BUTTON_MIN_Y && y < SETTINGS_BUTTON_MAX_Y)
                {
                    _state = InSettings;
                    _renderer.RenderSettings(_config);
                }
                

                if (x > RECORDS_BUTTON_MIN_X && x < RECORDS_BUTTON_MAX_X && y > RECORDS_BUTTON_MIN_Y && y < RECORDS_BUTTON_MAX_Y)
                {
                    _state = InRecords;
                    _renderer.RenderRecords(_config);
                    // _state = InEndGame;
                    //_renderer.RenderEndGame(_config,_logic.GetPoints());
                    // _renderer.RenderEnterName();
                }
                break;

            case InSettings:
                // проверяем не нажал ли пользователь на увеличение или уменьшение скорости игры
                // проверка на нажатие кнопок меню настроек
                if(x > SETTINGS_BACK_TO_MENU_BUTTON_MIN_X && x < SETTINGS_BACK_TO_MENU_BUTTON_MAX_X &&
                 y > SETTINGS_BACK_TO_MENU_BUTTON_MIN_Y && y < SETTINGS_BACK_TO_MENU_BUTTON_MAX_Y)
                { 
                    _state = InMenu;
                    _renderer.RenderMenu();
                }
                // проверка того что пользователь нажимает на кнопку и что увелчение/уменьшение скорости возможно
                if (x > SETTINGS_MINUS_SPEED_BUTTON_MIN_X && x < SETTINGS_MINUS_SPEED_BUTTON_MAX_X &&
                 y > SETTINGS_MINUS_SPEED_BUTTON_MIN_Y && y < SETTINGS_MINUS_SPEED_BUTTON_MAX_Y && _config.speed >1)
                 {
                    _config.SetNewSpeed(_config.speed-1);
                    _renderer.RenderSettings(_config);
                 }
                if (x > SETTINGS_PLUS_SPEED_BUTTON_MIN_X && x < SETTINGS_PLUS_SPEED_BUTTON_MAX_X &&
                 y > SETTINGS_PLUS_SPEED_BUTTON_MIN_Y && y < SETTINGS_PLUS_SPEED_BUTTON_MAX_Y && _config.speed < 3)
                 {
                    _config.SetNewSpeed(_config.speed+1);
                    _renderer.RenderSettings(_config);
                 }
                // проверка того что пользователь нажимает на кнопку и что изменение цвета возможно
                if (x > SETTINGS_MINUS_SIZE_BUTTON_MIN_X && x < SETTINGS_MINUS_SIZE_BUTTON_MAX_X &&
                y > SETTINGS_MINUS_SIZE_BUTTON_MIN_Y && y < SETTINGS_MINUS_SPEED_BUTTON_MAX_Y && _config.color > 1)
                {
                    _config.SetNewColor(_config.color-1);
                    _renderer.RenderSettings(_config);
                }
                if (x > SETTINGS_PLUS_SIZE_BUTTON_MIN_X && x < SETTINGS_PLUS_SIZE_BUTTON_MAX_X &&
                y > SETTINGS_PLUS_SIZE_BUTTON_MIN_Y && y < SETTINGS_PLUS_SPEED_BUTTON_MAX_Y && _config.color < 3)
                {
                    _config.SetNewColor(_config.color+1);
                    _renderer.RenderSettings(_config);
                }
                 
                break;
            case InRecords:
                // проверяем нажатие книпки выхода
                if(x > SETTINGS_BACK_TO_MENU_BUTTON_MIN_X && x < SETTINGS_BACK_TO_MENU_BUTTON_MAX_X &&
                 y > SETTINGS_BACK_TO_MENU_BUTTON_MIN_Y && y < SETTINGS_BACK_TO_MENU_BUTTON_MAX_Y)
                { 
                    _state = InMenu;
                    _renderer.RenderMenu();
                }
                break;
            case InGame:
                // проверка на выход из игры
                // сохранение данных о вводе к следующему обновлению игры (если ввод валидный)
                // проверка не нужно ли обновлять состояние игры
                // если нужно обновлять => обновляем на хранимом в данный момент вводе
                // Serial.print("In game pressed");
                _input = true;
                if (x > IN_GAME_BACK_TO_MENU_BUTTON_MIN_X && x < IN_GAME_BACK_TO_MENU_BUTTON_MAX_X &&
                 y > IN_GAME_BACK_TO_MENU_BUTTON_MIN_Y && y < IN_GAME_BACK_TO_MENU_BUTTON_MAX_Y)
                 {
                    _state = InMenu;
                    _renderer.RenderMenu();
                 }

                break;
            case InEndGame:
                // когда игрок проигрывает
                // _renderer.RenderEndGame(_config);
                if (x > CHANGE_NAME_BUTTON_1_UP_MIN_X && x < CHANGE_NAME_BUTTON_1_UP_MAX_X &&
                 y > CHANGE_NAME_BUTTON_1_UP_MIN_Y && y < CHANGE_NAME_BUTTON_1_UP_MAX_Y)
                {
                        number_of_first_letter = (number_of_first_letter - 1 < 0) ? 25 : number_of_first_letter;
                        _renderer.RenderEnterName();
                }
                if (x > CHANGE_NAME_BUTTON_1_DOWN_MIN_X && x < CHANGE_NAME_BUTTON_1_DOWN_MAX_X &&
                 y > CHANGE_NAME_BUTTON_1_DOWN_MIN_Y && y < CHANGE_NAME_BUTTON_1_DOWN_MAX_Y)
                 {
                    number_of_first_letter = (number_of_first_letter + 1 > 25) ? 0 : number_of_first_letter;
                    _renderer.RenderEnterName();
                 }
                    
                if (x > CHANGE_NAME_BUTTON_2_UP_MIN_X && x < CHANGE_NAME_BUTTON_2_UP_MAX_X &&
                 y > CHANGE_NAME_BUTTON_2_UP_MIN_Y && y < CHANGE_NAME_BUTTON_2_UP_MAX_Y)
                 {  
                    number_of_second_letter = (number_of_second_letter - 1) % 25;
                    _renderer.RenderEnterName();
                 }
                if (x > CHANGE_NAME_BUTTON_2_DOWN_MIN_X && x < CHANGE_NAME_BUTTON_2_DOWN_MAX_X &&
                 y > CHANGE_NAME_BUTTON_2_DOWN_MIN_Y && y < CHANGE_NAME_BUTTON_2_DOWN_MAX_Y)
                 {
                    number_of_second_letter = (number_of_second_letter + 1) % 25;
                    _renderer.RenderEnterName();
                 }
                
                if (x > CHANGE_NAME_BUTTON_3_UP_MIN_X && x < CHANGE_NAME_BUTTON_3_UP_MAX_X &&
                 y > CHANGE_NAME_BUTTON_3_UP_MIN_Y && y < CHANGE_NAME_BUTTON_3_UP_MAX_Y)
                 {
                    number_of_third_letter = (number_of_third_letter - 1) % 25;
                    _renderer.RenderEnterName();
                 }
                if (x > CHANGE_NAME_BUTTON_3_DOWN_MIN_X && x < CHANGE_NAME_BUTTON_3_DOWN_MAX_X &&
                 y > CHANGE_NAME_BUTTON_3_DOWN_MIN_Y && y < CHANGE_NAME_BUTTON_3_DOWN_MAX_Y)
                 {
                    number_of_third_letter = (number_of_third_letter + 1) % 25;
                    _renderer.RenderEnterName();
                 }
                if(x > SETTINGS_BACK_TO_MENU_BUTTON_MIN_X && x < SETTINGS_BACK_TO_MENU_BUTTON_MAX_X &&
                 y > SETTINGS_BACK_TO_MENU_BUTTON_MIN_Y && y < SETTINGS_BACK_TO_MENU_BUTTON_MAX_Y)
                 {  
                    name[0] = alphabet[number_of_first_letter];
                    name[1] = alphabet[number_of_second_letter];
                    name[2] = alphabet[number_of_third_letter];
                    _state = InMenu;
                    _renderer.RenderMenu();
                 }
                 if (!( _logic.GetPoints()> _config.records[2]))
                 {
                    _state = InMenu;
                    _renderer.RenderMenu();
                 }
            }
        }

        if (_ticks % (6 - _config.speed) == 0 && _state == InGame)
        {
            Serial.println("Update game");
            if (_logic.Update(_input))
            {
                _renderer.RenderGame(_logic.GetCharlieState(), _logic.GetLevel(), _logic.GetPoints(),_config.color);
            }
            else
            {
                _state = InEndGame;
                // _state = InMenu;
                _config.TrySetNewRecord(_logic.GetPoints(), name);
                // _renderer.RenderMenu();
                _renderer.RenderEndGame(_config, _logic.GetPoints());
            }
            _input = false;
        }
    };
};