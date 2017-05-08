#include <Arduino.h>
#include <debug.h>
#include <ultraschall.h>

//----------------- Ultraschall -----------------------
//                   3x HC-SR04
//prototype
#define US1_ECHO      PE11
#define US1_TRIGGER   PE10

#define US2_ECHO      PF6
#define US2_TRIGGER   PA12

#define US3_ECHO      PA13
#define US3_TRIGGER   PA11

#define ShowLED1 PE8
#define ShowLED2 PE9
#define ShowLED3 PE10

#define motor1 PD7 // right_forward
#define motor2 PD4 // left_forward
//#define motor3 PD6 // right_backward
//#define motor4 PD3 // left_backward

HardwareTimer pwmtimer(2);
uint16 maxduty, duty, duty2;
uint32 period, mypulse;

void UsHandler(int id, int entfernung)
{
  Print2Number("Sensor", id, entfernung);
}


void setup() 
{
  
  delay(150);
  debugInit();
  debugSetTimeStamp(DDEBUG_TIME_STAMP_ON);

   Serial1.begin(115200);
  // Ultraschallsensoren definieren
  addUSSensor(0, US1_TRIGGER, US1_ECHO, 220);
  addUSSensor(1, US2_TRIGGER, US2_ECHO, 220);
  addUSSensor(2, US3_TRIGGER, US3_ECHO, 220);

  UsAttachHandler(0, UsHandler);
  UsSetMessPause(100);  // Pause von 100 ms nach jeder Messung
                        // damit die Ausgabe im Terminal nicht zu schnell wird
                        // Im Normalfall würde keine so lange Pause programmiert werden.
                        // Die Pause ist nur nötig, wenn festgestellt wird, dass sich
                          // die Sensoren gegenseitig durch nachfolgende mehrfache Echos beeinflussen.
                        // Das ist Umgebungsabhängig.

  Serial1.println("activate Sensor 0 ---------------ff");
  aktiviereUSSensor(0);
  PrintUsSensorList();
  Serial1.println("activate Sensor 1 ---------------ff");
  aktiviereUSSensor(1);
  PrintUsSensorList();

  Serial1.println("activate Sensor 2 ---------------ff");
  aktiviereUSSensor(2);
  PrintUsSensorList();

  // ab jetzt wird im Hintergrund gemessen

  delay(1000);
  //Serial1.println("deactivate Sensor 2 ---------------");
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


  Serial1.println("Programm gestartet");
   pinMode(ShowLED1, OUTPUT);
   pinMode(ShowLED2, OUTPUT);
   pinMode(ShowLED3, OUTPUT);
   pwmtimer.pause();
   period = 10000;
   maxduty = pwmtimer.setPeriod(period);
   pwmtimer.refresh();
   pwmtimer.resume(); 
   pinMode(motor1, PWM);
   pinMode(motor2, PWM);
   //pinMode(motor3, PWM);
   //pinMode(motor4, PWM);

   
   if (UsLetzterGueltigerMesswert(0)>15){
      Serial1.println("test1");
      TurnRight(2020);
      delay(1000);
   }
   
    while (UsLetzterGueltigerMesswert(1)>15){
      if(UsLetzterGueltigerMesswert(0)<6){
        TurnLeft(2000);
        GoStraight(3000);
      }
      else if(UsLetzterGueltigerMesswert(0)>10){
        TurnRight(2000);
        GoStraight(3000);
      }
    }
      TurnLeft(2020);
      delay(1000);
      Serial1.println("test2");
      
}
    




void loop()
{

  /*TurnRight(2020);
  delay(1000);
  GoStraight(5000);
  delay(2000);
  TurnLeft(2020);
  delay(1000);
  GoStraight(5000);
  delay(2000);
  */
  /*
  Serial1.print("0 : ");
  Serial1.println(UsLetzterGueltigerMesswert(0));
  Serial1.print("1 : ");
  Serial1.println(UsLetzterGueltigerMesswert(1));
  Serial1.print("2 : ");
  Serial1.println("  ");
  */
  
// ------------front no barrier------------
while(UsLetzterGueltigerMesswert(0)>20){
  //calibrate right distance
  if(UsLetzterGueltigerMesswert(0)<6){
    TurnLeft(2000); 
  }
  else if(UsLetzterGueltigerMesswert(0)>10 && UsLetzterGueltigerMesswert(0)<15){
    TurnRight(2000);
  }
  //if right has exit, turn right
  else if(UsLetzterGueltigerMesswert(0)>=15){
    TurnRight(2020);
    delay(1000);
    //U turn
    if(UsLetzterGueltigerMesswert(0)>=15){
      TurnRight(2020);
      delay(1000);    
    }
    GoStraight(2000);
  }
}

//------------front HAS barrier------------

while(UsLetzterGueltigerMesswert(0)<=20){
  //turn right first
  if(UsLetzterGueltigerMesswert(0)>=15){
    TurnRight(2020);
    delay(1000);
    //U turn
    if(UsLetzterGueltigerMesswert(0)>=15){
      TurnRight(2020);
      delay(1000);    
    }
  }
  //if no right exit, turn left
  else if (UsLetzterGueltigerMesswert(2)>=15 && UsLetzterGueltigerMesswert(0)<15){
    TurnLeft(2020);
    delay(1000);
  }
  GoStraight(2000);
  }
}


void TurnRight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+444, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, 0);
  pwmWrite(motor2, duty2);
  //pwmWrite(motor3, 0);
  //pwmWrite(motor4, 0);

}


void TurnLeft(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+400, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty2);
  pwmWrite(motor2, 0);
  //pwmWrite(motor3, 0);
  //pwmWrite(motor4, 0);

}

void GoStraight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+400, 0, (int32)period, 0, (int32)maxduty);

  pwmWrite(motor1, duty2);
  pwmWrite(motor2, duty);
  //pwmWrite(motor3, 0);
  //pwmWrite(motor4, 0);

}

void TurnAround(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+400, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty2);
  pwmWrite(motor2, duty);
  //pwmWrite(motor3, 0);
  //pwmWrite(motor4, 0);
}





