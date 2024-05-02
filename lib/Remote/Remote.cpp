#include <Arduino.h>
#include <Remote.h>
#include <DataStructures.h>
#include <Logging.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <SPI.h>

Remote::Remote(): radio(RADIO_CE, RADIO_CSN) {};

void Remote::setup() {
    const byte writingAddress[6] = "912CR";

    radio.begin();

    radio.setChannel(110);
    radio.setPALevel(RF24_PA_LOW);
    radio.setDataRate(RF24_2MBPS);

    radio.openWritingPipe(writingAddress); // address

    radio.stopListening();
}

void Remote::send(RadioData *dataBuffer) {
    radio.write(dataBuffer, sizeof(*dataBuffer));
}

void Remote::print_details() {
    radio.printDetails();
}