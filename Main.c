#define BLYNK_TEMPLATE_ID "TMPL3w3sWRGHj"
#define BLYNK_TEMPLATE_NAME "Lora ESP8266"
#define BLYNK_AUTH_TOKEN "5wd0fAd81gkvgH4rmF1-upa5T6PXxlwN"
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <FirebaseESP8266.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "User";      
char pass[] = "User1234";   

#define FIREBASE_HOST "dexx-xxxxx-defxxxxt-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyASwxxxxxxxr46xcxxxx_bfyNXYntmzcw"
FirebaseConfig config;
FirebaseAuth authFirebase;
FirebaseData firebaseData;


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define LORA_SS   D8
#define LORA_RST  D0
#define LORA_DIO0 D1

void connectWiFi() {
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi Connected");
}

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED Init Failed");
    while (1);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  connectWiFi();
  Blynk.begin(auth, ssid, pass);
  config.host = FIREBASE_HOST;
  config.signer.tokens.legacy_token = FIREBASE_AUTH;
  Firebase.begin(&config, &authFirebase);

  LoRa.setPins(LORA_SS, LORA_RST, LORA_DIO0);
  if (!LoRa.begin(434E6)) {
    Serial.println("LoRa init failed. Check wiring.");
    while (1);
  }
  Serial.println("LoRa Receiver Started");
}

void loop() {
  Blynk.run();

  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String data = "";
    while (LoRa.available()) {
      data += (char)LoRa.read();
    }

    Serial.println("LoRa Data: " + data);

    float temperature = extractFloat(data, "T:", "C");
    float humidity = extractFloat(data, "H:", "%");
    float light = extractFloat(data, "L:", "Lux");
    float bmpTemp = extractFloat(data, "TP:", "C");
    float pressure = extractFloat(data, "P:", "hPa");
    float altitude = extractFloat(data, "A:", "m");
    int rain = extractInt(data, "Rain:", " ");
    int snow = extractInt(data, "Snow:", "");


    Firebase.setFloat(firebaseData, "/Weather/Temperature", temperature);
    Firebase.setFloat(firebaseData, "/Weather/Humidity", humidity);
    Firebase.setFloat(firebaseData, "/Weather/Light_Intensity", light);
    Firebase.setFloat(firebaseData, "/Weather/Atmospheric_Pressure", pressure);
    Firebase.setFloat(firebaseData, "/Weather/Altitude", altitude);
    Firebase.setInt(firebaseData, "/Weather/Rain_Status", rain);
    Firebase.setInt(firebaseData, "/Weather/Snow_Status", snow);

    
    Blynk.virtualWrite(V3, temperature);
    Blynk.virtualWrite(V9, humidity);
    Blynk.virtualWrite(V8, light);
    Blynk.virtualWrite(V7, pressure);
    Blynk.virtualWrite(V6, altitude);
    Blynk.virtualWrite(V10, snow);

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Weather Data:");
    display.print("Temp: "); display.println(temperature);
    display.print("Hum: "); display.println(humidity);
    display.print("Lux: "); display.println(light);
    display.print("Pres: "); display.println(pressure);
    display.print("Alt: "); display.println(altitude);
    display.print("Rain: "); display.println(rain ? "Yes" : "No");
    display.print("Snow: "); display.println(snow ? "Yes" : "No");
    display.display();
  }
}

float extractFloat(String src, String key, String unit) {
  int start = src.indexOf(key);
  if (start == -1) return -1;
  start += key.length();
  int end = src.indexOf(unit, start);
  return src.substring(start, end).toFloat();
}

int extractInt(String src, String key, String unit) {
  int start = src.indexOf(key);
  if (start == -1) return -1;
  start += key.length();
  int end = (unit == "") ? src.length() : src.indexOf(unit, start);
  return src.substring(start, end).toInt();
}
