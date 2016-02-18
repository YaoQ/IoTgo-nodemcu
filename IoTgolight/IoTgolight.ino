// www.linksprite.com 
//
// Listen light status from IoTgo IOT platform using nodeMCU
// and control the LED 
// Feb 11 2016
// Author: Qi Yao
// Website: learn.linksprite.com

#include <ESP8266WiFi.h>
#include <WString.h>

// replace with your API key, 
String apikey = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char* ssid = "xxxxxxxx";
const char* password = "xxxxxxxx";
const char* deviceID="xxxxxxxx";
 
const char* server = "xxxxxxxx"; // IP address of your local IoTgo or remote server.
 
#define LED_pin 16

WiFiClient client;
 
void setup() {                
  Serial.begin(115200);
  pinMode(LED_pin, OUTPUT);
  
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
  
  //send a POST reuqest to query the light's status
  if (client.connect(server,80)) {  
  String  postStr ="{";
          postStr +="\"action\":\"query\",";
          postStr +="\"apikey\":\"";
          postStr +=apikey;
          postStr +="\",";
          postStr +="\"deviceid\":\"";
          postStr += deviceID;
          postStr +="\",";
          postStr += "\"params\":";
          postStr += "[";
          postStr += "\"light\"";
          postStr +="]";
          postStr +="}";
 
     client.print("POST /api/http HTTP/1.1\n"); 
     client.print("Host: 192.168.1.100\n"); 
     client.print("Content-Type: application/json\n"); 
     client.print("Content-Length: "); 
     client.print(postStr.length()); 
     client.print("\n\n"); 
     client.print(postStr);     
  }
  delay(1000);

  String request = "";
  
  //parase the request from IoTgo server
  //The response message carry JSON seem like:
  //{"error":0,"params":{"light":"on"},"deviceid":"0200000001","apikey":"e435ae64-e85a-4a0a-8daa-cca39dd7da77"}
 
  while (client.available()) {
    char c = client.read();
    request +=c;
  }
  
  if (request!= NULL)
  {
    int index1 = request.indexOf("{");
    int index2 = request.indexOf("},");
    String param = request.substring(index1, index2+1);
    
    Serial.print("The param is ");
    Serial.println(param);
    
    if(param.indexOf("off")>0){
        digitalWrite(LED_pin, HIGH);    
        Serial.println("OFF");
    } else if(param.indexOf("on")>0){
        digitalWrite(LED_pin, LOW);    
        Serial.println("ON");
    }
    
  client.stop(); 
  Serial.println("Waiting...");    
  // 20 sec delay between updates
  delay(20000);  
  }
}

