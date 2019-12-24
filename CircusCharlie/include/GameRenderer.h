#pragma once
#include "UTFT.h"
#include "Buttons.h"
#include "Configuration.h"

//подключение шрифтов из библиотеки
extern uint8_t SmallFont[]; 
extern uint8_t BigFont[];
extern uint8_t SevenSegNumFont[];
// алфавит для ввода имени игрока

char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char name[] = "AAA";
int number_of_first_letter = 0;
int number_of_second_letter = 0;
int number_of_third_letter = 0;

class GameRenderer
{
private:
    UTFT _glcd;

public:
    GameRenderer(UTFT &glcd)
    {
        _glcd = glcd;
    };

    void RenderMenu()
    {
        _glcd.clrScr();
        _glcd.setFont(BigFont);

        // отрисовка заголовка
        _glcd.setBackColor(VGA_BLACK);
        _glcd.setColor(VGA_WHITE);
        _glcd.print("Circus Charlie", 50, 270, 270);

        // отрисовка кнопки начала игры
        _glcd.setBackColor(20, 115, 40);
        _glcd.setColor(20, 115, 40);
        _glcd.fillRect(START_GAME_BUTTON_MIN_X, START_GAME_BUTTON_MIN_Y, START_GAME_BUTTON_MAX_X, START_GAME_BUTTON_MAX_Y);
        _glcd.setColor(VGA_WHITE);
        _glcd.print("START", 102, 200, 270);

        // отрисовка кнопки настроек
        _glcd.setColor(20, 115, 40);
        _glcd.fillRect(SETTINGS_BUTTON_MIN_X, SETTINGS_BUTTON_MIN_Y, SETTINGS_BUTTON_MAX_X, SETTINGS_BUTTON_MAX_Y);
        _glcd.setColor(VGA_WHITE);
        _glcd.print("SETTINGS", 152, 220, 270);

        // отрисовка кнопки рекордов
        _glcd.setColor(20, 115, 40);
        _glcd.fillRect(RECORDS_BUTTON_MIN_X, RECORDS_BUTTON_MIN_Y, RECORDS_BUTTON_MAX_X, RECORDS_BUTTON_MAX_Y);
        _glcd.setColor(VGA_WHITE);
        _glcd.print("RECORDS", 202, 215, 270);
    }

    void PrerenderGame()
    {
        _glcd.clrScr();

        _glcd.setColor(VGA_GREEN);
        _glcd.fillRect(200, 320, 240, 0);

        _glcd.setColor(VGA_TEAL);
        _glcd.drawLine(30, 320, 30, 0);

        _glcd.setBackColor(VGA_BLACK);
        _glcd.setColor(VGA_WHITE);
        _glcd.print("Score: ",10, 310, 270);
        _glcd.print("< MENU",10, 100, 270);
    }

    void RenderGame(const uint8_t &charlieState, const uint8_t *obstacles, const uint8_t &score)
    {
        // отрисовка персонажа
        if (charlieState == 0)
        {
            _glcd.setColor(VGA_BLACK);
            _glcd.fillRect(160, 260, 180, 280);
            _glcd.setColor(VGA_RED);
            _glcd.fillRect(180, 260, 200, 280);
        }
        else
        {
            if (charlieState == 1)
            {
                _glcd.setColor(VGA_BLACK);
                _glcd.fillRect(180, 260, 200, 280);
            }
            _glcd.setColor(VGA_RED);
            _glcd.fillRect(160, 260, 180, 280);
        }
        // отрисовка препятствий 
        for (int i = 0; i < 16; ++i)
        {
            if (obstacles[i] == 1)
            {
                _glcd.setColor(VGA_GREEN);
                _glcd.fillRect(180, 320 - 20 * i, 200, 300 - 20 * i);
                _glcd.setColor(VGA_BLACK);
                _glcd.fillRect(180, 320 - 20 * (i + 1), 200, 300 - 20 * (i + 1));
            }
        }

        if(obstacles[0] == 0)
        {
            _glcd.setColor(VGA_BLACK);
            _glcd.fillRect(180, 320, 200, 300);
        }

        if (charlieState == 0)
        {
            _glcd.setColor(VGA_RED);
            _glcd.fillRect(180, 260, 200, 280);
        }

        // отрисовка текущего счета
        _glcd.setBackColor(VGA_BLACK);
        _glcd.setColor(VGA_WHITE);
        _glcd.setFont(SmallFont);
        String stringScore = String(score);
        _glcd.print(stringScore, 12, 210, 270);
    }

    void RenderSettings(Configuration &config)
    {
        _glcd.clrScr();
        _glcd.setColor(VGA_WHITE);
        _glcd.setBackColor(VGA_BLACK);
        _glcd.setFont(BigFont);
        _glcd.print("Speed", 50, 200 , 270);

        //кнопочки для увеличения и уменьшения скорости
        if (config.speed > 1)
            _glcd.fillRect(100, 285, 140, 245);
       
        if (config.speed < 3)
            _glcd.fillRect(100, 75, 140, 35);

        String stringSpeed = String(config.speed);
        _glcd.print(stringSpeed, 115, 170, 270);

        _glcd.setColor(VGA_BLACK);
   
        if (config.speed > 1)
            _glcd.fillRect(115, 280, 125, 250);
       
        if (config.speed < 3)
        {
            _glcd.fillRect(115, 70, 125, 40);
            _glcd.fillRect(105, 60, 135, 50);
        }

        _glcd.setColor(20, 115, 40);
        _glcd.fillRect(190, 35, 230, 285);
        _glcd.setColor(VGA_WHITE);
        _glcd.setBackColor(20, 115, 40);
        _glcd.print("BACK", 202,190, 270 );
    }

