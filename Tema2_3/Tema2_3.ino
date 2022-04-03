#include <EEPROM.h>

void writeS(int adress, String str)
{
  byte len = str.length();
  EEPROM.write(adress,len);
  for(int i=0; i<len; i++){
    EEPROM.write(adress+1+i, str[i]);
  }
}
String readS(int adress)
{
  int len = EEPROM.read(adress);
  char data[len+1];

  for(int i=0; i<len; i++){
    data[i] = EEPROM.read(adress+1+i);
  }
  data[len] = '\0';

  return String(data);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  writeS(15,"Hello !");
  Serial.println(readS(15));
}

void loop() {
  // put your main code here, to run repeatedly:

}
