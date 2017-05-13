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

#define general_speed 3500

HardwareTimer pwmtimer(2);
uint16 maxduty, duty, sduty, duty2, RF_dis;
uint32 period, mypulse;
float zangle;
int flag, i;
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
    delay(750);
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

  if(UsLetzterGueltigerMesswert(0) <= 12 && UsLetzterGueltigerMesswert(0)>=10){
    
    //Go Straight
    if(UsLetzterGueltigerMesswert(1) >20){
      GoStraight(general_speed);
    }
    if(UsLetzterGueltigerMesswert(1) <= 20){
      //Go Left
      if(UsLetzterGueltigerMesswert(2) <= 8 && UsLetzterGueltigerMesswert(2) != 0){
        StandLeft(general_speed);
        for(i = 0; i<600; i++){
          if(i%3 == 0){
            digitalWrite(motor1_R, HIGH);
            
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor1_R, LOW);
          }
          digitalWrite(motor1_R, LOW);
  digitalWrite(ShowLED1, HIGH);
        }
        flag = 3;
      }else if(UsLetzterGueltigerMesswert(2) > 8){
        StandLeft(general_speed);
        delay(300);
        TurnLeft(general_speed);
        delay(280);
        flag = 2;
      }
    }
  }else if(UsLetzterGueltigerMesswert(0) < 10){
    
    if(UsLetzterGueltigerMesswert(1) >20){
      SlowLeft(general_speed);
      //TurnLeft(general_speed);
    }
    if(UsLetzterGueltigerMesswert(1) <= 20){
      //Go Left
      if(UsLetzterGueltigerMesswert(2) <= 8 && UsLetzterGueltigerMesswert(2) != 0){
        StandLeft(general_speed);
        for(i = 0; i<600; i++){
          if(i%3 ==0){
            digitalWrite(motor1_R, HIGH);
            
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor1_R, LOW);
          }
          digitalWrite(motor1_R, LOW);
          
  digitalWrite(ShowLED1, HIGH);
        }
      }else if(UsLetzterGueltigerMesswert(2) > 8){
        StandLeft(general_speed);
        delay(300);
        TurnLeft(general_speed);
        delay(280);
        flag = 2;
      }
    }
//    delay(50);
  }else if(UsLetzterGueltigerMesswert(0) > 19 ){
    TurnRight(2000);
//    flag =1;
  }else if(UsLetzterGueltigerMesswert(0)<=19 && UsLetzterGueltigerMesswert(0)>12){
    if(UsLetzterGueltigerMesswert(1) >20){
      SlowRight(2000);
      //TurnLeft(general_speed);
    }
    if(UsLetzterGueltigerMesswert(1) <= 20){
      //Go Left
      if(UsLetzterGueltigerMesswert(2) <= 8 && UsLetzterGueltigerMesswert(2) != 0){
        StandLeft(general_speed);
        for(i = 0; i<600; i++){
          if(i%3 == 0){
            digitalWrite(motor1_R, HIGH);
            
  digitalWrite(ShowLED2, HIGH);
          }else{
            digitalWrite(motor1_R, LOW);
            
  digitalWrite(ShowLED1, HIGH);
          }
        }
        digitalWrite(motor1_R, LOW);
      }else if(UsLetzterGueltigerMesswert(2) > 8){
        StandLeft(general_speed);
        delay(300);
        TurnLeft(general_speed);
        delay(250);
        flag = 2;
      }
    }
  }
  /*if(flag == 1){
    delay(10);
    clean_pwm();
    delay(20);
    flag = 0;
  }else if(flag == 2){
    TurnRight(4000);
    delay(300);
  }else{
    delay(15);
//    clean_pwm();
//    delay(10);
  }*/
  /*
  if(UsLetzterGueltigerMesswert(1)>100){
    mypulse = 10000;
  }else if(UsLetzterGueltigerMesswert(1)>20){
    mypulse = 5000;
  }else if(UsLetzterGueltigerMesswert(1)<20){
    mypulse = 0;
  }
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty);
  pwmWrite(motor2, duty);
*/

  if(flag == 2){
    TurnRight(4000);
    delay(20);
    flag = 0;
  }else if(flag == 3){
    clean_pwm();
    digitalWrite(motor2_R, HIGH);
    digitalWrite(motor1_R, HIGH);
    delay(10);
    digitalWrite(motor2_R, LOW);
    digitalWrite(motor1_R, LOW);
    delay(20);
    if(UsLetzterGueltigerMesswert(0) < 6){
      SlowRight(2000);
      delay(10);
    }else if(UsLetzterGueltigerMesswert(2) < 6){
      SlowLeft(2000);
      delay(10);
    }
    if(UsLetzterGueltigerMesswert(2) > 12){
      delay(100);
      digitalWrite(motor2_R, LOW);
      digitalWrite(motor1_R, LOW);
      StandLeft(general_speed);
      delay(300);
      TurnLeft(general_speed);
      delay(250);
      flag = 0;
    }else if(UsLetzterGueltigerMesswert(0) > 12 && UsLetzterGueltigerMesswert(2) <= 12){
      digitalWrite(motor1_R, LOW);
      delay(100);
      digitalWrite(motor2_R, LOW);
      StandLeft(general_speed);
      delay(300);
      TurnLeft(general_speed);
      delay(250);
      flag = 0;
    }
  }
}
void TurnRight(uint32 mypulse){
//  void TurnRight(uint32 mypulse){ //BROWN CAR, MY BOARD, PROTOTYPE TurnRight(2020); delay(400);
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  //GoStraight(4000);
/*  if (UsLetzterGueltigerMesswert(0)<=10 || RF_count()<=10 ){
    TurnLeft(1000);
    delay(15);
  }else*/ 
  pwmWrite(motor1, int(duty2*0.6)); // 0.6也可用測到的距離成反比
  pwmWrite(motor2, int(duty*1.5));
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
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty2);
  pwmWrite(motor2, int(duty*0.65));
//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, 0);
}
void SlowRight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, int(duty2*0.8)); // 0.6也可用測到的距離成反比
  pwmWrite(motor2, duty);
//  pwmWrite(motor1_R, 0);
//  pwmWrite(motor2_R, 0);
}
void GoStraight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  digitalWrite(ShowLED1, LOW);
  pwmWrite(motor1, duty);
  pwmWrite(motor2, duty);
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

