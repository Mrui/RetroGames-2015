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
const int butUp = 2;
const int butDown = 0;
const int butJoy = 4;

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

int menu = 1;
char MENUITEM1[] = "Demo 1";
char MENUITEM2[] = "Menu 2";
char MENUITEM3[] = "Menu 3";
char MENUITEM4[] = "Menu 4";
char MENUITEM5[] = "Menu 5";
char MENUITEM6[] = "DEMO xD";

