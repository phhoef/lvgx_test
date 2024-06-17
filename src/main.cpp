#include <Arduino.h>

#if USE_LOVYAN == 1
#include "./LGVX.cpp"
static LGVX tft;
#else
#include <TFT_eSPI.h>
static TFT_eSPI tft(TFT_WIDTH, TFT_HEIGHT);
#endif

void setup()
{
  Serial.begin(115200);

#if USE_LOVYAN == 1
  tft.init();
  tft.initDMA();

  tft.fillScreen(tft.color565(255, 255, 255));
  tft.fillCircle(100, 100, 50, tft.color565(255, 0, 0));
  tft.fillCircle(150, 100, 50, tft.color565(0, 255, 0));
  tft.drawFastVLine(320 / 2, 0, 240, tft.color565(0, 255, 0));
#else
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_WHITE);
  tft.fillRect(50, 50, 100, 100, TFT_GREEN);
#endif
}

void loop()
{
  // put your main code here, to run repeatedly:
}