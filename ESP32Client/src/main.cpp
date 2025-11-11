#include <Arduino.h>
#include <WiFi.h>

#define LED_PIN 2
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
Command parseCommand(String message);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
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
    Command cmd = parseCommand(line);
    if (cmd.valid) {
      Serial.print("Parsed Command - Client ID: ");
      Serial.print(cmd.clientID);
      Serial.print(", LED State: ");
      Serial.println(cmd.ledState ? "ON" : "OFF");
      if(cmd.clientID == 0){ // assuming this device has client ID 1
        digitalWrite(LED_PIN, cmd.ledState ? HIGH : LOW);
      }
      // Here you can add code to control an LED based on cmd.ledState
    } else {
      Serial.println("Invalid command format");
    }

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

// command parsing function
Command parseCommand(String message) {
  Command cmd;
  message.trim();
  // Convert to lowercase to make it case-insensitive
  message.toLowerCase();

  // Expected format: client <id> led <on/off>
  if (message.startsWith("client ")) {
    int firstSpace = message.indexOf(' ');
    int secondSpace = message.indexOf(' ', firstSpace + 1);
    int thirdSpace = message.indexOf(' ', secondSpace + 1);

    if (secondSpace > 0 && thirdSpace > 0) {
      String idStr = message.substring(firstSpace + 1, secondSpace);
      String ledCmd = message.substring(thirdSpace + 1);

      int id = idStr.toInt();
      bool state = (ledCmd == "on");

      cmd.clientID = id;
      cmd.ledState = state;
      cmd.valid = true;
    }
  }
  return cmd;
}
