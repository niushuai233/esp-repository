
#include "BLEDevice.h"
#include "BLEUtils.h"
#include "esp_sleep.h"

#ifdef __cplusplus
extern "C" {
#endif

uint8_t temprature_sens_read();
//uint8_t g_phyFuns;

#ifdef __cplusplus
}
#endif

BLEAdvertising *pAdvertising;
// 家里买的测试机 F8:A7:63:A2:C3:3C
// 实际使用 
// 示例 uint8_t bleMac[6] = {0x11, 0x22, 0x33, 0x44, 0x55, 0x66};
uint8_t bleMac[6] = {0xF8, 0xA7, 0x63, 0xA2, 0xC3, 0x3C};
// 家里买的测试机 0x02010603033CFE17FF0002720000025F8093007B00C4F8A763A2C33C000120
// 实际使用 
// 0-30 前31组
// 示例 uint8_t bleRaw[] = {0x02,0x01,0x06,0x17,0xFF,0x00,0x01,0xB5,0x00,0x02,0x25,0xEC,0xD7,0x44,0x00,0x00,0x01,0xAA,0x91,0x77,0x67,0xAF,0x01,0x10,0x00,0x00,0x00,0x03,0x03,0x3C,0xFE};
uint8_t bleRaw[] = {0x02,0x01,0x06,0x03,0x03,0x3C,0xFE,0x17,0xFF,0x00,0x02,0x72,0x00,0x00,0x02,0x5F,0x80,0x93,0x00,0x7B,0x00,0xC4,0xF8,0xA7,0x63,0xA2,0xC3,0x3C,0x00,0x01,0x20};
// 如果复制出来的raw超过31组 那么把它改为true并维护下面的数组
boolean rawMoreThan31 = false;
// 31-end
uint8_t bleRaw32[] = {0x0C,0x09,0x52,0x54,0x4B,0x5F,0x42,0x54,0x5F,0x34,0x2E,0x31,0x00};

void ble_setup() {
  Serial.printf("enter ble_setup");
  
  // esp32没有提供设置蓝牙mac地址的api 通过查看esp32的源代码
  // 此操作将根据蓝牙mac算出base mac
  if (UNIVERSAL_MAC_ADDR_NUM == FOUR_UNIVERSAL_MAC_ADDR) {
    bleMac[5] -= 2;
  } else if (UNIVERSAL_MAC_ADDR_NUM == TWO_UNIVERSAL_MAC_ADDR) {
    bleMac[5] -= 1;
  }
  esp_base_mac_addr_set(bleMac);

  // 初始化
  BLEDevice::init("ESP32_BLE_DEVICE");

  // Create the BLE Server
  // BLEServer *pServer = BLEDevice::createServer(); // <-- no longer required to instantiate BLEServer, less flash and ram usage

  pAdvertising = BLEDevice::getAdvertising();

  // 设备信息设置成空白的
  BLEAdvertisementData oScanResponseData = BLEAdvertisementData();
  pAdvertising->setScanResponseData(oScanResponseData);

  // 里面有个 m_customScanResponseData = true; 和 m_customScanResponseData = true; 所以只能先随便设置一下
  BLEAdvertisementData oAdvertisementData = BLEAdvertisementData();
  pAdvertising->setAdvertisementData(oAdvertisementData);

  // 简单粗暴直接底层api重新设置一下抓到的raw
  esp_err_t errRc = ::esp_ble_gap_config_adv_data_raw(bleRaw, 31);
  if (errRc != ESP_OK) {
    Serial.printf("esp_ble_gap_config_adv_data_raw: %d\n", errRc);
  }
  // 超过31
  if (rawMoreThan31) {
    errRc = ::esp_ble_gap_config_scan_rsp_data_raw(bleRaw32, sizeof(bleRaw32));
    if (errRc != ESP_OK) {
      Serial.printf("esp_ble_gap_config_scan_rsp_data_raw: %d\n", errRc);
    }
  }

  pAdvertising->start();
}

void wifi_setup() {
    Serial.printf("enter wifi_setup");
}

void setup() {
  Serial.printf("enter setup");
  
  Serial.begin(115200);

  ble_setup();
  wifi_setup();
}

void loop() {
  // 闪红灯
  Serial.println("red light twinkling by loop ... at ");
  delay(1000);
  if (millis() > 60000) {
    esp_deep_sleep_start();
  }
}
