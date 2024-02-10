#include <Arduino.h>
#include <Inputs.h>
#include <DataStructures.h>
#include <InputsConfig.h>
#include <Logging.h>

Inputs::Inputs() {};

void Inputs::setup() {
    pinMode(JOYSTICK_X_AXIS_PIN, INPUT);
    pinMode(JOYSTICK_Y_AXIS_PIN, INPUT);
    pinMode(HOLONOM_JOYSTICK_X_AXIS_PIN, INPUT);
    //pinMode(HOLONOM_JOYSTICK_Y_AXIS_PIN, INPUT);

    // pinMode(GRABBER_HEIGHT_POTENTIOMETER, INPUT);
    // pinMode(GRABBER_OPENING_POTENTIOMETER, INPUT);

    // pinMode(SOLAR_PANEL_ROD_BUTTON, INPUT_PULLUP);
    // pinMode(MAGNETS_BUTTON, INPUT_PULLUP);
    pinMode(RIGH_PUSHER_BUTTON, INPUT_PULLUP);
    pinMode(LEFT_PUSHER_BUTTON, INPUT_PULLUP);
}

void Inputs::fetch(RadioData *dataBuffer) {
    dataBuffer->joystickData.holonomX = 255 - map(analogRead(JOYSTICK_X_AXIS_PIN), 0, 960, 0, 255);
    dataBuffer->joystickData.holonomY = map(analogRead(JOYSTICK_Y_AXIS_PIN), 0, 1024, 0, 255);
    dataBuffer->joystickData.x = 255 - map(analogRead(HOLONOM_JOYSTICK_X_AXIS_PIN), 0, 1024, 0, 255);

    debug(analogRead(JOYSTICK_X_AXIS_PIN));

    // dataBuffer->grabberHeight = analogRead(GRABBER_HEIGHT_POTENTIOMETER);
    // dataBuffer->grabberOpeningAngle = analogRead(GRABBER_OPENING_POTENTIOMETER);

    // dataBuffer->isRodDeployed = digitalRead(SOLAR_PANEL_ROD_BUTTON);
    // dataBuffer->areMagnetsEnabled = digitalRead(MAGNETS_BUTTON);
    dataBuffer->isRightPusherDeployed = digitalRead(RIGH_PUSHER_BUTTON);
    dataBuffer->isLeftPusherDeployed = digitalRead(LEFT_PUSHER_BUTTON);
}