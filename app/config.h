// IOT Device static settings
#define DEVICE_NAME "Jenny"
#define DEVICE_TYPE "TempSensor"

// WiFi connection and IOT-Hub connection settings
#define DEVICE_STANDARD_INTERVAL 10000
#define DEVICE_CONNECTIONSTRING "HostName=iot18-iothub.azure-devices.net;DeviceId=11;SharedAccessKey=0SwB+Q3xUL59pZZHHyfKAQhiTIGAZIfuSYLmju34M1E="
#define WIFI_SSID "IoT"
#define WIFI_PASS "IoT2018!"



// DHT Temperature and Humidity settings
#define DHT_PIN 12
#define DHT_TYPE DHT11

// LED 
#define LED_PIN 0

// EEPROM address configuration do not remove!
#define EEPROM_SIZE 512
#define SSID_LEN 32
#define PASS_LEN 32
#define CONNECTION_STRING_LEN 256
#define MESSAGE_MAX_LEN 256
