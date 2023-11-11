#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>

#define DEFAULT_SCORE 50

#define JOYSTICK_X_AXIS_PIN A1
#define JOYSTICK_Y_AXIS_PIN A2
#define HOLONOM_JOYSTICK_X_AXIS_PIN A3
#define HOLONOM_JOYSTICK_Y_AXIS_PIN A4

#define GRABBER_HEIGHT_POTENTIOMETER A5
#define GRABBER_OPENING_POTENTIOMETER A6

#define SOLAR_PANEL_ROD_BUTTON 3
#define MAGNETS_BUTTON 4

TM1637Display scoreDisplay(12, 13); // CLK, DIO

RF24 radio(9, 53); // CE, CSN

struct JoystickData {
  // Classic joystick
  byte x;
  byte y;

  // Holonom joystick
  byte holonomX;
  byte holonomY;
};

struct RadioData
{
  struct JoystickData joystickData;
  
  byte grabberHeight;
  byte grabberOpening;

  byte score;

  bool magnetsState;
  bool solarPanelRodState;
};

void setup() {
  auto setupInputs = []() {
    pinMode(JOYSTICK_X_AXIS_PIN, INPUT);
    pinMode(JOYSTICK_Y_AXIS_PIN, INPUT);
    pinMode(HOLONOM_JOYSTICK_X_AXIS_PIN, INPUT);
    pinMode(HOLONOM_JOYSTICK_Y_AXIS_PIN, INPUT);

    pinMode(GRABBER_HEIGHT_POTENTIOMETER, INPUT);
    pinMode(GRABBER_OPENING_POTENTIOMETER, INPUT);

    pinMode(SOLAR_PANEL_ROD_BUTTON, INPUT_PULLUP);
    pinMode(MAGNETS_BUTTON, INPUT_PULLUP);
  };

  auto setupScoreDisplay = []() {
    scoreDisplay.setBrightness(0x0f);
    scoreDisplay.showNumberDec(DEFAULT_SCORE);
  };

  auto setupRadio = []() {
    const byte transimittingAddress[6] = "912CR";

    radio.begin();

    radio.setPALevel(RF24_PA_MIN);
    radio.openWritingPipe(transimittingAddress); // address
  };

  Serial.begin(9600);

  Serial.println("Setup in progress...");

  setupInputs();
  Serial.println("Inputs setup completed");

  setupScoreDisplay();
  Serial.println("Score display setup completed");

  setupRadio();
  Serial.println("Radio setup completed");

  Serial.println("Controller ready!");
}

void loop() {
  struct RadioData dataToSend;

  auto getData = [&]() {
    struct JoystickData joystickData;
    dataToSend.grabberHeight = analogRead(GRABBER_HEIGHT_POTENTIOMETER);
    dataToSend.grabberOpening = analogRead(GRABBER_OPENING_POTENTIOMETER);

    dataToSend.magnetsState = digitalRead(MAGNETS_BUTTON);
    dataToSend.solarPanelRodState = digitalRead(SOLAR_PANEL_ROD_BUTTON);

    dataToSend.score = DEFAULT_SCORE; // TODO: Get score from the score board

    joystickData.x = analogRead(JOYSTICK_X_AXIS_PIN);
    joystickData.y = analogRead(JOYSTICK_Y_AXIS_PIN);
    joystickData.holonomX = analogRead(HOLONOM_JOYSTICK_X_AXIS_PIN);
    joystickData.holonomY = analogRead(HOLONOM_JOYSTICK_Y_AXIS_PIN);

    dataToSend.joystickData = joystickData;
  };

  auto sendData = [&]() {
    radio.write(&dataToSend, sizeof(dataToSend));
  };

  auto displayScore = [&]() {
    scoreDisplay.showNumberDec(dataToSend.score);
  };

  getData();
  sendData();
  displayScore();
}