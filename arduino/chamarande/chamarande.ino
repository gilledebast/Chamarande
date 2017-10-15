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

  Serial.begin(9600);

  Serial.println("Initialise...");
  
  motor_standby(false);
  motor_control(1, 100, AIN1, AIN2, PWMA);
  motor_control(1, 100, BIN1, BIN2, PWMB);
  delay(2000);
  motor_control(0, 100, AIN1, AIN2, PWMA);
  motor_control(0, 100, BIN1, BIN2, PWMB);  
  delay(2000);
  motor_standby(true);

  Serial.println("Ready!");
}

void loop() {

}

void motor_control(boolean direction, char speed, int IN1, int IN2, int PWM){ //speed from 0 to 100

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
