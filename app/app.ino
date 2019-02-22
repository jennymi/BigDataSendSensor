#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiUdp.h>
#include <AzureIoTHub.h>
#include <AzureIoTProtocol_MQTT.h>
#include <AzureIoTUtility.h>
#include "config.h"

static bool messagePending = false;
static bool messageSending = true;

static char *connectionString = DEVICE_CONNECTIONSTRING;;
static int interval = DEVICE_STANDARD_INTERVAL;
static char *ssid = WIFI_SSID;;
static char *pass = WIFI_PASS;

void initComponents()
{
    pinMode(LED_PIN, OUTPUT);
}


void initSerial()
{
    Serial.begin(115200);
}

void blinkLED()
{
    digitalWrite(LED_PIN, HIGH);
    delay(500);
    digitalWrite(LED_PIN, LOW);
}

void initWifi()
{
    Serial.printf("Connecting to SSID: %s.\r\n", ssid);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED)
    {
        WiFi.begin(ssid, pass);
        delay(10000);
    }
    Serial.printf("Connected to SSID %s.\r\n", ssid);
}

void initTime()
{
    time_t epochTime;
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (true)
    {
        epochTime = time(NULL);
        delay(5000);
        
        if (epochTime == 0)
        {
            delay(5000);
        }
        else
        {
            Serial.printf("Timestamp is: %lu.\r\n", epochTime);
            break;
        }
    }
}

static IOTHUB_CLIENT_LL_HANDLE iotHubClientHandle;
void setup()
{
    initComponents();
    initSerial();
    initWifi();
    initTime();
    initSensor();

    iotHubClientHandle = IoTHubClient_LL_CreateFromConnectionString(connectionString, MQTT_Protocol);
    IoTHubClient_LL_SetMessageCallback(iotHubClientHandle, receiveMessageCallback, NULL);
    IoTHubClient_LL_SetDeviceMethodCallback(iotHubClientHandle, deviceMethodCallback, NULL);
    IoTHubClient_LL_SetDeviceTwinCallback(iotHubClientHandle, twinCallback, NULL);
}

unsigned long currentMillis = 0;
unsigned long prevMillis = 0;

void loop()
{
    currentMillis = millis();
    
    if (!messagePending && messageSending)
    {
      if(currentMillis - prevMillis >= interval){
        prevMillis = currentMillis;
        
        char messagePayload[MESSAGE_MAX_LEN];
        readMessage(messagePayload);
        sendMessage(iotHubClientHandle, messagePayload);
      }
    }
    IoTHubClient_LL_DoWork(iotHubClientHandle);
    delay(10);
}
