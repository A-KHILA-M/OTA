///https://docs.google.com/spreadsheets/d/1uJD5nqwJE25X-TCBUowdOIqDUynoOkmlepiBBgUbSsQ/edit?usp=sharing    
#include <Arduino.h>
#include <ArduinoRS485.h>       // ArduinoModbus depends on the ArduinoRS485 library
#include <ArduinoModbus.h>                                                                
#include <WiFi.h>
#include <WiFiManager.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>
// Google Apps Script details
String GOOGLE_SCRIPT_ID = "AKfycbxr5XE4nkkThD2xDRzBaCCgBimwCt36BtTD_k8vbpqGDfGdvFrHQWISgpuVhBDNf0Cj";
String dynamicURL = "";         //Placeholder for the URL to fetch
String alert = "";
long dat[190] = {0};
//ROOM1
uint16_t R1Temp, R1Hum, R1Co2, R1Ethylene, R1pulpTemp;
uint16_t R1temp_high, R1temp_low, R1hum_high, R1hum_low, R1co2_high, R1co2_low, R1ethylene_high, R1ethylene_low;
uint16_t R1curr_stage, R1set_dose_interval, R1set_lockcycle, R1curr_dose_interval, R1curr_lockcycle, R1precool_time, R1precool_elapsed_time, R1dose_time, R1dose_elapsed_time, R1vent_time, R1vent_elapsedTime, R1hold_time, R1hold_elapsedTime;
uint16_t R1CompDo, R1HumDo, R1DampDo, R1SolDo;
//ROOM2 
uint16_t R2Temp, R2Hum, R2Co2, R2Ethylene, R2pulpTemp;
uint16_t R2temp_high, R2temp_low, R2hum_high, R2hum_low, R2co2_high, R2co2_low, R2ethylene_high, R2ethylene_low;
uint16_t R2curr_stage, R2set_dose_interval, R2set_lockcycle, R2curr_dose_interval, R2curr_lockcycle, R2precool_time, R2precool_elapsed_time, R2dose_time, R2dose_elapsed_time, R2vent_time, R2vent_elapsedTime, R2hold_time, R2hold_elapsedTime;
uint16_t R2CompDo, R2HumDo, R2DampDo, R2SolDo;
//ROOM3
uint16_t R3Temp, R3Hum, R3Co2, R3Ethylene, R3pulpTemp;
uint16_t R3temp_high, R3temp_low, R3hum_high, R3hum_low, R3co2_high, R3co2_low, R3ethylene_high, R3ethylene_low;
uint16_t R3curr_stage, R3set_dose_interval, R3set_lockcycle, R3curr_dose_interval, R3curr_lockcycle, R3precool_time, R3precool_elapsed_time, R3dose_time, R3dose_elapsed_time, R3vent_time, R3vent_elapsedTime, R3hold_time, R3hold_elapsedTime;
uint16_t R3CompDo, R3HumDo, R3DampDo, R3SolDo;
//ROOM4
uint16_t R4Temp, R4Hum, R4Co2, R4Ethylene, R4pulpTemp;
uint16_t R4temp_high, R4temp_low, R4hum_high, R4hum_low, R4co2_high, R4co2_low, R4ethylene_high, R4ethylene_low;
uint16_t R4curr_stage, R4set_dose_interval, R4set_lockcycle, R4curr_dose_interval, R4curr_lockcycle, R4precool_time, R4precool_elapsed_time, R4dose_time, R4dose_elapsed_time, R4vent_time, R4vent_elapsedTime, R4hold_time, R4hold_elapsedTime;
uint16_t R4CompDo, R4HumDo, R4DampDo, R4SolDo;
//ROOM5
uint16_t R5Temp, R5Hum, R5Co2, R5Ethylene, R5pulpTemp;
uint16_t R5temp_high, R5temp_low, R5hum_high, R5hum_low, R5co2_high, R5co2_low, R5ethylene_high, R5ethylene_low;
uint16_t R5curr_stage, R5set_dose_interval, R5set_lockcycle, R5curr_dose_interval, R5curr_lockcycle, R5precool_time, R5precool_elapsed_time, R5dose_time, R5dose_elapsed_time, R5vent_time, R5vent_elapsedTime, R5hold_time, R5hold_elapsedTime;
uint16_t R5CompDo, R5HumDo, R5DampDo, R5SolDo;
//ROOM6
uint16_t R6Temp, R6Hum, R6Co2, R6Ethylene, R6pulpTemp;
uint16_t R6temp_high, R6temp_low, R6hum_high, R6hum_low, R6co2_high, R6co2_low, R6ethylene_high, R6ethylene_low;
uint16_t R6curr_stage, R6set_dose_interval, R6set_lockcycle, R6curr_dose_interval, R6curr_lockcycle, R6precool_time, R6precool_elapsed_time, R6dose_time, R6dose_elapsed_time, R6vent_time, R6vent_elapsedTime, R6hold_time, R6hold_elapsedTime;
uint16_t R6CompDo, R6HumDo, R6DampDo, R6SolDo;
uint16_t sol_onTime, sol_offTime, CylinderPressure, Sensor_Mode;

