/*********Resources**********
http://tronixstuff.com/2010/10/20/tutorial-arduino-and-the-i2c-bus/
http://www.pointless.info/as1130_driver_how_to
http://hacker.instanet.net/forums/viewtopic.php?f=6&t=32
http://blog.huntgang.com/2014/06/17/arduino-tcrt5000-build-ir-sensor/
*****************************/


//Use Wire Library to do I2C communication  
#include <Wire.h>         

// AS1130 I2C Address - see table 6 page 12
#define AS1130ADDRESS     0x30
// 0x30 = default if PIN 17 floating

// RAM Section Selection - see table 7 page 14
#define REGISTERSELECTION 0xFD
#define NOP               0x00
#define FRAME0            0x01
#define FRAME1            0x02
#define FRAME2            0x03
#define FRAME3            0x04
#define FRAME4            0x05
#define FRAME5            0x06
#define FRAME6            0x07
#define FRAME7            0x08
#define FRAME8            0x09
#define FRAME9            0x0A
#define FRAME10           0x0B
#define FRAME11           0x0C
#define FRAME12           0x0D
#define FRAME13           0x0E
#define FRAME14           0x0F
#define FRAME15           0x10
#define FRAME16           0x11
#define FRAME17           0x12
#define FRAME18           0x13
#define FRAME19           0x14
#define FRAME20           0x15
#define FRAME21           0x16
#define FRAME22           0x17
#define FRAME23           0x18
#define FRAME24           0x19
#define FRAME25           0x1A
#define FRAME26           0x1B
#define FRAME27           0x1C
#define FRAME28           0x1D
#define FRAME29           0x1E
#define FRAME30           0x1F
#define FRAME31           0x20
#define FRAME32           0x21
#define FRAME33           0x22
#define FRAME34           0x23
#define FRAME35           0x24

#define BLINK&PWM0        0x40
#define BLINK&PWM1        0x41
#define BLINK&PWM2        0x42
#define BLINK&PWM3        0x43
#define BLINK&PWM4        0x44
#define BLINK&PWM5        0x45

#define DOTCORRECTION     0x80
#define CONTROLREGISTER   0xC0

// AS1130 Control Register - see table 13 page 20
#define PICTURE           0x00
#define MOVIE             0x01
#define MOVIEMODE         0x02
#define FRAMETIME         0x03
#define DISPLAYOPTION     0x04
#define CURRENTSOURCE     0x05
#define AS1130CONFIG      0x06
#define INTERRUPTMASK     0x07
#define INTERRUPTFRAME    0x08
#define SHUTDOWNOPENSHORT 0x09
#define I2CINTERFACE      0x0A
#define CLKSYNC           0x0B
#define INTERRUPTSTATUS   0x0E
#define AS1130STATUS      0x0F


