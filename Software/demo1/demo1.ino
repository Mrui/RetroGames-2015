/* RetroGames 2015
   A simple demo using Nokia 5110 Graphic LCD with no button functionality.
   Based on Nokia 5100 LCD Example Code by Jim Lindblom.
   
   by: Toni Heikkinen
*/
#include <SPI.h> // We'll use SPI to transfer data. Faster!
#include "definitions.h"
#include "bitmaps.h"
#include "TimerOne.h"

/* Font table:
This table contains the hex values that represent pixels for a
font that is 5 pixels wide and 8 pixels high. Each byte in a row
represents one, 8-pixel, vertical column of a character. 5 bytes
per character. */
static const byte ASCII[][5] = {
  // First 32 characters (0x00-0x19) are ignored. These are
  // non-displayable, control characters.
   {0x00, 0x00, 0x00, 0x00, 0x00} // 0x20
  ,{0x00, 0x00, 0x5f, 0x00, 0x00} // 0x21 !
  ,{0x00, 0x07, 0x00, 0x07, 0x00} // 0x22 "
  ,{0x14, 0x7f, 0x14, 0x7f, 0x14} // 0x23 #
  ,{0x24, 0x2a, 0x7f, 0x2a, 0x12} // 0x24 $
  ,{0x23, 0x13, 0x08, 0x64, 0x62} // 0x25 %
  ,{0x36, 0x49, 0x55, 0x22, 0x50} // 0x26 &
  ,{0x00, 0x05, 0x03, 0x00, 0x00} // 0x27 '
  ,{0x00, 0x1c, 0x22, 0x41, 0x00} // 0x28 (
  ,{0x00, 0x41, 0x22, 0x1c, 0x00} // 0x29 )
  ,{0x14, 0x08, 0x3e, 0x08, 0x14} // 0x2a *
  ,{0x08, 0x08, 0x3e, 0x08, 0x08} // 0x2b +
  ,{0x00, 0x50, 0x30, 0x00, 0x00} // 0x2c ,
  ,{0x08, 0x08, 0x08, 0x08, 0x08} // 0x2d -
  ,{0x00, 0x60, 0x60, 0x00, 0x00} // 0x2e .
  ,{0x20, 0x10, 0x08, 0x04, 0x02} // 0x2f /
  ,{0x3e, 0x51, 0x49, 0x45, 0x3e} // 0x30 0
  ,{0x00, 0x42, 0x7f, 0x40, 0x00} // 0x31 1
  ,{0x42, 0x61, 0x51, 0x49, 0x46} // 0x32 2
  ,{0x21, 0x41, 0x45, 0x4b, 0x31} // 0x33 3
  ,{0x18, 0x14, 0x12, 0x7f, 0x10} // 0x34 4
  ,{0x27, 0x45, 0x45, 0x45, 0x39} // 0x35 5
  ,{0x3c, 0x4a, 0x49, 0x49, 0x30} // 0x36 6
  ,{0x01, 0x71, 0x09, 0x05, 0x03} // 0x37 7
  ,{0x36, 0x49, 0x49, 0x49, 0x36} // 0x38 8
  ,{0x06, 0x49, 0x49, 0x29, 0x1e} // 0x39 9
  ,{0x00, 0x36, 0x36, 0x00, 0x00} // 0x3a :
  ,{0x00, 0x56, 0x36, 0x00, 0x00} // 0x3b ;
  ,{0x08, 0x14, 0x22, 0x41, 0x00} // 0x3c <
  ,{0x14, 0x14, 0x14, 0x14, 0x14} // 0x3d =
  ,{0x00, 0x41, 0x22, 0x14, 0x08} // 0x3e >
  ,{0x02, 0x01, 0x51, 0x09, 0x06} // 0x3f ?
  ,{0x32, 0x49, 0x79, 0x41, 0x3e} // 0x40 @
  ,{0x7e, 0x11, 0x11, 0x11, 0x7e} // 0x41 A
  ,{0x7f, 0x49, 0x49, 0x49, 0x36} // 0x42 B
  ,{0x3e, 0x41, 0x41, 0x41, 0x22} // 0x43 C
  ,{0x7f, 0x41, 0x41, 0x22, 0x1c} // 0x44 D
  ,{0x7f, 0x49, 0x49, 0x49, 0x41} // 0x45 E
  ,{0x7f, 0x09, 0x09, 0x09, 0x01} // 0x46 F
  ,{0x3e, 0x41, 0x49, 0x49, 0x7a} // 0x47 G
  ,{0x7f, 0x08, 0x08, 0x08, 0x7f} // 0x48 H
  ,{0x00, 0x41, 0x7f, 0x41, 0x00} // 0x49 I
  ,{0x20, 0x40, 0x41, 0x3f, 0x01} // 0x4a J
  ,{0x7f, 0x08, 0x14, 0x22, 0x41} // 0x4b K
  ,{0x7f, 0x40, 0x40, 0x40, 0x40} // 0x4c L
  ,{0x7f, 0x02, 0x0c, 0x02, 0x7f} // 0x4d M
  ,{0x7f, 0x04, 0x08, 0x10, 0x7f} // 0x4e N
  ,{0x3e, 0x41, 0x41, 0x41, 0x3e} // 0x4f O
  ,{0x7f, 0x09, 0x09, 0x09, 0x06} // 0x50 P
  ,{0x3e, 0x41, 0x51, 0x21, 0x5e} // 0x51 Q
  ,{0x7f, 0x09, 0x19, 0x29, 0x46} // 0x52 R
  ,{0x46, 0x49, 0x49, 0x49, 0x31} // 0x53 S
  ,{0x01, 0x01, 0x7f, 0x01, 0x01} // 0x54 T
  ,{0x3f, 0x40, 0x40, 0x40, 0x3f} // 0x55 U
  ,{0x1f, 0x20, 0x40, 0x20, 0x1f} // 0x56 V
  ,{0x3f, 0x40, 0x38, 0x40, 0x3f} // 0x57 W
  ,{0x63, 0x14, 0x08, 0x14, 0x63} // 0x58 X
  ,{0x07, 0x08, 0x70, 0x08, 0x07} // 0x59 Y
  ,{0x61, 0x51, 0x49, 0x45, 0x43} // 0x5a Z
  ,{0x00, 0x7f, 0x41, 0x41, 0x00} // 0x5b [
  ,{0x02, 0x04, 0x08, 0x10, 0x20} // 0x5c \
  ,{0x00, 0x41, 0x41, 0x7f, 0x00} // 0x5d ]
  ,{0x04, 0x02, 0x01, 0x02, 0x04} // 0x5e ^
  ,{0x40, 0x40, 0x40, 0x40, 0x40} // 0x5f _
  ,{0x00, 0x01, 0x02, 0x04, 0x00} // 0x60 `
  ,{0x20, 0x54, 0x54, 0x54, 0x78} // 0x61 a
  ,{0x7f, 0x48, 0x44, 0x44, 0x38} // 0x62 b
  ,{0x38, 0x44, 0x44, 0x44, 0x20} // 0x63 c
  ,{0x38, 0x44, 0x44, 0x48, 0x7f} // 0x64 d
  ,{0x38, 0x54, 0x54, 0x54, 0x18} // 0x65 e
  ,{0x08, 0x7e, 0x09, 0x01, 0x02} // 0x66 f
  ,{0x0c, 0x52, 0x52, 0x52, 0x3e} // 0x67 g
  ,{0x7f, 0x08, 0x04, 0x04, 0x78} // 0x68 h
  ,{0x00, 0x44, 0x7d, 0x40, 0x00} // 0x69 i
  ,{0x20, 0x40, 0x44, 0x3d, 0x00} // 0x6a j
  ,{0x7f, 0x10, 0x28, 0x44, 0x00} // 0x6b k
  ,{0x00, 0x41, 0x7f, 0x40, 0x00} // 0x6c l
  ,{0x7c, 0x04, 0x18, 0x04, 0x78} // 0x6d m
  ,{0x7c, 0x08, 0x04, 0x04, 0x78} // 0x6e n
  ,{0x38, 0x44, 0x44, 0x44, 0x38} // 0x6f o
  ,{0x7c, 0x14, 0x14, 0x14, 0x08} // 0x70 p
  ,{0x08, 0x14, 0x14, 0x18, 0x7c} // 0x71 q
  ,{0x7c, 0x08, 0x04, 0x04, 0x08} // 0x72 r
  ,{0x48, 0x54, 0x54, 0x54, 0x20} // 0x73 s
  ,{0x04, 0x3f, 0x44, 0x40, 0x20} // 0x74 t
  ,{0x3c, 0x40, 0x40, 0x20, 0x7c} // 0x75 u
  ,{0x1c, 0x20, 0x40, 0x20, 0x1c} // 0x76 v
  ,{0x3c, 0x40, 0x30, 0x40, 0x3c} // 0x77 w
  ,{0x44, 0x28, 0x10, 0x28, 0x44} // 0x78 x
  ,{0x0c, 0x50, 0x50, 0x50, 0x3c} // 0x79 y
  ,{0x44, 0x64, 0x54, 0x4c, 0x44} // 0x7a z
  ,{0x00, 0x08, 0x36, 0x41, 0x00} // 0x7b {
  ,{0x00, 0x00, 0x7f, 0x00, 0x00} // 0x7c |
  ,{0x00, 0x41, 0x36, 0x08, 0x00} // 0x7d }
  ,{0x10, 0x08, 0x08, 0x10, 0x08} // 0x7e ~
  ,{0x78, 0x46, 0x41, 0x46, 0x78} // 0x7f DEL
};

