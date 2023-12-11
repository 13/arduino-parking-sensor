#include <Arduino.h>
#include <LedController.hpp>
#include <NewPing.h>
#include "LedMatrixPatterns.h"
#include "version.h"

#if defined(ESP8266) || defined(ESP32)
// MAX7218
#define PIN_CLK D5
#define PIN_CS D8
#define PIN_DATA D7
// HC-SR04
#define TRIGGER_PIN D1
#define ECHO_PIN D2
#else
// MAX7218
#define PIN_CLK 12
#define PIN_CS 11
#define PIN_DATA 13
// HC-SR04
#define TRIGGER_PIN 10
#define ECHO_PIN 9
#endif

#define MAX_DISTANCE 350 // Change detection distance in cm [350]
#define MAX_DISTANCE_DIFF 5 // [2]

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

void setup()
{
  Serial.begin(115200);
  delay(10);
#ifdef VERBOSE
  lc.setIntensity(8); // Set the brightness (0 to 15)
  lc.clearMatrix();
  writeMatrix(lc, smile);
  delay(5000);
  lc.clearMatrix();
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
  // max7219
  lc.setIntensity(8); // Set the brightness (0 to 15)
  lc.clearMatrix();
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
    Serial.println(F("cm"));
#endif
    if (abs(distance - prevDistance) > MAX_DISTANCE_DIFF)
    {
#ifdef DEBUG
      Serial.print(F("> DistanceDiff: "));
      Serial.print(abs(distance - prevDistance));
      Serial.println(F("cm"));
#endif
      timeout = false;
    }
    prevDistance = distance;

    // Check if car is present
    if (distance > 0 && distance < MAX_DISTANCE)
    {
      if (!isCarPresent)
      {
#ifdef VERBOSE
        Serial.println(F("> Car: True"));
#endif
        isCarPresent = true;
      }
    }
    else
    {
      if (isCarPresent)
      {
#ifdef VERBOSE
        Serial.println(F("> Car: False"));
#endif
        isCarPresent = false;
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
        writeMatrix(lc, n1);
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
        writeMatrix(lc, n2);
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
        writeMatrix(lc, n3);
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
        writeMatrix(lc, n4);
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
        writeMatrix(lc, n5);
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