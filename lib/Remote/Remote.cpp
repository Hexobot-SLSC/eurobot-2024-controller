#include <Arduino.h>
#include <Remote.h>
#include <DataStructures.h>

Remote::Remote(): radio(9, 53) {};

void Remote::setup() {
    const byte writingAddress[6] = "912CR";

    radio.begin();

    radio.setPALevel(RF24_PA_MIN);
    radio.openWritingPipe(writingAddress); // address
    radio.stopListening();

    radio.flush_tx();
}

void Remote::send(RadioData *dataBuffer) {
    radio.write(&dataBuffer, sizeof(&dataBuffer));
}

void Remote::print_details() {
    radio.printDetails();
}