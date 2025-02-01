# XC8 PELCO D
- Program generated by DeepSeek chat

-----

## Proj1:

Explanation:
- USART Initialization:

- - The USART module is configured for a baud rate of 9600.

- - The receive interrupt (RCIE) is enabled to handle incoming data.

- Interrupt Service Routine (ISR):

- - The ISR is triggered when a byte is received.

- - The received byte is stored in the pelcoFrame buffer.

- - If a complete 7-byte frame is received, the frameReady flag is set.

- Main Loop:

- - The main loop checks the frameReady flag.

- - If a complete frame is ready, it processes the frame and resets the buffer and flags.

- Checksum Validation:

- - The checksum is calculated and validated before processing the command.

- Command Processing:

- - Replace the example logic with your specific application requirements.

- Notes:
- - The interrupt-driven approach ensures that the microcontroller can perform other tasks while waiting for data.

- - Ensure the baud rate matches the configuration of the device sending the PELCO D commands.

- - Debugging can be done by toggling an LED or printing debug information (if a debugger is available).

-----

## Proj2:

Explanation:

- Button Inputs:
- - The program monitors RB0-RB3 for button presses. When a button is pressed (logic low), the corresponding Pelco D frame is sent.

- Pelco D Frame:
- - The frame consists of 7 bytes:

- - - Start byte: 0xFF

- - - Address: 0x01 (can be changed as needed)

- - - Command 1 and Command 2: Define the movement (up, down, left, right, or combinations).

- - - Data 1 and Data 2: Define the speed (fixed at 0x20 for speed 1).

- - - Checksum: Sum of bytes 2 to 6.

- USART:
- - The USART is configured for 9600 baud, 8-bit data, no parity, and 1 stop bit.

- Combined Movements:
- - The program supports combined movements (e.g., up + left) by checking multiple buttons simultaneously.

- Notes:
- - The speed is fixed at 0x20 (speed 1) as per your requirement.

- - The program waits for the button to be released before continuing, to avoid sending multiple frames for a single press.

- - You can expand the program to include more commands or features as needed.
