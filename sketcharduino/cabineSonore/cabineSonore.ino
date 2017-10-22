/*
 *        FABRIQUE DU REVE LUCIDE
 *        github.com/undessens/fabrique_reve_lucide
 * 
 *        Author : Aurelien Conil
 *        Casemate Fablab de Grenoble
 */

#include<Wire.h>
#define ANALOGIN 4
#define DIGITALIN 5
#define DIGITALOUT 0
#define ANALOG_THRESH 10  

#define I2CADRESS_LED 56

int analogValue[ANALOGIN];
int analogPin[] = { A0, A1, A2, A3 };
int digitalinValue[DIGITALIN];
int digitalinPin[] = { 2,3, 4, 5, 6};
int digitaloutValue[DIGITALOUT];
int digitaloutPin[] = { };
byte ledI2C = 0;


void setup(){

    Serial.begin(57600);
   delay(1);
   Serial.print("Lucibox");
   
  
  for( int i=0 ;i<ANALOGIN ; i++){
    
    pinMode(analogPin[i], INPUT_PULLUP);
    analogValue[i] = analogRead(analogPin[i]);

  }

  for(int i=0 ; i<DIGITALIN ; i++ ){
    pinMode(digitalinPin[i], INPUT_PULLUP);
    digitalinValue[i] = digitalRead(digitalinPin[i]);
  }

    
    
  
    // Wire for I2C  : led and pad
    Wire.begin();


   for(int i=0 ; i<DIGITALOUT ; i++ ){
    pinMode(digitaloutPin[i], OUTPUT );
    digitaloutValue[i] = LOW;
    digitalWrite(digitaloutPin[i], HIGH);
    digitalWrite(digitaloutPin[i], LOW);
    }
   
   
   

}

void loop(){
  
  // READ analog pin and write to serial
  for (int i= 0; i<ANALOGIN; i++){
   
    
    int newValueAn = analogRead(analogPin[i]);

    //Send message if value is different
    if ( abs( newValueAn - analogValue[i]) > ANALOG_THRESH ){
     
      analogValue[i] = newValueAn;
      sendMessage(i, map(newValueAn, 0, 1024, 0, 255 )); 
      
    }

    }

    //READ digital pin and write to serial
    for (int i= 0; i<DIGITALIN; i++){


      int newValueDi;
      newValueDi = digitalRead(digitalinPin[i]);
    if( newValueDi != digitalinValue[i] ){

      digitalinValue[i] = newValueDi;
      int finalValue = 0;
      if (newValueDi) finalValue = 255;
      sendMessage(i + 20, newValueDi*255); 
      
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
            case 3: 
                   digitalWrite(digitaloutPin[3], value);
                break; 
            case 4: 
                 digitalWrite(digitaloutPin[4], value);
                break; 
     
       
             }

     if( channel > 19 && channel < 29 ){
                // Here are the led with I2C plugins
               sendLedI2C( channel - 20, value)  ;
              
             }
   
   

    
  }

  }
  
  
  
  delay(5);
  
  
}


void sendMessage(int address, int value){

 Serial.print(char(address) );
 if(value ==14) value = 13;
 Serial.print(char(value));
 //Serial.print(value);
 //Serial.print(byte(13));
 Serial.print(char(14));
 //Serial.println("--");
  
}

void sendLedI2C( int num, int value){

if (value > 0) 
{
  ledI2C = ledI2C | (1 << (num)); 
}
else {
  ledI2C = ledI2C ^ (1 << (num)); 
}
    Wire.beginTransmission(56);     //Begin the transmission to PCF8574
    Wire.write(ledI2C);                                //Send the data to PCF8574
    Wire.endTransmission(); 

}



