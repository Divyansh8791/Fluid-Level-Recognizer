// In the Fluid Level Recognizer project, the subscriber module is vital for receiving and processing data for effective 
// fluid level management. This module utilizes an ESP microcontroller (either ESP-32 or ESP8266) to subscribe to data 
// from the MQTTHQ server.


#include <WiFi.h>
#include <PubSubClient.h> 
#include<WiFiClient.h> 

 int cur_level;
 int lev=10; 
 int min_level=20;
 int max_level=75;
 char  newChar[7]; 
const char *ssid = "Redmi Note 8";  
const char *password = "Divyansh@1";  
 
const char *mqtt_broker = "public.mqtthq.com";
const char *topic = "Home/Water_tank";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(4 , OUTPUT);  
 Serial.begin(115200); 
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network"); 
 client.setServer(mqtt_broker, mqtt_port);
 client.setCallback(callback);
 while (!client.connected()) {
     String client_id = "ESP8266Client-";
     client_id += String(WiFi.macAddress());
     Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());
     if (client.connect(client_id.c_str())) {
         Serial.println("Public emqx mqtt broker connected");
     } else {
         Serial.print("failed with state ");
         Serial.print(client.state());
         delay(2000);
     }
 } 
 client.publish(topic, "Hi EMQX I'm ESP32 ^^");
 client.subscribe(topic);
}

 void callback(char *topic, byte *payload, unsigned int length) {
 Serial.print("Message arrived in topic: ");
 Serial.println(topic);
 Serial.print("Message:");   
 String pay="";
 for (int i = 0; i < length; i++) {
   pay += (char) payload[i];
    }
    cur_level=pay.toInt();
    Serial.print(cur_level);
    
  if(cur_level<=min_level){
    digitalWrite(4, LOW); 
  } else if(cur_level>=max_level) {
    digitalWrite(4, HIGH);
  } 
 Serial.println();
 Serial.println("-----------------------");
} 

void loop() { 
  client.loop(); 
  delay(100);
}
