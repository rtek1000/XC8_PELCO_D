#include <xc.h>

// Configuration bits
#pragma config FOSC = HS        // High-speed oscillator
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config MCLRE = ON       // MCLR pin function is digital input
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low-voltage programming disabled
#pragma config CPD = OFF        // Data EEPROM memory code protection off
#pragma config CP = OFF         // Flash program memory code protection off

// Define button pins
#define UP_BUTTON      RB0
#define DOWN_BUTTON    RB1
#define LEFT_BUTTON    RB2
#define RIGHT_BUTTON   RB3

// Pelco D protocol constants
#define PELCO_D_START_BYTE 0xFF
#define PELCO_D_ADDRESS    0x01  // Camera address (can be changed as needed)

// Debounce time in milliseconds
#define DEBOUNCE_TIME 200

// State machine states
typedef enum {
    STATE_IDLE,
    STATE_BUTTON_PRESSED,
    STATE_DEBOUNCE,
    STATE_WAIT_RELEASE
} State;

// Global variables
State currentState = STATE_IDLE;
unsigned char up_pressed = 0, down_pressed = 0, left_pressed = 0, right_pressed = 0;
unsigned long debounceStartTime = 0;

// Function prototypes
void USART_Init(void);
void USART_Transmit(unsigned char data);
void SendPelcoDFrame(unsigned char cmd1, unsigned char cmd2, unsigned char data1, unsigned char data2);
void DelayMs(unsigned int ms);
unsigned long GetElapsedTime(unsigned long startTime);

void main(void) {
    // Set PORTB as input for buttons
    TRISB = 0x0F;  // RB0-RB3 as input, others as output
    PORTB = 0x00;  // Clear PORTB

    // Initialize USART
    USART_Init();

    while (1) {
        // State machine implementation
        switch (currentState) {
            case STATE_IDLE:
                // Check if any button is pressed
                if (!UP_BUTTON || !DOWN_BUTTON || !LEFT_BUTTON || !RIGHT_BUTTON) {
                    currentState = STATE_BUTTON_PRESSED;
                }
                break;

            case STATE_BUTTON_PRESSED:
                // Record the time when the button was pressed
                debounceStartTime = GetElapsedTime(0);
                currentState = STATE_DEBOUNCE;
                break;

            case STATE_DEBOUNCE:
                // Wait for debounce time to elapse
                if (GetElapsedTime(debounceStartTime) >= DEBOUNCE_TIME) {
                    // Read button states after debounce
                    up_pressed = !UP_BUTTON;
                    down_pressed = !DOWN_BUTTON;
                    left_pressed = !LEFT_BUTTON;
                    right_pressed = !RIGHT_BUTTON;

                    // Check for combinations first (priority)
                    if (up_pressed && left_pressed) {
                        SendPelcoDFrame(0x00, 0x0C, 0x20, 0x20);  // Up + Left command
                    } else if (up_pressed && right_pressed) {
                        SendPelcoDFrame(0x00, 0x0A, 0x20, 0x20);  // Up + Right command
                    } else if (down_pressed && left_pressed) {
                        SendPelcoDFrame(0x00, 0x14, 0x20, 0x20);  // Down + Left command
                    } else if (down_pressed && right_pressed) {
                        SendPelcoDFrame(0x00, 0x12, 0x20, 0x20);  // Down + Right command
                    } else if (up_pressed) {
                        SendPelcoDFrame(0x00, 0x08, 0x00, 0x20);  // Up command
                    } else if (down_pressed) {
                        SendPelcoDFrame(0x00, 0x10, 0x00, 0x20);  // Down command
                    } else if (left_pressed) {
                        SendPelcoDFrame(0x00, 0x04, 0x20, 0x00);  // Left command
                    } else if (right_pressed) {
                        SendPelcoDFrame(0x00, 0x02, 0x20, 0x00);  // Right command
                    }

                    // Move to the next state
                    currentState = STATE_WAIT_RELEASE;
                }
                break;

            case STATE_WAIT_RELEASE:
                // Wait for all buttons to be released
                if (UP_BUTTON && DOWN_BUTTON && LEFT_BUTTON && RIGHT_BUTTON) {
                    currentState = STATE_IDLE;
                }
                break;
        }

        // Other routines can run here without being blocked
        // Example: Add additional tasks or logic here
    }
}

// Initialize USART for 9600 baud rate
void USART_Init(void) {
    TRISC6 = 0;  // TX pin as output
    SPBRG = 25;  // Baud rate = 9600 (for 4 MHz clock)
    TXSTAbits.SYNC = 0;  // Asynchronous mode
    TXSTAbits.TX9 = 0;   // 8-bit transmission
    TXSTAbits.TXEN = 1;  // Enable transmitter
    RCSTAbits.SPEN = 1;  // Enable serial port
}

// Transmit a byte via USART
void USART_Transmit(unsigned char data) {
    while (!TXSTAbits.TRMT);  // Wait for TX buffer to be empty
    TXREG = data;             // Transmit data
}

// Send a Pelco D frame
void SendPelcoDFrame(unsigned char cmd1, unsigned char cmd2, unsigned char data1, unsigned char data2) {
    unsigned char checksum;

    // Calculate checksum
    checksum = PELCO_D_ADDRESS + cmd1 + cmd2 + data1 + data2;

    // Send the frame
    USART_Transmit(PELCO_D_START_BYTE);
    USART_Transmit(PELCO_D_ADDRESS);
    USART_Transmit(cmd1);
    USART_Transmit(cmd2);
    USART_Transmit(data1);
    USART_Transmit(data2);
    USART_Transmit(checksum);
}

// Get elapsed time (in milliseconds)
unsigned long GetElapsedTime(unsigned long startTime) {
    // Implement a timer-based elapsed time function
    // For simplicity, this is a placeholder. Replace with actual timer code.
    static unsigned long counter = 0;
    counter++;
    return counter;  // Replace with actual elapsed time calculation
}
