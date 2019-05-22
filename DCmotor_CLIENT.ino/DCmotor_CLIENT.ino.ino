#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>

int motorPin_1 = D6;
int motorPin_2 = D8;
const char* ssid = "esp";
const char* password = "lol123345678";
const char* host = "192.168.4.1";
String data; 
float data2,data3;
WiFiServer server(80);

void moter_on(){
  digitalWrite(motorPin_1 ,LOW);
  digitalWrite(motorPin_2,HIGH);
  }
  void moter_off(){
  digitalWrite(motorPin_1 ,HIGH);
  digitalWrite(motorPin_2,HIGH);
  }
  
void setup(){
  int count = 0;
  pinMode(motorPin_1,OUTPUT);
  pinMode(motorPin_2, OUTPUT);
 // pinMode(motorPin,OUTPUT);
  Serial.begin(9600);
  delay(10);
  WiFi.mode(WIFI_STA);
  WiFi.begin("esp", "lol12345678");

  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
    count++;
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("Time for connection(s): ");
  Serial.print(count);
  Serial.println();
  Serial.println("IP address: ");
  Serial.print(WiFi.localIP());
  Serial.println();
}

void loop(){
 WiFiClient client;
 if((client.connect(host, 80))){
  Serial.println("Connected");
  Serial.print(host);
  Serial.println();
}


data = client.readStringUntil('\n');
data2 = data.toFloat();
Serial.print("data : ");
Serial.println(data2);

if(data2<=0){
  Serial.print("savedata : ");
  Serial.println(data3);
return;}
data3=data2;
Serial.println(data3);
if(data3 >80){
  moter_on();
}
else{moter_off();}
 Serial.println();
}
