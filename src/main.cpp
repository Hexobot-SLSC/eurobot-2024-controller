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
#define PUSHER_BUTTON 5

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

typedef struct InputData {
  struct JoystickData joystickData;
  
  byte grabberHeight;
  byte grabberOpeningAngle;

  byte score;

  bool areMagnetsEnabled;
  bool isRodDeployed;
  bool isPusherDeployed;
}InputData;

struct InputData lastInputData;

void getInputsData(struct InputData *inputData);
void sendData(struct InputData *inputData);

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
    pinMode(PUSHER_BUTTON, INPUT_PULLUP);
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
  InputData inputData;

  getInputsData(&inputData);
  sendData(&inputData);
  //displayScore();

  lastInputData = inputData;
}

void getInputsData(struct InputData *inputData) {
  JoystickData joystickData;

  joystickData.x = analogRead(JOYSTICK_X_AXIS_PIN);
  joystickData.y = analogRead(JOYSTICK_Y_AXIS_PIN);
  joystickData.holonomX = analogRead(HOLONOM_JOYSTICK_X_AXIS_PIN);
  joystickData.holonomY = analogRead(HOLONOM_JOYSTICK_Y_AXIS_PIN);

  inputData->joystickData = joystickData;
  
  inputData->grabberHeight = analogRead(GRABBER_HEIGHT_POTENTIOMETER);
  inputData->grabberOpeningAngle = analogRead(GRABBER_OPENING_POTENTIOMETER);
  
  inputData->score = DEFAULT_SCORE; // TODO: Get score from the score board

  inputData->isRodDeployed = digitalRead(SOLAR_PANEL_ROD_BUTTON);
  inputData->areMagnetsEnabled = digitalRead(MAGNETS_BUTTON);
  inputData->isPusherDeployed = digitalRead(PUSHER_BUTTON);
}

void sendData(struct InputData *inputData) {
  char code = 'J';

  radio.write(&code, sizeof(code));
  radio.write(&(inputData->joystickData), sizeof(inputData->joystickData));

  if (lastInputData.areMagnetsEnabled != inputData->areMagnetsEnabled) {
    code = 'M';
    
    radio.write(&code, sizeof(code));
    radio.write(&(inputData->areMagnetsEnabled), sizeof(inputData->areMagnetsEnabled));
  } else if (lastInputData.isRodDeployed != inputData->isRodDeployed) {
    code = 'R';
    
    radio.write(&code, sizeof(code));
    radio.write(&(inputData->isRodDeployed), sizeof(inputData->isRodDeployed));
  } else if (lastInputData.grabberHeight != inputData->grabberHeight) {
    code = 'H';
    
    radio.write(&code, sizeof(code));
    radio.write(&(inputData->grabberHeight), sizeof(inputData->grabberHeight));
  } else if (lastInputData.grabberOpeningAngle != inputData->grabberOpeningAngle) {
    code = 'O';
    
    radio.write(&code, sizeof(code));
    radio.write(&(inputData->grabberOpeningAngle), sizeof(inputData->grabberOpeningAngle));
  } else if (lastInputData.isPusherDeployed != inputData->isPusherDeployed) {
    code = 'P';
    
    radio.write(&code, sizeof(code));
    radio.write(&(inputData->isPusherDeployed), sizeof(inputData->isPusherDeployed));
  }

  // TODO : Add score
}