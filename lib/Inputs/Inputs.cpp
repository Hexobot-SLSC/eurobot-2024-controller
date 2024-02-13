#include <Arduino.h>
#include <Inputs.h>
#include <DataStructures.h>
#include <InputsConfig.h>
#include <Logging.h>
#include <EasyButton.h>
#include <BetterJoystick.h>

EasyButton rightPusherButton(RIGH_PUSHER_BUTTON);
EasyButton leftPusherButton(LEFT_PUSHER_BUTTON);

Joystick joystick(JOYSTICK_X_AXIS_PIN, NOT_A_PIN, NOT_A_PIN);
Joystick holonomJoystick(HOLONOM_JOYSTICK_X_AXIS_PIN, HOLONOM_JOYSTICK_Y_AXIS_PIN, NOT_A_PIN);

Inputs::Inputs() {};

void Inputs::setup() {
    // pinMode(GRABBER_HEIGHT_POTENTIOMETER, INPUT);
    // pinMode(GRABBER_OPENING_POTENTIOMETER, INPUT);

    // pinMode(SOLAR_PANEL_ROD_BUTTON, INPUT_PULLUP);
    // pinMode(MAGNETS_BUTTON, INPUT_PULLUP);
    rightPusherButton.begin();
    leftPusherButton.begin();
}

void Inputs::fetch(RadioData *dataBuffer) {
    dataBuffer->joystickData.x = 255 - map(
        joystick.x(JOYSTICK_X_AXIS_ZONE_MIN, JOYSTICK_X_AXIS_ZONE_MAX),
        JOYSTICK_X_AXIS_ZONE_MIN,
        JOYSTICK_X_AXIS_ZONE_MAX,
        0,
        255
    );

    dataBuffer->joystickData.holonomX = 255 - map(
        holonomJoystick.x(HOLONOM_JOYSTICK_X_AXIS_ZONE_MIN, HOLONOM_JOYSTICK_X_AXIS_ZONE_MAX),
        HOLONOM_JOYSTICK_X_AXIS_ZONE_MIN,
        HOLONOM_JOYSTICK_X_AXIS_ZONE_MAX,
        0,
        255
    );

    dataBuffer->joystickData.holonomY = map(
        holonomJoystick.y(HOLONOM_JOYSTICK_Y_AXIS_ZONE_MIN, HOLONOM_JOYSTICK_Y_AXIS_ZONE_MAX),
        HOLONOM_JOYSTICK_Y_AXIS_ZONE_MIN,
        HOLONOM_JOYSTICK_Y_AXIS_ZONE_MAX,
        0,
        255
    );

    debug(String("Joystick X: ") + String(dataBuffer->joystickData.x));
    debug(String("Holonom Joystick X: ") + String(dataBuffer->joystickData.holonomX));
    debug(String("Holonom Joystick Y: ") + String(dataBuffer->joystickData.holonomY));

    // dataBuffer->grabberHeight = analogRead(GRABBER_HEIGHT_POTENTIOMETER);
    // dataBuffer->grabberOpeningAngle = analogRead(GRABBER_OPENING_POTENTIOMETER);

    // dataBuffer->isRodDeployed = digitalRead(SOLAR_PANEL_ROD_BUTTON);
    // dataBuffer->areMagnetsEnabled = digitalRead(MAGNETS_BUTTON);
    dataBuffer->isRightPusherDeployed = rightPusherButton.read();
    dataBuffer->isLeftPusherDeployed = leftPusherButton.read();
}