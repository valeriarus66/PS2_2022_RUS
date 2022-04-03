#define LM35 A0

float lmvalue;
float tempr;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

lmvalue = analogRead(A5);

tempr = 100*(5.0/1023)*lmvalue;
if(tempr<26.00 && tempr>23.00)
{
  Serial.print("3, ");
  Serial.print(tempr);
  Serial.print("*C");
  Serial.println();
  delay(1000);
}
Serial.println(tempr);
delay(1000);
}
