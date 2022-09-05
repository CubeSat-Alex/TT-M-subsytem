#include <Wire.h>
#include <Adafruit_BMP085.h>
#define seaLevelPressure_hPa 1013.25

Adafruit_BMP085 bmp;

void BMP_setup(){
   if (!bmp.begin()) {
  }
  }
  // to read pressure write this in main < bmp.readPressure() >
  // to read sea level pressure write this in main < bmp.readSealevelPressure() >
  