/* The displayMap variable stores a buffer representation of the
pixels on our display. There are 504 total bits in this array,
same as how many pixels there are on a 84 x 48 display.

Each byte in this array covers a 8-pixel vertical block on the
display. Each successive byte covers the next 8-pixel column over
until you reach the right-edge of the display and step down 8 rows.

To update the display, we first have to write to this array, then
call the updateDisplay() function, which sends this whole array
to the PCD8544.

Because the PCD8544 won't let us write individual pixels at a
time, this is how we can make targeted changes to the display. */
byte displayMap[LCD_WIDTH * LCD_HEIGHT / 8] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0xC0,
0x20, 0x10, 0x10, 0x10, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0xF0, 0x10, 0x10, 0x00, 0x40, 0x40,
0x40, 0x80, 0x00, 0x00, 0xC0, 0x40, 0x40, 0x80, 0x40, 0x40, 0x80, 0x00, 0x00, 0xC0, 0x80, 0x40,
0x40, 0x80, 0x00, 0x00, 0x80, 0x40, 0x40, 0x80, 0x00, 0x00, 0xC0, 0x80, 0x40, 0x00, 0x80, 0x40,
0x40, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x08,
0x08, 0x00, 0x00, 0x03, 0x04, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x00,
0x00, 0x00, 0x06, 0x09, 0x09, 0x0F, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x0F, 0x00,
0x00, 0x3F, 0x08, 0x08, 0x08, 0x07, 0x00, 0x00, 0x07, 0x09, 0x09, 0x09, 0x00, 0x00, 0x0F, 0x00,
0x00, 0x00, 0x07, 0x09, 0x09, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x10, 0x08, 0x08, 0xF0,
0x00, 0x00, 0xF8, 0x10, 0x08, 0x00, 0xF0, 0x28, 0x28, 0x30, 0x00, 0x00, 0x10, 0x28, 0xC8, 0x00,
0x00, 0xF0, 0x28, 0x28, 0x30, 0x00, 0x00, 0xF8, 0x10, 0x08, 0x08, 0xF0, 0x00, 0x08, 0xFE, 0x08,
0x08, 0x00, 0x10, 0x28, 0xC8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07,
0x01, 0x01, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00,
0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01,
0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// Timer1 handler
void timer1Handler()
{
  setStr("K", 0, 0, BLACK);
  updateDisplay();
}

