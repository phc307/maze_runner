/******************************************************************************
 * The MIT License
 *
 * Copyright (c) 2015 Frank-Michael Krause
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/

#include <Arduino.h>
#include <debug.h>
#include <ultraschall.h>

/*
 * Demoprogramm für den Betrieb von Ultraschallsensoren über Interrupts.
 *
 * Diese Betriebsart hat den Vorteil, dass während der Messung (die einige 10ms dauern kann)
 * andere Dinge erledigt werden können.
 *
 * Es kann ständig im Hintergrund gemessen werden und der letzte gültige oder der aktuelle
 * Messwert per Funktion abgefragt werden.
 *
 * Außerdem kann eine Callbackfunktion pro Sensor registriert werden, die nach einer erfolgten
 * Messung aufgerufen wird. ZU beachten ist, dass diese Funktion im Interrupt-Kontext aufgerufen wird
 * und daher möglichst kurz (d.h. schnell abarbeitbar) sein.
 * Bei Debug-Ausgaben in der Handler-Funktion können sich die Ausgaben in Ausgaben aus loop() heraus "hineindrängeln"
 *
 */


//----------------- Ultraschall -----------------------
//                   3x HC-SR04

#define US1_ECHO      PE11
#define US1_TRIGGER   PE10

#define US2_ECHO      PF6
#define US2_TRIGGER   PA12

#define US3_ECHO      PA13
#define US3_TRIGGER   PA11

#define ShowLED1 PE8
#define ShowLED2 PE9

#define motor1 PD7
#define motor1_R PD6
#define motor2 PD4
#define motor2_R PD3

#define general_speed 2900

HardwareTimer pwmtimer(2);
uint16 maxduty, duty, sduty, duty2, RF_dis;
uint32 period, mypulse;
float zangle;
int flag, i, RightFlag, TooRightFlag, TooLeftFlag;
int start = 1;
void UsHandler(int id, int entfernung)
{
	Print2Number("Sensor", id, entfernung);
}
uint16 RF_count(){
  uint16 baseRF_part1 = UsLetzterGueltigerMesswert(0)*UsLetzterGueltigerMesswert(1);
  uint16 baseRF_part2 = sqrt(pow(UsLetzterGueltigerMesswert(0),2)+pow(UsLetzterGueltigerMesswert(1),2));
  RF_dis = baseRF_part1/baseRF_part2;
  return RF_dis;
}

void setup() 
{
  
	delay(150);
	debugInit();
	debugSetTimeStamp(DDEBUG_TIME_STAMP_ON);

   Serial1.begin(115200);
   Serial1.println("START");
	// Ultraschallsensoren definieren
	addUSSensor(0, US1_TRIGGER, US1_ECHO, 220);
	addUSSensor(1, US2_TRIGGER, US2_ECHO, 220);
	addUSSensor(2, US3_TRIGGER, US3_ECHO, 220);

	UsAttachHandler(0, UsHandler);
  UsAttachHandler(1, UsHandler);
  UsAttachHandler(2, UsHandler);
	UsSetMessPause(100);  // Pause von 100 ms nach jeder Messung
	                      // damit die Ausgabe im Terminal nicht zu schnell wird
	                      // Im Normalfall würde keine so lange Pause programmiert werden.
	                      // Die Pause ist nur nötig, wenn festgestellt wird, dass sich
                          // die Sensoren gegenseitig durch nachfolgende mehrfache Echos beeinflussen.
	                      // Das ist Umgebungsabhängig.

	Serial1.println("activate Sensor 0 ---------------");
	aktiviereUSSensor(0);
	PrintUsSensorList();
	Serial1.println("activate Sensor 1 ---------------");
	aktiviereUSSensor(1);
	PrintUsSensorList();

	Serial1.println("activate Sensor 2 ---------------");
	aktiviereUSSensor(2);
	PrintUsSensorList();

	// ab jetzt wird im Hintergrund gemessen
/*
	delay(1000);
	debugPrint("deactivate Sensor 2 ---------------");
	deaktiviereUSSensor(2);
	PrintUsSensorList();

	delay(1000);
	debugPrint("deactivate Sensor 1 ---------------");
	deaktiviereUSSensor(1);
	PrintUsSensorList();

	delay(1000);
	debugPrint("deactivate Sensor 0 ---------------");
	deaktiviereUSSensor(0);
	PrintUsSensorList();

	delay(1000);
	debugPrint("activate Sensor 0 ---------------");
	aktiviereUSSensor(0);
	PrintUsSensorList();

	delay(1000);
    debugPrint("activate Sensor 1 ---------------");
	aktiviereUSSensor(1);
	PrintUsSensorList();
  
  delay(1000);
  debugPrint("activate Sensor 2 ---------------");
  aktiviereUSSensor(2);
  PrintUsSensorList();

*/
	Serial1.println("Programm gestartet");
 pinMode(ShowLED1, OUTPUT);
 pinMode(ShowLED2, OUTPUT);
 pwmtimer.pause();
 period = 10000;
 maxduty = pwmtimer.setPeriod(period);
 pwmtimer.refresh();
 pwmtimer.resume(); 
 pinMode(motor1, PWM);
 pinMode(motor2, PWM);
 pinMode(motor1_R, OUTPUT);
 pinMode(motor2_R, OUTPUT);
/*
    Serial1.println("initial");
digitalWrite(ShowLED2, HIGH);
 StandRight(general_speed);
 
   Serial1.println("initial");
    digitalWrite(ShowLED1, HIGH);
    delay(400);
  */
  /*
  StandRight(2000);
  while(UsLetzterGueltigerMesswert(0) > 18){
    
  }
  GoStraight(general_speed);
  delay(100);
*/

}

