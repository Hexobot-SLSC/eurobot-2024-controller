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
#define LEFT_PUSHER_BUTTON 5
#define RIGH_PUSHER_BUTTON 6

TM1637Display scoreDisplay(12, 13); // CLK, DIO

RF24 radio(9, 53); // CE, CSN

typedef struct JoystickData {
  // Classic joystick
  byte x;
  byte y;

  // Holonom joystick
  byte holonomX;
  byte holonomY;
}JoystickData;

typedef struct RadioData {
  struct JoystickData joystickData;
  
  byte grabberHeight;
  byte grabberOpeningAngle;

  byte score;

  bool areMagnetsEnabled;
  bool isRodDeployed;
  bool isRightPusherDeployed;
  bool isLeftPusherDeployed;
}RadioData;

void displayScore(byte score);
void getInputsData(struct RadioData *dataToSend);
void sendData(struct RadioData *dataToSend);

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
    pinMode(RIGH_PUSHER_BUTTON, INPUT_PULLUP);
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
  RadioData dataToSend;

  getInputsData(&dataToSend);
  displayScore(dataToSend.score);
  sendData(&dataToSend);

  delay(100); // Delay of 100ms to avoid spamming the radio
}

void getInputsData(struct RadioData *dataToSend) {
  JoystickData joystickData;

  joystickData.x = analogRead(JOYSTICK_X_AXIS_PIN);
  joystickData.y = analogRead(JOYSTICK_Y_AXIS_PIN);
  joystickData.holonomX = analogRead(HOLONOM_JOYSTICK_X_AXIS_PIN);
  joystickData.holonomY = analogRead(HOLONOM_JOYSTICK_Y_AXIS_PIN);

  dataToSend->joystickData = joystickData;
  
  dataToSend->grabberHeight = analogRead(GRABBER_HEIGHT_POTENTIOMETER);
  dataToSend->grabberOpeningAngle = analogRead(GRABBER_OPENING_POTENTIOMETER);
  
  dataToSend->score = DEFAULT_SCORE; // TODO: Get score from the score board

  dataToSend->isRodDeployed = digitalRead(SOLAR_PANEL_ROD_BUTTON);
  dataToSend->areMagnetsEnabled = digitalRead(MAGNETS_BUTTON);
  dataToSend->isRightPusherDeployed = digitalRead(RIGH_PUSHER_BUTTON);
  dataToSend->isLeftPusherDeployed = digitalRead(LEFT_PUSHER_BUTTON);
}

void displayScore(byte score) {
  scoreDisplay.showNumberDec(score);
}

void sendData(struct RadioData *dataToSend) {
  radio.write(&dataToSend, sizeof(dataToSend));
}