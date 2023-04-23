///* ARDUINO CODE EXAMPLE, COPY AND PASTE IT IN NEW PROJECT */
#include <TimerOne.h>


unsigned int sensorValue[4] ; //= 0;  
      
String inputString = "";         // a String to hold incoming data
unsigned  long int count = 0;
unsigned long int NbrPoints = 0;
unsigned int activatedChannels  = 1;
unsigned int freeRunCmd    = 0;
bool SendingStarted = false;

unsigned int MaxChannels    = 4;

void setup() {
   
  Serial.begin(115200);
  inputString.reserve(200);
  Timer1.initialize(1000);
  Timer1.stop();
}
void serialSend(void)
{
      count++;
      for(int i=0 ; i< MaxChannels ; i++)
      {
        if (((activatedChannels >> i )& 0x01) == 1)
        {
            if(SendingStarted)
            {
              Serial.print(",");
            }
            sensorValue[i] = analogRead(i);
            Serial.print(sensorValue[i]);
            SendingStarted=true;
        }

      }    
      
      if (count<= NbrPoints-1)
      {
        Serial.print(";");
      }
      else
      {
          Serial.println("\n\r"); 
          Timer1.detachInterrupt(); //Timer1.stop();   
      }
      SendingStarted=false;

}
void loop() {
}


void serialEvent() {
      inputString= Serial.readStringUntil('\n');
      unsigned int firstCommaIndex   = inputString.indexOf(',');
      unsigned int secondCommaIndex  = inputString.indexOf(',', firstCommaIndex+1);
      unsigned int thirdCommaIndex   = inputString.indexOf(',', secondCommaIndex+1);
      unsigned int fourthCommaIndex  = inputString.indexOf(',', thirdCommaIndex+1);
      
      String NbrPointsCmd     = inputString.substring(0, firstCommaIndex);
      String TimerParam       = inputString.substring(firstCommaIndex+1, secondCommaIndex);
      String activatedChannelsCmd = inputString.substring(secondCommaIndex+1, thirdCommaIndex);
      String freeRun          = inputString.substring(thirdCommaIndex+1, inputString.length());
      
      //inputString="";

      NbrPoints = NbrPointsCmd.toInt();
      unsigned long int Timer1Step=TimerParam.toInt();
      activatedChannels=activatedChannelsCmd.toInt();
      freeRunCmd = freeRun.toInt();
      count=0; 
      
      Timer1.setPeriod(Timer1Step);
      Timer1.attachInterrupt(serialSend);
      Timer1.start();
 
}
