#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <TM1637Display.h>
#include <Encoder.h>
#include <printf.h>

// USER DEFINED //

/* Define wether to enable info logs or not. Add // before #define to disable. */
#define INFO_STATE

/* Define wether to enable debug logs or not. Add // before #define to disable. */
#define DEBUG_STATE

// END USER DEFINED //

#ifdef INFO_STATE
#define log(msg) \
Serial.print("[INFO] : "); \
Serial.println(msg);
#else
#define log(msg)
#endif

#ifdef DEBUG_STATE
#define debug(msg) \
Serial.print("[DEBUG] : "); \
Serial.println(msg);
#else
#define debug(msg)
#endif

#define SCORE_STEP 5
#define MIN_SCORE 20
#define MAX_SCORE 200

#define JOYSTICK_X_AXIS_PIN A1
#define JOYSTICK_Y_AXIS_PIN A2
#define HOLONOM_JOYSTICK_X_AXIS_PIN A3
#define HOLONOM_JOYSTICK_Y_AXIS_PIN A4

#define GRABBER_HEIGHT_POTENTIOMETER A5
#define GRABBER_OPENING_POTENTIOMETER A6

#define SOLAR_PANEL_ROD_BUTTON 99
#define MAGNETS_BUTTON 99
#define LEFT_PUSHER_BUTTON 5
#define RIGH_PUSHER_BUTTON 9
#define SEND_DATA_BUTTON 6

TM1637Display scoreDisplay(22, 23); // CLK, DIO
RF24 radio(3, 4); // CE, CSN
Encoder scoreEncoder(13, 13); // CLK, DT (DT must be an interrupt pin)

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
    pinMode(SEND_DATA_BUTTON, INPUT_PULLUP);
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
  printf_begin();

  log("Setup in progress...");

  setupInputs();
  debug("Inputs setup completed");

  setupScoreDisplay();
  debug("Score display setup completed");

  setupRadio();
  debug("Radio setup completed");

  log("Setup completed !")

  #ifdef DEBUG_STATE
  debug("Radio details :")
  radio.printDetails();
  #endif

  pinMode(LED_BUILTIN, OUTPUT);
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
