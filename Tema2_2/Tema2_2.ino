int pushButton =2;

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
pinMode(pushButton, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
int buttonState = digitalRead(pushButton);
if(buttonState == 1)
Serial.println("4, Inundatie!! Starea este 1");
else Serial.println("4, Nu e inundatie. Starea este 0");

delay(1000);
}
