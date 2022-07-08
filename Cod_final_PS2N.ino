#define BACK 2
#define UP 3
#define DOWN 4
#define OK 5

#define R_LED A4
#define B_LED A2
#define G_LED A3
#define LED A5

#define LM A0
#define FLOOD_PIN 13

#include <LiquidCrystal.h>
#include<EEPROM.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String val="", copy="";
char led_type;
int index=0;

float temp;
float lmvalue;

int option=0;
int msg_option=0;
int ctrl_option=0;
int option_main = 0;
int kmin = 0;
int kplus = 0;
int r=3;

enum BUTTONS{
  IS_OK,
  IS_BACK,
  IS_UP,
  IS_DOWN,
  IS_NONE,
  IS_MAX_NUM
};

enum MAIN{
  P_MESAJE,
  P_CONTROL,
  P_TEMPERATURA,
  P_INUNDATIE
};

enum MESAJE{
  S_NECITITE=0,
  S_CITITE,
  S_STERGERE
};

enum CONTROL{
  S_MANUAL=0,
  S_AUTOMAT
};

//////////////
char _CHOOSE_LED_ACTION(String val)
{
  char action;
  action = val[2];
    
  return action; 
}

void _CHANGE_RGB(long int red, long int green, long int blue)
{
  analogWrite(R_LED, red);
  analogWrite(G_LED, green);
  analogWrite(B_LED, blue);
}

void _LED_RGB_CONTROL()
{
   if(Serial.available())
   { 
     val = Serial.readString();
     led_type = val[0];
     
     if (led_type == '1')
     {
       if(val[2] == 'A')
         analogWrite(LED, 1023);
       else if(val[2] == 'S')
         analogWrite(LED, 0);
     }
     
     if (led_type == '2')
     {
        String red = val.substring(2,4);
        String green = val.substring(4,6);
        String blue = val.substring(6,8);
       
        long int i_red = strtol(red.c_str(), NULL, 16);
        long int i_green = strtol(green.c_str(), NULL, 16);
        long int i_blue = strtol(blue.c_str(), NULL, 16);
       
        _CHANGE_RGB(i_red, i_green, i_blue);
     } 
       
   } 
}

///////////////////
void _GetTemp()
{
  lmvalue = analogRead(LM);
  temp = 100*(5.0/1023)*lmvalue;

 
 lcd.print("3, ");
 lcd.print(temp);
 lcd.print("*C");
 delay(1000);
 //lcd.clear();
}

//////////////////
void _FLOOD_DETECTION()
{
  int flood_detection_value = digitalRead(FLOOD_PIN);
  
  if(flood_detection_value == HIGH)
    lcd.print("4,OK");
  else 
    lcd.print("4,Inundatie!!!");

    delay(1000);
  
}

//EEPROM
int i;
String temp_message;
typedef struct message {
  char msg[32];
  long date;
  bool ok;
}mesaj_t;
mesaj_t mesaj[10];
   
int eeAddress =0;   //Location we want the data to be put.
void _SAVE_EEPROM(String message)
{       
        copy=message;
        message.toCharArray(mesaj[i].msg,32);
        mesaj[i].ok= false;
      EEPROM.put(i*sizeof(mesaj[i]), mesaj[i]);
      eeAddress += sizeof(mesaj[i]);
      i++;
      if(i == 10)
      {
        i = 0;
        eeAddress=0;
      }
      EEPROM.put(900, i);
   
}

int _GetButtons(void){
  int aux=IS_NONE;
  if(digitalRead(OK) == LOW)
  {
    aux = IS_OK;
  }
  else if(digitalRead(BACK)== LOW)
  {
   aux = IS_BACK;
  }
  else if(digitalRead(UP)== LOW)
  {
    aux = IS_UP;
  }
  else if(digitalRead(DOWN)== LOW)
  {
    aux = IS_DOWN;
  }
  return aux;
}

/////////////
int _MENU_SCROLL(int &option)
{
  if(option > 3)
  option=0;
  else if(option<0)
  option=3;

  if(_GetButtons() == IS_UP)
  option--;
  else if(_GetButtons()== IS_DOWN)
  option++;
  
  return option;
}

////////////
int _MESAJE_MENU_SCROLL(int &option)
{
  if(option > 2)
  option = 0;
  else if(option < 0)
  option = 2;

  if(_GetButtons() == IS_UP)
  option--;
  else if(_GetButtons()== IS_DOWN)
  option++;
  else if(_GetButtons() == IS_BACK)
  option = _SELECT_MAIN_MENU();

  return option;
}

