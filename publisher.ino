 #include <PubSubClient.h>
#include<ESP8266WiFi.h>
#include<WiFiClient.h>
#include<ESP8266WebServer.h>


const int trigPin = D0;
const int echoPin = D1; 
long duration;
int distance;

 String pay="";
 int a;
 int lev=10;
 char  newChar[7];
 char level[5];
int total_level=25;
float remain_level_percent;
String level_str;

const char *ssid = "Redmi Note 8";  
const char *password = "Divyansh@1";  
 
const char *mqtt_broker = "public.mqtthq.com";
const char *topic = "Home/Water_tank";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(D4 , OUTPUT);
  pinMode(trigPin, OUTPUT);  
  pinMode(echoPin, INPUT);  
 Serial.begin(115200); 
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network"); 
 client.setServer(mqtt_broker, mqtt_port); 
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
} 


void loop() { 
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  int distance = duration*0.034/2;
  if(distance>total_level) {
    distance=total_level;
  }
  int remain_level_percent = 100-distance*100/total_level;
  
  Serial.println(distance);
  Serial.println(remain_level_percent);
  if (remain_level_percent > 100) {
    remain_level_percent=100;
  } else if (remain_level_percent < 0 ) { 
    remain_level_percent=0;
  }
  level_str = String(remain_level_percent);
  level_str.toCharArray(level, level_str.length() + 1);
  client.publish(topic, level);
  Serial.println(level);
  delay(1000);
}