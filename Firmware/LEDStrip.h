

#define FASTLED_ESP8266_RAW_PIN_ORDER
#include "FastLED.h"
#define NUM_LEDS1 144
#define LED_TYPE WS2812
#define COLOR_ORDER GRB
CRGB leds1[NUM_LEDS1];
#define PIN1 D5
int data = 255;
int r, g, b;
void setupLEDStrip()
{

    FastLED.addLeds<LED_TYPE, PIN1, COLOR_ORDER>(leds1, NUM_LEDS1).setCorrection(TypicalLEDStrip);
}

void ledColor(int r, int g, int b, int brightness)
{
    FastLED.setBrightness(brightness);
    for (int i = 0; i < NUM_LEDS1; i++)
    {
        leds1[i] = CRGB(r, g, b);
    }
    FastLED.show();
}
