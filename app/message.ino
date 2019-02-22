#include <Adafruit_Sensor.h>
#include <ArduinoJson.h>
#include <DHT.h>

static DHT dht(DHT_PIN, DHT_TYPE);
void initSensor()
{
    dht.begin();
}

void readMessage(char *payload)
{    
    time_t epochTime;
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");
    delay(2000);
    while (true)
    {
        epochTime = time(NULL);
        if (epochTime == 0)
        {
            //Serial.println("Fetching NTP epoch time failed! Waiting 2 seconds to retry.");
            delay(2000);
        }
        else
        {
            //Serial.printf("Fetched NTP epoch time is: %lu.\r\n", epochTime);
            break;
        }
    }
  
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();
    String latitude = "60.329323";
    String longitude = "40.068581";
  
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();
    
    // ENTER THE JSON MESSAGE DATA HERE USE CONFIG FILE TO STORE STATIC DATA LIKE DEVICE_NAME ETC.
    // USE THE ROOT-ELEMENT TO ENTER VALUES (eg. root["DeviceName"] = DEVICE_NAME;)
    root["Temperature"] = temperature;
    root["Humidity"] = humidity;
    root["epochTime"] = epochTime;
    root["Latitude"] = latitude;
    root["Longitude"] = longitude;
    root["DeviceType"] = DEVICE_TYPE;
    root["DeviceName"] = DEVICE_NAME;
    root.printTo(payload, MESSAGE_MAX_LEN);
}

void parseTwinMessage(char *message)
{
    StaticJsonBuffer<MESSAGE_MAX_LEN> jsonBuffer;
    JsonObject &root = jsonBuffer.parseObject(message);
    if (!root.success())
    {
        Serial.printf("Parse %s failed.\r\n", message);
        return;
    }

    if (root["desired"]["interval"].success())
    {
        interval = root["desired"]["interval"];
    }
    else if (root.containsKey("interval"))
    {
        interval = root["interval"];
    }
}
