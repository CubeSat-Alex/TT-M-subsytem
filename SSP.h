uint8_t receiveData[200];
uint8_t sendData[200], okData[10], errorData[10];

const byte  OBC = 0x01, ADCS = 0x10, TT = 0x11; //SUB SYSTEM ADDRESS
const byte Read = 0x06, Write = 0x07, ACK = 0X02, Ping = 0x00; // TYPE COMMAND

int lreceive = 0;

uint16_t CalCrc(uint16_t crc_initial, uint8_t* buf, uint16_t len, uint16_t POLY)
{
  unsigned int byte ;
  unsigned char k;
  unsigned short ACC, TOPBIT;
  unsigned short remainder = crc_initial;
  TOPBIT = 0x8000;
  for (byte = 0; byte < len; ++byte)
  {
    ACC = (unsigned)buf[byte] << 8;
    remainder ^= ACC;
    // printf("new remainder = %u \n",remainder);
    for (k = 8; k > 0; --k)
    {
      if (remainder & TOPBIT)
      {
        remainder = remainder << 1;
        remainder = remainder ^ POLY;
      }
      else
      {
        remainder = (remainder << 1);
      }
    }
  }
  remainder = remainder ^ 0x0000;
  // Serial.println(remainder,HEX);

  return remainder;
}



String dismantPacket()
{
  String data_received;
  uint8_t crc_buf[lreceive - 4];

  if (receiveData[0] == 0xc0 && receiveData[lreceive - 1] == 0xc0) // check FEND
  {
    if (receiveData[1] == TT && receiveData[2] == OBC) //CHECK DESTINATION AND SOURCE
      for (int i = 0; i < lreceive - 4; i++)
        crc_buf[i] = receiveData[i + 1];
    uint16_t crc_r = CalCrc(0x0000, crc_buf, sizeof(crc_buf) / sizeof(crc_buf[0]), 0x1021);
    byte a, b, c, d;
    a = crc_r % 16;
    crc_r = int(crc_r / 16);
    b = crc_r % 16;
    crc_r = int(crc_r / 16);
    c = crc_r % 16;
    crc_r = int(crc_r / 16);
    d = crc_r % 16;
    if ( receiveData[lreceive - 3] == (a | (b << 4)) && receiveData[lreceive - 2] == (c | (d << 4))) // check crc then get data
    {
      for (int i = 4; i < lreceive - 3; i++)
      {
        data_received.concat(char(receiveData[i]));
      }

    }

  }


  return data_received;
}



void buildPacket(String msg, uint8_t type, uint8_t dest, uint8_t src, int packetType)//
{
  int x = msg.length(); // length of data or msg
  uint8_t packet[x + 7], data[x], crc_buf[x + 3];

  //packet[1]=OBC;// add FEND and destination .
  strcpy(data, msg.c_str());
  packet[1] = dest;
  packet[2] = src;
  packet[3] = type;

  for (int i = 0; i < x; i++)
  {
    packet[i + 4] = data[i];
  }

  for (int i = 0; i < x + 3; i++)
    crc_buf[i] = packet[i + 1];
  packet[x + 6] = 0xc0;
  packet[0] = 0xc0;


  uint16_t crc_r = CalCrc(0x0000, crc_buf, sizeof(crc_buf) / sizeof(crc_buf[0]), 0x1021);
  byte a, b, c, d;
  a = crc_r % 16;
  crc_r = int(crc_r / 16);
  b = crc_r % 16;
  crc_r = int(crc_r / 16);
  c = crc_r % 16;
  crc_r = int(crc_r / 16);
  d = crc_r % 16;

  packet[x + 4] = (a | (b << 4));
  packet[x + 5] = (c | (d << 4));
  for (int i = 0; i < x + 7; i++)
  {
    if (packetType == 0)
    {
      errorData[i] = packet[i];
    }
    else if (packetType == 1)
    {
      okData[i] = packet[i];
    }
    else
    {
      sendData[i] = packet[i];
    }

  }
}
