// Pong with timer and all the buttons working.
/*
 * Functions available in this demo:
 * int checkCollision(int x, int y)
 * - Check if a pixel is already occupied (setted)
 * setPixel(int x, int y)            
 * clearPixel(int x, int y)                 
 * setLine(int x0, int y0, int x1, int y1, boolean bw)
 * setRect(int x0, int y0, int x1, int y1, boolean fill, boolean bw)
 * setCircle (int x0, int y0, int radius, boolean bw, int lineThickness)
 * setChar(char character, int x, int y, boolean bw)
 * setStr(char * dString, int x, int y, boolean bw)
 * setBitmap(char * bitArray)
 * clearDisplay(boolean bw)
 * gotoXY(int x, int y)
 * updateDisplay()
 * setContrast(byte contrast)
 * invertDisplay()
 */
 
#include <SPI.h> // We'll use SPI to transfer data. Faster!
#include "definitions.h"
#include "TimerOne.h"

// 30Hz updateDisplay freg
//ISR(TIMER1_COMPA_vect) {
void timerISR() {

  /*
  static int counter = 0;
  counter++;
  if ( counter == 2 )
  {
    moveBall();
    counter = 0;
  } */
  updateDisplay();
}

void setup() {
  Serial.begin(9600);

  lcdBegin(); // This will setup our pins, and initialize the LCD
  //analogWrite(blPin, 0); // blPin is ocnnected to BL LED
  //setContrast(50); // Good values range from 40-60
  delay(2000);

  clearDisplay(WHITE);
}

// Loop turns the display into a local serial monitor echo.
// Type to the Arduino from the serial monitor, and it'll echo
// what you type on the display. Type ~ to clear the display.
void loop()
{
  xPos = 15;
  yPos = 23;
  direction = 4;
  int lastPushed = 0;
  setRect(0, 0, 83, 47, 0, BLACK);
  ball.angle = random(1, 16);
  ball.x = random(25, 60);
  ball.y = random(10, 35);
  static int counter = 0;
  while (1)
  {
    if (digitalRead(butUp) == LOW && butUpLast == HIGH)
    {
      pushedButton = 0;
      butUpLast == LOW;
    }
    else if (digitalRead(butRight) == LOW && butRightLast == HIGH)
    {
      pushedButton = 1;
      butRightLast = LOW;
    }
    else if (digitalRead(butDown) == LOW && butDownLast == HIGH)
    {
      pushedButton = 2;
      butDownLast = LOW;
    }
    else if (digitalRead(butLeft) == LOW && butLeftLast == HIGH)
    {
      pushedButton = 3;
      butLeftLast = LOW;
    }
    else if (digitalRead(butJoy) == LOW && butJoyLast == HIGH)
      pushedButton = 4;
    else
      pushedButton = 50;
  
    if (digitalRead(butRight) == HIGH)
      butRightLast = HIGH;
    if (digitalRead(butDown) == HIGH)
      butDownLast = HIGH;
    if (digitalRead(butUp) == HIGH)
      butUpLast = HIGH;
    if (digitalRead(butLeft) == HIGH)
      butLeftLast = HIGH;

    valueJoyX = analogRead(joyX);
    valueJoyY = analogRead(joyY);
  
    if (pushedButton == 0)
      if (p2.y > 0)
        p2.y = p2.y - 1;
    if (pushedButton == 2) 
      if (p2.y < 47-8)
        p2.y = p2.y + 1;
    if (valueJoyY < zeroJoyY-100)
      if (p1.y > 0)
        p1.y = p1.y-1;
    if (valueJoyY > zeroJoyY+100)
      if (p1.y < 47-8)
        p1.y = p1.y+1;
    
    counter++;
    if (counter == 200)
    {
      moveBall();
      counter = 0;
    }
    drawGame(); 
  }
}

void drawGame()
{
  clearDisplay(WHITE);
  setRect(0, 0, 83, 47, 0, BLACK);
  setRect(p1.x, p1.y, p1.x+1, p1.y+8, 0, BLACK);
  setRect(p2.x, p2.y, p2.x+1, p2.y+8, 0, BLACK);
  setRect(ball.x, ball.y, ball.x+1, ball.y+1, 0, BLACK);
}

void moveBall()
{
  ball.x = ball.x + ANGLES[ball.angle-1][0];
  ball.y = ball.y + ANGLES[ball.angle-1][1];
  
  if ( ball.x < 2 || ball.x > 79 )
  {
    ball.angle = 16-(ball.angle-2);
  }
  if (ball.y < 2 || ball.y > 43)
  {
    if (ball.angle <= 9)
      ball.angle = 10 - ball.angle;
    else if (ball.angle > 9)
      ball.angle = 26 - ball.angle;
  }
}

int checkCollision(int x, int y)
{
  // First, double check that the coordinate is in range.
  if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT))
  {
    char tmp1c[5];
    byte shift = y % 8;
    byte check = displayMap[x + (y/8)*LCD_WIDTH];
    String tmp1 = String(check);
    tmp1.toCharArray(tmp1c, 5);
    setStr(tmp1c, 0, 0, BLACK);
    check = check>>shift & 0x01;
    if (check == 0x01)
    {
      clearDisplay(WHITE);
      setStr("C!", 25, 0, BLACK);
      setRect(0, 9, 83, 47, 0, BLACK);
      return 1;
    }
  }
  return 0;
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
  /* Direct LCD Command option */
  LCDWrite(LCD_COMMAND, 0x20); //Tell LCD that extended commands follow
  LCDWrite(LCD_COMMAND, 0x08 | 0xB1); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(LCD_COMMAND, 0x20); //Set display mode

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

  analogWrite(blPin, 0);

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);

  //Reset the LCD to a known state
  digitalWrite(rstPin, LOW);
  digitalWrite(rstPin, HIGH);

  LCDWrite(LCD_COMMAND, 0x21); //Tell LCD extended commands follow
  LCDWrite(LCD_COMMAND, 0xB1); //Set LCD Vop (Contrast)
  LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
  LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48 (try 0x13)
  //We must send 0x20 before modifying the display control mode
  LCDWrite(LCD_COMMAND, 0x20);
  LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode.

  Timer1.initialize(10000);                // trying to set a timer of length 10000 microseconds or 0.01 sec tick
  Timer1.attachInterrupt(timerISR,10000);  // trying to attach the service routine with the same - to execute on every tick
/*
  cli();//stop interrupts

  //set timer0 interrupt at 1kHz
  TCCR0A = 0;// set entire TCCR0A register to 0
  TCCR0B = 0;// same for TCCR0B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 2khz increments
  OCR0A = 255;// = (16*10^6) / (1000*64) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM01);
  // Set CS01 and CS00 bits for 64 prescaler
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE0A);

  //set timer1 interrupt at 30Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 520;// = (16*10^6) / (30*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  //set timer2 interrupt at 8kHz
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);

  sei();//allow interrupts
  */
}
