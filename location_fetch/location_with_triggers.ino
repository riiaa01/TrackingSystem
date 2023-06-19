#include <TinyGPS.h> 
#include <SoftwareSerial.h>
 int data=4; 
int start=2; 
int count=0; 
int count1; 
unsigned long temp=0; 

SoftwareSerial SIM900(7, 8);
TinyGPS gps;
 int tempSensorPin=A1; 
 int tempSensorValue; 



void setup() 
{ 
Serial.begin(9600);  
 SIM900.begin(9600); 
pinMode(data,INPUT); 
 pinMode(start,INPUT_PULLUP); 
}

void loop() 
{ 
Serial.print("GSM BASED GPS TRACKING SYSTEM :All OK"); 
 Serial.println();
temp=millis(); //used to return the number of milliseconds at the time, the Arduino board begins running the current program.
while(millis()<(temp+10000)) 
{ 
if(analogRead(data)<100) //reads the value from the specified analog pin.
{ 
count=count+1; 
} 
} 
count=count*6; 
count1=count/1000;
tempSensorValue=analogRead(tempSensorPin); 
tempSensorValue=(tempSensorValue*500)/1024;

heart_beat();
} 


void heart_beat() 
{ 
while((digitalRead(start)>0)){
  gps_loc();
if((count1>=29)||(tempSensorValue>=28)||(tempSensorValue<=20)); {
  Serial.println("Sensing danger");
  if(count1>=29 && tempSensorValue<28){
    Serial.print("Heart Beat: "); 
    Serial.print(count1); 
    Serial.println("BPM"); 
    Serial.print("Temperature:"); 
    Serial.println(tempSensorValue); 
    Serial.print("HeartBeat too fast, fear detected");
     Serial.println("");
  }
  else if(tempSensorValue>28 &&count1<29 ){
    Serial.print("Heart Beat: "); 
    Serial.print(count1); 
    Serial.println("BPM"); 
    Serial.print("Temperature:"); 
    Serial.println(tempSensorValue); 
    Serial.print("Temperature too high, fear detected");
     Serial.println("");
  }
else if(tempSensorValue<20 && count1<29){
   Serial.print("Heart Beat: "); 
    Serial.print(count1); 
    Serial.println("BPM"); 
    Serial.print("Temperature:"); 
    Serial.println(tempSensorValue); 
    Serial.print("Temperature too low, fear detected");
     Serial.println("");
}

else{
   Serial.print("Heart Beat: "); 
    Serial.print(count1); 
    Serial.println("BPM"); 
    Serial.print("Temperature:"); 
    Serial.println(tempSensorValue); 
    Serial.print("Heartbeat and temperature abnormal, fear detected");
     Serial.println("");
}

  gps_loc();
}

} 
}

void gps_loc()  
{ 
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      //Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }

  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    String googleMapsLink;

    gps.f_get_position(&flat, &flon, &age);   
    Serial.print("AT+CMGF=1\r"); 
    delay(400);
    Serial.println("AT + CMGS = \"+91xxxxxxxxxx\"");// recipient's mobile number with country code
    delay(300);
    Serial.print("Latitude = ");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" Longitude = ");
    Serial.println(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    String latitude= (String)flat;
    String longitude= (String)flon;
    googleMapsLink = "https://www.google.com/maps/place/" + latitude + "," + longitude;
    Serial.println("Here is the Google Maps link");
    Serial.println(googleMapsLink);
    delay(200);
    Serial.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    Serial.println();
  }
 
  Serial.println(failed);
} 

