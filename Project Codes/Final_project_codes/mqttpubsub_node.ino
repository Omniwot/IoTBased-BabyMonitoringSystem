/*Buzzer D2
 * DHT11 D5
 * Sound sensor D8
 */


#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

//Notes
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0

/************************* WiFi Access Point *********************************/
#define WLAN_SSID       "OPPO Reno2 F"
#define WLAN_PASS       "harshal222"
/************************* Adafruit.io Setup *********************************/
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                   // use 8883 for SSL
#define AIO_USERNAME    "Omniwot"
#define AIO_KEY         "aio_bnXw32C0Kn0Qd0k19qudXwlTnfDf"
/************* DHT11 Setup ********************************************/
#define DHTPIN 14
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
/************ Global State (you don't need to change this!) ******************/
// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
/****************************** Feeds ***************************************/
// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Publish temperature = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temp");
Adafruit_MQTT_Publish humidity = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/hum");
Adafruit_MQTT_Publish sounds = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/cry");
Adafruit_MQTT_Publish ac = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/ac");
Adafruit_MQTT_Subscribe onoffbutton = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/music-swing");
Adafruit_MQTT_Publish sound_alert = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/music-swing");
/*************************** Sketch Code ************************************/
// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();
void play_song(bool a);
int sound_digital = 15;

void setup() {
  
  pinMode(sound_digital, INPUT);  
  
  mqtt.subscribe(&onoffbutton);
  Serial.begin(115200);
  dht.begin();
  delay(10);
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
  
}
uint32_t x=0;

bool music=0; 

void loop() {
  
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();
  // Read humidity
  float h = dht.readHumidity();
  // Read temperature as Celsius
  float t = dht.readTemperature();
  //publish temperature and humidity
  Serial.print(F("\nTemperature: "));
  Serial.print(t);
  Serial.print(F("\nHumidity: "));
  Serial.print(h);
  temperature.publish(t);
  humidity.publish(h);
  //Code to turn ac on if temperature or humidity too high
  if(t>=25 || h>=75){
    ac.publish(1);
    }
   else{
    ac.publish(0);
   }

  //Subscribe to music feed
  Adafruit_MQTT_Subscribe *subscription;
  subscription = mqtt.readSubscription(5000);
    // Check if its the onoff button feed
    if (subscription == &onoffbutton) {
      Serial.print(F("On-Off button: "));
      Serial.println((char *)onoffbutton.lastread);
      
      if (atoi((char *)onoffbutton.lastread) == 0) {
        play_song(0); 
      }
      if (atoi((char *)onoffbutton.lastread) == 1) {
        play_song(1); 
      }
    }
    
  //Sound sensing
  int val_digital = digitalRead(sound_digital);
  Serial.println(val_digital);
  if(val_digital==1){
    sounds.publish("Baby is crying");
    sound_alert.publish(1);
    play_song(1); 
    sounds.publish("All is Well");
    sound_alert.publish(0);  
  }
  if(val_digital==0){
    sounds.publish("All is Well");
    sound_alert.publish(0);
  }
   
  delay(3000);
  
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
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

void play_song(bool a){
  
  int tempo = 140;

// change this to whichever pin you want to use
int buzzer = 4;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  //Merry Christmas
  // Score available at https://musescore.com/user/6208766/scores/1497501
  
  NOTE_C7,4, //1
  NOTE_F7,4, NOTE_F7,8, NOTE_G7,8, NOTE_F7,8, NOTE_E7,8,
  NOTE_D7,4, NOTE_D7,4, NOTE_D7,4,
  NOTE_G7,4, NOTE_G7,8, NOTE_A7,8, NOTE_G7,8, NOTE_F7,8,
  NOTE_E7,4, NOTE_C7,4, NOTE_C7,4,
  NOTE_A7,4, NOTE_A7,8, NOTE_AS7,8, NOTE_A7,8, NOTE_G7,8,
  NOTE_F7,4, NOTE_D7,4, NOTE_C7,8, NOTE_C7,8,
  NOTE_D7,4, NOTE_G7,4, NOTE_E7,4,

  NOTE_F7,2, NOTE_C7,4, //8 
  NOTE_F7,4, NOTE_F7,8, NOTE_G7,8, NOTE_F7,8, NOTE_E7,8,
  NOTE_D7,4, NOTE_D7,4, NOTE_D7,4,
  NOTE_G7,4, NOTE_G7,8, NOTE_A7,8, NOTE_G7,8, NOTE_F7,8,
  NOTE_E7,4, NOTE_C7,4, NOTE_C7,4,
  NOTE_A7,4, NOTE_A7,8, NOTE_AS7,8, NOTE_A7,8, NOTE_G7,8,
  NOTE_F7,4, NOTE_D7,4, NOTE_C7,8, NOTE_C7,8,
  NOTE_D7,4, NOTE_G7,4, NOTE_E7,4,
  NOTE_F7,2

  
};
if(a==1){
// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;


  // iterate over the notes of the melody.
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(buzzer, melody[thisNote], noteDuration * 0.9);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // stop the waveform generation before the next note.
    noTone(buzzer);
  }}
  else{
    noTone(buzzer);
  }


}