//36 frames * 12 segments * 2 bytes
uint8_t Frames_Data[0x360] =  {
  
    //12x11 rectangle 
  0b00000111, 0b11111111,    //CS0  Frame 0
  0b00000100, 0b00000001,    //CS1
  0b00000100, 0b00000001,    //CS2
  0b00000100, 0b00000001,    //CS3
  0b00000100, 0b00000001,    //CS4
  0b00000100, 0b00000001,    //CS5
  0b00000100, 0b00000001,    //CS6
  0b00000100, 0b00000001,    //CS7
  0b00000100, 0b00000001,    //CS8
  0b00000100, 0b00000001,    //CS9
  0b00000100, 0b00000001,    //CS10
  0b00000111, 0b11111111,    //CS11

    //10x9 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 1
  0b00000011, 0b11111110,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000011, 0b11111110,      //|
  0b00000000, 0b00000000,    //CS11

    //8x7 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 2
  0b00000000, 0b00000000,      //|
  0b00000001, 0b11111100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b11111100,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11

    //6x5 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 3
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b11111000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b11111000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11

    //4x3 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 4
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b01110000,      //|
  0b00000000, 0b01010000,      //|
  0b00000000, 0b01010000,      //|
  0b00000000, 0b01110000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11
  
    //2x1 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 5
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00100000,      //|
  0b00000000, 0b00100000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11
  
    //2x1 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 6
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00100000,      //|
  0b00000000, 0b00100000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11  
  
    //4x3 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 7
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b01110000,      //|
  0b00000000, 0b01010000,      //|
  0b00000000, 0b01010000,      //|
  0b00000000, 0b01110000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11  

    //6x5 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 8
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b11111000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b10001000,      //|
  0b00000000, 0b11111000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11
  
    //8x7 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 9
  0b00000000, 0b00000000,      //|
  0b00000001, 0b11111100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b00000100,      //|
  0b00000001, 0b11111100,      //|
  0b00000000, 0b00000000,      //|
  0b00000000, 0b00000000,    //CS11
  
    //10x9 rectangle 
  0b00000000, 0b00000000,    //CS0  Frame 10
  0b00000011, 0b11111110,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000010, 0b00000010,      //|
  0b00000011, 0b11111110,      //|
  0b00000000, 0b00000000,    //CS11
  
    //12x11 rectangle 
  0b00000111, 0b11111111,    //CS0  Frame 11
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000100, 0b00000001,      //|
  0b00000111, 0b11111111,    //CS11
  
    //All lit
  0b00000111, 0b11111111,    //CS0  Frame 12
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,      //|
  0b00000111, 0b11111111,    //CS11  
  
  //X
  0b00000100, 0b00000001,
  0b00000010, 0b00000010,
  0b00000001, 0b00000100,
  0b00000000, 0b10001000,
  0b00000000, 0b01010000,
  0b00000000, 0b00100000,
  0b00000000, 0b01010000,
  0b00000000, 0b10001000,
  0b00000001, 0b00000100,
  0b00000010, 0b00000010,
  0b00000100, 0b00000001,
  0b00000000, 0b00000000,
  
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000001, 0b00000100,
  0b00000000, 0b10001000,
  0b00000000, 0b01010000,
  0b00000000, 0b00100000,
  0b00000000, 0b01010000,
  0b00000000, 0b10001000,
  0b00000001, 0b00000100,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b01010000,
  0b00000000, 0b00100000,
  0b00000000, 0b01010000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  
  //csce
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,
  
  0b00000011, 0b10000110,
  0b00000000, 0b11000001,
  0b00000001, 0b10100001,
  0b00000011, 0b00000001,
  0b00000001, 0b11000110,
  0b00000000, 0b00000000,
  0b00000000, 0b00000000,
  0b00000111, 0b11000110,
  0b00000000, 0b01000001,
  0b00000001, 0b11000001,
  0b00000000, 0b01000001,
  0b00000111, 0b11000110,

//462
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
  
  0b00000000, 0b01100000,
  0b00000000, 0b01010000,
  0b00000000, 0b01111000,
  0b00000000, 0b01000000,
  0b00000000, 0b00001000,
  0b00000000, 0b00000100,
  0b00000000, 0b00110010,
  0b00000000, 0b00100100,
  0b00000001, 0b10011000,
  0b00000010, 0b00000000,
  0b00000001, 0b00000000,
  0b00000111, 0b10000000,
};



/***** Function to write commands and data to I2C bus ***************************/
uint8_t chip_write(uint8_t command, uint8_t data)
{
  Wire.beginTransmission(AS1130ADDRESS); 
  Wire.write(command); 
  Wire.write(data); 
  int returnTrans = Wire.endTransmission(); 
    
  return returnTrans;
}

/***** Function to write configuration settings to I2C bus ***************************/
void as_config(uint8_t ram, uint8_t command, uint8_t data)
{
  chip_write(REGISTERSELECTION, ram);
  chip_write(command, data);
}

