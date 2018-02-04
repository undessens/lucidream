

/*
 *        FABRIQUE DU REVE LUCIDE
 *        github.com/undessens/fabrique_reve_lucide
 *        github.com/undessens/lucidream
 * 
 *        Author : Aurelien Conil
 *        Casemate Fablab de Grenoble
 */

#include <Keypad.h>
#include <ButtonDebounce.h>
#define ANALOGIN 3    // Nombre de potentiometre
#define ANALOG_THRESH 10 
#define DIGITALOUT 1  // Nombre de leds

// ------------   POTENTIOMETRE --------------
int analogValue[ANALOGIN];
int analogPin[] = { A0, A1, A2 };
int piezoDelay[]={ 0, 0, 0 };

//-------------  CHEAP BUTTON -------------
ButtonDebounce button(3, 250);
boolean buttonState = false;

//--------------  KEYPAD --------------------
const byte ROWS = 3; // use 4X4 keypad for both instances
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {0, 1, 2, 3},
  {4, 5, 6, 7},
  {8, 9,10, 11}

  
};

// ------------   LEDS  --------------------
int digitaloutValue[DIGITALOUT];
int digitaloutPin[] = {2};


byte rowPins[ROWS] = {12, 11,10}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {9, 8, 7, 6}; //connect to the column pinouts of the keypad
Keypad kpd( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
unsigned long loopCount;
unsigned long startTime;
String msg;



void setup(){
  loopCount = 0;
  startTime = millis();
  msg = "";
  Serial.begin(57600);
  while(!Serial);
  Serial.print("Lucibox");


  for( int i=0 ;i<ANALOGIN ; i++){
    
    pinMode(analogPin[i], INPUT);
    analogValue[i] = analogRead(analogPin[i]);

  }

  for(int i=0 ; i<DIGITALOUT ; i++ ){
    pinMode(digitaloutPin[i], OUTPUT );
    digitaloutValue[i] = LOW;
    digitalWrite(digitaloutPin[i], HIGH);
    digitalWrite(digitaloutPin[i], LOW);
    }
   

}


char key;
void loop(){

   // READ analog pin and write to serial
  for (int i= 0; i<ANALOGIN; i++){
   
    
    int newValueAn = analogRead(analogPin[i]);

    //Send message if value is different
    if ( abs( newValueAn - analogValue[i]) > ANALOG_THRESH )
    {
     
      analogValue[i] = newValueAn;
      sendMessage(i, map(newValueAn, 0, 1024, 0, 255 )); 
      
      }

    }

  // READ THE CHEAP BUTTON
  button.update(); 
  if(button.state() == LOW && buttonState==HIGH){
    sendMessage(20, 255);
  }
  if(button.state() == HIGH && buttonState==LOW){
    sendMessage(20, 0);
  }
  buttonState = button.state();

   // READ THE KEYPAD
  if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {  // Report active key state : IDLE, PRESSED, HOLD, or RELEASED
                    case PRESSED:
                    sendMessage(21 + kpd.key[i].kchar, 255);
                break;
                    case HOLD:
                    msg = " HOLD.";
                break;
                    case RELEASED:
                    sendMessage(21 + kpd.key[i].kchar, 0);
                break;
                    case IDLE:
                    msg = " IDLE.";
            
                }

            }
        }
    }

   // Serial receive message
  while(Serial.available()>2){
    
    int channel = (Serial.read()) ;
    int value = (Serial.read()) ;
    char end_of_line = Serial.read();
    
    if(end_of_line == 13 ){
      
         switch(channel){
             case 0:
                    digitalWrite(digitaloutPin[0], value);
              break;
             case 1:
                   digitalWrite(digitaloutPin[1], value);
                break; 
            case 2: 
                   digitalWrite(digitaloutPin[2], value);
                break;
             
             }

    }
  }


}

void sendMessage(int address, int value){

// Normal way to send a message

 Serial.print(char(address) );
 if(value ==14) value = 13;
 Serial.print(char(value));
 Serial.print(char(14));

/*
//debug way to send a message
Serial.print("ad: ");
Serial.print(address);
Serial.print(" - val: ");
Serial.println(value);
 */
}
