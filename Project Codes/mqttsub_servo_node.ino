
#include <Servo.h>
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "OPPO Reno2 F"
#define WLAN_PASS       "harshal222"
/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Omniwot"
#define AIO_KEY         "aio_bnXw32C0Kn0Qd0k19qudXwlTnfDf"

/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

Adafruit_MQTT_Subscribe cradlebutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/music-swing");
Adafruit_MQTT_Subscribe acbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/ac");
void MQTT_connect();
Servo servo;
void rock_crradle();
void AC_switch(int a);



void setup() {
    
      pinMode(12, OUTPUT);
      Serial.begin(115200);
      servo.attach(2); //D4
      servo.write(0);
      delay(1000);
    
    // Connect to WiFi access point.
      Serial.println(); Serial.println();
      Serial.print("Connecting to ");
      Serial.println(WLAN_SSID);
      WiFi.begin(WLAN_SSID, WLAN_PASS);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println();
      Serial.println("WiFi connected");
      Serial.println("IP address: "); Serial.println(WiFi.localIP());
      mqtt.subscribe(&cradlebutton);
      mqtt.subscribe(&acbutton);
}

void loop() {
   MQTT_connect();
  Adafruit_MQTT_Subscribe *subscription;
  

  while(subscription = mqtt.readSubscription(5000)){
    // Check if its the onoff button feed
    if (subscription == &cradlebutton) {
      Serial.print(F("On-Off button: "));
      Serial.println((char *)cradlebutton.lastread);
     
      if (atoi((char *)cradlebutton.lastread) == 1) {
      //The following code runs the servo until the the last published value becomes zero 

        
           while(atoi((char *)cradlebutton.lastread) != 0){
            
           rock_cradle();
            MQTT_connect();
            Adafruit_MQTT_Subscribe *subscription;
          
            subscription = mqtt.readSubscription(3000);
              // Check if its the onoff button feed
              if (subscription == &cradlebutton) {
                Serial.print(F("On-Off button: "));
                Serial.println((char *)cradlebutton.lastread);
       
          }
      }
    }
    }
    //To switch ac
    if (subscription == &acbutton) {
      Serial.print(F("AC button: "));
      Serial.println((char *)acbutton.lastread);
     
      if (atoi((char *)acbutton.lastread) == 1) {
      //This will be changed later to control A/C
       AC_switch(1);
      
      }
      if (atoi((char *)acbutton.lastread) == 0) {
      //This will be changed later to control A/C
      AC_switch(0);
      
      }
    }

    
    
    }
    delay(1000);
}

void rock_cradle(){
   for (int i=0;i<10;i++){
          servo.write(0);

          delay(200);

          servo.write(180);

          delay(1000);
   }

}
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
void AC_switch(int a){
    if(a==1){
      digitalWrite(12, HIGH);
    }
    if(a==0){
      digitalWrite(12, LOW);
    }

}
