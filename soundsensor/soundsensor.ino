int sound_digital = D5;
int sound_analog = A0;

unsigned long act_milli;
unsigned long last_ping;

#include <ESP8266WiFi.h>
#include "Pushover.h"
 
const char* ssid = "WiFiSSID";
const char* password = "foobar";

Pushover po = Pushover("AppKey","UserKey", UNSAFE);

void setup(){
  Serial.begin(9600);
  Serial.setTimeout(2000);
  while(!Serial) { }
  Serial.println("I'm awake.");
  pinMode(sound_digital, INPUT);  
  pinMode(D4, OUTPUT);
   
  Serial.printf("Connecting to %s ", ssid);
  int retry_count = 0;
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED && retry_count < 20)
  {
	  delay(500);
	  Serial.print(".");
	  retry_count++;
  }
  
  Serial.println(" connected");
   
  digitalWrite(D4, LOW);
  delay(10000);
  digitalWrite(D4, HIGH);

  last_ping = millis();
}

void loop(){
  act_milli = millis();
  if ((act_milli-last_ping) > (100 * 1000)){
	if (WiFi.status() != WL_CONNECTED) {
	  int retry_count = 0;
	  WiFi.reconnect();
	  while ((WiFi.status() != WL_CONNECTED) && (retry_count < 20)) {
		  delay(500);
		  Serial.print(".");
		  retry_count++;
	  }
	}
	last_ping = act_milli;
  }
  
  // Serial.println("Test");  
  int val_digital = digitalRead(sound_digital);  
  int val_analog = analogRead(sound_analog);

  // Serial.print(val_analog);
  // Serial.print("\t");
  // Serial.println(val_digital);

  if (val_digital == HIGH)
  {
	 po.setMessage("Klingeling");
	 po.setPriority(1); // I want to be notified even in night mode
	 Serial.println(po.send());
	 delay(3000);
  }
  else
  {
  }
}
