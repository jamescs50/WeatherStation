

#include <ArduinoMqttClient.h>
#if defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_AVR_UNO_WIFI_REV2)
  #include <WiFiNINA.h>
#elif defined(ARDUINO_SAMD_MKR1000)
  #include <WiFi101.h>
#elif defined(ARDUINO_ESP8266_ESP12)
  #include <ESP8266WiFi.h>
#endif

#include "arduino_secrets.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)

// To connect with SSL/TLS:
// 1) Change WiFiClient to WiFiSSLClient.
// 2) Change port value from 1883 to 8883.
// 3) Change broker value to a server with a known SSL/TLS root certificate 
//    flashed in the WiFi module.

const bool debugging = false;

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);

const long interval = 1000;
unsigned long previousMillis = 0;

int count = 0;

//Weather Station Variables
//loop control
unsigned long thisTime;
unsigned long lastTime;
int controlMin = 0;

const int vaneSensorPin= A0;
const int Rrefer = 1000;

//Anemometer
unsigned volatile int windCounter = 0;
int windTime;
char windSpeedTopic = "weather/station1/windspeed";
char windDirTopic = "weather/station1/winddirection";


//Rain guage
unsigned volatile int rainCounter = 0;
int rainReadings[60]; //rain readings over the last hour
long rainTime;
char rainTopic = "weather/station1/rain";

boolean lightOn = false;

void setup() {
  if (debugging)
  {
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }

    // attempt to connect to Wifi network:
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);

  }
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    //Serial.print(".");
    delay(5000);
  }

  if (debugging)
  {
    //Serial.println("You're connected to the network");
    //Serial.println();

    // You can provide a unique client ID, if not set the library uses Arduino-millis()
    // Each client must have a unique client ID
    // mqttClient.setId("clientId");
  
    // You can provide a username and password for authentication
    // mqttClient.setUsernamePassword("username", "password");
  
    Serial.print("Attempting to connect to the MQTT broker: ");
    Serial.println(MQTT_SERVER);
  }
  if (!mqttClient.connect(MQTT_SERVER, MQTT_PORT)) {
    if (debugging)
    {
      Serial.print("MQTT connection failed! Error code = ");
      Serial.println(mqttClient.connectError());
    }
    while (1);
  }

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2,INPUT);
  pinMode(3,INPUT);
  pinMode(vaneSensorPin,INPUT);

  
  attachInterrupt(digitalPinToInterrupt(2), rain_ISR, FALLING);  //pin 2 (Uno)
  //while the rain guage is off at rest it is possible for the anemometer to rest with the switch on 
  // - so listen to the switch turning off instead of just off
  attachInterrupt(digitalPinToInterrupt(3), wind_ISR, FALLING);  //pin 2 (Uno)


}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time a message was sent
    previousMillis = currentMillis;

    //LED on board
    if (lightOn)
    {
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    }
    lightOn = !(lightOn);


    

    int wVaneVal = analogRead(vaneSensorPin);
    float wDirection = WindVaneDirection(wVaneVal);

    if (debugging)
    {
      Serial.print("Rain: ");
      Serial.print(rainCounter);

      //The wind speed of 0.33m/s causes the switch to close once.
      Serial.print(" Wind Speed: ");
      Serial.print(windCounter);
       
      Serial.print(" Wind Direction: ");
      Serial.println(wDirection);
    }


    
    
    mqttClient.beginMessage("weather/station1/rain");
    mqttClient.print(rainCounter);
    mqttClient.endMessage();

    mqttClient.beginMessage("weather/station1/windspeed");
    mqttClient.print(windCounter);
    mqttClient.endMessage();

    mqttClient.beginMessage("weather/station1/winddirection");
    mqttClient.print(wDirection);
    mqttClient.endMessage();

    rainCounter =0;
    windCounter = 0;

    

  }
}



void rain_ISR(){
  if (millis()-rainTime >10)
  {
    rainTime = millis();
    rainCounter ++;
  } 

}
void wind_ISR(){
  if (millis()-windTime >50)
  {
    windTime = millis();
    windCounter ++;
  } 
}


float WindVaneDirection(int sensorVal)
{
  float vl = (sensorVal * 5.0) / 1023.0;
  float Rtest = (5.0 - vl) * (Rrefer / vl);
  if (false)  //debug
  {
    Serial.print("Sensor Val: ");
    Serial.print(sensorVal);    
    //Serial.println(vl);    
    Serial.print(" Resistance: ");
    Serial.println(Rtest);
  }
  if (Rtest > 200000)
  {
    return -1; //error state = probably disconnected
  }
  else if (Rtest > 100000)
  {
    return 270;
  }
  else if(Rtest > 60000)
  {
    return 315;
  }
  else if(Rtest > 40000)
  {
    return 292.5;
  }
  else if(Rtest > 30000)
  {
    return 0;
  }
  else if(Rtest > 20000)
  {
    return 337.5;
  }
  else if(Rtest > 15000)
  {
    return 225;
  }
  else if(Rtest > 10000)
  {
    return 247.5;
  }
  else if(Rtest > 8000)
  {
    return 45;
  }
  else if(Rtest > 6000)
  {
    return 22.5;
  }
  else if(Rtest > 3500)
  {
    return 180;
  }
  else if(Rtest > 3000)
  {
    return 202.5;
  }
  else if(Rtest > 2000)
  {
    return 135;
  }
  else if(Rtest > 1300)
  {
    return 157.5;
  }
  else if(Rtest > 950)
  {
    return 90;
  }
  else if(Rtest > 800)
  {
    return 67.5;
  }
  else if(Rtest > 600)
  {
    return 112.5;
  }
  else
  {
    return -1; //resistance too low
  }
  
}