void setup()
{
  Serial.begin(9600);

  lcdBegin(); // This will setup our pins, and initialize the LCD
  analogWrite(blPin, 0); // blPin is ocnnected to BL LED
  setBitmap(print_logo);
  updateDisplay(); // with displayMap untouched, SFE logo
  setContrast(60); // Good values range from 40-60

  delay(2000);

  //lcdFunTime(); // Runs a 30-second demo of graphics functions

  // Wait for serial to come in, then clear display and go to echo
//  while (!Serial.available())
//    ;
  clearDisplay(WHITE);
  updateDisplay();
}

// Loop turns the display into a local serial monitor echo.
// Type to the Arduino from the serial monitor, and it'll echo
// what you type on the display. Type ~ to clear the display.
void loop()
{
  static int cursorX = 0;
  static int cursorY = 0;
  static char tmp1c[5];
  static char tmp2c[5];
  if (digitalRead(butLeft) == LOW && butLeftLast == HIGH)
  {
    pushedButton = 0;
    butLeftLast = LOW;
  }
  if (digitalRead(butRight) == LOW && butRightLast == HIGH)
  {
    pushedButton = 1;
    butRightLast = LOW;
  }
  if (digitalRead(butDown) == LOW && butDownLast == HIGH)
  {
    pushedButton = 2;
    butDownLast = LOW;
  }
  if (digitalRead(butUp) == LOW && butUpLast == HIGH)
  {
    pushedButton = 3;
    butUpLast == LOW;
  }
  if (digitalRead(butJoy) == LOW && butJoyLast == HIGH)
  {
    pushedButton = 4;
    butJoyLast = LOW;
  }

  if (digitalRead(butRight) == HIGH)
  {
    butRightLast = HIGH;
  }
  if (digitalRead(butDown) == HIGH)
  {
    butDownLast = HIGH;
  }
  if (digitalRead(butUp) == HIGH)
  {
    butUpLast = HIGH;
  }
  if (digitalRead(butLeft) == HIGH)
  {
    butLeftLast = HIGH;
  }

  valueJoyX = analogRead(joyX);
  valueJoyY = analogRead(joyY);

  clearDisplay(WHITE);

  switch ( pushedButton ) {
    case 0:
      setStr("left", 0, 0, BLACK);
      if (menu == 6)
      {
        lcdFunTime();
        menu = 1;
      }
      else if (menu == 1)
        demonstration1();
    break;
    case 1:
      setStr("right", 0, 0, BLACK);
    break;
    case 2:
      setStr("down", 0, 0, BLACK);
      if (menu < 6)
        menu++;
    break;
    case 3:
      setStr("up", 0, 0, BLACK);
      if (menu > 1)
        menu--;
    break;
    case 4:
      setStr("joy", 0, 0, BLACK);
    break;
    default:
    break;
  }

  setStr(MENUITEM1, 0, 8, BLACK);
  setStr(MENUITEM2, 0, 16, BLACK);
  setStr(MENUITEM3, 0, 24, BLACK);
  setStr(MENUITEM4, 42, 8, BLACK);
  setStr(MENUITEM5, 42, 16, BLACK);
  setStr(MENUITEM6, 42, 24, BLACK);
  switch ( menu ) {
    case 1:
      setStr(MENUITEM1, 0, 8, WHITE);
    break;
    case 2:
      setStr(MENUITEM2, 0, 16, WHITE);
    break;
    case 3:
      setStr(MENUITEM3, 0, 24, WHITE);
    break;
    case 4:
      setStr(MENUITEM4, 42, 8, WHITE);
    break;
    case 5:
      setStr(MENUITEM5, 42, 16, WHITE);
    break;
    case 6:
      setStr(MENUITEM6, 42, 24, WHITE);
    break;
    default:
    break;
  }

  setStr("X:", 40, 32, BLACK);
  String tmp1 = String(valueJoyX);
  tmp1.toCharArray(tmp1c, 5);
  setStr(tmp1c, 50, 32, BLACK);

  setStr("Y:", 40, 40, BLACK);
  String tmp2 = String(valueJoyY);
  tmp2.toCharArray(tmp2c, 5);
  setStr(tmp2c, 50, 40, BLACK);

  updateDisplay();
  pushedButton = 50;
//  if (Serial.available())
//  {
//    char c = Serial.read();
//
//    switch (c)
//    {
//    case '\n': // New line
//      cursorY += 8;
//      break;
//    case '\r': // Return feed
//      cursorX = 0;
//      break;
//    case '~': // Use ~ to clear the screen.
//      clearDisplay(WHITE);
//      updateDisplay();
//      cursorX = 0; // reset the cursor
//      cursorY = 0;
//      break;
//    default:
//      setChar(c, cursorX, cursorY, BLACK);
//      updateDisplay();
//      cursorX += 6; // Increment cursor
//      break;
//    }
//    // Manage cursor
//    if (cursorX >= (LCD_WIDTH - 4))
//    { // If the next char will be off screen...
//      cursorX = 0; // ... reset x to 0...
//      cursorY += 8; // ...and increment to next line.
//      if (cursorY >= (LCD_HEIGHT - 7))
//      { // If the next line takes us off screen...
//        cursorY = 0; // ...go back to the top.
//      }
//    }
//  }
}

