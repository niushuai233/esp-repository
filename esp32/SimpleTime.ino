#include <WiFi.h>
#include "time.h"

const char* ssid       = "RedmiK40";
const char* password   = "aa123456";

const char* ntpServer = "ntp.aliyun.com";
const long  gmtOffset_sec = 60 * 60 * 8;
const int   daylightOffset_sec = 0;

void printLocalTime()
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("获取时间失败");
    Serial.println("重新设置时间服务");
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    return;
  }
  Serial.println(&timeinfo, "%Y-%m-%d %H:%M:%S");
}

String translateEncryptionType(wifi_auth_mode_t encryptionType) {
  switch (encryptionType){
    case (WIFI_AUTH_OPEN):
      return "Open";
    case (WIFI_AUTH_WEP):
      return "WEP";
    case (WIFI_AUTH_WPA_PSK):
      return "WPA_PSK";
    case (WIFI_AUTH_WPA2_PSK):
      return "WPA2_PSK";
    case (WIFI_AUTH_WPA_WPA2_PSK):
      return "WPA_WPA2_PSK";
    case (WIFI_AUTH_WPA2_ENTERPRISE):
      return "WPA2_ENTERPRISE";
  }
}


void listNetworks() {

  // scan for nearby networks:

  Serial.println("** Scan Networks **");

  byte numSsid = WiFi.scanNetworks();

  // print the list of networks seen:

  Serial.print("number of available networks:");

  Serial.println(numSsid);

  // print the network number and name for each network found:

  for (int thisNet = 0; thisNet < numSsid; thisNet++) {

    Serial.print(thisNet);

    Serial.print(") ");

    Serial.print(WiFi.SSID(thisNet));

    Serial.print("\tMAC: ");
    
    Serial.print(WiFi.BSSIDstr(thisNet));

    Serial.print("\tSignal: ");

    Serial.print(WiFi.RSSI(thisNet));

    Serial.print(" dBm");

    Serial.print("\tEncryption: ");

    Serial.println(translateEncryptionType(WiFi.encryptionType(thisNet)));

  }
}

void setup()
{
  Serial.begin(115200);

  //connect to WiFi

  listNetworks();

  WiFi.begin(ssid, password);
  int count = 0;
  while (WiFi.status() != WL_CONNECTED) {
    if (count % 10 == 0) {
      Serial.printf("\n正在连接 %s 密码: %s", ssid, password);
    }
    delay(500);
    Serial.print(".");
    if (++count > 9) {
      Serial.println("\n连续尝试连接次数过多, 睡眠10s");
      delay(10000);
      Serial.println("睡眠完成, 继续尝试连接");
      count = 0;
    }
  }
  Serial.println("\n已连接");

  //init and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  //disconnect WiFi as it's no longer needed
  // WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
}

void loop()
{
  delay(1000);
  printLocalTime();
}
