// import libraries
#include <AFMotor.h>

// define variables
#define Trigger_pin A0 //left
#define Echo_pin A1 //left
#define Trigger_pin1 A2 // middle
#define Echo_pin1 A3 // middle
#define Trigger_pin2 A4 // right
#define Echo_pin2 A5 //right

#define Max_speed 200
#define sensor_R 10
#define sensor_L 9

int distance = 0;
int distance1 = 0;
int distance2 = 0;
int dis = 0;
int dis_1 = 0;
int dis_2 = 0;

int count = 0;

int L = 0;
int R = 0;
int line_L, line_R;
unsigned long measure = 0;
unsigned long measure1 = 0;
unsigned long measure2 = 0;

// initial motors pin
AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);
AF_DCMotor motor3(3, MOTOR34_1KHZ);
AF_DCMotor motor4(4, MOTOR34_1KHZ);

void setup(){
  Serial.begin(9600);

  //setup ultrasound sensor
  pinMode(Trigger_pin, OUTPUT);
  pinMode(Echo_pin, INPUT);
  pinMode(Trigger_pin1, OUTPUT);
  pinMode(Echo_pin1, INPUT);
  pinMode(Trigger_pin2, OUTPUT);
  pinMode(Echo_pin2, INPUT);

  //setup ir sensor
  pinMode(sensor_R, INPUT); //declare ir sensor as input
  pinMode(sensor_L, INPUT); //declare ir sensor as input

}

//-------------------------------------------------------fuction for following line ----------------------------------------------//
void check_line(){
  line_L = digitalRead(sensor_L);
  line_R = digitalRead(sensor_R);
  if(line_L == 1){
    Serial.print("left: ");
    Serial.println(line_L);
    right();
  }
  if(line_R == 1){
    Serial.print("right: ");
    Serial.println(line_R);
    left();
  }
  if(line_L == 0 && line_R == 0){
    Serial.println("forward");
    forward();
  }
  delay(2);
}

//-------------------------------------------------------fuction for calculating distance ----------------------------------------------//
unsigned int Distance(byte option){
  switch(option){
    case 0:
      digitalWrite(Trigger_pin,LOW);
      delayMicroseconds(2);
      digitalWrite(Trigger_pin,HIGH);
      delayMicroseconds(5);
      digitalWrite(Trigger_pin,LOW);

      measure = pulseIn(Echo_pin,HIGH,20000);
      distance = (measure/2/29.412);
      if (distance == 0){
        distance = 250;
      }
      return distance;
      break;
    case 1:
      digitalWrite(Trigger_pin1,LOW);
      delayMicroseconds(2);
      digitalWrite(Trigger_pin1,HIGH);
      delayMicroseconds(5);
      digitalWrite(Trigger_pin1,LOW);

      measure1 = pulseIn(Echo_pin1,HIGH,20000);
      distance1 = (measure1/2/29.412);
      if (distance1 == 0){
        distance1 = 250;
      }
      return distance1;
      break;
    case 2:
      digitalWrite(Trigger_pin2,LOW);
      delayMicroseconds(2);
      digitalWrite(Trigger_pin2,HIGH);
      delayMicroseconds(5);
      digitalWrite(Trigger_pin2,LOW);

      measure2 = pulseIn(Echo_pin2,HIGH,20000);
      distance2 = (measure2/2/29.412);
      if (distance2 == 0){
        distance2 = 250;
      }
      return distance2;
      break;
  }
}

//---------------------------------function for finding direction------------------------------------
void Obstacle() {
  
  dis = Distance(0);
  while(dis > 50){// change limited distance
    //test
    Serial.print("case 1: ");
    
    if(count == 1){
      dis_2 = Distance(2);
      //test
      Serial.print("d3 = ");
      Serial.println(dis_2);
      
      check_line();
      while(dis_2 > 20){
        delay(2000);
        right();
        if(digitalRead(sensor_L) == 1){
          count = 0;
          break;
        }
      }
    }
    else check_line();
    dis = Distance(0);
    //test
    Serial.print("d1 = ");
    Serial.println(dis);
  } 
  while((dis >= 20) && (dis <= 50)){// change limited distance
    //test
    Serial.print("case 2: ");
    check_line();
    for(int i = Max_speed; i >= 100; i-= 10){ // decrease speed 
      //test
//      Serial.print("i = ");
//      Serial.println(i);
      
      motor1.setSpeed(i);
      motor2.setSpeed(i);
      motor3.setSpeed(i);
      motor4.setSpeed(i);
      delay(100);
    }
    dis = Distance(0);
    
  }
  while(dis < 20){// change limited distance
    //test
    Serial.print("case 3: ");
    Stop();
    delay(200);
    dis_1 = Distance(1);
    //test
    Serial.print("d2 = ");
    Serial.println(dis_1);
    
    if(dis_1 > 20){
    while(1){
      Serial.println("left");
      left();
      count = 1;
       if(digitalRead(sensor_R) == 1) break;
//      delay(2000);
      break;
    }
    check_line();
    }
    else Stop();
    dis = Distance(1);
  }
}

void forward(){
  for(int i=0; i<=Max_speed; i+=5){// increase speed
    motor1.setSpeed(i);
    motor2.setSpeed(i);
    motor3.setSpeed(i);
    motor4.setSpeed(i);
  }

  motor1.run(FORWARD);  //rotate the motor clockwise
  motor2.run(FORWARD);  //rotate the motor clockwise
  motor3.run(FORWARD);  //rotate the motor clockwise
  motor4.run(FORWARD);  //rotate the motor clockwise
}

void backward(){
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void left(){
  motor1.setSpeed(Max_speed);
  motor2.setSpeed(Max_speed);
  motor3.setSpeed(Max_speed);
  motor4.setSpeed(Max_speed);
  
  motor1.run(BACKWARD); //rotate the motor anti-clockwise
  motor2.run(BACKWARD); //rotate the motor anti-clockwise
  motor3.run(FORWARD);  //rotate the motor clockwise
  motor4.run(FORWARD);  //rotate the motor clockwise
}

void right(){
  motor1.setSpeed(Max_speed);
  motor2.setSpeed(Max_speed);
  motor3.setSpeed(Max_speed);
  motor4.setSpeed(Max_speed);
  
  motor1.run(FORWARD);  //rotate the motor clockwise
  motor2.run(FORWARD);  //rotate the motor clockwise
  motor3.run(BACKWARD); //rotate the motor anti-clockwise
  motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void Stop(){
  motor1.run(RELEASE); //stop the motor when release the button
  motor2.run(RELEASE); //rotate the motor clockwise
  motor3.run(RELEASE); //stop the motor when release the button
  motor4.run(RELEASE); //stop the motor when release the button
}

void loop(){
  Obstacle();
}
