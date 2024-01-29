#ifndef __INPUTS_H__
#define __INPUTS_H__

#include <Arduino.h>
#include <DataStructures.h>

class Inputs {
    public:
        Inputs();

        void setup();
        void fetch(RadioData *dataBuffer);

    private:
};

#endif