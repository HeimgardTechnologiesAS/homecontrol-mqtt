# Library functions:

### Explanations:
* Reporting: device needs to send every X number of seconds update of state, or system will mark it as offline
* Control functions have direction Driver->Device and are used to set values
* Status functions have direction Device->Driver and are used as feedback to update values in driver. If not set differently, the default value will be used from driver.

### Explanation:
1. function
    * function identifier
    * default reporting time
    * value type
    * description

### List of functions:
1. control_power
    * cp
    * 0
    * bool
    * Receives on or off to control output

1. status_power
    * sp
    * 30
    * bool
    * Feedback to app if output/input is on or off

1. control_level
    * cl
    * 0
    * uint 0-10000
    * On device you will get number in this range when set in app

1. status_level
    * sl
    * 30
    * uint 0-10000
    * Device returns status of its analog output

1. status_temperature
    * st
    * 60
    * float
    * Reports back temperature in Degrees Celsius

1. status_temperature_target
    * stt
    * 60
    * float
    * Report back temperature target in Degrees Celsius

1. control_temperature_target
    * ctt
    * 0
    * float
    * Will receive temperature setpoint in Degress Celsius

1. control_color
    * cc
    * 0
    * uint 0-10000
    * On device you will get number in this range for each color (R,G,B) when set in app

1. status_color
    * sl
    * 30
    * uint 0-10000
    * Device returns the status of the current set color

1. status_motion
    * sm
    * 30
    * bool
    * Feedback to app if motion sensor is triggered

1. control_identify
    * ci
    * 0
    * bool
    * Will receive 1 if identify is triggered