void loop()
{
//	Print2Number("Sensor 2", UsLetzterGueltigerMesswert(1), UsLetzterGueltigerMesswertAlter(1));
//  Print2Number("Sensor 1", UsLetzterGueltigerMesswert(0), UsLetzterGueltigerMesswertAlter(0));
  Serial1.println("1 : ");
  Serial1.println(UsLetzterGueltigerMesswert(0));
  Serial1.println("2 : ");
  Serial1.println(UsLetzterGueltigerMesswert(1));
  Serial1.println("3 : ");
  Serial1.println(UsLetzterGueltigerMesswert(2));
  Serial1.println("flag : ");
  Serial1.println(flag);
  Serial1.println("  ");
//  zangle = (UsLetzterGueltigerMesswert(0))^2
  if(start == 1){
      while(UsLetzterGueltigerMesswert(0) > 20){
        StandRight(4000);
        delay(80);
        clean_pwm();
        delay(100);
      }
      
    clean_pwm();
    GoStraight(4000);
    delay(5);
    start = 0;
  }
  
  if(UsLetzterGueltigerMesswert(0) <= 10 && UsLetzterGueltigerMesswert(0)>=9){
    
    //Go Straight
    if(UsLetzterGueltigerMesswert(1) >18){
      GoStraight(general_speed);
    }else if(UsLetzterGueltigerMesswert(1) <= 18){
      //Go Left
      if(UsLetzterGueltigerMesswert(2) <= 22 && UsLetzterGueltigerMesswert(2) != 0){
        clean_pwm();
        if(UsLetzterGueltigerMesswert(2)>=UsLetzterGueltigerMesswert(0)){
        
        StandLeft(2000);
        for(i = 0; i<41; i++){
          if(i%3 ==0){
            digitalWrite(motor2_R, HIGH);
            delay(20);
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor2_R, LOW);
            delay(20);
          }
          digitalWrite(motor2_R, LOW);
          
  digitalWrite(ShowLED1, HIGH);
        }
        }else if(UsLetzterGueltigerMesswert(0)>UsLetzterGueltigerMesswert(2)){
        StandRight(2000);
        for(i = 0; i<43; i++){
          if(i%3 ==0){
            digitalWrite(motor1_R, HIGH);
            delay(20);
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor1_R, LOW);
            delay(20);
          }
          digitalWrite(motor1_R, LOW);
          
  digitalWrite(ShowLED1, HIGH);
        }
        }
        clean_pwm();
        delay(200);
        
        GoStraight(3500);
        delay(250);
      }else if(UsLetzterGueltigerMesswert(2) > 18){
        StandLeft(general_speed);
        delay(540);
        TurnLeft(general_speed);
        delay(50);
        flag = 2;
        GoStraight(4000);
        delay(20);
//        clean_pwm();
//        delay(1000);
      }
      if(TooRightFlag = 1 && UsLetzterGueltigerMesswert(1) > 19){
        StandLeft(2000);
        delay(15);
        clean_pwm();
        delay(100);
        TooRightFlag = 0;
      }else if(TooLeftFlag = 1 && UsLetzterGueltigerMesswert(1) > 19){
        StandRight(2000);
        delay(10);
        clean_pwm();
        delay(100);
        TooLeftFlag = 0;
      }
      
    }
  }else if(UsLetzterGueltigerMesswert(0) < 9){
    RightFlag = 1;
    TooLeftFlag = 1;
    if(UsLetzterGueltigerMesswert(1) >18){
      SlowLeft(general_speed);
      //TurnLeft(general_speed);
    }
    if(UsLetzterGueltigerMesswert(1) <= 18){
      //Go Left
      if(UsLetzterGueltigerMesswert(2) <= 22 && UsLetzterGueltigerMesswert(2) != 0){
        if(UsLetzterGueltigerMesswert(2)>=UsLetzterGueltigerMesswert(0)){
        
        StandLeft(2000);
        for(i = 0; i<41; i++){
          if(i%3 ==0){
            digitalWrite(motor2_R, HIGH);
            delay(20);
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor2_R, LOW);
            delay(20);
          }
          digitalWrite(motor2_R, LOW);
          
  digitalWrite(ShowLED1, HIGH);
        }
        }else if(UsLetzterGueltigerMesswert(0)>UsLetzterGueltigerMesswert(2)){
        StandRight(2000);
        for(i = 0; i<43; i++){
          if(i%3 ==0){
            digitalWrite(motor1_R, HIGH);
            delay(20);
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor1_R, LOW);
            delay(20);
          }
          digitalWrite(motor1_R, LOW);
          
  digitalWrite(ShowLED1, HIGH);
        }
        }
        clean_pwm();
        delay(200);
        GoStraight(3500);
        delay(250);
        
      }else if(UsLetzterGueltigerMesswert(2) > 18){
        StandLeft(general_speed);
        delay(550);
        TurnLeft(general_speed);
        delay(50);
        flag = 2;
        
        GoStraight(4000);
        delay(20);
//        clean_pwm();
//        delay(1000);
      }
    }