void setup()
{
  Serial.begin(9600);
  pinMode(A0, INPUT);
  Wire.begin();                     // start up I2C bus

// (1) define ram configuration
  as_config(CONTROLREGISTER, AS1130CONFIG, 0x01);  // 0b00000001 = ram config 1 (Table 20, Page 25) - makes 36 frames usable
// (2) Fill frames with data
int data = 0;
 for (int i=0x01; i<=0x24; i++)    // 0x01 to 0x24 are the addresses of the frames (0-35)
  {
    for (int j=0x00; j<=0x0B; j++)  // 0x00 to 0x0b are the Current Segments in each frame (CS0-CS11)
    {

      as_config(i, 2*j+1, Frames_Data[data]);    // i = frame address, 2*j+1 = frame register address (odd numbers) then second data byte
        
      data++;
      as_config(i, 2*j,   Frames_Data[data]);    // i = frame address, 2*j = frame register address (even numbers) then first data byte
          
      data++;      
    }
  }
 // (3) Set-up Blink & PWM sets
/***** Clear Blink & PWM Sets 1-5  and set brightness to maximum  **********************/
//See Table 10 page 16
//blinking turned off
  for (int i=0x40; i<=0x45; i++)   // 0x40 to 0x45 are the addresses of the Blink & PWM Sets (0-5)
  {
    for (int j=0x00; j<=0x17; j++) // 0x00 to 0x17 are the addresses of the Blink Frame Registers
    {
      as_config(i, j, 0x00);       // i= set number, j = address of Blink Frame, 0x00 = data byte
    }
    for (int k=0x18; k<=0x9b; k++) // 0x18 to 0x9B are the addresses of each individual LEDs PWM Register (132 LEDs can be set from 0 to 255 individually) 
    {
      as_config(i, k, 0xFF);       // i= set number, k = LED PWM Register for each individual LED, 0xFF = data byte [255 or max brightness]
    }
  }


// (6) Define Current Source
  as_config(CONTROLREGISTER, CURRENTSOURCE, 0xFF);       // brightness current set to 30mA (Table 19, Page 24)
// (7) Define Display Options
//  as_config(CONTROLREGISTER, PICTURE, 0b01001000);       // 0b01000000 = display picture (bits 0 - 5 are frame number to display (Table 14, Page 21)
//    as_config(CONTROLREGISTER, MOVIEMODE, 0b00100011);     // number of frames to play (Table 16, Page 22)
//    as_config(CONTROLREGISTER, FRAMETIME, 0b00001111);     //delay time between frames (Table 17, Page 23)
//    as_config(CONTROLREGISTER, DISPLAYOPTION, 0b11101011); // play movie endlessly and scan CS0 to CS11 in each frame (Table 18, Page 24)
//    as_config(CONTROLREGISTER, MOVIE, 0b01000000);         // start movie - first frame is chosen by 1st 6 bits (Table 16, Page 22)
  
}

void loop() 
{
  int sensor = analogRead(A0);
  Serial.println(sensor);
// (8) Start Display  
  uint8_t rate = 0b00001111;
  //uint8_t current_frame = 0b01000000;
  as_config(CONTROLREGISTER, SHUTDOWNOPENSHORT, 0b00000011); // Enable the LEDs (ie make them visible)
  if(sensor > 800){
    
    as_config(CONTROLREGISTER, MOVIEMODE, 0b00100011);     // number of frames to play (Table 16, Page 22)
    as_config(CONTROLREGISTER, FRAMETIME, rate);     //delay time between frames (Table 17, Page 23)
    as_config(CONTROLREGISTER, DISPLAYOPTION, 0b00101011); // play movie once and scan CS0 to CS11 in each frame (Table 18, Page 24)
    as_config(CONTROLREGISTER, MOVIE, 0b01000000);         // start movie - first frame is chosen by 1st 6 bits (Table 16, Page 22)
  }
  //speed up the animations
  else{
    uint8_t speed = 0b00000001;
    as_config(CONTROLREGISTER, MOVIEMODE, 0b00100011);     // number of frames to play (Table 16, Page 22)
    as_config(CONTROLREGISTER, FRAMETIME, rate);     //delay time between frames (Table 17, Page 23)
    as_config(CONTROLREGISTER, DISPLAYOPTION, 0b00101011); // play movie endlessly and scan CS0 to CS11 in each frame (Table 18, Page 24)
    as_config(CONTROLREGISTER, MOVIE, 0b01000000); 
  }
    
}

