#include <SPI.h>         
#include <TimeLib.h>     
#include <Ethernet.h>   
#include <Time.h>
#include <Udp.h>
#include <TimeLib.h>
#include <Wire.h>
#include <DS1307RTC.h> 

const int GSR=A0;
int sensorValue=0;
int gsr_average=0;
 
void setup(){

  Serial.begin(9600);
  while (!Serial) ; // wait until Arduino Serial Monitor opens
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
  else
     Serial.println("RTC has set the system time");  
}
 
void loop(){
  long sum=0;
  if (Serial.available()) {
    time_t t = processSyncMessage();
    if (t != 0) {
      RTC.set(t);   
      setTime(t);          
    }

  }

  for(int i=0;i<10;i++)           
      {
      sensorValue=analogRead(GSR);
      sum += sensorValue;
      delay(5);
      }
   gsr_average = sum/10;
   Serial.println(gsr_average);
   digitalClockDisplay(); 
   delay(1000);
}

void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year()); 
  Serial.println(); 
}

void printDigits(int digits){
  
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


#define TIME_HEADER  "T"  

unsigned long processSyncMessage() {
  unsigned long pctime = 0L;
  const unsigned long DEFAULT_TIME = 1357041600; // Jan 1 2013 

  if(Serial.find(TIME_HEADER)) {
     pctime = Serial.parseInt();
     return pctime;
     if( pctime < DEFAULT_TIME) { // check the value is a valid time (greater than Jan 1 2013)
       pctime = 0L; 
     }
  }
  return pctime;
}



