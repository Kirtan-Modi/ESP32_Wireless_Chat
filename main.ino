#include <esp_now.h>
#include <WiFi.h>

// ============================================
// ⚠️ CHANGE THESE TWO LINES FOR EACH ESP32
// For ESP32 A: myName = "ESP32-A", fill peerMAC with ESP32 B's MAC address
// For ESP32 B: myName = "ESP32-B", fill peerMAC with ESP32 A's MAC address
// ============================================
const char* myName = "ESP32-A";
uint8_t peerMAC[] = {0xEF, 0x34, 0x56, 0x78, 0x90, 0xAB}; // MAC address of other ESP32

// ============================================

typedef struct Message {
  char sender[20];
  char text[500];
} Message;

Message outgoing;
Message incoming;
esp_now_peer_info_t peerInfo;

// Called when a message is sent
void onSent(const uint8_t *mac, esp_now_send_status_t status) {
  if (status != ESP_NOW_SEND_SUCCESS) {
    Serial.println("[STATUS] Send Failed ✗");
  }
}

// Called when a message is received
void onReceive(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  memcpy(&incoming, data, sizeof(incoming));
  Serial.println("---------------------------");
  Serial.print("[");
  Serial.print(incoming.sender);
  Serial.print("] ");
  Serial.println(incoming.text);
  Serial.println("---------------------------");
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  Serial.print("My MAC: ");
  Serial.println(WiFi.macAddress());
  Serial.print("My Name: ");
  Serial.println(myName);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("[ERROR] ESP-NOW init failed!");
    return;
  }

  // Register callbacks
  esp_now_register_send_cb(onSent);
  esp_now_register_recv_cb(onReceive);

  // Register peer
  memcpy(peerInfo.peer_addr, peerMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("[ERROR] Failed to add peer!");
    return;
  }

  Serial.println("===========================");
  Serial.println(" ESP-NOW Chat Ready!");
  Serial.println(" Type a message + Enter");
  Serial.println("===========================");
}

void loop() {
  // Read message from Serial Monitor (PC → ESP)
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.length() > 0) {
      // Pack the message
      strncpy(outgoing.sender, myName, sizeof(outgoing.sender));
      strncpy(outgoing.text, input.c_str(), sizeof(outgoing.text));

      // Send via ESP-NOW
      esp_now_send(peerMAC, (uint8_t *)&outgoing, sizeof(outgoing));

      Serial.print("[ME] ");
      Serial.println(input);
    }
  }
}
