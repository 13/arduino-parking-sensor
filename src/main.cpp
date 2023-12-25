#include <Arduino.h>
#include <LedController.hpp>
#include <NewPing.h>
#include "LedMatrixPatterns.h"
#include "wsData.h"
#include "helpers.h"
#include "credentials.h"

#if defined(ESP8266) || defined(ESP32)
// MAX7218
#define PIN_CLK D5
#define PIN_CS D8
#define PIN_DATA D7
// HC-SR04
#define ECHO_PIN D1
#define TRIGGER_PIN D2
#endif

#define DISPLAY_INTENSITY 0 // Set the brightness (0 to 15) [0] 8
#define MIN_DISTANCE 0      //
#define MAX_DISTANCE 190    // [216]
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

#if defined(ESP8266)
String hostname = "esp8266-";
#endif
#if defined(ESP32)
String hostname = "esp32-";
#endif

// WiFi & MQTT
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);
wsData myData;
AsyncWebServer server(80);
AsyncWebSocket ws("/ws");
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 3600);

long mqttLastReconnectAttempt = 0;
int wsDataSize = 0;
uint8_t connectedClients = 0;

unsigned long previousMinute = 0;

// supplementary functions
#ifdef VERBOSE
// one minute mark
#define MARK
unsigned long lastMillis = 0L;
uint32_t countMsg = 0;
#endif

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
#if defined(ESP8266) || defined(ESP32)
  Serial.print(F("> Node ID: "));
  Serial.println(getUniqueID());
  hostname += getUniqueID();
#endif
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
  initFS();
  checkWiFi();
  mqttClient.setServer(mqtt_server, mqtt_port);
#ifdef MQTT_SUBSCRIBE
  mqttClient.setCallback(onMqttMessage);
#endif
  if (WiFi.status() == WL_CONNECTED)
  {
    initMDNS();
    connectToMqtt();
    timeClient.begin();
    timeClient.update();
    myData.boottime = timeClient.getEpochTime();
  }
  // Initalize websocket
  initWebSocket();
}

void loop()
{
  ws.cleanupClients();
#ifdef REQUIRES_INTERNET
  checkWiFi();
#endif
  checkMqtt();
#ifdef MARK
  printMARK();
#endif
  unsigned long currentMillis = millis();

  // Check if it's time to update the display
  if (currentMillis - lastUpdate >= UPDATE_INTERVAL)
  {
    lastUpdate = currentMillis;

    // Measure distance
    unsigned int distance = sonar.ping_cm();
    myData.distance = distance;
#ifdef DEBUG
    Serial.print(F("> Distance: "));
    Serial.print(distance);
    Serial.print(F("cm; Prev:"));
    Serial.print(prevDistance);
    Serial.println(F("cm"));
#endif
    int tmpDistance = distance - prevDistance;
    int distanceDiff = abs(tmpDistance);
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
        myData.car = isCarPresent;
        notifyClients();
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
        myData.car = isCarPresent;
        notifyClients();
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
