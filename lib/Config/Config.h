#ifndef CONFIG_H
#define CONFIG_H

// USER DEFINED //

/* Add // before #define to disable a setting. */

/* Define wether to enable info logs or not. */
#define INFO_STATE

/* Define wether to enable debug logs or not. Impact performances when enabled */
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
#define HEARTBEAT_DELAY 60

// IO //

#define RADIO_CE 48
#define RADIO_CSN 49

#define SCORE_ENCODER_CLK 3
#define SCORE_ENCODER_DT 2

#define DISPLAY_CLK A0
#define DISPLAY_DIO A1

#define HOLONOM_JOYSTICK_X_AXIS A5
#define HOLONOM_JOYSTICK_Y_AXIS A12

#define CLASSIC_JOYSTICK_X_AXIS A9

#define GRABBER_HEIGHT_POTENTIOMETER A10
#define GRABBER_OPENING_POTENTIOMETER A11

#define LEFT_PUSHER_BUTTON 4
#define RIGH_PUSHER_BUTTON 8

// END USER DEFINED //

#endif