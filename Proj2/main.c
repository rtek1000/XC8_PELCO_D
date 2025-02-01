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

// Function prototypes
void USART_Init(void);
void USART_Transmit(unsigned char data);
void SendPelcoDFrame(unsigned char cmd1, unsigned char cmd2, unsigned char data1, unsigned char data2);

void main(void) {
    // Set PORTB as input for buttons
    TRISB = 0x0F;  // RB0-RB3 as input, others as output
    PORTB = 0x00;  // Clear PORTB

    // Initialize USART
    USART_Init();

    while (1) {
        // Check for button presses and send corresponding Pelco D frame
        if (UP_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x08, 0x00, 0x20);  // Up command
            while (UP_BUTTON == 0);  // Wait for button release
        }
        if (DOWN_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x10, 0x00, 0x20);  // Down command
            while (DOWN_BUTTON == 0);  // Wait for button release
        }
        if (LEFT_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x04, 0x20, 0x00);  // Left command
            while (LEFT_BUTTON == 0);  // Wait for button release
        }
        if (RIGHT_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x02, 0x20, 0x00);  // Right command
            while (RIGHT_BUTTON == 0);  // Wait for button release
        }

        // Check for combined movements (e.g., up + left)
        if (UP_BUTTON == 0 && LEFT_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x0C, 0x20, 0x20);  // Up + Left command
            while (UP_BUTTON == 0 || LEFT_BUTTON == 0);  // Wait for both buttons to release
        }
        if (UP_BUTTON == 0 && RIGHT_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x0A, 0x20, 0x20);  // Up + Right command
            while (UP_BUTTON == 0 || RIGHT_BUTTON == 0);  // Wait for both buttons to release
        }
        if (DOWN_BUTTON == 0 && LEFT_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x14, 0x20, 0x20);  // Down + Left command
            while (DOWN_BUTTON == 0 || LEFT_BUTTON == 0);  // Wait for both buttons to release
        }
        if (DOWN_BUTTON == 0 && RIGHT_BUTTON == 0) {
            SendPelcoDFrame(0x00, 0x12, 0x20, 0x20);  // Down + Right command
            while (DOWN_BUTTON == 0 || RIGHT_BUTTON == 0);  // Wait for both buttons to release
        }
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
