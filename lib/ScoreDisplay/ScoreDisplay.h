#ifndef __SCOREDISPLAY_H__
#define __SCOREDISPLAY_H__

#include <Arduino.h>
#include <TM1637Display.h>

class ScoreDisplay {
    private:
        TM1637Display display;
    public:
        ScoreDisplay();

        void setup();
        void update(byte score);

};

#endif