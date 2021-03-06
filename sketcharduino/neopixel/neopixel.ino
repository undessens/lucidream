/*
 *        FABRIQUE DU REVE LUCIDE
 *        github.com/undessens/fabrique_reve_lucide
 *        github.com/undessens/lucidream
 * 
 *        Author : Aurelien Conil
 *        Casemate Fablab de Grenoble
 */

 #include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN            6
#define NUMPIXELS      4


#define ANALOGIN 6    // Nombre de potentiometre
#define DIGITALIN 8   // Nombre de boutons
#define DIGITALOUT 12  // Nombre de leds
#define ANALOG_THRESH 10 

//--------------- NEOPIXEL ----------------
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int digitaloutValue[DIGITALOUT];

// ------------   POTENTIOMETRE --------------
int analogValue[ANALOGIN];
int analogPin[] = { A0, A1, A2,A3,A4,A5 };

// ------------   BOUTONS  ------------------
int digitalinValue[DIGITALIN];
int digitalinPin[] =  { 2,3,4,5,6,7,8 ,9};



void setup(){

    Serial.begin(57600);
    while(!Serial);
    Serial.print("Lucibox");
   
  
  for( int i=0 ;i<ANALOGIN ; i++){
    
    pinMode(analogPin[i], INPUT);
    analogValue[i] = analogRead(analogPin[i]);

  }

  for(int i=0 ; i<DIGITALIN ; i++ ){
    pinMode(digitalinPin[i], INPUT_PULLUP);
    digitalinValue[i] = digitalRead(digitalinPin[i]);
  }

  pixels.begin();

   
      

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
    int newValueDi = !digitalRead(digitalinPin[i]);  
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
                   setNeoPixel(0, value, 0,0);
              break;
             case 1:
                   setNeoPixel(0, 0,value,0);
                break; 
            case 2: 
                   setNeoPixel(0, 0,0 ,value);
                break;
           case 3:
                   setNeoPixel(1, value, 0,0);
              break;
             case 4:
                   setNeoPixel(1, 0,value,0);
                break; 
            case 5: 
                   setNeoPixel(1, 0,0 ,value);
                break;
             case 6:
                   setNeoPixel(2, value, 0,0);
              break;
             case 7:
                   setNeoPixel(2, 0,value,0);
                break; 
            case 8: 
                   setNeoPixel(2, 0,0 ,value);
                break;
            case 9:
                   setNeoPixel(3, value, 0,0);
              break;
             case 10:
                   setNeoPixel(3, 0,value,0);
                break; 
            case 11: 
                   setNeoPixel(3, 0,0 ,value);
                break;

            
          
             

             
             }


      
       
    }
   
   

    
  }
  
  
  
  delay(5);
  
  
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

void setNeoPixel(int channel, int r, int v, int b){

  int finalr, finalv, finalb;
  if( r == 0 ){
    finalr= digitaloutValue[channel];
  }else{
    digitaloutValue[channel] = r;
    finalr = r;
  }
  
  if( v == 0 ){
    finalv= digitaloutValue[channel+1];
  }else{
    digitaloutValue[channel+1] = v;
    finalv = v;
  }
  
  if( b == 0 ){
    finalb= digitaloutValue[channel+2];
  }else{
    digitaloutValue[channel+2] = b;
    finalb = b;
  }
  pixels.setPixelColor(channel, pixels.Color(finalr,finalv,finalb));
  pixels.show();


}



