#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
WiFiServer server(80);

int dust_pin = D8, LED_R = D5, LED_G = D6, LED_B = D7;
float dust_result[3], avr_dust;
unsigned long duration;//지속시간
unsigned long starttime;//시작시간
unsigned long sampletime_ms = 30000;//sampe 30s ; 셈플 측정시간
unsigned long lowpulseoccupancy = 0;//LOP
float ratio = 0;//비율
float concentration = 0;//농도
float pcsPerCF = 0;
float ugm3 = 0;

void setup() {
  pinMode(dust_pin, INPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);
  starttime = millis();//get the current time;
  WiFi.mode(WIFI_AP);
  WiFi.softAP("esp", "lol12345678");
  
  server.begin();
  Serial.begin(9600);
  IPAddress IP = WiFi.softAPIP();
  //Serial.flush();
  Serial.println();
  Serial.print("Server IP is: ");
  Serial.println(IP);

}

void loop() {
  
WiFiClient client = server.available();
  if (!client) { 
    return;}
  Serial.println("connect");
  
  Serial.print("Sent data: ");
  Serial.print(ESP.getChipId());
  Serial.println();
  for(int i=0;i<=10;i++)
  {client.print(avr_dust);}
  Serial.println(avr_dust);
delay(100);
   for(int i =0; i<3;i++){
      while((millis() - starttime) < sampletime_ms) {
    check_dust();
    }
    dust_result[i]=cal_dust();}
    
    avr_dust=((dust_result[0]+dust_result[1]+dust_result[2])/3);
    Serial.print("평균농도 : ");
    Serial.println(avr_dust);
    if(avr_dust<=0.2){//파랑
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_B, HIGH);
      }
    else if(avr_dust<=0.3){//초록
      digitalWrite(LED_R, LOW);
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_B, LOW);
      }
    else if(avr_dust<=1){//노랑
      digitalWrite(LED_G, HIGH);
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_B, LOW);
      }
    else{//빨강
      digitalWrite(LED_G, LOW);
      digitalWrite(LED_R, HIGH);
      digitalWrite(LED_B, LOW);
      }

//*
  
//delay(1000);
//*/
}
float cal_dust(){
  ratio = lowpulseoccupancy / (sampletime_ms * 10.0); // Integer percentage 0=>100
    concentration = 1.1 * pow(ratio, 3) - 3.8 * pow(ratio, 2) + 520 * ratio + 0.62; // using spec sheet curve//pow():지수연산
    pcsPerCF = concentration * 100;
    ugm3 = pcsPerCF / 13000;//(7000으로 하는 곳도 잇음 테스트 요망)
    Serial.print("concentration = ");
    Serial.print(ugm3);
    Serial.println(" ㎍/m³");
    Serial.println("\n");
    //재측정 작업
    lowpulseoccupancy = 0;
    starttime = millis();
    
  return ugm3;
  }
void check_dust(){
  duration = pulseIn(dust_pin, LOW);//해당 핀으로 신호가 들어오는 시간 측정
  lowpulseoccupancy = lowpulseoccupancy + duration;
  Serial.print(".");
  delay(1000);
  }


