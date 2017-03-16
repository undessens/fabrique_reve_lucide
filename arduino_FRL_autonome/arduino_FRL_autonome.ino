/*
 *        FABRIQUE DU REVE LUCIDE
 *        github.com/undessens/fabrique_reve_lucide
 * 
 *        Author : Aurelien Conil
 *        Casemate Fablab de Grenoble
 */

#define ANALOGIN 5
#define DIGITALIN 8
#define DIGITALOUT 4
#define ANALOG_THRESH 4


int analogValue[ANALOGIN];
int analogPin[] = { A0, A1, A2, A3, A4 };
int digitalinValue[DIGITALIN];
int digitalinPin[] = { 2, 3, 4, 5, 12, 9, 10, 11};   
int digitaloutValue[DIGITALOUT];
int digitaloutPin[] = { 6, 7, 8, A5 };


void setup(){
  
  for( int i=0 ;i<ANALOGIN ; i++){
    
    pinMode(analogPin[i], INPUT_PULLUP);
    analogValue[i] = analogRead(analogPin[i]);

  }

  for(int i=0 ; i<DIGITALIN ; i++ ){
    pinMode(digitalinPin[i], INPUT_PULLUP);
    digitalinValue[i] = digitalRead(digitalinPin[i]);
  }


   for(int i=0 ; i<DIGITALOUT ; i++ ){
    pinMode(digitaloutPin[i], OUTPUT );
    digitaloutValue[i] = LOW;
    digitalWrite(digitaloutPin[i], HIGH);
    digitalWrite(digitaloutPin[i], LOW);
    }
   
   Serial.begin(57600);
   delay(100);
   Serial.print("lucibox");
   
   

}

void loop(){
  
  // READ analog pin and write to serial
  for (int i= 0; i<ANALOGIN; i++){
   
    
    int newValueAn = analogRead(analogPin[i]);

    //Send message if value is different
    if ( abs( newValueAn - analogValue[i]) > ANALOG_THRESH ){
      
      analogValue[i] = constrain(newValueAn, 9, 233);
      sendMessage(i, map(newValueAn, 9, 233, 0, 255 )); 
      
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

             

             
             }



      
       
    }
   
   

    
  }
  
  
  
  delay(5);
  
  
}


void sendMessage(int address, int value){

 Serial.print(char(address) );
 if(value ==14) value = 13;
 Serial.print(char(value));
 Serial.print(char(14));

  
}