// Email timing and interval
unsigned long lastEmailTime = 0; // Store the last email send time
const unsigned long emailInterval = 60000;   //5 * 60 * 1000; // 5 minutes in milliseconds

RS485Class RS1_485(Serial2, 17, 16, 5, 5, 0);
ModbusRTUServerClass ModbusRTUServer1;
WiFiClient espClient;
int status = WL_IDLE_STATUS;
WiFiManager wifiManager;
unsigned long previousMillis = 0;
const int interval = 60000;
void fetchDynamicURL() {
    HTTPClient http;
    String url5 = "https://script.google.com/macros/s/" +GOOGLE_SCRIPT_ID+ "/exec?read";
    http.begin(url5.c_str());
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    int httpCode = http.GET();
    if (httpCode > 0) {
        String payload = http.getString();
        Serial.println(httpCode);
        Serial.println(payload);
        // Parse JSON response to get the URL
        DynamicJsonDocument doc(1024);
        DeserializationError error = deserializeJson(doc, payload);
        if (!error) {
            dynamicURL = doc["url5"].as<String>(); // Assuming the spreadsheet returns {"url5": "<your_url>"}
            Serial.println("Fetched URL: " + dynamicURL);
        } else {
            Serial.println("JSON parsing error");
        }
    } else {
        Serial.println("Error on HTTP request");
    }
    http.end();
}
void spreadsheet_comm1(float r1Temp, float r1Hum, float r1Co2, float r1Ethylene, float r1pulpTemp, float r1temp_high, float r1temp_low, float r1hum_high, float r1hum_low, float r1co2_high, float r1co2_low, float r1ethylene_high, float r1ethylene_low,  float r1curr_stage, float r1set_dose_interval, float r1set_lockcycle, float r1curr_dose_interval, float r1curr_lockcycle, float r1precool_time, float r1precool_elapsed_time, float r1dose_time, float r1dose_elapsed_time, float r1vent_time, float r1vent_elapsedTime, float r1hold_time, float r1hold_elapsedTime, float r1CompDo, float r1HumDo, float r1DampDo, float r1SolDo, float sol_ontime, float sol_offtime, float cylinderPressure, float sensor_Mode, float r2Temp, float r2Hum, float r2Co2, float r2Ethylene, float r2pulpTemp, float r2temp_high, float r2temp_low, float r2hum_high, float r2hum_low, float r2co2_high, float r2co2_low, float r2ethylene_high, float r2ethylene_low,  float r2curr_stage, float r2set_dose_interval, float r2set_lockcycle, float r2curr_dose_interval, float r2curr_lockcycle, float r2precool_time, float r2precool_elapsed_time, float r2dose_time, float r2dose_elapsed_time, float r2vent_time, float r2vent_elapsedTime, float r2hold_time, float r2hold_elapsedTime, float r2CompDo, float r2HumDo, float r2DampDo, float r2SolDo, float r3Temp, float r3Hum, float r3Co2, float r3Ethylene, float r3pulpTemp, float r3temp_high, float r3temp_low, float r3hum_high, float r3hum_low, float r3co2_high, float r3co2_low, float r3ethylene_high, float r3ethylene_low,  float r3curr_stage, float r3set_dose_interval, float r3set_lockcycle, float r3curr_dose_interval, float r3curr_lockcycle, float r3precool_time, float r3precool_elapsed_time, float r3dose_time, float r3dose_elapsed_time, float r3vent_time, float r3vent_elapsedTime, float r3hold_time, float r3hold_elapsedTime, float r3CompDo, float r3HumDo, float r3DampDo, float r3SolDo, float r4Temp, float r4Hum, float r4Co2, float r4Ethylene, float r4pulpTemp, float r4temp_high, float r4temp_low, float r4hum_high, float r4hum_low, float r4co2_high, float r4co2_low, float r4ethylene_high, float r4ethylene_low,  float r4curr_stage, float r4set_dose_interval, float r4set_lockcycle, float r4curr_dose_interval, float r4curr_lockcycle, float r4precool_time, float r4precool_elapsed_time, float r4dose_time, float r4dose_elapsed_time, float r4vent_time, float r4vent_elapsedTime, float r4hold_time, float r4hold_elapsedTime, float r4CompDo, float r4HumDo, float r4DampDo, float r4SolDo, float r5Temp, float r5Hum, float r5Co2, float r5Ethylene, float r5pulpTemp, float r5temp_high, float r5temp_low, float r5hum_high, float r5hum_low, float r5co2_high, float r5co2_low, float r5ethylene_high, float r5ethylene_low,  float r5curr_stage, float r5set_dose_interval, float r5set_lockcycle, float r5curr_dose_interval, float r5curr_lockcycle, float r5precool_time, float r5precool_elapsed_time, float r5dose_time, float r5dose_elapsed_time, float r5vent_time, float r5vent_elapsedTime, float r5hold_time, float r5hold_elapsedTime, float r5CompDo, float r5HumDo, float r5DampDo, float r5SolDo, float r6Temp, float r6Hum, float r6Co2, float r6Ethylene, float r6pulpTemp, float r6temp_high, float r6temp_low, float r6hum_high, float r6hum_low, float r6co2_high, float r6co2_low, float r6ethylene_high, float r6ethylene_low,  float r6curr_stage, float r6set_dose_interval, float r6set_lockcycle, float r6curr_dose_interval, float r6curr_lockcycle, float r6precool_time, float r6precool_elapsed_time, float r6dose_time, float r6dose_elapsed_time, float r6vent_time, float r6vent_elapsedTime, float r6hold_time, float r6hold_elapsedTime, float r6CompDo, float r6HumDo, float r6DampDo, float r6SolDo)
{    
    if (dynamicURL.isEmpty()) 
    {
        Serial.println("Dynamic URL not set!");
        return;
    }
        HTTPClient http;
        http.begin(dynamicURL);
        http.addHeader("Content-Type", "application/json");
        
         String payload = "{";
        payload += "\"sensor1\": {\"r1curr_stage\":" + String(r1curr_stage) + 
                 ",\"r1Temp\":" + String(r1Temp) + 
                 ",\"r1Hum\":" + String(r1Hum) + 
                 ",\"r1Co2\":" + String(r1Co2) + 
                 ",\"r1Ethylene\":" + String(r1Ethylene) + 
                 ",\"r1pulpTemp\":" + String(r1pulpTemp) + 
                 ",\"r1temp_high\":" + String(r1temp_high) + 
                 ",\"r1temp_low\":" + String(r1temp_low) + 
                 ",\"r1hum_high\":" + String(r1hum_high) + 
                 ",\"r1hum_low\":" + String(r1hum_low) + 
                 ",\"r1co2_high\":" + String(r1co2_high) + 
                 ",\"r1co2_low\":" + String(r1co2_low) + 
                 ",\"r1ethylene_high\":" + String(r1ethylene_high) + 
                 ",\"r1ethylene_low\":" + String(r1ethylene_low) + 
                 ",\"r1set_dose_interval\":" + String(r1set_dose_interval) + 
                 ",\"r1set_lockcycle\":" + String(r1set_lockcycle) +
                 ",\"r1curr_dose_interval\":" + String(r1curr_dose_interval) + 
                 ",\"r1curr_lockcycle\":" + String(r1curr_lockcycle) + 
                 ",\"r1precool_time\":" + String(r1precool_time) + 
                 ",\"r1precool_elapsed_time\":" + String(r1precool_elapsed_time) + 
                 ",\"r1dose_time\":" + String(r1dose_time) + 
                 ",\"r1dose_elapsed_time\":" + String(r1dose_elapsed_time) + 
                 ",\"r1vent_time\":" + String(r1vent_time) + 
                 ",\"r1vent_elapsedTime\":" + String(r1vent_elapsedTime) + 
                 ",\"r1hold_time\":" + String(r1hold_time) + 
                 ",\"r1hold_elapsedTime\":" + String(r1hold_elapsedTime) + 
                 ",\"r1CompDo\":" + String(r1CompDo) + 
                 ",\"r1HumDo\":" + String(r1HumDo) +
                 ",\"r1DampDo\":" + String(r1DampDo) + 
                 ",\"r1SolDo\":" + String(r1SolDo) + 
                 ",\"sol_ontime\":" + String(sol_ontime) + 
                 ",\"sol_offtime\":" + String(sol_offtime) + 
                 ",\"cylinderPressure\":" + String(cylinderPressure) + 
                 ",\"sensor_Mode\":" + String(sensor_Mode) + "},";
        payload += "\"sensor2\": {\"r2curr_stage\":" + String(r2curr_stage) + 
                 ",\"r2Temp\":" + String(r2Temp) + 
                 ",\"r2Hum\":" + String(r2Hum) + 
                 ",\"r2Co2\":" + String(r2Co2) + 
                 ",\"r2Ethylene\":" + String(r2Ethylene) + 
                 ",\"r2pulpTemp\":" + String(r2pulpTemp) + 
                 ",\"r2temp_high\":" + String(r2temp_high) + 
                 ",\"r2temp_low\":" + String(r2temp_low) + 
                 ",\"r2hum_high\":" + String(r2hum_high) + 
                 ",\"r2hum_low\":" + String(r2hum_low) + 
                 ",\"r2co2_high\":" + String(r2co2_high) + 
                 ",\"r2co2_low\":" + String(r2co2_low) + 
                 ",\"r2ethylene_high\":" + String(r2ethylene_high) + 
                 ",\"r2ethylene_low\":" + String(r2ethylene_low) + 
                 ",\"r2set_dose_interval\":" + String(r2set_dose_interval) + 
                 ",\"r2set_lockcycle\":" + String(r2set_lockcycle) +
                 ",\"r2curr_dose_interval\":" + String(r2curr_dose_interval) + 
                 ",\"r2curr_lockcycle\":" + String(r2curr_lockcycle) + 
                 ",\"r2precool_time\":" + String(r2precool_time) + 
                 ",\"r2precool_elapsed_time\":" + String(r2precool_elapsed_time) + 
                 ",\"r2dose_time\":" + String(r2dose_time) + 
                 ",\"r2dose_elapsed_time\":" + String(r2dose_elapsed_time) + 
                 ",\"r2vent_time\":" + String(r2vent_time) + 
                 ",\"r2vent_elapsedTime\":" + String(r2vent_elapsedTime) + 
                 ",\"r2hold_time\":" + String(r2hold_time) + 
                 ",\"r2hold_elapsedTime\":" + String(r2hold_elapsedTime) + 
                 ",\"r2CompDo\":" + String(r2CompDo) + 
                 ",\"r2HumDo\":" + String(r2HumDo) +
                 ",\"r2DampDo\":" + String(r2DampDo) + 
                 ",\"r2SolDo\":" + String(r2SolDo) + "},";
        payload += "\"sensor3\": {\"r3curr_stage\":" + String(r3curr_stage) + 
                 ",\"r3Temp\":" + String(r3Temp) + 
                 ",\"r3Hum\":" + String(r3Hum) + 
                 ",\"r3Co2\":" + String(r3Co2) + 
                 ",\"r3Ethylene\":" + String(r3Ethylene) + 
                 ",\"r3pulpTemp\":" + String(r3pulpTemp) + 
                 ",\"r3temp_high\":" + String(r3temp_high) + 
                 ",\"r3temp_low\":" + String(r3temp_low) + 
                 ",\"r3hum_high\":" + String(r3hum_high) + 
                 ",\"r3hum_low\":" + String(r3hum_low) + 
                 ",\"r3co2_high\":" + String(r3co2_high) + 
                 ",\"r3co2_low\":" + String(r3co2_low) + 
                 ",\"r3ethylene_high\":" + String(r3ethylene_high) + 
                 ",\"r3ethylene_low\":" + String(r3ethylene_low) + 
                 ",\"r3set_dose_interval\":" + String(r3set_dose_interval) + 
                 ",\"r3set_lockcycle\":" + String(r3set_lockcycle) +
                 ",\"r3curr_dose_interval\":" + String(r3curr_dose_interval) + 
                 ",\"r3curr_lockcycle\":" + String(r3curr_lockcycle) + 
                 ",\"r3precool_time\":" + String(r3precool_time) + 
                 ",\"r3precool_elapsed_time\":" + String(r3precool_elapsed_time) + 
                 ",\"r3dose_time\":" + String(r3dose_time) + 
                 ",\"r3dose_elapsed_time\":" + String(r3dose_elapsed_time) + 
                 ",\"r3vent_time\":" + String(r3vent_time) + 
                 ",\"r3vent_elapsedTime\":" + String(r3vent_elapsedTime) + 
                 ",\"r3hold_time\":" + String(r3hold_time) + 
                 ",\"r3hold_elapsedTime\":" + String(r3hold_elapsedTime) + 
                 ",\"r3CompDo\":" + String(r3CompDo) + 
                 ",\"r3HumDo\":" + String(r3HumDo) +
                 ",\"r3DampDo\":" + String(r3DampDo) + 
                 ",\"r3SolDo\":" + String(r3SolDo) + "},";
        payload += "\"sensor4\": {\"r4curr_stage\":" + String(r4curr_stage) + 
                 ",\"r4Temp\":" + String(r4Temp) + 
                 ",\"r4Hum\":" + String(r4Hum) + 
                 ",\"r4Co2\":" + String(r4Co2) + 
                 ",\"r4Ethylene\":" + String(r4Ethylene) + 
                 ",\"r4pulpTemp\":" + String(r4pulpTemp) + 
                 ",\"r4temp_high\":" + String(r4temp_high) + 
                 ",\"r4temp_low\":" + String(r4temp_low) + 
                 ",\"r4hum_high\":" + String(r4hum_high) + 
                 ",\"r4hum_low\":" + String(r4hum_low) + 
                 ",\"r4co2_high\":" + String(r4co2_high) + 
                 ",\"r4co2_low\":" + String(r4co2_low) + 
                 ",\"r4ethylene_high\":" + String(r4ethylene_high) + 
                 ",\"r4ethylene_low\":" + String(r4ethylene_low) + 
                 ",\"r4set_dose_interval\":" + String(r4set_dose_interval) + 
                 ",\"r4set_lockcycle\":" + String(r4set_lockcycle) +
                 ",\"r4curr_dose_interval\":" + String(r4curr_dose_interval) + 
                 ",\"r4curr_lockcycle\":" + String(r4curr_lockcycle) + 
                 ",\"r4precool_time\":" + String(r4precool_time) + 
                 ",\"r4precool_elapsed_time\":" + String(r4precool_elapsed_time) + 
                 ",\"r4dose_time\":" + String(r4dose_time) + 
                 ",\"r4dose_elapsed_time\":" + String(r4dose_elapsed_time) + 
                 ",\"r4vent_time\":" + String(r4vent_time) + 
                 ",\"r4vent_elapsedTime\":" + String(r4vent_elapsedTime) + 
                 ",\"r4hold_time\":" + String(r4hold_time) + 
                 ",\"r4hold_elapsedTime\":" + String(r4hold_elapsedTime) + 
                 ",\"r4CompDo\":" + String(r4CompDo) + 
                 ",\"r4HumDo\":" + String(r4HumDo) +
                 ",\"r4DampDo\":" + String(r4DampDo) + 
                 ",\"r4SolDo\":" + String(r4SolDo) + "},";
        payload += "\"sensor5\": {\"r5curr_stage\":" + String(r5curr_stage) + 
                 ",\"r5Temp\":" + String(r5Temp) + 
                 ",\"r5Hum\":" + String(r5Hum) + 
                 ",\"r5Co2\":" + String(r5Co2) + 
                 ",\"r5Ethylene\":" + String(r5Ethylene) + 
                 ",\"r5pulpTemp\":" + String(r5pulpTemp) + 
                 ",\"r5temp_high\":" + String(r5temp_high) + 
                 ",\"r5temp_low\":" + String(r5temp_low) + 
                 ",\"r5hum_high\":" + String(r5hum_high) + 
                 ",\"r5hum_low\":" + String(r5hum_low) + 
                 ",\"r5co2_high\":" + String(r5co2_high) + 
                 ",\"r5co2_low\":" + String(r5co2_low) + 
                 ",\"r5ethylene_high\":" + String(r5ethylene_high) + 
                 ",\"r5ethylene_low\":" + String(r5ethylene_low) + 
                 ",\"r5set_dose_interval\":" + String(r5set_dose_interval) + 
                 ",\"r5set_lockcycle\":" + String(r5set_lockcycle) +
                 ",\"r5curr_dose_interval\":" + String(r5curr_dose_interval) + 
                 ",\"r5curr_lockcycle\":" + String(r5curr_lockcycle) + 
                 ",\"r5precool_time\":" + String(r5precool_time) + 
                 ",\"r5precool_elapsed_time\":" + String(r5precool_elapsed_time) + 
                 ",\"r5dose_time\":" + String(r5dose_time) + 
                 ",\"r5dose_elapsed_time\":" + String(r5dose_elapsed_time) + 
                 ",\"r5vent_time\":" + String(r5vent_time) + 
                 ",\"r5vent_elapsedTime\":" + String(r5vent_elapsedTime) + 
                 ",\"r5hold_time\":" + String(r5hold_time) + 
                 ",\"r5hold_elapsedTime\":" + String(r5hold_elapsedTime) + 
                 ",\"r5CompDo\":" + String(r5CompDo) + 
                 ",\"r5HumDo\":" + String(r5HumDo) +
                 ",\"r5DampDo\":" + String(r5DampDo) + 
                 ",\"r5SolDo\":" + String(r5SolDo) + "},";
        payload += "\"sensor6\": {\"r6curr_stage\":" + String(r6curr_stage) + 
                 ",\"r6Temp\":" + String(r6Temp) + 
                 ",\"r6Hum\":" + String(r6Hum) + 
                 ",\"r6Co2\":" + String(r6Co2) + 
                 ",\"r6Ethylene\":" + String(r6Ethylene) + 
                 ",\"r6pulpTemp\":" + String(r6pulpTemp) + 
                 ",\"r6temp_high\":" + String(r6temp_high) + 
                 ",\"r6temp_low\":" + String(r6temp_low) + 
                 ",\"r6hum_high\":" + String(r6hum_high) + 
                 ",\"r6hum_low\":" + String(r6hum_low) + 
                 ",\"r6co2_high\":" + String(r6co2_high) + 
                 ",\"r6co2_low\":" + String(r6co2_low) + 
                 ",\"r6ethylene_high\":" + String(r6ethylene_high) + 
                 ",\"r6ethylene_low\":" + String(r6ethylene_low) + 
                 ",\"r6set_dose_interval\":" + String(r6set_dose_interval) + 
                 ",\"r6set_lockcycle\":" + String(r6set_lockcycle) +
                 ",\"r6curr_dose_interval\":" + String(r6curr_dose_interval) + 
                 ",\"r6curr_lockcycle\":" + String(r6curr_lockcycle) + 
                 ",\"r6precool_time\":" + String(r6precool_time) + 
                 ",\"r6precool_elapsed_time\":" + String(r6precool_elapsed_time) + 
                 ",\"r6dose_time\":" + String(r6dose_time) + 
                 ",\"r6dose_elapsed_time\":" + String(r6dose_elapsed_time) + 
                 ",\"r6vent_time\":" + String(r6vent_time) + 
                 ",\"r6vent_elapsedTime\":" + String(r6vent_elapsedTime) + 
                 ",\"r6hold_time\":" + String(r6hold_time) + 
                 ",\"r6hold_elapsedTime\":" + String(r6hold_elapsedTime) + 
                 ",\"r6CompDo\":" + String(r6CompDo) + 
                 ",\"r6HumDo\":" + String(r6HumDo) +
                 ",\"r6DampDo\":" + String(r6DampDo) + 
                 ",\"r6SolDo\":" + String(r6SolDo) + "}";
        payload += "}";
        
        int httpResponseCode = http.POST(payload);
        Serial.println("Response code: " + String(httpResponseCode));
        Serial.println("Response: " + http.getString());
        
        http.end();
}
void setup()
{
  Serial.begin(19200);
  wifiManager.setDebugOutput(true);
//  wifiManager.resetSettings();
  while (!Serial) {
    delay(100);
  }
   // Serial.println("Connected to WiFi");
  RS1_485.begin(19200,50,50);  
  if (!ModbusRTUServer1.begin(RS1_485,6,19200,SERIAL_8N1)) 
  {     
    Serial.println("Failed to start Modbus RTU Server!");
    while (1);
  }
  ModbusRTUServer1.configureHoldingRegisters(0x00,190);
  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("WiFi connection lost. Reconnecting...");
    // Attempt to reconnect
    WiFi.begin();
    unsigned long startAttemptTime = millis();
    // Wait for connection attempt to timeout
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) 
    {
      delay(100);
    }
    if (WiFi.status() != WL_CONNECTED) 
    {
      Serial.println("Failed to reconnect to WiFi");
      delay(5000); // Wait before retrying
      return; // Exit the loop if reconnection fails
    }
    Serial.println("Reconnected to WiFi");
  }
  fetchDynamicURL();
  // Start OTA
    ArduinoOTA.setHostname("ESP32_OTA");  // Optional: Set custom hostname

    ArduinoOTA.onStart([]() {
        Serial.println("OTA Update Start");
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nOTA Update Complete");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("OTA Progress: %u%%\r", (progress * 100) / total);
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });

    ArduinoOTA.begin();  // Initialize OTA
}
void loop()
{
   if (!wifiManager.autoConnect("OTA_IOT")) {

    Serial.println("Failed to connect and hit timeout");
  }

  // Check WiFi connection status
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    // Attempt to reconnect
    WiFi.begin();
    unsigned long startAttemptTime = millis();
    // Wait for connection attempt to timeout
    while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < 10000) {
      delay(100);
    }
    if (WiFi.status() != WL_CONNECTED) {
      Serial.println("Failed to reconnect to WiFi");
      delay(5000); // Wait before retrying
      return; // Exit the loop if reconnection fails
    }
    Serial.println("Reconnected to WiFi");
  }
  ArduinoOTA.handle();  // Handle OTA updates
  ModbusRTUServer1.poll(); //ROOM 1

  for(int i=0;i<190;i++){
    dat[i] = ModbusRTUServer1.holdingRegisterRead(i);
    Serial.println("dat["+String(i)+"] : "+String(dat[i]));
   // delay(6000);
  }
  R1Temp=dat[1];
  R1Hum =dat[2];
  R1Co2=dat[3];
  R1Ethylene=dat[4];
  R1pulpTemp=dat[5];
  R1temp_high=dat[6];
  R1temp_low=dat[7];
  R1hum_high=dat[8];
  R1hum_low=dat[9];
  R1co2_high=dat[10];
  R1co2_low=dat[11];
  R1ethylene_high=dat[12];
  R1ethylene_low=dat[13];
  R1curr_stage=dat[14];
  R1set_dose_interval =dat[15];
  R1set_lockcycle=dat[16];
  R1curr_dose_interval=dat[17];
  R1curr_lockcycle=dat[18];

  sol_onTime=dat[19];
  sol_offTime=dat[20];

  R1precool_time =dat[21];
  R1precool_elapsed_time =dat[22];
  R1dose_time =dat[23];
  R1dose_elapsed_time =dat[24];
  R1vent_time=dat[25];
  R1vent_elapsedTime=dat[26];
  R1hold_time =dat[27];
  R1hold_elapsedTime=dat[28];
  R1CompDo=dat[29];
  R1HumDo=dat[30];
  R1DampDo=dat[31];
  R1SolDo=dat[32];
  
  R2Temp=dat[33];
  R2Hum =dat[34];
  R2Co2=dat[35];
  R2Ethylene=dat[36];
  R2pulpTemp=dat[37];
  R2temp_high=dat[38];
  R2temp_low=dat[39];
  R2hum_high=dat[40];
  R2hum_low=dat[41];
  R2co2_high=dat[42];
  R2co2_low=dat[43];
  R2ethylene_high=dat[44];
  R2ethylene_low=dat[45];
  R2curr_stage=dat[46];
  R2set_dose_interval =dat[47];
  R2set_lockcycle=dat[48];
  R2curr_dose_interval=dat[49];
  R2curr_lockcycle=dat[50];
  R2precool_time =dat[51];
  R2precool_elapsed_time =dat[52];
  R2dose_time =dat[53];
  R2dose_elapsed_time =dat[54];
  R2vent_time=dat[55];
  R2vent_elapsedTime=dat[56];
  R2hold_time =dat[57];
  R2hold_elapsedTime=dat[58];
  R2CompDo=dat[59];
  R2HumDo=dat[60];
  R2DampDo=dat[61];
  R2SolDo=dat[62];

  R3Temp=dat[63];
  R3Hum =dat[64];
  R3Co2=dat[65];
  R3Ethylene=dat[66];
  R3pulpTemp=dat[67];
  R3temp_high=dat[68];
  R3temp_low=dat[69];
  R3hum_high=dat[70];
  R3hum_low=dat[71];
  R3co2_high=dat[72];
  R3co2_low=dat[73];
  R3ethylene_high=dat[74];
  R3ethylene_low=dat[75];
  R3curr_stage=dat[76];
  R3set_dose_interval =dat[77];
  R3set_lockcycle=dat[78];
  R3curr_dose_interval=dat[79];
  R3curr_lockcycle=dat[80];
  R3precool_time =dat[81];
  R3precool_elapsed_time =dat[82];
  R3dose_time =dat[83];
  R3dose_elapsed_time =dat[84];
  R3vent_time=dat[85];
  R3vent_elapsedTime=dat[86];
  R3hold_time =dat[87];
  R3hold_elapsedTime=dat[88];
  R3CompDo=dat[89];
  R3HumDo=dat[90];
  R3DampDo=dat[91];
  R3SolDo=dat[92];
  CylinderPressure=dat[93];
  Sensor_Mode=dat[94];

  R4Temp=dat[95];
  R4Hum =dat[96];
  R4Co2=dat[97];
  R4Ethylene=dat[98];
  R4pulpTemp=dat[99];
  R4temp_high=dat[100];
  R4temp_low=dat[101];
  R4hum_high=dat[102];
  R4hum_low=dat[103];
  R4co2_high=dat[104];
  R4co2_low=dat[105];
  R4ethylene_high=dat[106];
  R4ethylene_low=dat[107];
  R4curr_stage=dat[108];
  R4set_dose_interval =dat[109];
  R4set_lockcycle=dat[110];
  R4curr_dose_interval=dat[111];
  R4curr_lockcycle=dat[112];
  R4precool_time =dat[113];
  R4precool_elapsed_time =dat[114];
  R4dose_time =dat[115];
  R4dose_elapsed_time =dat[116];
  R4vent_time=dat[117];
  R4vent_elapsedTime=dat[118];
  R4hold_time =dat[119];
  R4hold_elapsedTime=dat[120];
  R4CompDo=dat[121];
  R4HumDo=dat[122];
  R4DampDo=dat[123];
  R4SolDo=dat[124];

  R5Temp=dat[125];
  R5Hum =dat[126];
  R5Co2=dat[127];
  R5Ethylene=dat[128];
  R5pulpTemp=dat[129];
  R5temp_high=dat[130];
  R5temp_low=dat[131];
  R5hum_high=dat[132];
  R5hum_low=dat[133];
  R5co2_high=dat[134];
  R5co2_low=dat[135];
  R5ethylene_high=dat[136];
  R5ethylene_low=dat[137];
  R5curr_stage=dat[138];
  R5set_dose_interval =dat[139];
  R5set_lockcycle=dat[140];
  R5curr_dose_interval=dat[141];
  R5curr_lockcycle=dat[142];
  R5precool_time =dat[143];
  R5precool_elapsed_time =dat[144];
  R5dose_time =dat[145];
  R5dose_elapsed_time =dat[146];
  R5vent_time=dat[147];
  R5vent_elapsedTime=dat[148];
  R5hold_time =dat[149];
  R5hold_elapsedTime=dat[150];
  R5CompDo=dat[151];
  R5HumDo=dat[152];
  R5DampDo=dat[153];
  R5SolDo=dat[154];

  R6Temp=dat[155];
  R6Hum =dat[156];
  R6Co2=dat[157];
  R6Ethylene=dat[158];
  R6pulpTemp=dat[159];
  R6temp_high=dat[160];
  R6temp_low=dat[161];
  R6hum_high=dat[162];
  R6hum_low=dat[163];
  R6co2_high=dat[164];
  R6co2_low=dat[165];
  R6ethylene_high=dat[166];
  R6ethylene_low=dat[167];
  R6curr_stage=dat[168];
  R6set_dose_interval =dat[169];
  R6set_lockcycle=dat[170];
  R6curr_dose_interval=dat[171];
  R6curr_lockcycle=dat[172];
  R6precool_time =dat[173];
  R6precool_elapsed_time =dat[174];
  R6dose_time =dat[175];
  R6dose_elapsed_time =dat[176];
  R6vent_time=dat[177];
  R6vent_elapsedTime=dat[178];
  R6hold_time =dat[179];
  R6hold_elapsedTime=dat[180];
  R6CompDo=dat[181];
  R6HumDo=dat[182];
  R6DampDo=dat[183];
  R6SolDo=dat[184];
 
   unsigned long currentMillis = millis();
  // Send data every 5 minutes
  if (currentMillis - previousMillis >= interval) 
 {
    previousMillis = currentMillis;
    // Send data for Room 1
    spreadsheet_comm1(R1Temp, R1Hum, R1Co2, R1Ethylene, R1pulpTemp, R1temp_high, R1temp_low, R1hum_high, R1hum_low, R1co2_high, R1co2_low, R1ethylene_high, R1ethylene_low, R1curr_stage, R1set_dose_interval, R1set_lockcycle, R1curr_dose_interval, R1curr_lockcycle, R1precool_time, R1precool_elapsed_time, R1dose_time, R1dose_elapsed_time, R1vent_time, R1vent_elapsedTime, R1hold_time, R1hold_elapsedTime, R1CompDo, R1HumDo, R1DampDo, R1SolDo, sol_onTime, sol_offTime, CylinderPressure, Sensor_Mode, R2Temp, R2Hum, R2Co2, R2Ethylene, R2pulpTemp, R2temp_high, R2temp_low, R2hum_high, R2hum_low, R2co2_high, R2co2_low, R2ethylene_high, R2ethylene_low, R2curr_stage, R2set_dose_interval, R2set_lockcycle, R2curr_dose_interval, R2curr_lockcycle, R2precool_time, R2precool_elapsed_time, R2dose_time, R2dose_elapsed_time, R2vent_time, R2vent_elapsedTime, R2hold_time, R2hold_elapsedTime, R2CompDo, R2HumDo, R2DampDo, R2SolDo, R3Temp, R3Hum, R3Co2, R3Ethylene, R3pulpTemp, R3temp_high, R3temp_low, R3hum_high, R3hum_low, R3co2_high, R3co2_low, R3ethylene_high, R3ethylene_low, R3curr_stage, R3set_dose_interval, R3set_lockcycle, R3curr_dose_interval, R3curr_lockcycle, R3precool_time, R3precool_elapsed_time, R3dose_time, R3dose_elapsed_time, R3vent_time, R3vent_elapsedTime, R3hold_time, R3hold_elapsedTime, R3CompDo, R3HumDo, R3DampDo, R3SolDo, R4Temp, R4Hum, R4Co2, R4Ethylene, R4pulpTemp, R4temp_high, R4temp_low, R4hum_high, R4hum_low, R4co2_high, R4co2_low, R4ethylene_high, R4ethylene_low, R4curr_stage, R4set_dose_interval, R4set_lockcycle, R4curr_dose_interval, R4curr_lockcycle, R4precool_time, R4precool_elapsed_time, R4dose_time, R4dose_elapsed_time, R4vent_time, R4vent_elapsedTime, R4hold_time, R4hold_elapsedTime, R4CompDo, R4HumDo, R4DampDo, R4SolDo, R5Temp, R5Hum, R5Co2, R5Ethylene, R5pulpTemp, R5temp_high, R5temp_low, R5hum_high, R5hum_low, R5co2_high, R5co2_low, R5ethylene_high, R5ethylene_low, R5curr_stage, R5set_dose_interval, R5set_lockcycle, R5curr_dose_interval, R5curr_lockcycle, R5precool_time, R5precool_elapsed_time, R5dose_time, R5dose_elapsed_time, R5vent_time, R5vent_elapsedTime, R5hold_time, R5hold_elapsedTime, R5CompDo, R5HumDo, R5DampDo, R5SolDo, R6Temp, R6Hum, R6Co2, R6Ethylene, R6pulpTemp, R6temp_high, R6temp_low, R6hum_high, R6hum_low, R6co2_high, R6co2_low, R6ethylene_high, R6ethylene_low, R6curr_stage, R6set_dose_interval, R6set_lockcycle, R6curr_dose_interval, R6curr_lockcycle, R6precool_time, R6precool_elapsed_time, R6dose_time, R6dose_elapsed_time, R6vent_time, R6vent_elapsedTime, R6hold_time, R6hold_elapsedTime, R6CompDo, R6HumDo, R6DampDo, R6SolDo);
}
}
