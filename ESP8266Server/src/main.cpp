#include <Arduino.h>
#include <ESP8266WiFi.h>

const char* ssid     = "SSID";
const char* pass = "PASSWORD";


WiFiServer wifiServer(8090);

IPAddress local_IP(192, 168, 1, 184);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);   //optional
char answerServer[20];
WiFiClient clients[2];
int j = 0;
bool ledStatus = false;
char* serverAnswer;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  if (!WiFi.config(local_IP, gateway, subnet, dns)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, pass);
  Serial.println("Connecting..");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    
  }
  Serial.print("Connected to WiFi. IP:");
  Serial.println(WiFi.localIP());
  wifiServer.begin();
}

void loop() {
  WiFiClient client = wifiServer.accept();
  if(client){
    Serial.println("new clients found...");
    for(byte i = 0; i < 2; i++){
       if(!clients[i]){
         client.print("Hello, Client Number:");
         client.println(String(i));
         clients[i] = client;
         break;
       }
    }
  }
  else{
    Serial.println("no new clients");
  }
  for(byte i = 0; i < 2; i++){
    while(clients[i] && clients[i].available() > 0){
      Serial.write(clients[i].read());
    }
    Serial.println();
  }

  for(byte i = 0; i < 2; i++){
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
  }
  delay(1000);
  
  for (byte i = 0; i < 2; i++) {
    if (clients[i] && !clients[i].connected()) {
      clients[i].stop();
    }
  }


  // put your main code here, to run repeatedly:

}