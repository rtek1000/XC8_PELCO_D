# XC8 PELCO D
- Program generated by DeepSeek chat

-----

## Proj1:

Key Features: Interrupt-Driven USART Reception:

The USART receive interrupt is used to handle incoming data.

Only one byte is processed per interrupt to minimize interrupt latency.

PELCO D Protocol Handling:

The program waits for the start byte (0xFF).

It collects the 7-byte PELCO D frame and validates the checksum.

Global Variables:

A buffer (pelcoFrame) stores the incoming PELCO D frame.

A flag (frameReady) indicates when a complete frame has been received.

-----

## Proj2:

This program monitors 4 input pins (for up, down, left, and right movements) and sends the corresponding Pelco D protocol frame via USART at 9600 baud when a button is pressed.

Assumptions: The 4 buttons are connected to PORTB (RB0, RB1, RB2, RB3).

The USART TX pin is connected to RC6.

The microcontroller is running at 4 MHz (for 9600 baud rate).

To handle button combinations with priority and a validation time of at least 200ms, we can implement a debouncing mechanism and a priority system. The idea is to:

Debounce the buttons: Ensure that button presses are stable for at least 200ms before considering them valid.

Prioritize combinations: Check for combinations first, and if no combination is detected, check for single-button presses.

It is possible to implement the functionality without blocking the program while waiting for the button to be released. This can be achieved by using a state machine and non-blocking code. The idea is to:

Use a state machine to track the current state of the system (e.g., idle, button pressed, debouncing, etc.).

Use timers to handle debouncing and button release detection without blocking the main loop.

Allow other routines to run concurrently by avoiding while loops that wait for button releases.
