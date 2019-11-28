#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         
#include <FirebaseArduino.h> //https://github.com/tzapu/WiFiManager

#define FIREBASE_HOST "yusufskripsi.firebaseio.com"
#define FIREBASE_AUTH "DKlOA2bZe5e9wqf2ifagntXlnsoOtFC1CnnId4CB"

const int LDR = D1; // Defining LDR PIN 
int input_val = 0;  // Varible to store Input values
const int LED = D0;
void Relasi();
//deklarasi untuk millis
unsigned long startMillis;
unsigned long currentMillis;
unsigned long hasil;
long Second;
long minutes;
long hour;
String durasi; 
String relasi;
void setup() {
    // put your setup code here, to run once:

    Serial.begin(115200);
    startMillis = millis();
    pinMode(LDR, INPUT);
    pinMode(LED, OUTPUT);
    //WiFiManager
    //Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wifiManager;
    //reset saved settings
    wifiManager.resetSettings();
    
    //set custom ip for portal
    wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,10), IPAddress(10,0,1,10), IPAddress(255,255,255,0));

    //fetches ssid and pass from eeprom and tries to connect
    //if it does not connect it starts an access point with the specified name
    //here  "AutoConnectAP"
    //and goes into a blocking loop awaiting configuration
    wifiManager.autoConnect("AutoConnectAPLDR");
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
    Firebase.setInt("Sensor/LDR/active", 1);
    Firebase.setString("Sensor/LDR/connection", "");
    Firebase.setInt("Sensor/LDR/data", 1);
    Firebase.setString("Sensor/LDR/dconnect", StartMilisString);
    Firebase.setString("Sensor/LDR/name", "LDR");
    Firebase.setString("Sensor/LDR/durasi", durasi);
}


void loop() {
    // put your main code here, to run repeatedly:
   durasi = "";
   input_val = digitalRead(LDR);      // Reading Input
   Serial.print("LDR value is : " );                        
   Serial.println(input_val);        // Writing input on serial monitor. 
   Firebase.setInt("Sensor/LDR/data",input_val);
   if(Firebase.failed()){
           Serial.println(Firebase.error());
           Serial.println("tidak terkirim");
         }else{
         Serial.println("Uploded");
        }
    Relasi();
//    delay (1000);
    //ngitung durasi

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
    Firebase.setString("Sensor/LDR/durasi", durasi);
    //sampai sini ngitung durasi
   delay(1000);
}
        
void Relasi(){

    currentMillis =millis();
    hasil = currentMillis - startMillis;
    Second = hasil / 1000% 60;
    minutes = hasil / (60*1000)% 60;
    hour = hasil / (60*60*1000);
    FirebaseObject Aktuator = Firebase.get("Aktuator");
    JsonVariant variant = Aktuator.getJsonVariant();

    for( const auto& kv : variant.as<JsonObject>() ) {
    Serial.println(kv.key);
    String nama_aktuator = kv.key;
    if(Firebase.getString("Aktuator/"+nama_aktuator+"/connection") == "LDR")
    {
     
      Firebase.setInt("Aktuator/"+nama_aktuator+"/data",input_val == 0? 1 : 0);        
   
    }
    }     
}
