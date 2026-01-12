#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <commandHandler.h>

const char* ssid     = "ESP_MODEM";
const char* password = "12345678";


WiFiServer wifiServer(8090);

IPAddress local_IP(192, 168, 4, 3);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 4, 1);   //optional

char answerServer[20];
int j = 0;
bool ledStatus = false;
char* serverAnswer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(500);
  
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);
  Serial.print("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }
  // Start WiFi Access Point
  // WiFi.softAP(ssid, password);
  // Serial.println("AP MODE STARTED");
  // Serial.print("SSID: ");
  // Serial.println(ssid);
  // Serial.print("IP: ");
  // Serial.println(WiFi.softAPIP());
  // Serial.println();
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
  wifiServer.begin();
}

void loop() {
  WiFiClient client = wifiServer.accept();
  if(client){
    if(NewClient(client, MAX_CLIENTS)){
      Serial.println("New Client Connected");
    }
  }
  /*for(byte i = 0; i < MAX_CLIENTS; i++){
    if(clients[i]){
      clients[i].println("client " + String(i) + " send your data");
      // clients[i].println("send your data");
      Serial.println("command has sent to client " + String(i));
    }
  }*/
  for(byte i = 0; i < MAX_CLIENTS; i++){
    while(clients[i] && clients[i].available() > 0){
      Serial.write(clients[i].read());
    }
    // Serial.println();
  }

  /*for(byte i = 0; i < MAX_CLIENTS; i++){
    if(clients[i]){
      if((i == 0) && (ledStatus == false)){
        clients[i].println("client 0 led on");
        ledStatus = true;
      }
      else if((i == 0) && (ledStatus == true)){
        clients[i].println("client 0 led off");
        ledStatus = false;
      }
      else{
        clients[i].println("send your data");
      }
      // clients[i].println("send your data");
      Serial.println("Data Has been Sent");
    }
  }*/
  delay(1000);
  
  for (byte i = 0; i < MAX_CLIENTS; i++) {
    if (clients[i] && !clients[i].connected()) {
      clients[i].stop();
    }
  }


  // put your main code here, to run repeatedly:

}