#if defined(ESP8266)
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#else
#include <WiFi.h>
#endif

//needed for library
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>  
#include <HTTPClient.h>
AsyncWebServer server(80);
DNSServer dns;
String serverName = "http://tian.websiteszoo.tw/p/";
unsigned long lastTime = 0;
unsigned long timerDelay = 1000;
String uid = "R20220300003";
String tocken = "$2y$10$PGHFP97oCRgssk41XzGweObB5OhSAJApbgIRnNlTv/RfyvpK9zKIW";
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    AsyncWiFiManager wifiManager(&server,&dns);
    wifiManager.autoConnect("AutoConnectAP01");    
    Serial.println("connected...yeet :)");        //print a msg if the connect success
    pinMode(4, OUTPUT);                          
    digitalWrite(4, HIGH);                      //also make pin 4 LED ON
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      HTTPClient http;

      String serverPath = serverName + uid +"/"+tocken;
      
      // Your Domain name with URL path or IP address with path
      http.begin(serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      // using URL to control LED ON or OFF
      if (httpResponseCode>0) {
        String payload = http.getString();
        Serial.println(payload);
        if(payload == "ON"){
             digitalWrite(4, LOW);
             Serial.println("LED ON");
        }
        if(payload == "OFF"){
             digitalWrite(4, HIGH);
             Serial.println("LED OFF");
        }
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}
