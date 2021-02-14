//control
unsigned long thisTime;
unsigned long lastTime;
int controlMin = 0;

//WindVane
const int vaneSensorPin= A0;
const int Rrefer = 100000;  //strength of comparison resistor

//Anemometer
int windCounter;
int windTime;

//Rain guage
int rainCounter;
long rainTime;
int rainReadings[60]; //rain readings over the last hour




void setup() {
  // put your setup code here, to run once:
  attachInterrupt(digitalPinToInterrupt(2), wind_ISR, HIGH);  //pin 2 (Uno)
  attachInterrupt(digitalPinToInterrupt(3), rain_ISR, HIGH);  //pin 3 (Uno)
 
  Serial.begin(9600); 
}

void loop() {
  thisTime = millis();
  if ((thisTime - lastTime) >= 1000) //a min since last run time
  {
    lastTime = thisTime; //reset the clock
    Serial.println(controlMin);
    
    //read the wind vane
    int wVaneVal = analogRead(vaneSensorPin);
    Serial.print("Wind Direction: ");
    Serial.println(WindVaneDirection(wVaneVal));

    //store the rain counter
    rainReadings[controlMin] = rainCounter;
    rainCounter =0;
    Serial.print("Rain last min: ");
    Serial.print(rainReadings[controlMin]);
    int rainTotal = 0;
    for (int i = 0; i < 60;i ++){
      rainTotal += rainReadings[i];
    }
    Serial.print(" Rain last hour: ");
    Serial.println(rainTotal);

    //anemometer
    Serial.print("Wind Speed Reading: ");
    Serial.println(windCounter);
    windCounter = 0;


    //move the minute control
    controlMin ++;
    if (controlMin >= 60){
      controlMin = 0;
    }
  }
}

void wind_ISR(){
  if (millis()-windTime >50)
  {
    windTime = millis();
    windCounter ++;
    detachInterrupt(digitalPinToInterrupt(2));
    attachInterrupt(digitalPinToInterrupt(2), wind_reset_ISR, HIGH);
  } 
}

void wind_reset_ISR(){
  detachInterrupt(digitalPinToInterrupt(2));
  attachInterrupt(digitalPinToInterrupt(2), wind_ISR, LOW);

}


void rain_ISR(){
  if (millis()-rainTime >50)
  {
    rainTime = millis();
    rainCounter ++;
    detachInterrupt(digitalPinToInterrupt(3));
    attachInterrupt(digitalPinToInterrupt(3), rain_reset_ISR, HIGH);
  } 

}

void rain_reset_ISR(){
  detachInterrupt(digitalPinToInterrupt(3));
  attachInterrupt(digitalPinToInterrupt(3), rain_ISR, LOW);

}


float WindVaneDirection(int sensorVal)
{
  Serial.println(sensorVal);
  float vl = (sensorVal * 5.0) / 1023.0;
  Serial.println(vl);
  float Rtest = (5.0 - vl) * (Rrefer / vl);
  Serial.println(Rtest);
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