////////////
int _CONTROL_MENU_SCROLL(int &option)
{
  if(option > 1)
  option = 0;
  else if(option < 0)
  option = 1;

  if(_GetButtons() == IS_UP)
  option--;
  else if(_GetButtons()== IS_DOWN)
  option++;
  else if(_GetButtons() == IS_BACK)
  option = _SELECT_MAIN_MENU();

  return option;
}
////////////
int _SELECT_MAIN_MENU()
{
  int current_menu;
  int next_menu = _MENU_SCROLL(option);

  if(next_menu == 0)
    current_menu = P_MESAJE;
      else if(next_menu == 1)
        current_menu = P_CONTROL;
          else if(next_menu == 2)
          current_menu = P_TEMPERATURA;
            else if(next_menu == 3)
              current_menu = P_INUNDATIE;
   return current_menu;
}

////////////
int _SELECT_MESAJE_MENU()
{
  int current_menu;
  int next_menu = _MESAJE_MENU_SCROLL(msg_option);
  
  if(next_menu == 0)
    current_menu = S_NECITITE;
  else if(next_menu == 1)
    current_menu = S_CITITE;
  else if(next_menu == 2)
    current_menu = S_STERGERE;
    
  return current_menu;
}

////////////
int _SELECT_CONTROL_MENU()
{
  int current_menu;
  int next_menu = _CONTROL_MENU_SCROLL(ctrl_option);
  
  if(next_menu == 0)
    current_menu = S_MANUAL;
  else if(next_menu == 1)
    current_menu = S_AUTOMAT;
    
  return current_menu;
}

////////////
void _FUNCTIONAL_MENU()
{
  int opt= _MENU_SCROLL(option);
  switch(opt){
    case 0:
    lcd.print("Mesaje");
    if(_GetButtons() == IS_OK)
    {
      lcd.clear();
      while(_GetButtons() != IS_BACK)
      {
       delay(1000);
        lcd.clear();
        
        lcd.print("Submeniu MESAJE");
        lcd.setCursor(0,1);
        switch(_SELECT_MESAJE_MENU())
        {
          case 0: 
            lcd.print("Necitite");
            break;
          case 1:
            lcd.print("Citite");
            break;
          case 2:
            lcd.print("Stergere");
            
            break;
        }
       }
    }
    break;
    case 1:
    lcd.print("Control");
     if(_GetButtons() == IS_OK)
    {
      lcd.clear();
      
      while(_GetButtons() != IS_BACK)
      {
        delay(1000);
        lcd.clear();
        lcd.print("Submeniu CONTROL");
        lcd.setCursor(0,1);
        switch(_SELECT_CONTROL_MENU())
        {
          case 0: 
            lcd.print("Manual");
            break;
          case 1:
            lcd.print("Automat");
            break;
        }
       }
    }
    break;
    case 2:
   
    lcd.print("Temperatura");
    if(_GetButtons() == IS_OK)
      {
        lcd.clear();
        while(_GetButtons() != IS_BACK)
        {
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,1);
          _GetTemp();
         }
      }
    break;
    case 3:
    lcd.print("Inundatie");
    if(_GetButtons() == IS_OK)
      {
        lcd.clear();
        while(_GetButtons() != IS_BACK)
        {
          delay(1000);
          lcd.clear();
          lcd.setCursor(0,1);
          _FLOOD_DETECTION();
         }
      }
    break;
  }
}

//////////////
int _PasswordCheckPlus(int &kplus)
{
  if(digitalRead(UP) == LOW)
  kplus++;

 return kplus;
}

int _PasswordCheckMin(int &kmin){
   if(digitalRead(DOWN) == LOW)
  kmin++;

  return kmin;
}

  //////////////
  void setup(){
    lcd.begin(16,2);
    Serial.begin(9600);
    pinMode(OK, INPUT_PULLUP);
    pinMode(BACK, INPUT_PULLUP);
    pinMode(UP, INPUT_PULLUP);
    pinMode(DOWN, INPUT_PULLUP);

    pinMode(R_LED, OUTPUT);
    pinMode(B_LED, OUTPUT);
    pinMode(G_LED, OUTPUT);
    pinMode(LED, OUTPUT);

    EEPROM.get(900, i);
     for(int it=0;it<10;it++)
 { 
   Serial.print(it);
   Serial.print(' ');
   EEPROM.get(eeAddress,mesaj[it]);
   Serial.println(mesaj[it].msg);
   eeAddress+=sizeof(mesaj[it]);
 }
  }

  /////////////
  void loop(){

    _LED_RGB_CONTROL();
    _FUNCTIONAL_MENU();
    if(val!=copy)
    _SAVE_EEPROM(val);
    
    delay(1000); 
    lcd.clear();
  }
