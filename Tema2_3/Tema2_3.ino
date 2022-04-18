#include <EEPROM.h>
//char mesaje 10 mesaje a cate 32 [10][32].
char msg[10][32]{
  "msg1",
  "msg2",
  "msg3",
  "msg4",
  "msg5",
  "msg6",
  "msg7",
  "msg8",
  "msg9",
  "msg10"
};

char reloadedTags[10][32];

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(9600);
  while(!Serial);
  EEPROM.put(0,msg);
  EEPROM.get(0,reloadedTags);
  for( int tag=0; tag<10; tag++)
  {
    for (int c=0; c<32;c++)
    {
      Serial.print(reloadedTags[tag][c]);
    }
    Serial.println();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
     //delay(1000);

}
