#include <Adafruit_GFX.h>    // Core graphics library
#include <Seeed_TFT2.h>
#include <SPI.h>

#define NUM_STARS 40

unsigned int x[NUM_STARS];
unsigned int y[NUM_STARS];
unsigned int v[NUM_STARS];

void setup() {
  Serial.begin(9600);
  Serial.println(F("Starfield!"));

  Tft.begin();
  Tft.fillScreen(BLACK);
  
  for (int i = 0; i < NUM_STARS; i++) {
    x[i] = random(0, Tft.width());
    y[i] = random(0, Tft.height());
    v[i] = random(1, 3);
  }
}

void loop() {
  for (int i = 0; i < NUM_STARS; i++) {
    Tft.drawPixel(y[i], x[i], BLACK);
    x[i]-=v[i];
    if (x[i] <= 0) {
      x[i] = Tft.width();
      y[i] = random(0, Tft.height());
    }
    Tft.drawPixel(x[i], y[i], WHITE);
  }
  
  delay(50);
}
