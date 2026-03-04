# ESP32 Two-Way Communication via ESP-NOW

Wireless peer-to-peer communication between two ESP32 devices using the ESP-NOW protocol. Messages are typed on a PC via UART (Serial Monitor) and transmitted wirelessly to the other ESP32 — no router or internet required.

---

## How It Works

```
PC ──USB/UART──► ESP32 A ──ESP-NOW/WiFi──► ESP32 B ──UART/USB──► PC
PC ◄─USB/UART── ESP32 A ◄──ESP-NOW/WiFi── ESP32 B ◄─UART/USB── PC
```

- You type a message in the Serial Monitor on either PC
- The ESP32 picks it up via UART and sends it wirelessly using ESP-NOW
- The receiving ESP32 prints the message to its Serial Monitor

---

## Requirements

| Item | Details |
|------|---------|
| Hardware | 2× ESP32 Dev Module |
| IDE | Arduino IDE |
| Board Package | ESP32 by Espressif (via Boards Manager) |
| Library | None — `esp_now.h` is built into the ESP32 Arduino core |
| Baud Rate | 115200 |
| Serial Monitor | Line ending set to **Newline** |

---

## Setup Steps

### Step 1 — Install ESP32 Board Package

In Arduino IDE:
1. Go to **File → Preferences**
2. Add this URL to "Additional Boards Manager URLs":
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
3. Go to **Tools → Board → Boards Manager**
4. Search `esp32` by Espressif and click **Install**

---

### Step 2 — Get MAC Addresses

Flash the following sketch on **each ESP32 one at a time** and note the MAC address printed in the Serial Monitor:

```cpp
#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  Serial.println(WiFi.macAddress());
}

void loop() {}
```

You should see something like:
```
A4:CF:12:B5:77:3E   ← this is ESP32 A's MAC
7C:9E:BD:F0:22:A1   ← this is ESP32 B's MAC
```

---

### Step 3 — Flash ESP32 A

- Open `ESP32_Chat_A/ESP32_Chat_A.ino`
- Set `myName = "ESP32-A"`
- Fill `peerMAC[]` with **ESP32 B's** MAC address

```cpp
const char* myName = "ESP32-A";
uint8_t peerMAC[] = {0x7C, 0x9E, 0xBD, 0xF0, 0x22, 0xA1};
```

---

### Step 4 — Flash ESP32 B

- Open `ESP32_Chat_B/ESP32_Chat_B.ino`
- Set `myName = "ESP32-B"`
- Fill `peerMAC[]` with **ESP32 A's** MAC address

```cpp
const char* myName = "ESP32-B";
uint8_t peerMAC[] = {0xA4, 0xCF, 0x12, 0xB5, 0x77, 0x3E};
```

---

### Step 5 — Run

1. Connect both ESP32s to your PC via USB
2. Open **two Serial Monitor windows** (one per COM port) at **115200 baud**
3. Set line ending to **Newline** on both
4. Type a message on either side and press **Enter**

---

## Expected Output

**ESP32-A Serial Monitor**
```
===========================
 ESP-NOW Chat Ready!
 Type a message + Enter
===========================
My MAC: A4:CF:12:B5:77:3E

[ME] Hello from A!
---------------------------
[ESP32-B] Hey A, got your message!
---------------------------
```

**ESP32-B Serial Monitor**
```
===========================
 ESP-NOW Chat Ready!
 Type a message + Enter
===========================
My MAC: 7C:9E:BD:F0:22:A1

---------------------------
[ESP32-A] Hello from A!
---------------------------
[ME] Hey A, got your message!
```

---

## Project Structure

```
Communication_using_ESP32/
│
├── ESP32_Chat_A/
│   └── ESP32_Chat_A.ino       # Flash on ESP32 A
│
├── ESP32_Chat_B/
│   └── ESP32_Chat_B.ino       # Flash on ESP32 B
│
└── README.md
```

---

## Key Notes

- **No WiFi router needed** — ESP-NOW is a direct device-to-device protocol
- **Range** — up to ~200m in open space
- **Latency** — very low, typically under 10ms
- Only prints an error status if a message **fails** to send — silent on success
- MAC address format: `A4:CF:12:B5:77:3E` → `{0xA4, 0xCF, 0x12, 0xB5, 0x77, 0x3E}`

---

## Author

**Kirtan Modi**  
B.E. Electronics & Communication Engineering  
L.D. College of Engineering, Ahmedabad  
[GitHub](https://github.com/Kirtan Modi) · [LinkedIn](https://www.linkedin.com/in/kirtanmodi-32b9b0322)
