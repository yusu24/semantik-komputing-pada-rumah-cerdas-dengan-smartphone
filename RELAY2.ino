#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         
#include <FirebaseArduino.h> //https://github.com/tzapu/WiFiManager

#define FIREBASE_HOST "yusufskripsi.firebaseio.com"
#define FIREBASE_AUTH "DKlOA2bZe5e9wqf2ifagntXlnsoOtFC1CnnId4CB"

const int RELAY = D1; // Defining LDR PIN 
int input_val = 0;  // Varible to store Input values
const int LED = D5;
String sensor;
//deklarasi untuk millis
unsigned long startMillis;
unsigned long currentMillis;
unsigned long hasil;
long Second;
long minutes;
long hour;
String durasi; 

void setup() {
    // put your setup code here, to run once:

    Serial.begin(115200);
    startMillis = millis();
    pinMode(RELAY, OUTPUT);
    digitalWrite(RELAY, HIGH);
    pinMode(LED, OUTPUT);
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    wifiManager.resetSettings();
    
    //set custom ip for portal
    wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,4), IPAddress(10,0,1,4), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAP3");
    //or use this for auto generated name ESP + ChipID
    wifiManager.autoConnect();

    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");

            //ngitung durasi
    currentMillis =millis();
    hasil = currentMillis - startMillis;
    Second = hasil / 1000% 60;
    minutes = hasil / (60*1000)% 60;
    hour = hasil / (60*60*1000);

    //concat long to string
    String StartMilisString;
    StartMilisString.concat(hour);
    StartMilisString.concat(":");
    StartMilisString.concat(minutes);
    StartMilisString.concat(":");
    StartMilisString.concat(Second); 
    Serial.print("connect = ");
    Serial.println(StartMilisString); 
    Serial.print("durasi awal = ");
    Serial.println(durasi);
    
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.setInt("Aktuator/Relay2/active", 1);
    Firebase.setString("Aktuator/Relay2/connection", "");
    Firebase.setInt("Aktuator/Relay2/data", 1);
    Firebase.setString("Aktuator/Relay2/dconnect", StartMilisString);
    Firebase.setString("Aktuator/Relay2/name", "Relay2");    
    Firebase.setString("Aktuator/Relay2/durasi", durasi);
}

void loop() {
    durasi = "";
    int data = Firebase.getInt("Aktuator/Relay2/data");
      if(data == 0 ){
        digitalWrite(RELAY, LOW);
      Serial.println(data);  
      }else{
        digitalWrite(RELAY, HIGH);
     Serial.println(data);  
      }

    currentMillis =millis();
    hasil = currentMillis - startMillis;
    Second = hasil / 1000% 60;
    minutes = hasil / (60*1000)% 60;
    hour = hasil / (60*60*1000);

    durasi.concat(hour);
    durasi.concat(":");
    durasi.concat(minutes);
    durasi.concat(":");
    durasi.concat(Second);

    Serial.print("Durasi = ");
    Serial.println(durasi);
    Firebase.setString("Aktuator/Relay2/durasi", durasi);
    //sampai sini ngitung durasi
   delay(1000);       
    
}
