// www.linksprite.com 
//
// Update DTH11 data on IoTgo IOT platform using nodeMCU 
// Feb 11 2016
// Author: Qi Yao
// Website: learn.linksprite.com
 
#include <DHT.h>
#include <ESP8266WiFi.h>
 
// replace with your API key, 
String apikey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* ssid = "xxxxxxxx";
const char* password = "xxxxxxxx";
const char* deviceID="xxxxxxxx";
 
const char* server = "xxxxxxxx"; // your local IoTgo or offical server.
#define DHTPIN 2 // what pin we're connected to
 
DHT dht(DHTPIN, DHT11,15);
WiFiClient client;
 
void setup() {                
  Serial.begin(115200);
  delay(10);
  dht.begin();
  
  WiFi.begin(ssid, password);
 
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
}
 
 
void loop() {
   
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) {
  Serial.println("Failed to read from DHT sensor!");
  return;
  }  
  
 if (client.connect(server,80)) {  
  String  postStr ="{";
          postStr +="\"action\":\"update\",";
          postStr +="\"apikey\":\"";
          postStr +=apikey;
          postStr +="\",";
          postStr +="\"deviceid\":\"";
          postStr += deviceID;
          postStr +="\",";
          postStr += "\"params\":";
          postStr += "{";
          postStr += "\"humidity\":\"";
          postStr += String(h);
          postStr +="\",";
          postStr += "\"temperature\":\"";
          postStr += String(t);
          postStr +="\"";
          postStr +="}";
          postStr +="}";
 
     //send POST request with JSON
     client.print("POST /api/http HTTP/1.1\n"); 
     client.print("Host: 192.168.1.100\n"); // replace your own host IP or host name 
     client.print("Connection: close\n"); 
     client.print("Content-Type: application/json\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);
           
     Serial.print("Temperature: ");
     Serial.print(t);
     Serial.print(" degrees Celcius Humidity: "); 
     Serial.print(h);
     Serial.println("% send to Thingspeak");    
  }
  client.stop();
   
  Serial.println("Waiting...");    
  // 20 sec delay between updates
  delay(20000);  
}
