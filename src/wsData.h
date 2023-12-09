// wsData.h

#ifndef WSDATA_H
#define WSDATA_H

#include <Arduino.h>
#include <ArduinoJson.h>

const int MAX_PACKETS = 5;

struct wsData
{
    int uptime;
    int rssi;
    int memfree;
    int memfrag;
    String ssid;
    String ip;
    String mac;
    String hostname;
    String desc;
    String resetreason;
    String version;
    time_t boottime;
    time_t timestamp;
    // parking sensor
    boolean car;
    int distance;

    String toJson()
    {
        // Create a JSON document
        DynamicJsonDocument doc(2048);

        // Add fields to the document
        doc["uptime"] = uptime;
        doc["rssi"] = rssi;
        doc["memfree"] = memfree;
        doc["memfrag"] = memfrag;
        doc["ssid"] = ssid;
        doc["ip"] = ip;
        doc["mac"] = mac;
        doc["hostname"] = hostname;
        doc["desc"] = desc;
        doc["resetreason"] = resetreason;
        doc["version"] = version;
        doc["boottime"] = boottime;
        doc["timestamp"] = timestamp;
        // parking sensor
        doc["car"] = car;
        doc["distance"] = distance;

        // Serialize the document to a JSON string
        String jsonString;
        serializeJson(doc, jsonString);
        // doc.~BasicJsonDocument(); // destroy

        return jsonString;
    }
};

#endif // WSDATA_H
