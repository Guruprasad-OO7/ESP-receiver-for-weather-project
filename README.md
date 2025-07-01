This project receives weather data from an STM32-based LoRa transmitter. The ESP8266 receives, parses, and updates real-time data to:

🌐 Firebase Realtime Database

📲 Blynk IoT Dashboard

🖥️ 0.96" OLED Display (SSD1306)

No simulated values are used — it processes actual LoRa packets.

🧰 Hardware Used
Module	Function
ESP8266 (NodeMCU)	Core receiver + WiFi + Firebase/Blynk
LoRa SX1278	Receives sensor data from STM32
OLED SSD1306	Displays data locally via I2C

🔌 Wiring (LoRa to ESP8266)
LoRa Pin	ESP8266 Pin
VCC	3.3V
GND	GND
SCK	D5 (GPIO14)
MISO	D6 (GPIO12)
MOSI	D7 (GPIO13)
NSS (CS)	D8 (GPIO15)
RST	D0 (GPIO16)
DIO0	D1 (GPIO5)

OLED (I2C):

SDA → D2

SCL → D1

📦 Functional Features
✅ Receives LoRa packets like:
"T:25C H:62% L:48Lux TP:24.3C P:1009.3 hPa A:62.8 m Rain:0 Snow:1"

📤 Uploads parsed data to Firebase

🔔 Sends real-time updates to Blynk

🖥️ Shows clean output on OLED screen

📶 Auto reconnects to WiFi & Firebase
