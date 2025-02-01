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

// Define baud rate for USART
#define BAUD_RATE 9600
#define _XTAL_FREQ 4000000      // 4 MHz oscillator frequency

// Global variables
volatile uint8_t pelcoFrame[7]; // Buffer to store PELCO D frame
volatile uint8_t frameIndex = 0; // Index for storing received bytes
volatile uint8_t frameReady = 0; // Flag to indicate a complete frame is ready

// Function prototypes
void USART_Init(void);
void ProcessPelcoD(uint8_t *data);

void main(void) {
    // Initialize USART
    USART_Init();

    // Enable global interrupts
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;

    while (1) {
        // Check if a complete PELCO D frame is ready
        if (frameReady) {
            // Process the PELCO D frame
            ProcessPelcoD((uint8_t *)pelcoFrame);

            // Reset the frame buffer and flags
            frameIndex = 0;
            frameReady = 0;
        }
    }
}

// Initialize USART
void USART_Init(void) {
    // Set baud rate
    SPBRG = (_XTAL_FREQ / (16 * BAUD_RATE)) - 1;

    // Enable USART transmitter and receiver
    TXSTAbits.TXEN = 1; // Enable transmitter
    RCSTAbits.SPEN = 1; // Enable serial port
    RCSTAbits.CREN = 1; // Enable continuous reception

    // Enable USART receive interrupt
    PIE1bits.RCIE = 1;

    // Set TX and RX pins as input/output
    TRISBbits.TRISB1 = 1; // RX pin as input
    TRISBbits.TRISB2 = 0; // TX pin as output
}

// USART receive interrupt service routine
void __interrupt() ISR(void) {
    if (PIR1bits.RCIF) { // Check if USART receive interrupt occurred
        uint8_t receivedByte = RCREG; // Read the received byte

        // Wait for the start byte (0xFF)
        if (frameIndex == 0 && receivedByte == 0xFF) {
            pelcoFrame[frameIndex++] = receivedByte; // Store the start byte
        } else if (frameIndex > 0 && frameIndex < 7) {
            pelcoFrame[frameIndex++] = receivedByte; // Store the next bytes
        }

        // Check if a complete frame has been received
        if (frameIndex == 7) {
            frameReady = 1; // Set the frame ready flag
        }

        PIR1bits.RCIF = 0; // Clear the interrupt flag
    }
}

// Process PELCO D frame
void ProcessPelcoD(uint8_t *data) {
    // Calculate checksum (sum of bytes 2 to 6)
    uint8_t checksum = data[1] + data[2] + data[3] + data[4] + data[5];

    // Verify checksum
    if (checksum == data[6]) {
        // Checksum is valid, process the command
        uint8_t address = data[1];
        uint8_t command1 = data[2];
        uint8_t command2 = data[3];
        uint8_t data1 = data[4];
        uint8_t data2 = data[5];

        // Example: Print the received command (for debugging)
        // You can replace this with your actual logic
        if (command1 == 0x00 && command2 == 0x08) {
            // Example: Move camera up
            // Implement your logic here
        } else if (command1 == 0x00 && command2 == 0x10) {
            // Example: Move camera down
            // Implement your logic here
        }
    } else {
        // Invalid checksum, ignore the frame
    }
}
