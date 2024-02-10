#ifndef CONFIG_H
#define CONFIG_H

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
#define HEARTBEAT_DELAY 100

// END USER DEFINED //

#endif