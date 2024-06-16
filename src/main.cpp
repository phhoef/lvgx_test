#include <Arduino.h>
#include "./LGVX.cpp"

static LGVX tft;

void setup()
{
  Serial.begin(115200);

  tft.init();
  // tft.initDMA();
  tft.setRotation(1);
  // tft.setBrightness(255);

  tft.fillScreen(tft.color565(0, 0, 0));
  tft.fillCircle(100, 100, 50, tft.color565(255, 0, 0));
}

void loop()
{
  // put your main code here, to run repeatedly:
}