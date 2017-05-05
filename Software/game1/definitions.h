/* RETROgames 2015 WS example code with working timer and
   already build some features needed for the game. Very simple
   and can easily made better.

   Based on Nokia 5100 LCD Example Code by: Jim Lindblom and Nathan Seidle
   RETROgames additions (button controls and timers) by Toni Heikkinen
   date: 26.9.2015
   license: Beerware. Feel free to use, reuse, and modify this
   code as you see fit. If you find it useful, and we meet someday,
   you can buy me a beer.
  
    Graphic LCD Pin ---------- Arduino/Galileo Pin
       1-VCC       ----------------  5V
       2-GND       ----------------  GND
       3-SCE       ----------------  6
       4-RST       ----------------  5
       5-D/C       ----------------  7
       6-DN(MOSI)  ----------------  11
       7-SCLK      ----------------  13
       8-LED       - 330 Ohm res --  9
   The SCLK, DN(MOSI), must remain where they are, but the other
   pins can be swapped. The LED pin should remain a PWM-capable
   pin. Don't forget to stick a current-limiting resistor in line
   between the LCD's LED pin and Arduino pin 9!
*/

/* PCD8544-specific defines: */
#define LCD_COMMAND  0
#define LCD_DATA     1

/* 84x48 LCD Defines: */
#define LCD_WIDTH   84 // Note: x-coordinates go wide
#define LCD_HEIGHT  48 // Note: y-coordinates go high
#define WHITE       0  // For drawing pixels. A 0 draws white.
#define BLACK       1  // A 1 draws black.

/* Pin definitions:
Most of these pins can be moved to any digital or analog pin.
DN(MOSI)and SCLK should be left where they are (SPI pins). The
LED (backlight) pin should remain on a PWM-capable pin. */
const int scePin = 6;   // SCE - Chip select, pin 3 on LCD.
const int rstPin = 5;   // RST - Reset, pin 4 on LCD.
const int dcPin = 7;    // DC - Data/Command, pin 5 on LCD.
const int sdinPin = 11;  // DN(MOSI) - Serial data, pin 6 on LCD.
const int sclkPin = 13;  // SCLK - Serial clock, pin 7 on LCD.
const int blPin = 9;    // LED - Backlight LED, pin 8 on LCD.

// Button definitions
const int butLeft = 1;
const int butRight = 3;
const int butUp = 4;
const int butDown = 0;
const int butJoy = 2;

// Joystick
const int joyY = A0;
const int joyX = A1;

// Joystick inputvalues
volatile int valueJoyX = 0;
volatile int valueJoyY = 0;

// Button last state:
int butLeftLast = HIGH;
int butRightLast = HIGH;
int butDownLast = HIGH;
int butUpLast = HIGH;
int butJoyLast = HIGH;

volatile int pushedButton = 0;
volatile byte direction = 0;
volatile int xPos = 0;
volatile int yPos = 0;
