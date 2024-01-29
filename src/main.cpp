#include <Arduino.h>
#include <printf.h>
#include <Logging.h>
#include <Encoder.h>
#include <Inputs.h>
#include <Remote.h>
#include <ScoreDisplay.h>

// USER DEFINED //

/* Add // before #define to disable a setting. */

/* Define wether to enable info logs or not. */
#define INFO_STATE

/* Define wether to enable debug logs or not. */
#define DEBUG_STATE

/* The default score to display on the robot.
Cannot disable. */
#define DEFAULT_SCORE 40

/* The score step to add to the score when the encoder is rotated.
Cannot disable*/
#define SCORE_STEP 5

/* The minimum score the robot can have.
Cannot disable*/
#define MIN_SCORE 20

/* The maximum score the robot can have.
Cannot disable*/
#define MAX_SCORE 200

/* Delay between each cycle of the loop.
Should be sync with the one on the controller.
----------------------------------------------------------------
/!\ THIS IS A CRITICAL SETTING. DO NOT SET WITHOUT KNOWLEDGE /!\.
The lower it is set the higher the power consumption will be.
Making the delay too high will make the robot unresponsive.
Making the delay too low will make the robot consume too much power and
risks devices damages and power supply issues.
----------------------------------------------------------------
Cannot disable. */
#define HEARTBEAT_DELAY 30

// END USER DEFINED //


#define SEND_DATA_BUTTON 6

Inputs inputs;
Remote remote;
ScoreDisplay scoreDisplay;
Encoder scoreEncoder(13, 13); // CLK, DT (DT must be an interrupt pin)

byte score = DEFAULT_SCORE; // DEFAULT
byte lastScore;

byte getNewScore();
bool canSendData();

void setup() {

  Serial.begin(9600);
  printf_begin();

  log("Setup in progress...");

  pinMode(SEND_DATA_BUTTON, INPUT_PULLUP);
  inputs.setup();  
  debug("Inputs setup completed");

  scoreDisplay.setup();
  debug("Score display setup completed");

  remote.setup();
  debug("Radio setup completed");

  log("Setup completed !")

  #ifdef DEBUG_STATE
  debug("Radio details :")
  remote.print_details();
  #endif
}

void loop() {
  RadioData dataToSend;

  delay(HEARTBEAT_DELAY); // Delay of 20ms to avoid spamming the radio

  byte newScore = getNewScore();
  if (score != newScore) {
    score = newScore;
    scoreDisplay.update(score);
  }

  if (!canSendData()) {
    return;
  }

  inputs.fetch(&dataToSend);
  dataToSend.score = newScore;

  remote.send(&dataToSend);
}

bool canSendData() {
  return (digitalRead(SEND_DATA_BUTTON) == true);
}

byte getNewScore() {
  return score + constrain(scoreEncoder.readAndReset() * SCORE_STEP, MIN_SCORE, MAX_SCORE);
}
