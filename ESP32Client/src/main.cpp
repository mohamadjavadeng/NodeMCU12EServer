  
#include <WiFi.h>
#include <ArduinoJson.h>
#include <commandHandler.h>

#define LED_PIN 2
const char* ssid     = "ESP_MODEM";
const char* password = "12345678";


extern WiFiClient client;
unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
void connectToServer();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  while (!Serial) {
    delay(5); // wait for serial port to connect. Needed for native USB
  }
  Serial.println();
  Serial.print("Connecting to (");
  Serial.print("SSID: " + String(ssid) + ", Password: " + String(password) + "):");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  connectToServer();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
    Serial.println("Disconnected from server");
    connectToServer();
  }
  while (client.available()) {
    String line = client.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(line);
    handleCommand(line);

  }

  if(millis() - lastConnectionTime > 3000 && client.connected()) {
    lastConnectionTime = millis();
    String message = "Hello from ESP32 at " + String(millis());
    client.println(message);
    Serial.print("Sent: ");
    Serial.println(message);
    client.flush();
  }
  delay(10);
}



