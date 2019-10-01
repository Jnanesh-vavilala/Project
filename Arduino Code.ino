

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#define FIREBASE_HOST "homeautomation-3304d.firebaseio.com/"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "9MfduoDs6Ccy1a1YCezBnZra7rhfTbhkUdWbW8RB"  
#define WIFI_SSID "Jstar"                                               //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "jstar123" 

#define Relay1 16 //P16
int val1;

#define Relay2 17 //P17
int val2;

#define Relay3 18  //P18
int val3;

#define Relay4 19 //P19
int val4;
void setup() 
{
  Serial.begin(115200);                                                   // Select the same baud rate if you want to see the datas on Serial Monitor
  pinMode(Relay1,OUTPUT);
  pinMode(Relay2,OUTPUT);
  pinMode(Relay3,OUTPUT);
  pinMode(Relay4,OUTPUT);

  digitalWrite(Relay1,HIGH);
  digitalWrite(Relay2,HIGH);
  digitalWrite(Relay3,HIGH);
  digitalWrite(Relay4,HIGH);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("first wifi connected:");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  Firebase.setInt("S1",0);                     //Here the varialbe"S1","S2","S3" and "S4" needs to be the one which is used in our Firebase and MIT App Inventor
  Firebase.setInt("S2",0); 
  Firebase.setInt("S3",0); 
  Firebase.setInt("S4",0); 
  
  String u=Firebase.getString("usrnm");
  String p=Firebase.getString("passwd");
  
  int ul=u.length();
  int pl=p.length();
  
  String wifiu=u.substring(2,ul-2);
  String wifip=p.substring(2,pl-2);
  
  char wustr[ul];
  char wpstr[pl];
  
  strcpy(wustr, wifiu.c_str()); 
  strcpy(wpstr, wifip.c_str()); 
  
  Serial.println(wustr);
  Serial.println(wpstr);
  
  if(wustr!=WIFI_SSID)
  {
  WiFi.begin(wustr,wpstr);
  Serial.print("connecting");
  while (WiFi.status()!=WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("second wifi connected:");
  Serial.println(WiFi.localIP());
  }
}


void loop() 
{
  
      
  val1=Firebase.getString("S1").toInt();                                        //Reading the value of the varialble Status from the firebase
  
  if(val1==1)                                                             // If, the Status is 1, turn on the Relay1
     {
      digitalWrite(Relay1,HIGH);
      Serial.println("light 1 ON");
    }
    else if(val1==0)                                                      // If, the Status is 0, turn Off the Relay1
    {                                      
      digitalWrite(Relay1,LOW);
      Serial.println("light 1 OFF");
    }

  val2=Firebase.getString("S2").toInt();                                        //Reading the value of the varialble Status from the firebase
  
  if(val2==1)                                                             // If, the Status is 1, turn on the Relay2
     {
      digitalWrite(Relay2,HIGH);
      Serial.println("light 2 ON");
    }
    else if(val2==0)                                                      // If, the Status is 0, turn Off the Relay2
    {                                      
      digitalWrite(Relay2,LOW);
      Serial.println("light 2 OFF");
    }

   val3=Firebase.getString("S3").toInt();                                        //Reading the value of the varialble Status from the firebase
  
  if(val3==1)                                                             // If, the Status is 1, turn on the Relay3
     {
      digitalWrite(Relay3,HIGH);
      Serial.println("light 3 ON");
    }
    else if(val3==0)                                                      // If, the Status is 0, turn Off the Relay3
    {                                      
      digitalWrite(Relay3,LOW);
      Serial.println("light 3 OFF");
    }

   val4=Firebase.getString("S4").toInt();                                        //Reading the value of the varialble Status from the firebase
  
  if(val4==1)                                                             // If, the Status is 1, turn on the Relay4
     {
      digitalWrite(Relay4,HIGH);
      Serial.println("light 4 ON");
    }
    else if(val4==0)                                                      // If, the Status is 0, turn Off the Relay4
    {                                      
      digitalWrite(Relay4,LOW);
      Serial.println("light 4 OFF");
    }  
    
    
}
