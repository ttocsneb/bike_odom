# Bike Odometer

[![Build Status](https://travis-ci.com/ttocsneb/bike_odom.svg?token=8LjpCSuHky9LYSfsZNxq&branch=master)](https://travis-ci.com/ttocsneb/bike_odom)

A modular bke odometer that will get and display the speed and distance of the
bike. It will also have the ability to control other devices such as a
head-light/brake-light.

Bike Odometer is primarily a software base, but also providing a default
hardware. Each official hardware will be stored on a separate branch.

## Hardware Requirements

### Rotary Encoder

To get the speed of the bike, a rotary encoder type sensor will be used. The
sensor should connect to one of the interrupt <!-- TODO add link --> pins.

> In my case, a magnet and a reed switch is used as the rotary encoder.

Normally, the odometer is in sleep mode. When the wheels start turning, the
odometer will wake up (because of the interrupt). If the bike has stopped for a
prolonged amount of time, the odometer will go to sleep, waiting for the wheels
to turn again.

### USB Interface

Some settings for the odometer can only be changed through serial, so a USB
Interface will be required.

### User Input

The input used will be push buttons. I would recommend 3, though there is
nothing that says you can't have more or less. The only requirement for buttons
is that they all connect to the same PCINT port. <!-- Insert Link --> There
will be several functions that the buttons can do:

* Unit: Change display unit (SI, Imperial)
* **Display**: Change the main display mode (Speed, Trip Distance, Total Distance)
* **Reset**: Reset the trip—_Total Distance should not ever be reset_
* **Lights**: Toggle the headlights

^_**Bolded** Items are recommended button configurations_

These buttons change settings, so if a button isn't mapped, you can still
change it's setting.

### Display

A display that can output at least numbers is required—This is an odometer
after all. Aside from this requirement, the display can be anything you want.
A large amount of freedom is given to the display: a function pointer should be
provided for updating the display (it will get called only when data has
changed). The main loop is reserved for the display so that it can have as much
control as needed.

### Lights

Lights are optional, but having support for them is always good. If lights will
be supported, they should be placed on pwm available pins, even if the lights
aren't planned to be analog.

### Diagram

> TODO

## Serial Interface

All settings will be modifiable through the serial interface. To make it easier
to reconfigure, a terminal application will be developed to communicate with
the odometer for you, but until then you will have to enter the commands
directly to the odometer by hand.

There are several commands that you can use to interact with the odometer:

| Description                 | Command                                   | Return    |
|-----------------------------|-------------------------------------------|-----------|
| Read Setting                | read \[reg\]                              | \[value\] |
| Read Setting bits           | rbits \[reg\] \[bit\] \[size\]            | \[value\] |
| Write Setting               | write \[reg\] \[value\]                   |           |
| Write Setting bits          | wbits \[reg\] \[bit\] \[size\] \[value\]  |           |
| Save Settings to EEPROM     | save                                      |           |
| Reset Settings to default   | reset                                     |           |
| Reload Settings from EEPROM | reload                                    |           |
| Reboot System               | restart                                   |           |

> The register of a setting is it's location in EEPROM.

## Settings

### Rotary Encoder Settings

1. Wheel-Diameter (mm)
2. Wheel-Step-Conversion (Conversion from steps to rotations)
3. Wheel-Unit-Conversion (Conversion from meter to unit)
4. Display-Unit (up to 4 character unit name)

### Sleep Settings

5. Sleep-Time

### Button Settings

The button settings are stored in a single register, each button is 3 bits long,
and so up to 9 buttons are supported:

6. Button-Register
    * 0-2: Button-0
    * 3-5: Button-1
    * 6-8: Button-2
    * 9-11: Button-3
    * ...
    * 27-29: Button-9

#### Button Action

##### Bits 0-1

0. unit
1. display
2. reset
3. lights

##### Bit 2

0. Increase
1. Decrease

### Light Settings

There are 4 settings for the two lights each setting having for registers:

* Head-Lights-On
    7. On-Brightness
    8. On-Time
    9. Off-Brightness
    10. Off-Time
* Head-Lights-Off
    11. On-Brightness
    12. On-Time
    13. Off-Brightness
    14. Off-Time
* Head-Accelerating
    15. On-Brightness
    16. On-Time
    17. Off-Brightness
    18. Off-Time
* Head-Decelerating
    19. On-Brightness
    20. On-Time
    21. Off-Brightness
    22. Off-Time
* Tail-Lights-On
    23. On-Brightness
    24. On-Time
    25. Off-Brightness
    26. Off-Time
* Tail-Lights-Off
    27. On-Brightness
    28. On-Time
    29. Off-Brightness
    30. Off-Time
* Tail-Accelerating
    31. On-Brightness
    32. On-Time
    33. Off-Brightness
    34. Off-Time
* Tail-Decelerating
    35. On-Brightness
    36. On-Time
    37. Off-Brightness
    38. Off-Time

There are also settings for the behavior for each mode. They are consolidated
into one register using bitwise operations.

39. Light-Behaviors
    * 0-1: Head-Lights-On
    * 1-2: Head-Lights-Off
    * 2-3: Head-Lights-Accelerating
    * 3-4: Head-Lights-Decelerating
    * 4-5: Tail-Lights-On
    * 5-6: Tail-Lights-Off
    * 6-7: Tail-Lights-Accelerating
    * 7-8: Tail-Lights-Decelerating

Each Behavior can have the value as follows:

0. Inherit (): Inherit the value from Lights-On (If this is set for Lights-On,
then behavior 1 is assumed)
1. Solid (Brightness): The light should be on at a constant brightness
2. Blink (On-Brightness, On-Time, Off-Brightness, Off-Time): the light should blink
3. Sweep (On-Brightness, On-Time, Off-Brightness, Off-Time): the light should
smoothly change brightness.

> Parameters used are in parentheses.

### Data

These are not settings, but Long Term Variables (LTV), some are read only, while
others are writeable.

40. Trip-Distance
41. Trip-Max-Speed
42. Trip-Ave-Speed
43. Odometer-Distance **Read Only**
44. Odometer-Max-Speed **Read Only**
45. Odometer-Ave-Speed **Read Only**