/* Demonstration 1 about buttons with no timer enabled. */
void demonstration1()
{
  int end = 0;
  int xPos = 15;
  int yPos = 8;
  menu = 1;
  pushedButton = 4;
  clearDisplay(WHITE);
  setStr("Demonstration 1", 0, 0, BLACK);
  setStr("Digital", 0, 8, BLACK);
  setStr("inputs...", 10, 16, BLACK);
  setStr("No timers", 0, 24, BLACK);
  updateDisplay();
  delay(2500);
  while ( end == 0 )
  {
    if (digitalRead(butLeft) == LOW)
    {
      if (butLeftLast == HIGH && xPos > 4)
          xPos = xPos-5;
      butRightLast = LOW;
    }
    else if (digitalRead(butRight) == LOW)
    {
      if (butRightLast == HIGH && xPos < 78)
          xPos = xPos+5;
      butRightLast = LOW;
    }
    else if (digitalRead(butDown) == LOW)
    {
      if (butDownLast == HIGH && yPos < 40)
        yPos = yPos+3;
      else if (yPos > 40)
        yPos = 4;
      butDownLast = LOW;
    }
    else if (digitalRead(butUp) == LOW)
    {
      if (butUpLast == HIGH && yPos > 4)
        yPos = yPos-3;
      else if (yPos < 4)
        yPos = 40;
      butUpLast == LOW;
    }
    else if (digitalRead(butJoy) == LOW)
    {
      if (butJoyLast == HIGH)
        end = 1;
    }

    if (digitalRead(butLeft) == HIGH)
      butLeftLast = HIGH;
    if (digitalRead(butRight) == HIGH)
      butRightLast = HIGH;
    if (digitalRead(butDown) == HIGH)
      butDownLast = HIGH;
    if (digitalRead(butUp) == HIGH)
      butUpLast = HIGH;
    if (digitalRead(butJoy) == HIGH)
      butJoyLast == HIGH;
    clearDisplay(WHITE);
    setCircle(xPos, yPos, 3, BLACK, 1);
    updateDisplay();
  }
}
/* This function serves as a fun demo of the graphics driver
functions below. */
void lcdFunTime()
{
  setBitmap(print_logo);
  updateDisplay();
  delay(1500);
  clearDisplay(WHITE); // Begin by clearing the display
  setStr("presents",18, 20, BLACK);
  updateDisplay();
  delay(1500);
  randomSeed(analogRead(A0));
  setBitmap(welcome2);
  updateDisplay();
  delay(3000);

  setBitmap(demo1);
  updateDisplay();
  delay(150);
  setBitmap(demo2);
  updateDisplay();
  delay(150);
  setBitmap(demo3);
  updateDisplay();
  delay(150);
  setBitmap(demo4);
  updateDisplay();
  delay(150);
  setBitmap(demo5);
  updateDisplay();
  delay(150);
  setBitmap(demo6);
  updateDisplay();
  delay(150);
  setBitmap(demo7);
  updateDisplay();
  delay(150);
  setBitmap(demo8);
  updateDisplay();
  delay(150);

//  clearDisplay(WHITE);

  /* setPixel Example */
  const int pixelCount = 100;
  for (int i=0; i<pixelCount; i++)
  {
    // setPixel takes 2 to 3 parameters. The first two params
    // are x and y variables. The third optional variable is
    // a "color" boolean. 1 for black, 0 for white.
    // setPixel() with two variables will set the pixel with
    // the color set to black.
    // clearPixel() will call setPixel with with color set to
    // white.
    setPixel(random(0, LCD_WIDTH), random(0, LCD_HEIGHT-15));
    // After drawing something, we must call updateDisplay()
    // to actually make the display draw something new.

 //    delay(2);
  }
  updateDisplay();
  delay(1000);
  // Seizure time!!! Err...demoing invertDisplay()
  for (int i=0; i<5; i++)
  {
    invertDisplay(); // This will swap all bits in our display
    delay(200);
    invertDisplay(); // This will get us back to where we started
    delay(200);
  }
  delay(2000);

  /* setLine Example */
  clearDisplay(WHITE); // Start fresh
//  int x0 = LCD_WIDTH/2;
//  int y0 = LCD_HEIGHT/2;
//  for (float i=0; i<2*PI; i+=PI/8)
//  {
//    // Time to whip out some maths:
//    const int lineLength = 24;
//    int x1 = x0 + lineLength * sin(i);
//    int y1 = y0 + lineLength * cos(i);
//
//    // setLine(x0, y0, x1, y1, bw) takes five variables. The
//    // first four are coordinates for the start and end of the
//    // line. The last variable is the color (1=black, 0=white).
//    setLine(x0, y0, x1, y1, BLACK);
//    updateDisplay();
//    delay(100);
//  }

  setStr("Intel Galileo", 2, 10, BLACK);
  setStr("embedded", 10, 18, BLACK);
  setStr("system", 15, 26, BLACK);
  updateDisplay();
  delay(1000);

  // Demo some backlight tuning
//  for (int j=0; j<2; j++)
//  {
//    for (int i=255; i>=0; i-=5)
//    {
//      analogWrite(blPin, i); // blPin is ocnnected to BL LED
//      delay(20);
//    }
//    for (int i=0; i<256; i+=5)
//    {
//      analogWrite(blPin, i);
//      delay(20);
//    }
//  }
  clearDisplay(WHITE);
  setStr("We offer you:", 0, 0, BLACK);
  updateDisplay();
  delay(700);
  setStr("-Programming!", 5, 8, BLACK);
  updateDisplay();
  delay(700);
  setStr("-Food!!", 5, 16, BLACK);
  updateDisplay();
  delay(700);
  setStr("-Cool people!", 5, 24, BLACK);
  updateDisplay();
  delay(700);
  setStr("-Sauna!", 5, 32, BLACK);
  updateDisplay();
  delay(2000);
  /* setRect Example */
  clearDisplay(WHITE); // Start fresh



  // setRect takes six parameters (x0, y0, x1, y0, fill, bw)
  // x0, y0, x1, and y0 are the two diagonal corner coordinates
  // fill is a boolean, which determines if the rectangle is
  // filled in. bw determines the color 0=white, 1=black.
  for (int x=0; x<LCD_WIDTH; x+=8)
  { // Swipe right black
    setRect(0, 0, x, LCD_HEIGHT, 1, BLACK);
    updateDisplay();
    delay(10);
  }
  for (int x=0; x<LCD_WIDTH; x+=8)
  { // Swipe right white
    setRect(0, 0, x, LCD_HEIGHT, 1, WHITE);
    updateDisplay();
    delay(10);
  }
  for (int x=0; x<12; x++)
  { // Shutter swipe
    setRect(0, 0, x, LCD_HEIGHT, 1, 1);
    setRect(11, 0, x+12, LCD_HEIGHT, 1, BLACK);
    setRect(23, 0, x+24, LCD_HEIGHT, 1, BLACK);
    setRect(35, 0, x+36, LCD_HEIGHT, 1, BLACK);
    setRect(47, 0, x+48, LCD_HEIGHT, 1, BLACK);
    setRect(59, 0, x+60, LCD_HEIGHT, 1, BLACK);
    setRect(71, 0, x+72, LCD_HEIGHT, 1, BLACK);
    updateDisplay();
    delay(10);
  }
  // 3 Dee!
  setRect(25, 10, 45, 30, 0, WHITE);
  setRect(35, 20, 55, 40, 0, WHITE);
  setLine(25, 10, 35, 20, WHITE);
  setLine(45, 30, 55, 40, WHITE);
  setLine(25, 30, 35, 40, WHITE);
  setLine(45, 10, 55, 20, WHITE);
  updateDisplay();
  delay(1000);

  clearDisplay(WHITE);

  setStr("AWESOME", 0, 0, BLACK);
  setStr("GRAPHICS", 15, 8, BLACK);
  updateDisplay();
  delay(700);
  setStr("FULL HD", 5, 16, BLACK);
  updateDisplay();
  delay(700);
  setStr("(almost)", 25, 24, BLACK);
  updateDisplay();
  delay(3000);
  /* setCircle Example */
  clearDisplay(WHITE);
  // setCircle takes 5 parameters -- x0, y0, radius, bw, and
  // lineThickness. x0 and y0 are the center coords of the circ.
  // radius is the...radius. bw is the color (0=white, 1=black)
  // lineThickness is the line width of the circle, 1 = smallest
  // thickness moves in towards center.
  for (int i=0; i<20; i++)
  {
    int x = random(0, LCD_WIDTH);
    int y = random(0, LCD_HEIGHT);

    setCircle(x, y, i, BLACK, 1);
    updateDisplay();
    delay(100);
  }
  delay(1000);

  /* setChar & setStr Example */
  // setStr takes 4 parameters: an array of characters to print,
  // x and y coordinates for the top-left corner. And a color
  setStr("EBIN xD", 14, 10, WHITE);
  updateDisplay();
  delay(2000);

  /* setBitmap Example */
  // setBitmap takes one parameter, an array of the same size
  // as our screen.
  setBitmap(end_logo);
  updateDisplay();
  delay(2000);
}
// Because I keep forgetting to put bw variable in when setting...
void setPixel(int x, int y)
{
  setPixel(x, y, BLACK); // Call setPixel with bw set to Black
}

