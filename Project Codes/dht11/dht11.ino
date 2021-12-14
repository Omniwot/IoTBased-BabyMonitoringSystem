/*int led = D7;
int sound_digital = D8;
int sound_analog = A0;

void setup(){
  Serial.begin(9600);
  pinMode(led, OUTPUT);
  pinMode(sound_digital, INPUT);  
}

void loop(){
  int val_digital = digitalRead(sound_digital);
  int val_analog = analogRead(sound_analog);

  Serial.print(val_analog);
  Serial.print("\t");
  Serial.println(val_digital);

  if (val_digital == HIGH)
  {
    digitalWrite (led, HIGH);
    delay(3000);
    }
  else
  {
    digitalWrite (led, LOW);
    }
}*/
#include "DHTesp.h"
DHTesp dht;
void setup()
{
Serial.begin(9600);
Serial.println();
Serial.println("Status\t\tHumidity (%)\t\tTemperature (C)\t");
dht.setup(14, DHTesp::DHT11); // GPIO14
}
void loop()
{
//delay(dht.getMinimumSamplingPeriod() + 100);
delay(1500);
float humidity = dht.getHumidity();
float temperature = dht.getTemperature();
Serial.print("Status: ");
Serial.print(dht.getStatusString());
Serial.print("\t");
Serial.print("Humidity (%): ");
Serial.print(humidity, 1);
Serial.print("\t");
Serial.print("Temperature (C): ");
Serial.print(temperature, 1);
Serial.print("\t");
Serial.println();
}
