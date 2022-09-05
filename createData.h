#define led 7
int type = 0;  //0 -> error , 1 --> ok , 2 -> data
int lengthOfData;
void(* resetFunc) (void) = 0; //declare reset function @ address 0

String get_Data(char command)
{
  String data;

  Serial.println(command);
  switch (command)
  {
    case 'A':
      type = 2;
      break;
    case 'Z':     //  ping
      type = 1;
      lengthOfData = 9;
      break;
    case 'X':
      type = 1;
      lengthOfData = 9;
      digitalWrite(led, HIGH);
      break;
    case 'Y':
      type = 1;
      lengthOfData = 9;
      digitalWrite(led, LOW);
      break;
    case 'R':
      type = 1;
      lengthOfData = 9;
      resetFunc();  //call reset
      break;
    default:
      type = 0;
      lengthOfData = 12;
  }
  return data;
}
