#include <avr/pgmspace.h>
#include "FastLED.h"

#define NUM_LEDS 15
#define NUM_STRIPS 4

#define DATA_PIN_FIRST 12
#define DATA_PIN_SECOND 5
#define DATA_PIN_THIRD 11
#define DATA_PIN_FOURTH 3

#define HALL_PIN 2

CRGB leds0[NUM_LEDS];
CRGB leds90[NUM_LEDS];
CRGB leds180[NUM_LEDS];
CRGB leds270[NUM_LEDS];

CRGB *strips[NUM_STRIPS] = {leds0, leds90, leds180, leds270};
uint16_t stripAngles[] = {8192, 24576, 40960, 57344};
volatile boolean turning = false;
volatile uint32_t revTime = 0;              //time at which hall switch got the signal
volatile uint32_t lastRevTime = 0;
uint32_t currentAngle = 0;
uint32_t revPeriod = 0;      //time between revolutions
const uint16_t maxAngle = 0xFFFF;
const uint16_t angleOffset = 25486; //to get correct orientation (52792 should be good for riding)

const uint16_t timeOutInterval = 3000;
const uint16_t scaleInterval = 10;
uint16_t counterInterval = 5;
uint32_t mainTime, lastScaleTime, drawTime, lastCounterTime = 0;
const uint8_t imageWidth = 248;
uint8_t currentFrame = 0;
uint16_t sizeOfImage = 8680; // = 248*35
uint8_t hue, sat, val;
int32_t x, y;
float zoomX = 1.15;
float zoomY = 1.15;
int scaleValue = 140;
int fireScaleValue = 220;
int counter = 0;
int circleParts = 138;

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN_FIRST, GRB>(leds0, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_SECOND, GRB>(leds90, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_THIRD, GRB>(leds180, NUM_LEDS);
  FastLED.addLeds<WS2812B, DATA_PIN_FOURTH, GRB>(leds270, NUM_LEDS);
  pinMode(HALL_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(HALL_PIN), hallSignal, FALLING);
}

void hallSignal() {
  lastRevTime = revTime;
  revTime = millis();
  turning = true;
}


uint32_t getCurrentAngle(int stripNumber) {
  currentAngle = ((drawTime * maxAngle) / revPeriod) + stripAngles[stripNumber] + angleOffset;
  currentAngle = maxAngle - (currentAngle % maxAngle);
  return (currentAngle);
}

void setOneLedOneStrip(int stripNumber, int led) {
  //CRGB strip[NUM_LEDS] = *strips[stripNumber];
  strips[stripNumber][led].setHSV(hue, sat, val);
}
void setOneLedAllStrips(int led) {
  for (int i = 0; i < NUM_STRIPS; i++) {
    //CRGB strip[NUM_LEDS] = *strips[i];
    strips[i][led].setHSV(hue, sat, val);
  }
}

void setAllLedsAllStrips() {
  for (int j = 0; j < NUM_LEDS; j++) {
    for (int i = 0; i < NUM_STRIPS; i++) {
      //CRGB strip[NUM_LEDS] = *strips[i];
      strips[i][j].setHSV(hue, sat, val);
    }
  }
}

void scaleAllStrips(int value) {
  for (int i = 0; i < NUM_LEDS; i++) {
    leds0[i].nscale8(value);
    leds90[i].nscale8(value);
    leds180[i].nscale8(value);
    leds270[i].nscale8(value);
  }
}

void animation1() {
  hue = (mainTime >> 7) % 256;
  sat = 255;
  val = 255;
  if (mainTime - lastCounterTime > revPeriod / circleParts) {
    scaleAllStrips(scaleValue);
    for (int i = 0; i < NUM_STRIPS;  i++) {
      setOneLedOneStrip(i, (counter + i * 4) % NUM_LEDS);
      hue += 32;
      setOneLedOneStrip(i, ((counter + 1) + i * i * 4) % NUM_LEDS);
    }
    counter = (counter + 1) % NUM_LEDS;
    lastCounterTime = mainTime;
  }
}
void fire() {
  hue = random(0, 20);
  sat = 255;
  val = random(2) == 1 ? 255 : 0;
  if (mainTime - lastCounterTime > revPeriod / circleParts) {
    scaleAllStrips(fireScaleValue);
    for (int j = 0; j < 30;j++) {
      for (int i = 0; i < NUM_STRIPS;  i++) {
        setOneLedOneStrip(i, random(NUM_LEDS));
      }
    }
    counter = (counter + 1) % NUM_LEDS;
    lastCounterTime = mainTime;
  }

}

void loop() {
  mainTime = millis();
  revPeriod = revTime - lastRevTime;
  drawTime = mainTime - revTime;
  if (mainTime - revTime > timeOutInterval) {        //timeout(stop detection)
    turning = false;
  }
  /* if (mainTime - lastScaleTime > scaleInterval) {     //change between frames
     scaleAllStrips(scaleValue);
     lastScaleTime = mainTime;
   }
   */
  if (turning) {
    animation1();
    //fire();
  }

  //turn off all strips while not turning
  else if (!turning) {
    val = 0;
    setAllLedsAllStrips();
  }
  FastLED.show();
}

