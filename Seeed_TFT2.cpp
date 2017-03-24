#include <Wire.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>

#include "ClickEncoder.h"
#include "TimerOne.h"
#include "AnalogMovingAverage.h"
#include "Seeed_TFT2.h"

// Pin constants
const uint8_t pin_encoderA = 9;
const uint8_t pin_encoderB = 10;
const uint8_t pin_encoderBtn = 8;

const float step_constant = 6.710886;

uint32_t freq = 1000;


ClickEncoder encoder = ClickEncoder(
	pin_encoderA, pin_encoderB,
	pin_encoderBtn,
	4 // 4 steps per notch, with accelleration
);


/**
    Timer One ISR
*/
void timerIsr() {
  encoder.service();
}

void init_timer() {
  Timer1.initialize(1000);
  Timer1.attachInterrupt(timerIsr);
}


/**

*/
void setup() {
  Serial.begin(9600);
  Wire.begin();
  Tft.begin();
  Tft.setRotation(3);
  Tft.fillScreen(BLACK);
  
  Tft.setCursor(0,0);
  Tft.setTextColor(YELLOW);
  Tft.print("Funktion Gen");
  Tft.drawLine(0, 10, 50, 10, GREEN);
  Tft.drawLine(20, 20, 50, 50, RED);

  init_timer();
}


void send_diff(int16_t diff) {
  if (diff) {
    Wire.beginTransmission(0x42);
    if (diff < 0) {
      diff *= -1;
      while (diff > 0) {
        if (diff >= 100) {
          Wire.write(0x23);
          diff -= 100;
        } else if (diff >= 10) {
          Wire.write(0x22);
          diff -= 10;
        } else if (diff >= 1) {
          Wire.write(0x21);
          diff -= 1;
        }
      }
    } else {
      while (diff > 0) {
        if (diff >= 100) {
          Wire.write(0x33);
          diff -= 100;
        } else if (diff >= 10) {
          Wire.write(0x32);
          diff -= 10;
        } else if (diff >= 1) {
          Wire.write(0x31);
          diff -= 1;
        }
      }
    }
    Wire.endTransmission();
  }
}


void loop() {
  int16_t delta = encoder.getValue();
  if (delta) {
    send_diff(delta * 10);
  }
}
