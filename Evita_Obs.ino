#include <Servo.h>;

//Inicializo el servo
Servo servo1;

//pines Arduino
//pines entradas analagicas como salida digital
//LED RGB
int red = A0;
int green = A1;
int blue = A2;

// pines digitales
// Buzzer
int bz = 3;

//Sensor Ultrasonido
int tg = 4;
int eco = 5;

//Servo motor attach pin 10

// Entradas Modulo Puente H L298
int M11 = 6;
int M12 = 7;
int M21 = 8;
int M22 = 9;

//Variables
bool p1 =false; //variable prueba del sensor
int dist = 0; //distancia detectada por el sensor ultrasonido
int time1 = 0; //tiempo del sonido del sensor

//grados servo
int gr;
bool serR,serL;

//espacio libre
bool placeFree;

//timers
long unsigned t1,t2,t3;
bool acT1,acT2,acT3;

//distancia limite
int distlim=30;

void setup() {
  Serial.begin(9600);
  // Conifguramos los pines como entradas o salidas
  //Entradas
  //Echo del Sensor Ultrasonido
  pinMode(eco,INPUT);
  //Salidas
  //Led RGB
  pinMode(red,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  // Buzzer
  pinMode(bz,OUTPUT);
  //trig Sensor Ultrasonido
  pinMode(tg,OUTPUT);
  //Entrada Modulo puente H L298
  pinMode(M11,OUTPUT);
  pinMode(M12,OUTPUT);
  pinMode(M21,OUTPUT);
  pinMode(M22,OUTPUT);

  servo1.attach(10);
  //Prueba de electronica funcionando
  //Servo se lleva a 0 grados y 90 grados
  servo1.write(0);
  delay(500);
  servo1.write(90);
  delay(500);
  //Prueba de Led
  digitalWrite(red,HIGH);
  delay(500);
  digitalWrite(green,HIGH);
  digitalWrite(red,LOW);
  delay(500);
  digitalWrite(blue,HIGH);
  digitalWrite(red,LOW);
  digitalWrite(green,LOW);
  delay(500);
  digitalWrite(blue,LOW);
  //prueba buzzer
  analogWrite(bz,155);
  delay(250);
  analogWrite(bz,0);
  delay(100);
  analogWrite(bz,155);
  delay(250);
  analogWrite(bz,0);
  // prueba sensor ultrasonido distancia 20cm
  while(p1==false){
  digitalWrite(tg,LOW);
  delayMicroseconds(20);
  digitalWrite(tg,HIGH);
  delayMicroseconds(20);
  digitalWrite(tg,LOW);
  time1=pulseIn(eco,HIGH);
  dist=int((time1*0.034)/2);
  if(dist<=20){
    p1=true;
     digitalWrite(green,HIGH);
     delay(100);
     digitalWrite(green,LOW);
     delay(100);
     digitalWrite(green,HIGH);
     delay(100);
     digitalWrite(green,LOW);
  }
  }
  gr=90;
  serL=true;
  serR=false;
  acT1=true;
  acT2=true;
  acT3=true;
  
}

void loop() {
  // put your main code here, to run repeatedly:
if(placeFree==false){
  //Se inicia el contador de los led,
  if(acT1==true){
    acT1=false;
    t1=millis();
  }
  //se inicia el contador del buzzer
  if(acT3==true){
    acT3=false;
    t3=millis();
  }
  

   
  if(servo1.read()==90 && ultra()<50){
  digitalWrite(M11,HIGH);
  digitalWrite(M12,LOW);
  digitalWrite(M21,HIGH);
  digitalWrite(M22,LOW);
  }
  if(servo1.read()<90 && ultra()<50){
  digitalWrite(M11,LOW);
  digitalWrite(M12,HIGH);
  digitalWrite(M21,HIGH);
  digitalWrite(M22,LOW); 
  
  }
  if(servo1.read()>90 && ultra()<50){
  digitalWrite(M11,HIGH);
  digitalWrite(M12,LOW);
  digitalWrite(M21,LOW);
  digitalWrite(M22,HIGH);
 
  }
  //Enciende el Led Rojo
  if(millis()-t1>=50){
  digitalWrite(red,HIGH);
  t1=millis();
  }
  if(millis()-t1>=150){
    digitalWrite(red,LOW);
  }
// Suena el Buzzer mas rapido  
if(millis()-t3>=50){
  analogWrite(bz,0);
   }
 if(millis()-t3>=150){
  analogWrite(bz,50);
  acT3=true;
 } 

 if(ultra()>=distlim){placeFree=true;acT1=true;digitalWrite(red,LOW);}
}
if(ultra()<=distlim){placeFree=false;}
if(ultra()>=distlim && placeFree==true){
  //se desplaza hacia adelante
  if(acT1==true){t1=millis();acT1=false;}
  if(acT2==true){t2=millis();acT2=false;}
  if(acT3==true){t3=millis();acT3=false;}
  digitalWrite(M11,LOW);
  digitalWrite(M12,HIGH);
  digitalWrite(M21,LOW);
  digitalWrite(M22,HIGH);
  if(gr<0 && serR==true){serR=false;serL=true;}
  if(gr>180 && serL==true){serR=true;serL=false;}
 //Encienden los LEDs de color verde y azul 
  if(millis()-t2>=200){
  digitalWrite(green,HIGH);  
  }
    if(millis()-t2>=400){
  digitalWrite(green,LOW);  
  }
    if(millis()-t2>=600){
  digitalWrite(blue,HIGH);  
  }
    if(millis()-t2>=800){
  digitalWrite(blue,LOW);  
  acT2=true;}
// se mueve el servo
  if(millis()-t1>=40){
  if(serL==true){gr+=18;}
  if(serR==true){gr-=18;}
  servo1.write(gr);
  acT1=true;
 }
//suena el Buzzer lento
 if(millis()-t3>=200){
  analogWrite(bz,80);
 }
  if(millis()-t3>=250){
  analogWrite(bz,0);
  }
   if(millis()-t3>=650){
    acT3=true;
 }
 }
}
int ultra(){
 digitalWrite(tg,LOW);
  delayMicroseconds(20);
  digitalWrite(tg,HIGH);
  delayMicroseconds(20);
  digitalWrite(tg,LOW);
  time1=pulseIn(eco,HIGH);
  dist=int((time1*0.034)/2);
return dist;
}


