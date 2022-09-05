#include <SPI.h>
#include "LDR.h"
#include "GPS.h"
#include "BMP180.h"
#include "createData.h"
#include "SSP.h"

String data , data_received, ok = "OK", error = "ERROR";

long now ;
char* carac_data;
int csend = 0, creveive = 0, endData = 0;

bool startSend = false, createData = true,readData=false;


void setup (void)
{

  Serial.begin(115200);
  pinMode(led, OUTPUT);
  pinMode(MISO, OUTPUT);  // have to send on master in, slave out

  LDR_setup();
  BMP_setup();
  GPS_setup();

  buildPacket(ok, Write, TT, OBC, 1);
  buildPacket(error, Write, TT, OBC, 0);

  SPCR |= bit(SPE);    // turn on SPI in slave mode
  SPCR |= bit(SPIE);    // turn on interrupts

  now = millis();
}
// end of setup


// SPI interrupt routine
ISR (SPI_STC_vect)
{
  if (startSend)
  {
    if (type == 1) 
    {
      SPDR = okData[csend++];
    }
    else if (type == 0)
    {
      SPDR = errorData[csend++];
    }
    else
    {
      SPDR = sendData[csend++];
    }
    if (csend >= lengthOfData)
    {
      startSend = false;
      createData=true;
    }

  }
  
  else
  {

    if ((SPDR == 0xc0))
    {
      endData++;
      createData=false;
    }
    else if (SPDR == 0x23)
    {
      csend = 0;
      startSend = true;
    }
    if (endData > 0)
    {
      receiveData[creveive++] = SPDR;
    }
    if (endData == 2)
    { lreceive = creveive;
      creveive = 0;
      createData = true;
      endData = 0;
    }
  }



} // end of interrupt service routine (ISR) SPI_STC_vect

void loop (void)
{
  if (readData)
  {

    for (int i = 0; i < lreceive; i++)
    {

      Serial.print(receiveData[i]);
      Serial.print(" ,");
    }
    Serial.println("");
    data_received = dismantPacket();
    carac_data = data_received.c_str();
    data = get_Data(carac_data[0]);
    Serial.println(data_received);
    readData = false;
  }
  if (millis() - now > 300&&createData ) {
    data = "{\"LDR1\":" + String(get_ldr1()) + "," + "\"LDR2\":" + String(get_ldr2()) + "," + "\"LDR3\":" + String(get_ldr3()) + "," + "\"LDR4\":" + String(get_ldr4()) + "," + "\"T\":" + String(bmp.readTemperature()) + "," + "\"A\":" + String(bmp.readAltitude()) + "," + "\"P\":" + String(bmp.readPressure()) + "," + get_Gps() + "}";
    lengthOfData = data.length() + 7;
    buildPacket(data, Write, TT, OBC, 2);
    now = millis();
  }
}  //
