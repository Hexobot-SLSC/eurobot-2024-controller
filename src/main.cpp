#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>

#define DEFAULT_SCORE 50

const uint64_t writingAddress = 2024134UL;
const uint64_t readingAddress = 314592654UL;

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

    radio.setChannel(99);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_LOW);
    radio.openReadingPipe(0, readingAddress); // address
    radio.openWritingPipe(writingAddress); // address
  };

  auto connectToRobotAsync = []() {
    while (true) {
      radio.startListening();

      delay(1000);

      radio.stopListening();

      while (radio.available()) {
        unsigned char data;

        radio.read(&data, sizeof(data));
        
        if (data != '!') continue;
        
        Serial.println("Robot connected!");

        const unsigned char response = '!';

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

  Serial.println("Robot ready!");
}

void loop() {
  
}