void clearPixel(int x, int y)
{
  setPixel(x, y, WHITE); // call setPixel with bw set to white
}

// This function sets a pixel on displayMap to your preferred
// color. 1=Black, 0= white.
void setPixel(int x, int y, boolean bw)
{
  // First, double check that the coordinate is in range.
  if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT))
  {
    byte shift = y % 8;

    if (bw) // If black, set the bit.
      displayMap[x + (y/8)*LCD_WIDTH] |= 1<<shift;
    else   // If white clear the bit.
      displayMap[x + (y/8)*LCD_WIDTH] &= ~(1<<shift);
  }
}

// setLine draws a line from x0,y0 to x1,y1 with the set color.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void setLine(int x0, int y0, int x1, int y1, boolean bw)
{
  int dy = y1 - y0; // Difference between y0 and y1
  int dx = x1 - x0; // Difference between x0 and x1
  int stepx, stepy;

  if (dy < 0)
  {
    dy = -dy;
    stepy = -1;
  }
  else
    stepy = 1;

  if (dx < 0)
  {
    dx = -dx;
    stepx = -1;
  }
  else
    stepx = 1;

  dy <<= 1; // dy is now 2*dy
  dx <<= 1; // dx is now 2*dx
  setPixel(x0, y0, bw); // Draw the first pixel.

  if (dx > dy)
  {
    int fraction = dy - (dx >> 1);
    while (x0 != x1)
    {
      if (fraction >= 0)
      {
        y0 += stepy;
        fraction -= dx;
      }
      x0 += stepx;
      fraction += dy;
      setPixel(x0, y0, bw);
    }
  }
  else
  {
    int fraction = dx - (dy >> 1);
    while (y0 != y1)
    {
      if (fraction >= 0)
      {
        x0 += stepx;
        fraction -= dy;
      }
      y0 += stepy;
      fraction += dx;
      setPixel(x0, y0, bw);
    }
  }
}

