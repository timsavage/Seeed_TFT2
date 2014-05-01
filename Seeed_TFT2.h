/*
 2012 Copyright (c) Seeed Technology Inc.

 Authors: Albert.Miao & Loovee, 
 Visweswara R (with initializtion code from TFT vendor),
 Tim Savage (port to use Adafruit GFX)

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/
#ifndef SEEED_TFT2_h
#define SEEED_TFT2_h

#if defined(ARDUINO) && ARDUINO >= 100
#define SEEEDUINO
#include <Arduino.h>
#else
#include <WProgram.h>
#endif
#include <avr/pgmspace.h>
#include <Adafruit_GFX.h>

#include <SPI.h>

//Basic Colors
#define RED		0xf800
#define GREEN	0x07e0
#define BLUE	0x001f
#define BLACK	0x0000
#define YELLOW	0xffe0
#define WHITE	0xffff

//Other Colors
#define CYAN		0x07ff	
#define BRIGHT_RED	0xf810	
#define GRAY1		0x8410  
#define GRAY2		0x4208  
#define MAGENTA     0xf81f

//TFT resolution 240*320
#define TFT_WIDTH 240
#define TFT_HEIGHT 320

#if defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)

#define TFT_CS_LOW  {DDRE |= 0x08;PORTE &=~ 0x08;}
#define TFT_CS_HIGH {DDRE |= 0x08;PORTE |=  0x08;}
#define TFT_DC_LOW  {DDRH |= 0x08;PORTH &=~ 0x08;}
#define TFT_DC_HIGH {DDRH |= 0x08;PORTH |=  0x08;}
#define TFT_BL_OFF  {DDRH |= 0x10;PORTH &=~ 0x10;}
#define TFT_BL_ON   {DDRH |= 0x10;PORTH |=  0x10;}

#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 54   // can be a digital pin, this is A0
#define XP 57   // can be a digital pin, this is A3

#elif defined(__AVR_ATmega32U4__)

#define TFT_CS_LOW  {DDRC |= 0x40;PORTC &=~ 0x40;}
#define TFT_CS_HIGH {DDRC |= 0x40;PORTC |=  0x40;}
#define TFT_DC_LOW  {DDRD |= 0x80;PORTD &=~ 0x80;}
#define TFT_DC_HIGH {DDRD |= 0x80;PORTD |=  0x80;}
#define TFT_BL_OFF  {DDRE |= 0x40;PORTE &=~ 0x40;}
#define TFT_BL_ON   {DDRE |= 0x40;PORTE |=  0x40;}

#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 18   // can be a digital pin, this is A0
#define XP 21   // can be a digital pin, this is A3

#else
#define TFT_CS_LOW  {DDRD |= 0x20;PORTD &=~ 0x20;}
#define TFT_CS_HIGH {DDRD |= 0x20;PORTD |=  0x20;}
#define TFT_DC_LOW  {DDRD |= 0x40;PORTD &=~ 0x40;}
#define TFT_DC_HIGH {DDRD |= 0x40;PORTD |=  0x40;}
#define TFT_BL_OFF  {DDRD |= 0x80;PORTD &=~ 0x80;}
#define TFT_BL_ON   {DDRD |= 0x80;PORTD |=  0x80;}

#define YP A2   // must be an analog pin, use "An" notation!
#define XM A1   // must be an analog pin, use "An" notation!
#define YM 14   // can be a digital pin, this is A0
#define XP 17   // can be a digital pin, this is A3

#endif

#define TS_MINX 116*2
#define TS_MAXX 890*2
#define TS_MINY 83*2
#define TS_MAXY 913*2


// Commands
#define ILI9341_NOP	    0x00

#define ILI9341_DINVOFF 0x20
#define ILI9341_DINVON  0x21

#define ILI9341_CASET   0x2A
#define ILI9341_PASET   0x2B
#define ILI9341_RAMWR   0x2C
#define ILI9341_RAMRD   0x2E


class Seeed_TFT2 : public Adafruit_GFX
{
public:
	Seeed_TFT2();

    inline void sendCommand(uint8_t index)
    {
        TFT_DC_LOW;
        TFT_CS_LOW;
        SPI.transfer(index);
        TFT_CS_HIGH;
    }

    inline void writeData(uint8_t data)
    {
        TFT_DC_HIGH;
        TFT_CS_LOW;
        SPI.transfer(data);
        TFT_CS_HIGH;
    }
    
    inline void sendData(uint16_t data)
    {
        uint8_t data1 = data >> 8;
        uint8_t data2 = data & 0xff;
        TFT_DC_HIGH;
        TFT_CS_LOW;
        SPI.transfer(data1);
        SPI.transfer(data2);
        TFT_CS_HIGH;
    }

    uint8_t readRegister(uint8_t Addr, uint8_t xParameter)
    {
        uint8_t data=0;
        sendCommand(0xd9);
        writeData(0x10 + xParameter);
        TFT_DC_LOW;
        TFT_CS_LOW;
        SPI.transfer(Addr);
        TFT_DC_HIGH;
        data = SPI.transfer(0);
        TFT_CS_HIGH;
        return data;
    }
    
	void begin(void);
	uint8_t readID(void);
	void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
	
	virtual void 
		drawPixel(int16_t x, int16_t y, uint16_t color),
		drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color),
		drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color),
		fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color),
		fillScreen(uint16_t color),
		invertDisplay(boolean i);
		
};

extern Seeed_TFT2 Tft;

#endif //SEEED_TFT2_h
