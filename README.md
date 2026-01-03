Water Quality Monitoring System (Arduino + ESP32)
📌 Overview

This project is a Water Quality Monitoring System that measures Turbidity, pH, and Total Dissolved Solids (TDS) using sensors connected to a microcontroller and publishes the data to ThingSpeak for remote monitoring.

The system is split into two Arduino sketches that work together:

Sensor Controller Code – Reads sensor values and sends formatted data via Serial.

ESP32 IoT Code – Receives the sensor data, parses it, connects to Wi-Fi, and uploads it to ThingSpeak.

🧩 Project Architecture
[ Sensors ]
   │
   ▼
[ Arduino / Sensor MCU ]
   │  (Serial Communication)
   ▼
[ ESP32 ]
   │  (Wi-Fi)
   ▼
[ ThingSpeak Cloud Dashboard ]

📁 Files in This Repository
| File Name               | Description                                                    |
| ----------------------- | -------------------------------------------------------------- |
| `waterquality.ino`      | Reads turbidity, pH, and TDS sensors and sends data via Serial |
| `waterQualityEsp32.ino` | Receives sensor data, parses it, and uploads it to ThingSpeak  |


🔌 Hardware Requirements

ESP32 Development Board

Arduino (or ESP32 ADC pins if combined)

Turbidity Sensor

pH Sensor Module

TDS Sensor Module

Jumper wires

Breadboard

Stable power supply

🔗 Sensor Connections (waterquality.ino)
| Sensor           | Pin |
| ---------------- | --- |
| Turbidity Sensor | A2  |
| pH Sensor        | A1  |
| TDS Sensor       | A0  |

Note: Ensure all sensors share a common GND.

🧠 How the Code Works
1️⃣ Sensor Controller (waterquality.ino)

Reads analog values from:

Turbidity sensor

pH sensor

TDS sensor

Uses averaging to reduce noise.

Formats data as a readable string:
Turbidity: 512, pH: 7.45, TDS: 123 ppm
Sends the data via Serial communication to the ESP32.

2️⃣ ESP32 IoT Controller (waterQualityEsp32.ino)

Connects to Wi-Fi using provided credentials.

Listens for incoming Serial data.

Parses the sensor values from the received string.

Sends data to ThingSpeak every 15 seconds using HTTP requests.

Handles Wi-Fi reconnection and data validation.
🌐 ThingSpeak Configuration

Update the following values in waterQualityEsp32.ino:
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
String apiKey = "YOUR_THINGSPEAK_WRITE_API_KEY";
unsigned long channelNumber = YOUR_CHANNEL_NUMBER;

ThingSpeak Fields Mapping
| Field   | Sensor    |
| ------- | --------- |
| Field 1 | Turbidity |
| Field 2 | pH        |
| Field 3 | TDS       |

 ⏱ Data Upload Interval

Default upload interval: 15 seconds
unsigned long timerDelay = 15000;
You can adjust this based on your needs.

✅ Requirements & Libraries
Required Arduino Libraries

WiFi.h

HTTPClient.h

ArduinoJson.h

EEPROM.h

🚀 How to Run the Project

Upload waterquality.ino to the sensor controller.

Upload waterQualityEsp32.ino to the ESP32.

Open Serial Monitor (115200 baud).

Ensure Wi-Fi connects successfully.

Check ThingSpeak dashboard for live data updates.

🛠 Troubleshooting

Ensure baud rates match on both devices.

Confirm correct COM ports.

Verify Wi-Fi credentials.

Make sure sensors are powered correctly.

Check ThingSpeak channel field configuration.

📈 Future Improvements

Add LCD/OLED display

Add alert notifications

Use MQTT instead of HTTP

Battery power optimization

Mobile dashboard integration

👤 Author

Bernard Ephraim
Water Quality Monitoring Project
