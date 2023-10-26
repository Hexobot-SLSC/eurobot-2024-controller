#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>

#define DEFAULT_SCORE 50

const byte readingAddress[6] = "912RC";
const byte transimittingAddress[6] = "912CR";

TM1637Display scoreDisplay(12, 13); // CLK, DIO

RF24 radio(9, 53); // CE, CSN

void setup() {
  auto setupDisplay = []() {
    const uint8_t middleBar[1] = {SEG_G};
    scoreDisplay.setBrightness(0x0f);
    scoreDisplay.setSegments(middleBar, 4, 0);
  };

  auto setupRadio = []() {
    radio.begin();

    radio.setPALevel(RF24_PA_MIN);
    radio.openReadingPipe(0, readingAddress); // address
    radio.openWritingPipe(transimittingAddress); // address
  };

  auto connectToRobotAsync = []() {
    while (true) {
      radio.startListening();

      delay(1000);

      while (radio.available()) {
        char data;

        radio.read(&data, sizeof(data));
        
        if (data != '!') continue;

        radio.stopListening();

        const char response = '!';

        radio.write(&response, sizeof(response));

        return;
      };
    }
  };

  Serial.begin(9600);

  Serial.println("Setup in progress...");

  setupDisplay();
  Serial.println("Display setup completed");

  setupRadio();
  Serial.println("Radio setup completed");

  Serial.println("Initialization completed!");


  Serial.println("Attempting to connect to robot...");
  connectToRobotAsync();
  Serial.println("Connected to robot");

  radio.flush_rx();

  Serial.println("Controller ready!");
}

void loop() {
  
}