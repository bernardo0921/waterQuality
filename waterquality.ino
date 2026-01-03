#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

// WiFi credentials
const char* ssid = "Chaley..wossop";
const char* password = "AffordablE";

// ThingSpeak settings
const char* server = "http://api.thingspeak.com/update";
String apiKey = "X4IQO454AC7EPBU0";  // Replace with your ThingSpeak Write API Key
unsigned long channelNumber = 0;      // Replace with your ThingSpeak Channel Number

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;  // Send data every 15 seconds

// Data variables

float turbidityValue = 0.0;
float pHValue = 0.0;
float tdsValue = 0.0;
bool dataReceived = false;

void setup() {
  Serial.begin(115200);  // For communication with Arduino
  
  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
  
  Serial.println("ESP32 Ready to receive data...");
}

void loop() {
  // Read data from Arduino via Serial
  if (Serial.available()) {
    String receivedData = Serial.readStringUntil('\n');
    receivedData.trim();
    
    // Parse the received data
    if (parseData(receivedData)) {
      dataReceived = true;
      Serial.println("Data received and parsed successfully");
      Serial.println("Turbidity: " + String(turbidityValue, 0) + " (analog)");
      Serial.println("pH: " + String(pHValue, 2));
      Serial.println("TDS: " + String(tdsValue, 0) + " ppm");
    }
  }
  
  // Send data to ThingSpeak periodically
  if ((millis() - lastTime) > timerDelay && dataReceived) {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED) {
      sendToThingSpeak();
      lastTime = millis();
    } else {
      Serial.println("WiFi Disconnected");
    }
  }
}

bool parseData(String data) {
  // Expected format: "Turbidity: 512, pH: 7.45, TDS: 123 ppm"
  
  // Find turbidity value (raw analog value)
  int turbidityIndex = data.indexOf("Turbidity: ");
  if (turbidityIndex != -1) {
    int startIndex = turbidityIndex + 11;  // Length of "Turbidity: "
    int endIndex = data.indexOf(",", startIndex);
    if (endIndex != -1) {
      turbidityValue = data.substring(startIndex, endIndex).toFloat();
    }
  }
  
  // Find pH value
  int pHIndex = data.indexOf("pH: ");
  if (pHIndex != -1) {
    int startIndex = pHIndex + 4;  // Length of "pH: "
    int endIndex = data.indexOf(",", startIndex);
    if (endIndex != -1) {
      pHValue = data.substring(startIndex, endIndex).toFloat();
    }
  }
  
  // Find TDS value
  int tdsIndex = data.indexOf("TDS: ");
  if (tdsIndex != -1) {
    int startIndex = tdsIndex + 5;  // Length of "TDS: "
    int endIndex = data.indexOf(" ppm", startIndex);
    if (endIndex != -1) {
      tdsValue = data.substring(startIndex, endIndex).toFloat();
    }
  }
  
  // Return true if all values were found
  return (turbidityIndex != -1 && pHIndex != -1 && tdsIndex != -1);
}

void sendToThingSpeak() {
  HTTPClient http;
  
  // Create the URL with parameters
  String url = String(server) + "?api_key=" + apiKey + 
               "&field1=" + String(turbidityValue, 0) + 
               "&field2=" + String(pHValue, 2) + 
               "&field3=" + String(tdsValue, 0);
  
  Serial.println("Sending data to ThingSpeak...");
  Serial.println("URL: " + url);
  
  http.begin(url);
  
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    String response = http.getString();
    Serial.println("HTTP Response code: " + String(httpResponseCode));
    Serial.println("Response: " + response);
    
    if (httpResponseCode == 200) {
      Serial.println("Data sent successfully to ThingSpeak!");
    }
  } else {
    Serial.print("Error sending data. HTTP Response code: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
}

// Alternative function to send data using WiFiClient (if HTTPClient doesn't work)
void sendToThingSpeak_Alternative() {
  WiFiClient client;
  
  if (client.connect("api.thingspeak.com", 80)) {
    String postStr = apiKey;
    postStr += "&field1=" + String(turbidityValue, 0);
    postStr += "&field2=" + String(pHValue, 2);
    postStr += "&field3=" + String(tdsValue, 0);
    postStr += "\r\n\r\n";
    
    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
    
    Serial.println("Data sent to ThingSpeak using WiFiClient");
  } else {
    Serial.println("Failed to connect to ThingSpeak");
  }
  
  client.stop();
}
