#include <SoftwareSerial.h>
#include <TinyGPS++.h>
#define  RXPin  3
#define  TXPin  4
// Create a software serial port called "gpsSerial"
SoftwareSerial gpsSerial(RXPin, TXPin);
TinyGPSPlus gps;
void GPS_setup()
{ 
    gpsSerial.begin(9600);
}

String get_Gps()
{
  String data;
  if (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
       if (gps.location.isValid())
     {

       data="\"T\":"+ String((gps.location.lat(), 6))+","+"\"P\":"+String((gps.location.lng(), 6));
     }
    else
     {
       data="\"X\":-1,""\"Y\":-1";
     }
    }
  }

  if (millis() > 2000 && gps.charsProcessed() < 10)
  {
    data="\"X\":-2,""\"Y\":-2";
  }
    return data;
}
