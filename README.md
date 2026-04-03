# NHL Stenden Project BattleBot

## Hardware Integration: I/O Mapping and Control Logic

BattleBot is an autonomous Arduino Nano robot designed to compete in various races aganist other robots, utilizing precise components such as ultrasonic sensors, a servo motor and line tracking sensors to maximize accuracy and speed.

## Selected Hardware for the Project

* Arduino Nano microcontroller
* PCB with an Arduino UNO footprint for additional shields
* Metal chassis
* Powerbank (10.000 mAh)
* 2 On/Off switches, for the logic circuit and the motor circuit
* A gripper with a servo motor
* 4 Neopixels
* Motor driver suitable for two electromotors
* 2 Electromotors
* 2 Ultrasonic distance sensor
* 8 Analogue line follower sensors

## Pin Mapping and I/O Configuration

### Motor Pins

| Variable Name | Pin | Type | Function |
| :--- | :---: | :---: | :--- |
| FORWARD_LEFT | 6 | PWM | Left motor forward drive |
| FORWARD_RIGHT | 5 | PWM | Right motor forward drive |
| BACK_LEFT | 11 | PWM | Left motor reverse drive |
| BACK_RIGHT | 10 | PWM | Right motor reverse drive |

### Object Sensing

| Variable Name | Pin | Type | Function |
| :--- | :---: | :---: | :--- |
| TRIGGER_FRONT | 12 | Digital | Front ultrasonic trigger pulse |
| ECHO_FRONT | 13 | Digital | Front ultrasonic echo return |
| TRIGGER_RIGHT | 7 | Digital | Right ultrasonic trigger pulse |
| ECHO_RIGHT | 8 | Digital | Right ultrasonic echo return |

### Line Tracking

| Variable Name | Pin | Type | Function |
| :--- | :---: | :---: | :--- |
| SENSOR_PINS[] | A0-A7 | Analog | 8-Channel Infrared (IR) Sensor Array |

### Gripper

| Variable Name | Pin | Type | Function |
| :--- | :---: | :---: | :--- |
| SERVO_PIN | 9 | PWM | Gripper servo motor control |

### Status Display (NeoPixels)

| Variable Name | Pin | Type | Function |
| :--- | :---: | :---: | :--- |
| NEO_PIXEL | 4 | Digital | RGB status LEDs |

## Additional Information

* Learning Foundation: The [week_assignments/](./week_assignments/) folder contains various Arduino exercises developed during the learning phase. These served as preparation for the final race logic.

* Documentation Scope: To maintain clarity, this README file only maps the hardware and pin configurations used in the final code. Some of the assignments might use different pins for the same hardware or different components which were not found useful enough to be used in the final project.

* Race Final Code: The [battlebot_race_day_final/](./battlebot_race_day_final/) folder contains the optimized, final code used during the race event.

* Repository Origins: This repository is an organized version of the original "demo" and practice files used during the project. It has been restructured to provide a clear and professional view. [Original repository](https://github.com/rodrigo-castanheira/KlankerBattleBot-0016)

## Project Contributors

This project was implemented by Group TD-INF-IT1C:

* Jakub Mazur
* Rodrigo de Araújo Castanheira

## Other Team Members Repositories

* [Justinas and Oleh](https://github.com/JustinasLaunikonis/BattleBot-Robobr)
* [Annemiek and Herbert](https://github.com/HerbertDJD/Battlebot_BB032)