    void RenderRecords(Configuration &config)
    {
        _glcd.clrScr();
        _glcd.setColor(VGA_WHITE);
        _glcd.setBackColor(VGA_BLACK);
        _glcd.setFont(BigFont);
        _glcd.print("Records:", 50, 230 , 270);
        _glcd.print("1.", 100, 300, 270);
        String stringRecord1 = String(config.records[0]);
        _glcd.print(stringRecord1, 100, 270, 270);
        _glcd.print("2.", 120, 300, 270);
        String stringRecord2 = String(config.records[1]);
        _glcd.print(stringRecord2, 120, 270, 270);
        _glcd.print("3.", 140, 300, 270);
        String stringRecord3 = String(config.records[2]);
        _glcd.print(stringRecord3, 140, 270, 270);
        _glcd.setColor(20, 115, 40);
        _glcd.fillRect(190, 35, 230, 285);
        _glcd.setColor(VGA_WHITE);
        _glcd.setBackColor(20, 115, 40);
        _glcd.print("BACK", 202,190, 270 );
    }
    void RenderEndGame(Configuration &config, const uint8_t &score)
    {   
        // анимация окончания игры
        _glcd.clrScr();
        _glcd.setColor(VGA_WHITE);
        _glcd.setBackColor(VGA_BLACK);
        _glcd.setFont(BigFont);
        _glcd.print("GAME OVER", 110, 230 , 270);
        delay(1000);
        _glcd.clrScr();
        _glcd.setColor(VGA_PURPLE);
        _glcd.print("GAME OVER", 110, 230 , 270);
        delay(1000);
        _glcd.setColor(VGA_WHITE);
        _glcd.print("GAME OVER", 110, 230 , 270);
        delay(2000);

        // если игрок установил новый рекорд, то ему предлагается ввести свое имя
        if (score >= config.records[2])
        {
            _glcd.clrScr();
            _glcd.print("New record!", 20, 245 , 270);
            _glcd.print("Enter your name:", 40, 280 , 270);

            // кнопочки выбора букв
            _glcd.fillCircle(83, 250, 10);
            _glcd.drawRect(105, 270, 145, 230);
            _glcd.fillCircle(168, 250, 10);
            String first_letter = String(alphabet[number_of_first_letter]);
            _glcd.print(first_letter,120, 255, 270);

            _glcd.fillCircle(83, 160, 10);
            _glcd.drawRect(105, 180, 145, 140);
            _glcd.fillCircle(168, 160, 10);
            String second_letter = String(alphabet[number_of_second_letter]);
            _glcd.print(second_letter,120, 165, 270);

            _glcd.fillCircle(83, 70, 10);
            _glcd.drawRect(105, 90, 145, 50);
            _glcd.fillCircle(168, 70, 10);
            String third_letter = String(alphabet[number_of_third_letter]);
            _glcd.print(third_letter,120, 75, 270);

            _glcd.setColor(VGA_PURPLE);
            _glcd.fillRect(190, 35, 230, 285);
            _glcd.setColor(VGA_WHITE);
            _glcd.setBackColor(VGA_PURPLE);
            _glcd.print("OK", 202,175, 270 );
 
        } 
    }
    void RenderEnterName()
    {   
        _glcd.setBackColor(VGA_BLACK);
        _glcd.clrScr();
        _glcd.print("New record!", 20, 245 , 270);
        _glcd.print("Enter your name:", 40, 280 , 270);

        // кнопочки выбора букв
        _glcd.fillCircle(83, 250, 10);
        _glcd.drawRect(105, 270, 145, 230);
        _glcd.fillCircle(168, 250, 10);
        String first_letter = String(alphabet[number_of_first_letter]);
        _glcd.print(first_letter,120, 255, 270);

        _glcd.fillCircle(83, 160, 10);
        _glcd.drawRect(105, 180, 145, 140);
        _glcd.fillCircle(168, 160, 10);
        String second_letter = String(alphabet[number_of_second_letter]);
        _glcd.print(second_letter,120, 165, 270);

        _glcd.fillCircle(83, 70, 10);
        _glcd.drawRect(105, 90, 145, 50);
        _glcd.fillCircle(168, 70, 10);
        String third_letter = String(alphabet[number_of_third_letter]);
        _glcd.print(third_letter,120, 75, 270);
        
        _glcd.setColor(VGA_PURPLE);
        _glcd.fillRect(190, 35, 230, 285);
        _glcd.setColor(VGA_WHITE);
        _glcd.setBackColor(VGA_PURPLE);
        _glcd.print("OK", 202, 175, 270);
    }

    
};