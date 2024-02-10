#include <Arduino.h>
#include <Config.h>
#include <printf.h>
#include <Logging.h>
#include <Encoder.h>
#include <Inputs.h>
#include <Remote.h>
#include <ScoreDisplay.h>

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

  delay(HEARTBEAT_DELAY);

  byte newScore = getNewScore();
  if (score != newScore) {
    score = newScore;
    scoreDisplay.update(score);
  }

  if (!canSendData()) {
    return;
  }


  inputs.fetch(&dataToSend);

  //dataToSend.score = newScore;

  remote.send(&dataToSend);
}

bool canSendData() {
  return (digitalRead(SEND_DATA_BUTTON) == true);
}

byte getNewScore() {
  return score + constrain(scoreEncoder.readAndReset() * SCORE_STEP, MIN_SCORE, MAX_SCORE);
}
