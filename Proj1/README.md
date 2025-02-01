Key Features:
Interrupt-Driven USART Reception:

The USART receive interrupt is used to handle incoming data.

Only one byte is processed per interrupt to minimize interrupt latency.

PELCO D Protocol Handling:

The program waits for the start byte (0xFF).

It collects the 7-byte PELCO D frame and validates the checksum.

Global Variables:

A buffer (pelcoFrame) stores the incoming PELCO D frame.

A flag (frameReady) indicates when a complete frame has been received.

-----

Explanation:
USART Initialization:

The USART module is configured for a baud rate of 9600.

The receive interrupt (RCIE) is enabled to handle incoming data.

Interrupt Service Routine (ISR):

The ISR is triggered when a byte is received.

The received byte is stored in the pelcoFrame buffer.

If a complete 7-byte frame is received, the frameReady flag is set.

Main Loop:

The main loop checks the frameReady flag.

If a complete frame is ready, it processes the frame and resets the buffer and flags.

Checksum Validation:

The checksum is calculated and validated before processing the command.

Command Processing:

Replace the example logic with your specific application requirements.

Notes:
The interrupt-driven approach ensures that the microcontroller can perform other tasks while waiting for data.

Ensure the baud rate matches the configuration of the device sending the PELCO D commands.

Debugging can be done by toggling an LED or printing debug information (if a debugger is available).
