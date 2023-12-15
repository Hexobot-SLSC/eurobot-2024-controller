#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>
#include <Encoder.h>

#define SCORE_STEP 5
#define MIN_SCORE 20
#define MAX_SCORE 200

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
#define SEND_DATA_BUTTON 7

TM1637Display scoreDisplay(12, 13); // CLK, DIO
RF24 radio(7, 8); // CE, CSN
Encoder scoreEncoder(18, 19); // CLK, DT (DT must be an interrupt pin)

typedef struct JoystickData {
  // Classic joystick
  byte x;
  byte y;

  // Holonom joystick
  byte holonomX;
  byte holonomY;
} JoystickData;

typedef struct RadioData {
  struct JoystickData joystickData;
  
  byte grabberHeight;
  byte grabberOpeningAngle;

  byte score;

  bool areMagnetsEnabled;
  bool isRodDeployed;
  bool isRightPusherDeployed;
  bool isLeftPusherDeployed;
} RadioData;

byte score = 90; // DEFAULT

void getInputsData(RadioData *dataToSend);
void sendData(RadioData *dataToSend);
void updateScore();
bool canSendData();

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
    pinMode(LEFT_PUSHER_BUTTON, INPUT_PULLUP);
  };

  auto setupScoreDisplay = []() {
    scoreDisplay.setBrightness(0x0f);
    scoreDisplay.showNumberDec(score);
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

  delay(20); // Delay of 20ms to avoid spamming the radio

  if (!canSendData()) {
    return;
  }

  getInputsData(&dataToSend);
  updateScore();
  dataToSend.score = score;
  sendData(&dataToSend);
}

bool canSendData() {
  return (digitalRead(SEND_DATA_BUTTON) == true);
}

void getInputsData(struct RadioData *dataToSend) {
  JoystickData joystickData;

  joystickData.x = map(analogRead(JOYSTICK_X_AXIS_PIN), 0, 1023, 0, 255);
  joystickData.y = map(analogRead(JOYSTICK_Y_AXIS_PIN), 0, 1023, 0, 255);
  joystickData.holonomX = map(analogRead(HOLONOM_JOYSTICK_X_AXIS_PIN), 0, 1023, 0, 255);
  joystickData.holonomY = map(analogRead(HOLONOM_JOYSTICK_Y_AXIS_PIN), 0, 1023, 0, 255);

  dataToSend->joystickData = joystickData;
  
  dataToSend->grabberHeight = map(analogRead(GRABBER_HEIGHT_POTENTIOMETER), 0, 1023, 0, 255);
  dataToSend->grabberOpeningAngle = map(analogRead(GRABBER_OPENING_POTENTIOMETER), 0, 1023, 0, 255);

  dataToSend->isRodDeployed = digitalRead(SOLAR_PANEL_ROD_BUTTON);
  dataToSend->areMagnetsEnabled = digitalRead(MAGNETS_BUTTON);
  dataToSend->isRightPusherDeployed = digitalRead(RIGH_PUSHER_BUTTON);
  dataToSend->isLeftPusherDeployed = digitalRead(LEFT_PUSHER_BUTTON);
}

void updateScore() {
  byte lastScore = score;
  score += constrain(scoreEncoder.readAndReset() * SCORE_STEP, MIN_SCORE, MAX_SCORE);

  if (lastScore == score) return;
  scoreDisplay.showNumberDec(score);
}

void sendData(struct RadioData *dataToSend) {
  radio.write(dataToSend, sizeof(*dataToSend));
}
