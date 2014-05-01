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
 Foundation, Inc.,51 Franklin St,Fifth Floor, Boston, MA 02110-1301 USA

*/

#include <Seeed_TFT2.h>
#include <SPI.h>

Seeed_TFT2::Seeed_TFT2() 
	: Adafruit_GFX(TFT_WIDTH, TFT_HEIGHT) 
{}

void Seeed_TFT2::begin (void)
{
    SPI.begin();
    
    TFT_CS_HIGH;
    TFT_DC_HIGH;
    uint8_t i=0, TFTDriver=0;
    for(i=0;i<3;i++)
    {
        TFTDriver = readID();
    }
    delay(500);
    sendCommand(0x01);
    delay(200);

    sendCommand(0xCF);
    writeData(0x00);
    writeData(0x8B);
    writeData(0X30);

    sendCommand(0xED);
    writeData(0x67);
    writeData(0x03);
    writeData(0X12);
    writeData(0X81);

    sendCommand(0xE8);
    writeData(0x85);
    writeData(0x10);
    writeData(0x7A);

    sendCommand(0xCB);
    writeData(0x39);
    writeData(0x2C);
    writeData(0x00);
    writeData(0x34);
    writeData(0x02);

    sendCommand(0xF7);
    writeData(0x20);

    sendCommand(0xEA);
    writeData(0x00);
    writeData(0x00);

    sendCommand(0xC0);  // Power control
    writeData(0x1B);    // VRH[5:0]

    sendCommand(0xC1);  // Power control
    writeData(0x10);    // SAP[2:0];BT[3:0]

    sendCommand(0xC5);  // VCM control
    writeData(0x3F);
    writeData(0x3C);

    sendCommand(0xC7);  // VCM control2
    writeData(0XB7);

    sendCommand(0x36);  // Memory Access Control
    writeData(0x08);

    sendCommand(0x3A);
    writeData(0x55);

    sendCommand(0xB1);
    writeData(0x00);
    writeData(0x1B);

    sendCommand(0xB6);  // Display Function Control
    writeData(0x0A);
    writeData(0xA2);


    sendCommand(0xF2);  // 3Gamma Function Disable
    writeData(0x00);

    sendCommand(0x26);  // Gamma curve selected
    writeData(0x01);

    sendCommand(0xE0);  // Set Gamma
    writeData(0x0F);
    writeData(0x2A);
    writeData(0x28);
    writeData(0x08);
    writeData(0x0E);
    writeData(0x08);
    writeData(0x54);
    writeData(0XA9);
    writeData(0x43);
    writeData(0x0A);
    writeData(0x0F);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);
    writeData(0x00);

    sendCommand(0XE1);  // Set Gamma
    writeData(0x00);
    writeData(0x15);
    writeData(0x17);
    writeData(0x07);
    writeData(0x11);
    writeData(0x06);
    writeData(0x2B);
    writeData(0x56);
    writeData(0x3C);
    writeData(0x05);
    writeData(0x10);
    writeData(0x0F);
    writeData(0x3F);
    writeData(0x3F);
    writeData(0x0F);

    sendCommand(0x11);  // Exit Sleep
    delay(120);
    sendCommand(0x29);  // Display on
}

uint8_t Seeed_TFT2::readID(void)
{
    uint8_t i = 0;
    uint8_t data[3] ;
    uint8_t ID[3] = {0x00, 0x93, 0x41};
    uint8_t ToF = 1;
    for(i = 0; i < 3; i++)
    {
        data[i] = readRegister(0xd3, i + 1);
        if(data[i] != ID[i]) ToF = 0;
    }
    if(!ToF)
    {
        Serial.print(F("Read TFT ID failed, ID should be 0x09341, but read ID = 0x"));
        for(i = 0; i < 3; i++)
        {
            Serial.print(data[i],HEX);
        }
        Serial.println();
    }
    return ToF;
}

void Seeed_TFT2::setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) 
{
    sendCommand(ILI9341_CASET);
    sendData(x0);
    sendData(x1);
    sendCommand(ILI9341_PASET);
    sendData(y0);
    sendData(y1);   
    sendCommand(ILI9341_RAMWR);
}

void Seeed_TFT2::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	setAddrWindow(x, y, x, y);
    sendData(color);
}

void Seeed_TFT2::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	setAddrWindow(x + w, y, x, y);
    for(int i=0; i < w; i++) {
		sendData(color);
	}
}

void Seeed_TFT2::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	setAddrWindow(x, y, x, y + h);
    for(int i=0; i < h; i++) {
		sendData(color);
	}
}

void Seeed_TFT2::fillScreen(uint16_t color) 
{
	fillRect(0, 0, _width, _height, color);
}

// fill a rectangle
void Seeed_TFT2::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) 
{
	// rudimentary clipping (drawChar w/big text requires this)
	if((x >= _width) || (y >= _height)) return;
	if((x + w - 1) >= _width)  w = _width  - x;
	if((y + h - 1) >= _height) h = _height - y;

	setAddrWindow(x, y, x + w - 1, y + h - 1);
	uint8_t hi = color >> 8, lo = color;

	TFT_DC_HIGH;
	TFT_CS_LOW;
	for(y = h; y > 0; y--) {
		for( x = w; x>0; x--) {
			SPI.transfer(hi);
			SPI.transfer(lo);
		}
	}
	TFT_CS_HIGH;
}

void Seeed_TFT2::invertDisplay(boolean i) {
  sendCommand(i ? ILI9341_DINVON : ILI9341_DINVOFF);
}

Seeed_TFT2 Tft = Seeed_TFT2();

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
