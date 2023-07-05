#include <ESP8266WiFi.h>

const char* ssid = "AndroidAP536C";
const char* pass = "sss22222";

WiFiServer wifiServer(8090);
char answerServer[20];
WiFiClient clients[2];
int j = 0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting..");
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
         client.println(i);
         clients[i] = client;
         break;
       }
    }
  }
  for(byte i = 0; i < 2; i++){
    while(clients[i] && clients[i].available() > 0){
      Serial.write(clients[i].read());
    }
  }

  // put your main code here, to run repeatedly:

}