// setRect will draw a rectangle from x0,y0 top-left corner to
// a x1,y1 bottom-right corner. Can be filled with the fill
// parameter, and colored with bw.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void setRect(int x0, int y0, int x1, int y1, boolean fill, boolean bw)
{
  // check if the rectangle is to be filled
  if (fill == 1)
  {
    int xDiff;

    if(x0 > x1)
      xDiff = x0 - x1; //Find the difference between the x vars
    else
      xDiff = x1 - x0;

    while(xDiff > 0)
    {
      setLine(x0, y0, x0, y1, bw);

      if(x0 > x1)
        x0--;
      else
        x0++;

      xDiff--;
    }
  }
  else
  {
    // best way to draw an unfilled rectangle is to draw four lines
    setLine(x0, y0, x1, y0, bw);
    setLine(x0, y1, x1, y1, bw);
    setLine(x0, y0, x0, y1, bw);
    setLine(x1, y0, x1, y1, bw);
  }
}

// setCircle draws a circle centered around x0,y0 with a defined
// radius. The circle can be black or white. And have a line
// thickness ranging from 1 to the radius of the circle.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void setCircle (int x0, int y0, int radius, boolean bw, int lineThickness)
{
  for(int r = 0; r < lineThickness; r++)
  {
    int f = 1 - radius;
    int ddF_x = 0;
    int ddF_y = -2 * radius;
    int x = 0;
    int y = radius;

    setPixel(x0, y0 + radius, bw);
    setPixel(x0, y0 - radius, bw);
    setPixel(x0 + radius, y0, bw);
    setPixel(x0 - radius, y0, bw);

    while(x < y)
    {
      if(f >= 0)
      {
        y--;
        ddF_y += 2;
        f += ddF_y;
      }
      x++;
      ddF_x += 2;
      f += ddF_x + 1;

      setPixel(x0 + x, y0 + y, bw);
      setPixel(x0 - x, y0 + y, bw);
      setPixel(x0 + x, y0 - y, bw);
      setPixel(x0 - x, y0 - y, bw);
      setPixel(x0 + y, y0 + x, bw);
      setPixel(x0 - y, y0 + x, bw);
      setPixel(x0 + y, y0 - x, bw);
      setPixel(x0 - y, y0 - x, bw);
    }
    radius--;
  }
}

