#include <Arduino.h>
#include <LedController.hpp>
#include <NewPing.h>
#include "LedMatrixPatterns.h"
#include "version.h"

#define VERBOSE
#define DEBUG

// MAX7218
#define PIN_CLK 15  // SCLK
#define PIN_CS 10   //
#define PIN_DATA 16 // MOSI
// HC-SR04
#define ECHO_PIN 2
#define TRIGGER_PIN 3

#define DISPLAY_INTENSITY 0 // Set the brightness (0 to 15) [0] 8
#define MIN_DISTANCE 0      //
#define MAX_DISTANCE 350    // Change detection distance in cm [350]
#define MAX_DISTANCE_DIFF 5 // [2] 5

LedController lc = LedController(PIN_DATA, PIN_CLK, PIN_CS, 1);
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

const unsigned long UPDATE_INTERVAL = 100; // Update display every 200 milliseconds
unsigned long lastUpdate = 0;
unsigned int state = 0;

const unsigned int MAX_TIMEOUT = 500;
unsigned int counterTimeout = 0;
boolean timeout = false;
unsigned int prevDistance = 0;

boolean isCarPresent = false;

void initSerial()
{
  Serial.begin(115200);
  delay(10);
}

void printBootMsg()
{
#ifdef DEBUG
  delay(5000);
#endif
  // Start Boot
  Serial.println(F("> "));
  Serial.println(F("> "));
  Serial.print(F("> Booting... Compiled: "));
  Serial.println(VERSION);
#ifdef VERBOSE
  Serial.print(("> Mode: "));
  Serial.print(F("VERBOSE "));
#ifdef DEBUG
  Serial.print(F("DEBUG"));
#endif
  Serial.println();
#endif
}

void setup()
{
  initSerial();
  printBootMsg();
  initDisplay(lc, DISPLAY_INTENSITY);
}

void loop()
{
  unsigned long currentMillis = millis();

  // Check if it's time to update the display
  if (currentMillis - lastUpdate >= UPDATE_INTERVAL)
  {
    lastUpdate = currentMillis;

    // Measure distance
    unsigned int distance = sonar.ping_cm();
#ifdef DEBUG
    Serial.print(F("> Distance: "));
    Serial.print(distance);
    Serial.print(F("cm; Prev:"));
    Serial.print(prevDistance);
    Serial.println(F("cm"));
#endif
    int distanceDiff = abs(distance - prevDistance);
    if (distanceDiff > MAX_DISTANCE_DIFF)
    {
#ifdef DEBUG
      Serial.print(F("> DistanceDiff: "));
      Serial.print(distanceDiff);
      Serial.println(F("cm"));
#endif
      timeout = false;
    }
    prevDistance = distance;

    // Check if car is present
    if (distance > MIN_DISTANCE && distance < MAX_DISTANCE)
    {
      if (!isCarPresent)
      {
        isCarPresent = true;
#ifdef VERBOSE
        Serial.print(F("> Car: "));
        Serial.println(isCarPresent);
#endif
      }
    }
    else
    {
      if (isCarPresent)
      {
        isCarPresent = false;
#ifdef VERBOSE
        Serial.print(F("> Car: "));
        Serial.println(isCarPresent);
#endif
      }
    }

    // check distance for 8x8 display
    if (distance == 0)
    {
      if (state != 0 && !timeout)
      {
        writeMatrix(lc, null);
        state = 0;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < 2)
    {
      if (state != 11 && !timeout)
      {
        writeMatrix(lc, ex);
        state = 11;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < 20 && !timeout)
    {
      if (state != 1)
      {
        writeMatrix(lc, smile);
        state = 1;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < 30 && !timeout)
    {
      if (state != 2)
      {
        writeMatrix(lc, num1);
        state = 2;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < 40 && !timeout)
    {
      if (state != 3)
      {
        writeMatrix(lc, num2);
        state = 3;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < 50 && !timeout)
    {
      if (state != 4)
      {
        writeMatrix(lc, num3);
        state = 4;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < 60 && !timeout)
    {
      if (state != 5)
      {
        writeMatrix(lc, num4);
        state = 5;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < 70 && !timeout)
    {
      if (state != 6 && !timeout)
      {
        writeMatrix(lc, num5);
        state = 6;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else if (distance < MAX_DISTANCE)
    {
      if (state != 10 && !timeout)
      {
        writeMatrix(lc, arrow);
        state = 10;
        counterTimeout = 0;
#ifdef VERBOSE
        Serial.print(F("> Distance: "));
        Serial.print(distance);
        Serial.println(F("cm"));
        Serial.print(F("> 8x8: "));
        Serial.println(state);
#endif
      }
      else
      {
        counterTimeout++;
      }
    }
    else
    {
      if (state != 0)
      {
#ifdef VERBOSE
        Serial.println(F("> 8x8: OFF"));
#endif
        writeMatrix(lc, null);
        state = 0;
      }
    }
    if (counterTimeout > MAX_TIMEOUT)
    {
      if (state != 0)
      {
#ifdef VERBOSE
        Serial.println(F("> 8x8: OFF by Timeout"));
#endif
        writeMatrix(lc, null);
        state = 0;
        timeout = true;
      }
      counterTimeout = MAX_TIMEOUT + 1;
    }
  }
}