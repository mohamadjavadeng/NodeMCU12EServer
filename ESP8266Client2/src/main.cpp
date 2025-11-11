#include <Arduino.h>
#include <ESP8266WiFi.h>


const char* ssid     = "TP-Link_ECA4";
const char* password = "50867387";

const char* PIP = "192.168.1.184";
const int PORT = 8090;

struct Command {
  int clientID = -1;
  bool ledState = false;
  bool valid = false;
};

WiFiClient client;
unsigned long lastConnectionTime = 0;         // last time you connected to the server, in milliseconds
void connectToServer();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial) {
    delay(5); // wait for serial port to connect. Needed for native USB
  }
  Serial.println();
  Serial.print("Connecting to ");
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
  }

  if(millis() - lastConnectionTime > 3000 && client.connected()) {
    lastConnectionTime = millis();
    String message = "Hello from WEMOS at " + String(millis());
    client.println(message);
    Serial.print("Sent: ");
    Serial.println(message);
    client.flush();
  }
  delay(10);
}

// put function definitions here:
void connectToServer() {
  Serial.print("Connecting to ");
  Serial.print(PIP);
  Serial.print(":");
  Serial.println(PORT);

  if (client.connect(PIP, PORT)) {
    Serial.println("Connected to server");
    // You can send initial data to the server here if needed
  } else {
    Serial.println("Connection to server failed");
  }
}