// This function will draw a char (defined in the ASCII table
// near the beginning of this sketch) at a defined x and y).
// The color can be either black (1) or white (0).
void setChar(char character, int x, int y, boolean bw)
{
  byte column; // temp byte to store character's column bitmap
  for (int i=0; i<5; i++) // 5 columns (x) per character
  {
    column = ASCII[character - 0x20][i];
    for (int j=0; j<8; j++) // 8 rows (y) per character
    {
      if (column & (0x01 << j)) // test bits to set pixels
        setPixel(x+i, y+j, bw);
      else
        setPixel(x+i, y+j, !bw);
    }
  }
}

// setStr draws a string of characters, calling setChar with
// progressive coordinates until it's done.
// This function was grabbed from the SparkFun ColorLCDShield
// library.
void setStr(char * dString, int x, int y, boolean bw)
{
  while (*dString != 0x00) // loop until null terminator
  {
    setChar(*dString++, x, y, bw);
    x+=5;
    for (int i=y; i<y+8; i++)
    {
      setPixel(x, i, !bw);
    }
    x++;
    if (x > (LCD_WIDTH - 5)) // Enables wrap around
    {
      x = 0;
      y += 8;
    }
  }
}

// This function will draw an array over the screen. (For now) the
// array must be the same size as the screen, covering the entirety
// of the display.
void setBitmap(char * bitArray)
{
  for (int i=0; i<(LCD_WIDTH * LCD_HEIGHT / 8); i++)
    displayMap[i] = bitArray[i];
}

