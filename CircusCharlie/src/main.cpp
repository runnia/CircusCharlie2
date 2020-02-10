#include <Arduino.h>
#include "TouchScreen.h"
#include "UTFT.h"
#include <EEPROM.h>
#include "GameManager.h"

const uint8_t RS = A2;
const uint8_t WR = A1;
const uint8_t CS = A3;
const uint8_t RST = A4;
const uint8_t SER = A0;
const uint8_t YP = A2;
const uint8_t XM = A3;
const uint8_t YM = 8;
const uint8_t XP = 9;
//  Определяем экстремумы для значений считываемых с аналоговых входов при определении точек нажатия на TouchScreen:
const int tsMinX = 140;    // соответствующий точке начала координат по оси X
const int tsMinY = 110;    // соответствующий точке начала координат по оси Y
const int tsMaxX = 955;    // соответствующий максимальной точке координат по оси X
const int tsMaxY = 910;    // соответствующий максимальной точке координат по оси Y
const int minPress = 5;    // соответствующий минимальной степени нажатия на TouchScreen
const int maxPress = 1000; // соответствующий максимальной степени нажатия на TouchScreen



// нужные цвета
//(20, 115, 40) - зеленый
//(13, 56, 163) - синий
//(240, 115, 26) - оранжевый
//(240, 115, 26) - бежевый

UTFT _glcd(TFT28UNO, RS, WR, CS, RST, SER);
TouchScreen ts = TouchScreen(XP, YP, XM, YM);
GameManager game(_glcd);
uint16_t x = -1;
uint16_t y = -1;

void setup()

{
    //EEPROM.write(3,1);
    Serial.begin(9600);
    _glcd.InitLCD();
    _glcd.clrScr();
    game.InitGameManager();
}

void loop()
{
    // считываем ввод каждые 100мс и если он есть просто передаем его в GameManager
    // GameManager сам решит, что с ним делать

    // устанаваливаем передваемый ввод на невалидный
    x = y = INVALID_INPUT;

    TSPoint p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (p.z > minPress && p.z < maxPress)
    {
        p.x = map(p.x, tsMinX, tsMaxX, 0, 320);
        p.y = map(p.y, tsMinY, tsMaxY, 0, 240);

        // сведение координат рендера и ввода в одно начало
        x = p.y;
        y = 320 - p.x;

        Serial.print("(");
        Serial.print(x);
        Serial.print(", ");
        Serial.print(y);
        Serial.println(")");
     }

    // // передаем ввод в GameManager
    game.Tick(x, y);

    // EEPROM.write(16,1);
    //EEPROM.write(3,1);
    // for (int i = 1; i < 3; ++i){
    //     EEPROM.write(i,0);
    // }

    delay(100);
}