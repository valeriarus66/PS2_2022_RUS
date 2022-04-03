int led=8;
String str="";

//declaring RGB pins in PWM mode
int red=9;
int green=13;
int blue=11;


/***brightness control variables***/
String readSerial = "";
int redBrightness = 0;
int greenBrightness = 0;
int blueBrightness = 0;

void setup() {
  // put your setup code here, to run once:
pinMode(led, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  pinMode(blue, OUTPUT);
  Serial.begin(9600);
  Serial.println(" '1 A' LED-ul aprins,'1 S' LED-ul stins.");
}

void loop() {
  // put your main code here, to run repeatedly:
if(Serial.available()>0)
 {
   readSerial = Serial.readString();
   //ex: "2 FF00FF" - LED Mov,
   String readSerial1 = readSerial.substring(2,4);
   String readSerial3 = readSerial.substring(4,6);
   String readSerial2 = readSerial.substring(6,8);
   
   
   int RedVal = strtol(readSerial1.c_str(), NULL,16);
   int BlueVal = strtol(readSerial2.c_str(), NULL,16);
   int GreenVal = strtol(readSerial3.c_str(), NULL,16);
   
   Serial.println(RedVal);
     Serial.println(GreenVal);
   Serial.println(BlueVal);
 Serial.println();
 
   
   analogWrite(red,RedVal);
   analogWrite(blue,BlueVal);
   analogWrite(green,GreenVal);
   
  
   
}
}
