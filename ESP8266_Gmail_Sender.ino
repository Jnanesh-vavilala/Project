#include <ESP8266WiFi.h>
#include "Gsender.h"
#include <FirebaseArduino.h>
#include "DHT.h"

#pragma region Globals

#define FIREBASE_HOST "homeautomation-3304d.firebaseio.com"  
#define FIREBASE_AUTH "9MfduoDs6Ccy1a1YCezBnZra7rhfTbhkUdWbW8RB"
#define WIFI_SSID "Jstar" // Change the name of your WIFI
#define WIFI_PASSWORD "jstar123" // Change the password of your WIFI
#define DHTPIN 14    // Data Pin of DHT 11 , for NodeMCU D5 GPIO no. is 14

#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

uint8_t connection_state = 0;                    // Connected to WIFI or not
uint16_t reconnect_interval = 10000;             // If not connected wait time to try again
#pragma endregion Globals

uint8_t WiFiConnect(const char* nSSID = nullptr, const char* nPassword = nullptr)
{
    static uint16_t attempt = 0;
    Serial.print("Connecting to ");
    if(nSSID) {
        WiFi.begin(nSSID, nPassword);  
        Serial.println(nSSID);
    } else {
        WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
        Serial.println(WIFI_SSID);
    }

    uint8_t i = 0;
    while(WiFi.status()!= WL_CONNECTED && i++ < 50)
    {
        delay(200);
        Serial.print(".");
    }
    ++attempt;
    Serial.println("");
    if(i == 51) {
        Serial.print("Connection: TIMEOUT on attempt: ");
        Serial.println(attempt);
        if(attempt % 2 == 0)
            Serial.println("Check if access point available or SSID and Password\r\n");
        return false;
    }
    Serial.println("Connection: ESTABLISHED");
    Serial.print("Got IP address: ");
    Serial.println(WiFi.localIP());
    return true;
}

void Awaits()
{
    uint32_t ts = millis();
    while(!connection_state)
    {
        delay(50);
        if(millis() > (ts + reconnect_interval) && !connection_state){
            connection_state = WiFiConnect();
            ts = millis();
        }
    }
}
const int trig=D0;
const int echo=D1;

void setup()
{
    Serial.begin(115200);
    connection_state = WiFiConnect();
    if(!connection_state)  // if not connected to WIFI
        Awaits();          // constantly trying to connect
        pinMode(D0,OUTPUT);
    pinMode(D1,INPUT);
     dht.begin();
    Serial.println ("");
    Serial.println ("WiFi Connected!");
    Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);

    
}
long duration,cm;
String sp=" ";
String msg1="Start",msg,msg2;
int i=0;

void loop(){
  i=i+1;
  float h = dht.readHumidity();
  float t = dht.readTemperature();  // Reading temperature as Celsius (the default)
  Firebase.setFloat ("Temp",t);
  Firebase.setFloat ("Humidity",h);
  delay(200);
  
   digitalWrite(trig,LOW);
  delayMicroseconds(2);
  digitalWrite(trig,HIGH);
  delayMicroseconds(5);
  digitalWrite(trig,LOW);

  duration=pulseIn(echo,HIGH);
  cm=microsecondsToCentimeters(duration);
  Serial.print(cm);
  Serial.println(" cm");
  if(cm<=10)
  {
     Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    String subject = "IOT INTRUDER ALERT";
    String msg="Please check your premesis, There is an unathorised access !!! ";
    if(gsender->Subject(subject)->Send("srajan7309@gmail.com",msg)) {
        Serial.println("Message send.");
    } else {
        Serial.print("Error sending message: ");
        Serial.println(gsender->getError());
    }
    
  }
  msg2=msg2+sp+t+sp+sp;
  Serial.println(i);
  Serial.println(" ");
  Serial.println(t);
  delay(1000);
  if(i>=20)
  {
    Serial.println(msg2);
    i=0;
     Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    String subject = "IOT GMAIL TEMPERATURE ACQUISITION";
   
    if(gsender->Subject(subject)->Send("srajan7309@gmail.com",msg2)) {
        Serial.println("Message  Temperature send.");
    } else {
        Serial.print("Temperature sending message: ");
        Serial.println(gsender->getError());
    }
    msg1=" ";
    msg1="Start";
    delay(100);
    
  }
  if(t>=30)
  {
    Serial.println("Alert message");
    i=0;
     Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
    String subject = "IOT GMAIL TEMPERATURE ACQUISITION";
    
    if(gsender->Subject(subject)->Send("srajan7309@gmail.com","Temperture crosses the limit")) {
        Serial.println("Message  Temperature send.");
    } else {
        Serial.print("Temperature sending message: ");
        Serial.println(gsender->getError());
    }
   
    delay(100);
    
  }
  delay(1000);
  }
  long microsecondsToCentimeters(long microseconds)
{
return microseconds/29/2;  
}