//    delay(50);
  }else if(UsLetzterGueltigerMesswert(0) > 20 ){
      if(RightFlag == 1){
       StandLeft(2000);
        delay(30);
        RightFlag = 0;
      }else{
       TurnRight(1700);
      }
//    flag =1;
//    digitalWrite(ShowLED2, HIGH);
//    digitalWrite(ShowLED1, LOW);
  }else if(UsLetzterGueltigerMesswert(0)<=20 && UsLetzterGueltigerMesswert(0)>10){
    TooRightFlag = 1;
    if(UsLetzterGueltigerMesswert(1) >18){
      SlowRight(2000);
      //TurnLeft(general_speed);
    }
    if(UsLetzterGueltigerMesswert(1) <= 18){
      //Go Left
      if(UsLetzterGueltigerMesswert(2) <= 22 && UsLetzterGueltigerMesswert(2) != 0){
        if(UsLetzterGueltigerMesswert(2)>=UsLetzterGueltigerMesswert(0)){
        
        StandLeft(2000);
        for(i = 0; i<41; i++){
          if(i%3 ==0){
            digitalWrite(motor2_R, HIGH);
            delay(20);
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor2_R, LOW);
            delay(20);
          }
          digitalWrite(motor2_R, LOW);
          
  digitalWrite(ShowLED1, HIGH);
        }
        }else if(UsLetzterGueltigerMesswert(0)>UsLetzterGueltigerMesswert(2)){
        StandRight(2000);
        for(i = 0; i<43; i++){
          if(i%3 ==0){
            digitalWrite(motor1_R, HIGH);
            delay(20);
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor1_R, LOW);
            delay(20);
          }
          digitalWrite(motor1_R, LOW);
          
  digitalWrite(ShowLED1, HIGH);
        }
        }
        clean_pwm();
        delay(200);
        GoStraight(3500);
        delay(250);
      }else if(UsLetzterGueltigerMesswert(2) > 18){
        StandLeft(general_speed);
        delay(500);
        TurnLeft(general_speed);
        delay(50);
        flag = 2;
        
        GoStraight(4000);
        delay(20);
//        clean_pwm();
//        delay(1000);
      }
    }
  }
       

if(flag == 2){
    GoStraight(4000);
    delay(5);
    flag = 0;
}
}
void TurnRight(uint32 mypulse){
 
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, int(duty2*0.6)); // 0.6也可用測到的距離成反比
  pwmWrite(motor2, int(duty*1.7));
//  Serial1.println("right");
  

//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, 0);
  
}
void TurnLeft(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty2);
  pwmWrite(motor2, duty/2);
//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, 0);
}
void SlowLeft(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
//  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, int(duty));
  pwmWrite(motor2, int(duty*0.7));
//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, 0);
}
void SlowRight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, int(duty2*0.95)); // 0.6也可用測到的距離成反比
  pwmWrite(motor2, int(duty*1.3));
//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, 0);
}
void GoStraight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  digitalWrite(ShowLED1, LOW);
  pwmWrite(motor1, int(duty));
  pwmWrite(motor2, int(duty));
//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, 0);
}
void StandRight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, 0);
  pwmWrite(motor2, duty);
//  pwmWrite(motor1_R, duty);
//  pwmWrite(motor2_R, 0);
}
void StandLeft(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty);
  pwmWrite(motor2, 0);
//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, duty);
}
void clean_pwm(){
  pwmWrite(motor1, 0);
  pwmWrite(motor2, 0);
}

