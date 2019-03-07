#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;
#include<SD.h>
#include <SPI.h>
unsigned long timer=0;
const int CS=4;
int i=0;
int id=0;

void setup()
{
  Serial.begin(9600);  
  //RTC initializing
   Wire.begin();
    RTC.begin();
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
     
  }
  //RTC.adjust(DateTime(__DATE__, __TIME__));
   
  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for native USB port only
  }
 
  Serial.print("Initializing SD card...");
  delay(2000);

  // see if the card is present and can be initialized:
  if (!SD.begin(CS)) 
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  //card initialized
  Serial.println("card initialized.");
   
   File root = SD.open("/");
                            // to extract textfile number
  i= printDirectory(root);  //i=textfile number.printDirectory returns the number of the last file in which data was written
   Serial.println(i);

  int k=0;
  String h="vol"+String(i)+"."+"txt";//txtfile name
  File data = SD.open(h);
  if (data) {
    data.seek(data.size()-25);//position pointr 25(guess estimate size of 6 voltage values including comma,\n,\r etc) bytes from the end of file
    while(1)
    {
      char c=data.read();
      if((c=='\n')||(c=='\r'))
      {  k-=1;
        data.seek(data.size()-(25+k));//if \n,\r is encountered then,k value decreased by one so that pointer points at the serial number
         break;
      }
      else
      {
          k+=1;
        data.seek(data.size()-(25+k));// if not then pointer shifts further from end by one
      }
         
    }
    while(data.available())
    {
       char c=data.read();
    if(c=='.') //(eg. 100. 8.33,4.55.....)->the end of the serial number is accompanied by '.' . hence,till it is not not encountered the loop continues to extcract the last serial number
    {
       break;   
    }
    else
    {
      id=id*10;
      id=id+(c-'0'); // extraction of last serial number and storing it in id
    }
    
  }
 
  data.close();
  //printing serial number on serial monitor
  Serial.print("last no.:");
  Serial.println(id); 
    }
   

   
   
if(id!=0)
{
 data=SD.open(h,FILE_WRITE);
 if(data)
 {
  data.println("***************");   // demarcation
  Serial.println("***************");
  data.close();
}
}
  
}

int printDirectory(File dir) {
   int b=0;
  while (true) {
    
    File entry =  dir.openNextFile();//opens next file
    
    
    if (! entry) {
      
      break;//if no file is left,breaks fro while loop.
    }
    
    
    //chech if the entry is a directory or file
    if (!entry.isDirectory()) { 
         b=0;
      int a=strlen(entry.name());
     char c=entry.name()[0];//if file,char c stores first character of file name
       
         if(c=='V')
         {  
          for(int i=3;i<a;i++)
          {
            c=entry.name()[i];
            if(c=='.')
            {
              break;
            }
            else
            {
              b=b*10;
              b=b+(c-'0');
            }
          }
         }
      
    } 
    
    entry.close();
  
  }  
    return b;
    
}



void loop()
{
  if((millis()-timer)>=1000)
  {
  id++;// increment of serial number for another set of data 
 
    
     String h="vol"+String(i)+"."+"txt";//name of txt file
   
    File  data= SD.open(h,FILE_WRITE); 
  if(data)
     {  
      
        if (data.size()>= 2000) //size limit
        {
          data.close();
          i=i+1;
          String h="vol"+String(i)+"."+"txt"; //name of txt file
        File data = SD.open(h,FILE_WRITE);
        }
        
         data.print(id);
        data.print(". ");
       Serial.print(h);
       Serial.print("|");
       Serial.print(id);
       Serial.print(". ");
      
    for(int ap=0;ap<6;ap++)
    {
    int s=analogRead(ap);
     float sensor=(5.0/1023.0)*s; //converting voltages of analog pins which is in binary form to human readable form and storing them in string
      
       //printing size of file on serial monitor
   
         data.print(sensor);
        Serial.print(sensor);
          if(ap<5)
          {
          data.print(",");
          Serial.print(",");
          } 
          else
          {
            data.print("\t");
            Serial.print("\t");
          }
    
 }  
    DateTime now = RTC.now();
     
    data.print(now.year(), DEC);
    Serial.print(now.year(), DEC);
    data.print('/');
    Serial.print('/');
    data.print(now.month(), DEC);
    Serial.print(now.month(), DEC);
    data.print('/');
    Serial.print('/');
    data.print(now.day(), DEC);
    Serial.print(now.day(), DEC);
     data.print(' ');
      Serial.print(' ');
    data.print(now.hour(), DEC);
    Serial.print(now.hour(), DEC);
    data.print(':');
    Serial.print(':');
    data.print(now.minute(), DEC);
    Serial.print(now.minute(), DEC);
    data.print(':');
    Serial.print(':');
    data.print(now.second(), DEC);
     Serial.print(now.second(), DEC);
    data.println(); 
     Serial.println(); 
     
      Serial.println(data.size());
      
   data.close();
  }
 timer=millis();
  }
  
}

 
