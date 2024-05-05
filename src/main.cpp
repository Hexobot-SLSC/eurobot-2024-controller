#include <Arduino.h>
#include <Config.h>
#include <printf.h>
#include <Logging.h>
#include <Encoder.h>
#include <Inputs.h>
#include <Remote.h>
#include <ScoreDisplay.h>

Inputs inputs;
Remote remote;
ScoreDisplay scoreDisplay;
Encoder scoreEncoder(SCORE_ENCODER_CLK, SCORE_ENCODER_DT); // CLK, DT (DT must be an interrupt pin)

byte score = DEFAULT_SCORE; // DEFAULT
byte lastScore;

byte getNewScore();

void setup() {
  Serial.begin(9600);
  printf_begin();

  log("Setup in progress...");

  inputs.setup();  
  debug("Inputs setup completed");

  scoreDisplay.setup();
  debug("Score display setup completed");

  #ifdef DEBUG_STATE
  debug("Radio details :")
  //remote.print_details();
  #endif

  bool remoteSuccess = false;
  
  while (!remoteSuccess) {
    remoteSuccess = remote.setup();
    debug(remoteSuccess)
    if (!remoteSuccess) {
      log("Remote setup failed, retrying in 5 seconds...");
      for (byte i = 0; i < 5; i++) {
        scoreDisplay.set(5 - i);
        delay(1000);
      }
    }
  }
  debug("Radio setup completed");

  log("Setup completed !")
  
  scoreDisplay.update(score);
}

void loop() {
  RadioData dataToSend;

  delay(HEARTBEAT_DELAY);

  byte newScore = getNewScore();
  if (score != newScore) {
    score = newScore;
    scoreDisplay.update(score);
  }

  inputs.fetch(&dataToSend);

  dataToSend.score = newScore;

  remote.send(&dataToSend);
}

byte getNewScore() {
  short add = scoreEncoder.readAndReset();
  return constrain(score + add / 4 * SCORE_STEP, MIN_SCORE, MAX_SCORE);
}
