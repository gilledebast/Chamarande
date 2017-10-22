/* ----------------------------------------------------------------------------------------------------
 * Chamarande, 2017
 * Update: 01/10/17
 * 
 * V1
 * Written by Bastien DIDIER
 * more info :
 *
 * ----------------------------------------------------------------------------------------------------
 */ 

//TB6612FNG Dual Motor Driver pololu
#define PWMA        11  // Motor A PWM
#define AIN2        10  // Motor A In 2
#define AIN1         9  // Motor A In 1
#define STBY         8  // Standby
#define BIN1         7  // Motor A In 1
#define BIN2         6  // Motor A In 2
#define PWMB         5  // Motor A PWM

#define led         13

void setup() {

  pinMode(PWMA,OUTPUT);
  pinMode(AIN2,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(STBY,OUTPUT);
  pinMode(BIN1,OUTPUT);
  pinMode(BIN2,OUTPUT);
  pinMode(PWMB,OUTPUT);
  
  pinMode(led,OUTPUT);

  Serial.begin(115200);

  Serial.println("Initialise...");
  
  motor_standby(false);

  
  motor_control(0,100,"A");
  motor_control(0,100,"B");
  
  //motor_control(1, 100, AIN1, AIN2, PWMA);
  //motor_control(1, 100, BIN1, BIN2, PWMB);
  //delay(2000);
  //motor_control(0, 100, AIN1, AIN2, PWMA);
  //motor_control(0, 100, BIN1, BIN2, PWMB);  
  delay(200000);
  motor_standby(true);

  Serial.println("Ready!");
}

void loop() {}

void serialEvent() {
  while (Serial.available()) {
    char inChar = Serial.read();
    Serial.print("Received :");
    Serial.println(inChar);

    if(inChar == 'P') {
      go("forward",100);
    } else if(inChar == 's') {
      go("backward",100);
    } else if(inChar == 'q') {
      go("left",100);
    } else if(inChar == 'd') {
      go("right",100);
    } else { 
      Serial.println("Unknow Command");
    }
  }
}

void go(String direction, char speed){
  
  motor_standby(false);
  
  if(direction == "forward") {
    motor_control(0,speed,"A");
    motor_control(1,speed,"B");
  } else if(direction == "backward") {
    motor_control(1,speed,"A");
    motor_control(0,speed,"B");
  } else if(direction == "left") {
    motor_control(0,speed,"A");
    motor_control(0,speed,"B");
  } else if(direction == "right") {
    motor_control(0,speed,"A");
    motor_control(0,speed,"B");
  } else {
    Serial.println("GO ? Unknow Command");
  }

  delay(1000);
  motor_standby(true);
}

void motor_control(boolean direction, char speed, String Motor){ //speed from 0 to 100

  int IN1,IN2,PWM;
  
  if(Motor == "A"){
    IN1 = AIN1;
    IN2 = AIN2;
    PWM = PWMA;
  } else if (Motor == "B"){
    IN1 = BIN1;
    IN2 = BIN2;
    PWM = PWMB;
  }
  
  if (direction == 0) {
    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
  } else {
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
  }
  
  if(speed < 0){
    speed = 0;
  }else if (speed > 100){
    speed = 100;
  }
  
  byte PWMvalue=0;
  PWMvalue = map(abs(speed),0,100,50,255); //anything below 50 is very weak
  analogWrite(PWM,PWMvalue);
}

void motor_standby(boolean state) { //low power mode
 if (state == true){
   digitalWrite(STBY,LOW);
} else{
   digitalWrite(STBY,HIGH);
 }
}
