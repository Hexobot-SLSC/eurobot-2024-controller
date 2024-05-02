#include <Arduino.h>
#include <ScoreDisplay.h>
#include <TM1637Display.h>
#include <Config.h>

ScoreDisplay::ScoreDisplay(): display(DISPLAY_CLK, DISPLAY_DIO) {};

void ScoreDisplay::setup(byte score) {
    display.setBrightness(0x0f);
    display.showNumberDec(score, false);
}

void ScoreDisplay::update(byte score) {
    display.showNumberDec(score);
}