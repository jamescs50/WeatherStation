const int sensorPin= A1;
const int Rrefer = 10000;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int sensorVal = analogRead(sensorPin);
  Serial.print("   Direction: ");
  Serial.println(WindVaneDirection(sensorVal));
 
  delay(1000);
}

float WindVaneDirection(int sensorVal)
{
  float vl = (sensorVal * 5.0) / 1023.0;
  float Rtest = (5.0 - vl) * (Rrefer / vl);
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
