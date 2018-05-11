#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <SocketIoClient.h>
#include <DHT.h>

#define DHTTYPE DHT11
#define DHTPIN D4

DHT dht(DHTPIN,DHTTYPE);

ESP8266WiFiMulti WiFiMulti;
SocketIoClient webSocket;

char payload[100];
/*
void event(const char * payload, size_t length) {
  Serial.printf("Got message: %s\n", payload);
}
*/
void ledControle(const char * payload, size_t length){
  if(payload[0]== '1'){
    digitalWrite(D1, 1);
    //webSocket.emit("ledStatus", '1');
  }
   else{
     digitalWrite(D1, 0);
     //webSocket.emit("ledStatus", '0');
  }   
}

void setup() {
    Serial.begin(115200);
    pinMode(D1, OUTPUT);
    Serial.setDebugOutput(true);
    Serial.println();
    
      for(uint8_t t = 4; t > 0; t--) {
          Serial.printf("[SETUP] BOOT WAIT %d...\n", t);
          Serial.flush();
          delay(1000);
      }

    WiFiMulti.addAP("SM-T377W9469", "nkec7076");

    while(WiFiMulti.run() != WL_CONNECTED) {
        delay(100);
    }

    //webSocket.on("messag", event);
    webSocket.begin("192.168.43.237",3000);
}
  
float lastTemp=0;
float lastHum=0;  
unsigned long currentTime;
unsigned long previousTime = millis();
void loop() {
  currentTime = millis();
  if(currentTime > previousTime + 1000){
    previousTime = currentTime;
    float t =  dht.readTemperature();
    float h = dht.readHumidity();
    if((t != lastTemp)||(h != lastHum)){
      lastTemp = t;
      lastHum = h;
      String data = "{";
      data += "\"temperature\":"; 
      data += t; 
      data += ",";
      data += "\"humidite\":"; 
      data += h;
      data += "}";
      data.toCharArray( payload, 100 );
        
    } 
    webSocket.emit("esp8266", payload);
  }   
    
    webSocket.on("led8266", ledControle);      
    //webSocket.on("message", event);
    webSocket.loop();             
}

