/*
 *        FABRIQUE DU REVE LUCIDE
 *        github.com/undessens/fabrique_reve_lucide
 * 
 *        Author : Aurelien Conil
 *        Casemate Fablab de Grenoble
 */

//#include <Servo.h>
#include<Wire.h>
#define ANALOGIN 5
#define DIGITALIN 5
#define DIGITALOUT 2

#define I2CADRESS_LED 56

int analogValue[ANALOGIN];
int analogValueMin[ANALOGIN];
int analogValueMax[ANALOGIN];
int analogPin[] = { A0, A1, A2, A3, A4 };
int digitalinValue[DIGITALIN];
int digitalinPin[] = { 2, 3, 4, 5 , 6};
int digitaloutValue[DIGITALOUT];
int digitaloutPin[] = {7, 8 };
byte ledI2C = 0;

// --- Servo not available
//Servo servo1;
//int servo1pin = 13;

void setup(){
  
  for( int i=0 ;i<ANALOGIN ; i++){
    
    pinMode(analogPin[i], INPUT_PULLUP);
    analogValue[i] = analogRead(analogPin[i]);
    analogValueMin[i] = 120; 
    analogValueMax[i] = 880;

  }

  for(int i=0 ; i<DIGITALIN ; i++ ){
    pinMode(digitalinPin[i], INPUT_PULLUP);
    digitalinValue[i] = digitalRead(digitalinPin[i]);
  }

    for(int i=0 ; i<DIGITALOUT ; i++ ){
    pinMode(digitaloutPin[i], OUTPUT );
    digitaloutValue[i] = LOW;
    digitalWrite(digitaloutPin[i], LOW);
    }
    
    // Servo not available
    //servo1.attach(servo1pin);
    //servo1.write(0);
  
    // Wire for I2C plugins
    Wire.begin();

   
   
   Serial.begin(57600);
   delay(100);
   Serial.println("Fabrique_FRL");
   
   

}

void loop(){
  
  // READ analog pin and write to serial
  for (int i= 0; i<ANALOGIN; i++){
   
    
    int newValueAn = analogRead(analogPin[i]);

    //Update min and max
    if ( newValueAn > analogValueMax[i] ) analogValueMax[i] = newValueAn;
    if ( newValueAn < analogValueMin[i] ) analogValueMin[i] = newValueAn;

    //Send message if value is different
    if ( abs( newValueAn - analogValue[i]) > 5 ){
     
      analogValue[i] = newValueAn;
      sendMessage(i, map(newValueAn, analogValueMin[i], analogValueMax[i], 0, 255 )); 
      
    }

    }

    //READ digital pin and write to serial
    for (int i= 0; i<DIGITALIN; i++){
    int newValueDi = digitalRead(digitalinPin[i]);  
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
                    analogWrite(digitaloutPin[0], value);
              break;
             case 1:
                   analogWrite(digitaloutPin[1], value);
                break; 
            case 2: 
                   analogWrite(digitaloutPin[2], value);
                break;
            case 3: 
                   analogWrite(digitaloutPin[3], value);
                break; 
            case 4: 
                 analogWrite(digitaloutPin[4], value);
                break; 
            case 5: 
                 
//                 int duration 
//                 cli();
//                  long start = micros();
//                  digitalWrite(PIN, HIGH);
//                  while (micros() - start < duration)
//                  ;
//                  digitalWrite(PIN, LOW);
//                  sei();
                  break;
             

             
             }

             if( channel > 19 && channel < 29 ){
                // Here are the led with I2C plugins
               sendLedI2C( channel - 20, value)  ;
              
             }

      
       
    }
   
   

    
  }
  
  
  
  delay(20);
  
  
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

