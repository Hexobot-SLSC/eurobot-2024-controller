#ifndef __REMOTE_H__
#define __REMOTE_H__

#include <Arduino.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <DataStructures.h>

class Remote {
    public:
        Remote();

        bool setup();
        bool hasFailure();
        void send(RadioData *dataBuffer);
        void print_details();
    
    private:
        RF24 radio;
};

#endif