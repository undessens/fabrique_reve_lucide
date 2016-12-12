#include <Servo.h>

int analogValue[6];
int analogPin[] = { A0, A1, A2, A3, A4, A5 };
int digitalinValue[6];
int digitalinPin[] = { 2, 3, 4, 5, 6, 7 };
int digitaloutValue[6];
int digitaloutPin[] = { 8, 9, 10,11, 12, 13 };
Servo servo1;
int servo1pin = 13;

void setup(){
  
  for( int i = 0;i<6 ; i++){
    
    pinMode(analogPin[i], INPUT_PULLUP);
    analogValue[i] = analogRead(analogPin[i]); 
    
    pinMode(digitalinPin[i], INPUT_PULLUP);
    digitalinValue[i] = digitalRead(digitalinPin[i]);
    
    pinMode(digitaloutPin[i], OUTPUT );
    digitaloutValue[i] = LOW;
    digitalWrite(digitaloutPin[i], LOW);
    
    servo1.attach(servo1pin);
    servo1.write(0);
  

}

   
   
   Serial.begin(57600);
   delay(100);
   Serial.println("Truc");
   
   

}

void loop(){
  
  // READ analog pin and write to serial
  for (int i= 0; i<6; i++){
   
    
    int newValueAn = analogRead(analogPin[i]);
    if ( abs( newValueAn - analogValue[i]) > 5 ){
     
      analogValue[i] = newValueAn;
      sendMessage(i, map(newValueAn, 0, 1024, 0, 255 )); 
      
    }
    
    int newValueDi = digitalRead(digitalinPin[i]);  
    if( newValueDi != digitalinValue[i] ){

      digitalinValue[i] = newValueDi;
      int finalValue = 0;
      if (newValueDi) finalValue = 255;
      sendMessage(i + 20, !newValueDi*255); 
      
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
                 //analogWrite(digitaloutPin[5], value);
                 //servo1.write(map(value, 0, 255, 0, 180));
                 int duration 
                 cli();
                  long start = micros();
                  digitalWrite(PIN, HIGH);
                  while (micros() - start < duration)
                  ;
                  digitalWrite(PIN, LOW);
                  sei();
                  break;

             
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

