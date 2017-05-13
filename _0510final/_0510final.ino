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
//#define motor4 PD3 // left_forward

HardwareTimer pwmtimer(2);
uint16 maxduty, duty, duty2, RF_dis, LF_dis;
uint32 period, mypulse;

uint16 RF_count(){
  uint16 baseRF_part1 = UsLetzterGueltigerMesswert(0)*UsLetzterGueltigerMesswert(1);
  uint16 baseRF_part2 = sqrt(pow(UsLetzterGueltigerMesswert(0),2)+pow(UsLetzterGueltigerMesswert(1),2));
  RF_dis = baseRF_part1/baseRF_part2;
  return RF_dis;
}
uint16 LF_count(){
  uint16 baseLF_part1 = UsLetzterGueltigerMesswert(2)*UsLetzterGueltigerMesswert(1);
  uint16 baseLF_part2 = sqrt(pow(UsLetzterGueltigerMesswert(2),2)+pow(UsLetzterGueltigerMesswert(1),2));
  LF_dis = baseLF_part1/baseLF_part2;
  return LF_dis;
}
void UsHandler(int id, int entfernung)
{
  Print2Number("Sensor", id, entfernung);
}

//----- control functions------------------------

void Stop(uint32 mypulse){
  pwmWrite(motor1, 0);
  pwmWrite(motor2, 0);
  //pwmWrite(motor3, 0);
  Serial1.println("stop");
  delay(1000);
}
void TurnRight(uint32 mypulse){ //BROWN CAR, MY BOARD, PROTOTYPE TurnRight(2020); delay(400);
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);
  //GoStraight(4000);
  if (UsLetzterGueltigerMesswert(0)<=8 || RF_count()<=6 ){
    TurnLeft(1000);
  }
  else{
  pwmWrite(motor1, int(0.5*duty2)); // 0.6也可用測到的距離成反比
  pwmWrite(motor2, duty);
  Serial1.println("right");
  }
}
void TurnLeft(uint32 mypulse){//BROWN CAR, MY BOARD, PROTOTYPE TurnLeft(2020); delay(600);
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+480, 0, (int32)period, 0, (int32)maxduty);

  if (UsLetzterGueltigerMesswert(0)<=8 || LF_count()<=6 ){
    TurnRight(1000);
  }
  else{
  pwmWrite(motor1, duty2); // 0.6也可用測到的距離成反比
  pwmWrite(motor2, int(0.6*duty));
  Serial1.println("left");
  }
}
void GoStraight(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+500, 0, (int32)period, 0, (int32)maxduty);

  pwmWrite(motor1, duty2);
  pwmWrite(motor2, duty);
  //pwmWrite(motor3, 0);
  //pwmWrite(motor4, 0);
}
void straightCorrect(){
  if(UsLetzterGueltigerMesswert(0)<6){
    TurnLeft(2000); 
    GoStraight(2000);
    Serial1.println("straight too close");
  }else if(UsLetzterGueltigerMesswert(0)>10 && UsLetzterGueltigerMesswert(0)<17){
    TurnRight(2000); 
    GoStraight(2000);
    Serial1.println("straight too far");
  }else if(UsLetzterGueltigerMesswert(0)>=17){
    Stop(0);
    TurnRight(2000);
    delay(1000);
  }else{
    GoStraight(2000);
    Serial1.println("straight ok");
  }
}
void TurnAround(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  duty2 = map((int32)mypulse+400, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty2);
  pwmWrite(motor2, duty);
  //pwmWrite(motor3, 0);
  //pwmWrite(motor4, 0);
}
void Back(uint32 mypulse){
}
void Uturn(uint32 mypulse){
  duty = map((int32)mypulse, 0, (int32)period, 0, (int32)maxduty);
  pwmWrite(motor1, duty);
  pwmWrite(motor2, -duty);
  //pwmWrite(motor3, 0);
  //pwmWrite(motor4, 0);
}


void initPos(){
// ---------------------initial find right wall turnright----------------------    
  if (UsLetzterGueltigerMesswert(0)>15){
    Serial1.println("test1");
    TurnRight(2020);
    delay(600);
    GoStraight(2000);
  }
 
  while (UsLetzterGueltigerMesswert(1)>15){
  if(UsLetzterGueltigerMesswert(0)<8){
    TurnLeft(2000);
    GoStraight(2000);
  }
  else if(UsLetzterGueltigerMesswert(0)>10){
    TurnRight(2000);
    GoStraight(2000);
  }
  else{
    GoStraight(2000);
  } 
 }
 TurnLeft(2020);
 delay(600); 
}
 
//------------------------setup   here------------------------
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
  UsSetMessPause(100);  

  Serial1.println("activate Sensor 0 ---------------ff");
  aktiviereUSSensor(0);
  PrintUsSensorList();
  Serial1.println("activate Sensor 1 ---------------ff");
  aktiviereUSSensor(1);
  PrintUsSensorList();
  Serial1.println("activate Sensor 2 ---------------ff");
  aktiviereUSSensor(2);
  PrintUsSensorList();
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


  //initPos();  //note: initPos() and firstDo() is the same. I don't know who wrote it, just choose one

}

// -------------------- loop here ----------------------------

void loop()
{
  GoStraight(3000);
  if(UsLetzterGueltigerMesswert(0)>18){
    GoStraight(3000);
    TurnRight(3000);
  }

  if(UsLetzterGueltigerMesswert(1)<=10){
    Stop(0);
  }

  /*
  int rSensor = UsLetzterGueltigerMesswert(0);
  int mSensor = UsLetzterGueltigerMesswert(1);
  int lSensor = UsLetzterGueltigerMesswert(2);
  if(mSensor<10 || rSensor<3 || lSensor<3){
    Stop(0);
    Serial1.println("emergency stop!!");
    
    Serial1.print(UsLetzterGueltigerMesswert(2));
    Serial1.print(" , ");
    Serial1.print(UsLetzterGueltigerMesswert(1));
    Serial1.print(" , ");
    Serial1.println(UsLetzterGueltigerMesswert(0));
    
    Serial1.println(RF_count());
    Serial1.println(LF_count());
    
  }else{
    GoStraight(2000);
    straightCorrect();
    Serial1.println(UsLetzterGueltigerMesswert(0));
  }
  */
 
 
  
}