// This function clears the entire display either white (0) or
// black (1).
// The screen won't actually clear until you call updateDisplay()!
void clearDisplay(boolean bw)
{
  for (int i=0; i<(LCD_WIDTH * LCD_HEIGHT / 8); i++)
  {
    if (bw)
      displayMap[i] = 0xFF;
    else
      displayMap[i] = 0;
  }
}

// Helpful function to directly command the LCD to go to a
// specific x,y coordinate.
void gotoXY(int x, int y)
{
  LCDWrite(0, 0x80 | x);  // Column.
  LCDWrite(0, 0x40 | y);  // Row.  ?
}

// This will actually draw on the display, whatever is currently
// in the displayMap array.
void updateDisplay()
{
  gotoXY(0, 0);
  for (int i=0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++)
  {
    LCDWrite(LCD_DATA, displayMap[i]);
  }
}

// Set contrast can set the LCD Vop to a value between 0 and 127.
// 40-60 is usually a pretty good range.
void setContrast(byte contrast)
{
  LCDWrite(LCD_COMMAND, 0x21); //Tell LCD that extended commands follow
  LCDWrite(LCD_COMMAND, 0x80 | contrast); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(LCD_COMMAND, 0x20); //Set display mode
}

/* There are two ways to do this. Either through direct commands
to the display, or by swapping each bit in the displayMap array.
We'll leave both methods here, comment one or the other out if
you please. */
void invertDisplay()
{
  /* Direct LCD Command option
  LCDWrite(LCD_COMMAND, 0x20); //Tell LCD that extended commands follow
  LCDWrite(LCD_COMMAND, 0x08 | 0xB1); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(LCD_COMMAND, 0x20); //Set display mode  */

  /* Indirect, swap bits in displayMap option: */
  //for (int i=0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++)
  //{
  //  displayMap[i] = ~displayMap[i] & 0xFF;
  //}
  updateDisplay();
}

// There are two memory banks in the LCD, data/RAM and commands.
// This function sets the DC pin high or low depending, and then
// sends the data byte
void LCDWrite(byte data_or_command, byte data)
{
  //Tell the LCD that we are writing either to data or a command
  digitalWrite(dcPin, data_or_command);

  //Send the data
  digitalWrite(scePin, LOW);
  SPI.transfer(data); //shiftOut(sdinPin, sclkPin, MSBFIRST, data);
  digitalWrite(scePin, HIGH);
}
//This sends the magical commands to the PCD8544
void lcdBegin(void)
{
  //Configure control pins
  pinMode(scePin, OUTPUT);
  pinMode(rstPin, OUTPUT);
  pinMode(dcPin, OUTPUT);
  pinMode(sdinPin, OUTPUT);
  pinMode(sclkPin, OUTPUT);
  pinMode(blPin, OUTPUT);
  pinMode(butLeft, INPUT);
  pinMode(butRight, INPUT);
  pinMode(butUp, INPUT);
  pinMode(butDown, INPUT);
  pinMode(butJoy, INPUT);

  analogWrite(blPin, 255);

//  Timer1.initialize(100000);                // trying to set a timer of length 10000 microseconds or 0.01 sec tick
//  Timer1.attachInterrupt(timer1Handler,100000);  // trying to attach the service routine with the same - to execute on every tick
//  Timer1.stop();

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  //Reset the LCD to a known state
  digitalWrite(rstPin, LOW);
  digitalWrite(rstPin, HIGH);

  LCDWrite(LCD_COMMAND, 0x21); //Tell LCD extended commands follow
  LCDWrite(LCD_COMMAND, 0xB0); //Set LCD Vop (Contrast)
  LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
  LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48 (try 0x13)
  //We must send 0x20 before modifying the display control mode
  LCDWrite(LCD_COMMAND, 0x20);
  LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode.
}
