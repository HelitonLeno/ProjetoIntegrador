/*
*  ============================================================
*  Projeto integrador RobÃ´ autonomo e controlado por bluetooth
*  ============================================================
*  @Autor -> Heliton Leno de Souza 04/06/2017
*  ============================================================
*/

#include <Servo.h> 
#include <SoftwareSerial.h>

SoftwareSerial bluetooth(10, 11); 

#define MOTOR1   A0     
#define MOTOR2   A1     
#define MOTOR3   A2     
#define MOTOR4   A3     
#define speed    A4     
#define serv      3     
#define M1E       5     
#define M2E       6     
#define echo      8
#define trig      9     

void  standAlone();
void  trigPulse();     
void  decision();   
float testDistance();     
void  speedAdjust();   
void  robotForward();    
void  robotBackward();     
void  robotRight();    
void  robotLeft();     
void  robotStop();     

float distanceCm;
float distanceRight;     
float distanceLeft;      
int   pot = 0x00;     
char  caracter;
Servo servo; 

void setup(){
 bluetooth.begin(9600); 

 pinMode(MOTOR1, OUTPUT);
 pinMode(MOTOR2, OUTPUT);
 pinMode(MOTOR3, OUTPUT);
 pinMode(MOTOR4, OUTPUT);
 pinMode(M1E,    OUTPUT);
 pinMode(M2E,    OUTPUT);
 pinMode(trig,   OUTPUT);       
 pinMode(serv,   OUTPUT);       
 pinMode(echo,    INPUT);
   
 digitalWrite(trig, LOW);     
 servo.attach(serv);
 servo.write(80);    
  
 delay(500);  
} 

void loop(){
  if (bluetooth.available()) {
    caracter = bluetooth.read();
  }

  if ( caracter == 'X' ) standAlone();
  if ( caracter == 'x' ) robotStop();
  if ( caracter == 'F' ) robotForward();
  if ( caracter == 'B' ) robotBackward();
  if ( caracter == 'L' ) robotLeft();
  if ( caracter == 'R' ) robotRight();
  if ( caracter == 'S' ) robotStop();
} 

void standAlone(){
  speedAdjust();
  
  robotForward();
  
  distanceCm = testDistance();
  
  if(distanceCm < 20){
      decision();
  }

   delay(80);
}

float testDistance(){
  float pulse;
  trigPulse();
  
  pulse = pulseIn(echo, HIGH);

  return (pulse/58.82);      
} 

void trigPulse(){
   digitalWrite(trig,HIGH);  
   delayMicroseconds(10);    
   digitalWrite(trig,LOW);   
} 

void speedAdjust(){
    pot = analogRead(speed);   
    
    pot = map(pot, 0, 1023, 0, 255); 
    
    analogWrite(M1E,pot);
    analogWrite(M2E,pot);
} 


void decision()      {
   robotStop();      
   delay(500);   

   servo.write(0);  
   delay(500);

   distanceRight = testDistance();
   delay(1000);

   servo.write(175);
   delay(500);

   distanceLeft = testDistance();
   delay(1000);

   servo.write(80);
   delay(500);
   
   if(distanceRight > distanceLeft){
      robotBackward();
      delay(500); 

      robotRight();
      delay(500);

      robotForward();
   
   }else{
      robotBackward();
      delay(500);

      robotLeft();
      delay(500);

      robotForward();
   }
} 

void robotForward(){
   speedAdjust();

   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);
   digitalWrite(MOTOR3, HIGH);
   digitalWrite(MOTOR4, LOW);
} 

void robotBackward(){
   speedAdjust();

   digitalWrite(MOTOR1, LOW);
   digitalWrite(MOTOR2, HIGH);
   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, HIGH);
} 

void robotRight(){
   speedAdjust();
   digitalWrite(MOTOR1, LOW);
   digitalWrite(MOTOR2, HIGH);
   digitalWrite(MOTOR3, HIGH);
   digitalWrite(MOTOR4, LOW);
} 

void robotLeft(){
   speedAdjust();

   digitalWrite(MOTOR1, HIGH);
   digitalWrite(MOTOR2, LOW);
   digitalWrite(MOTOR3, LOW);
   digitalWrite(MOTOR4, HIGH);
} 

void robotStop(){
   digitalWrite(MOTOR1,  LOW);
   digitalWrite(MOTOR2,  LOW);
   digitalWrite(MOTOR3,  LOW);
   digitalWrite(MOTOR4,  LOW);
}
