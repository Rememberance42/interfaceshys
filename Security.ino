#include <WiFi.h>
#include <esp_now.h>
#include <esp_wifi.h>

// If your board prints through UART0 and plain Serial shows nothing, uncomment:
// #define Serial Serial0

#define LED_PIN 48  // onboard RGB LED (check your board)
#define ESP_CHANNEL 1

// Keys must be exactly 16 bytes. Both boards need the SAME PMK and LMK.
static const char ESP_NOW_PMK[] = "pmkshjshdjsdhfcdfdsf";
static const char ESP_NOW_LMK[] = "lmkshjshdjsdhfcdfdsf";

// MAC address of the RECEIVER board
uint8_t peer_mac[6] = {0x68, 0xb6, 0xb3, 0x53, 0x01, 0xa8};

const char dataR[] = "255|0|0";
const char dataG[] = "0|255|0";
const char dataB[] = "0|0|255";

// Arduino-ESP32 core 3.3+ signatures
void onSent(const esp_now_send_info_t *tx_info, esp_now_send_status_t status) {
  const uint8_t *m = tx_info->des_addr;
  Serial.printf("%s %02X:%02X:%02X:%02X:%02X:%02X\n",
                status == ESP_NOW_SEND_SUCCESS ? "ESP_NOW_SEND_SUCCESS" : "ESP_NOW_SEND_FAIL",
                m[0], m[1], m[2], m[3], m[4], m[5]);
}

void onRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  const uint8_t *m = info->src_addr;
  Serial.printf("Data received: %02X:%02X:%02X:%02X:%02X:%02X %.*s\n",
                m[0], m[1], m[2], m[3], m[4], m[5], len, (const char *)data);
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ESP_CHANNEL, WIFI_SECOND_CHAN_NONE);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }
  esp_now_register_recv_cb(onRecv);
  esp_now_register_send_cb(onSent);
  esp_now_set_pmk((const uint8_t *)ESP_NOW_PMK);

  esp_now_peer_info_t peer = {};
  memcpy(peer.peer_addr, peer_mac, 6);
  peer.channel = 0;  // use current channel
  peer.ifidx = WIFI_IF_STA;
  memcpy(peer.lmk, ESP_NOW_LMK, ESP_NOW_KEY_LEN);
  peer.encrypt = true;
  if (esp_now_add_peer(&peer) != ESP_OK) {
    Serial.println("Failed to add peer");
  }
  Serial.println("Ready");
}

void loop() {
  static uint8_t count = 0;

  switch (count) {
    case 0:
      esp_now_send(peer_mac, (const uint8_t *)dataR, sizeof(dataR));
      neopixelWrite(LED_PIN, 255, 0, 0);
      break;
    case 1:
      esp_now_send(peer_mac, (const uint8_t *)dataG, sizeof(dataG));
      neopixelWrite(LED_PIN, 0, 255, 0);
      break;
    case 2:
      esp_now_send(peer_mac, (const uint8_t *)dataB, sizeof(dataB));
      neopixelWrite(LED_PIN, 0, 0, 255);
      break;
  }

  count = (count + 1) % 3;
  delay(1000);
}
