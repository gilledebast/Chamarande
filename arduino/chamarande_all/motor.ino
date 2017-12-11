/*
 * Function for TB6612FNG Dual Motor Driver pololu
 * 
 */

int speed = 100;

void left(int ms_delay){
  motor_standby(false);
  motor_control(0,"A");
  motor_control(0,"B");
  delay(ms_delay);
  motor_standby(true);
}

void right(int ms_delay){
  motor_standby(false);
  motor_control(1,"A");
  motor_control(1,"B");
  delay(ms_delay);
  motor_standby(true);
}

void forward(int ms_delay){
  motor_standby(false);
  motor_control(0,"A");
  motor_control(1,"B");
  delay(ms_delay);
  motor_standby(true);
}

void backward(int ms_delay){
  motor_standby(false);
  motor_control(1,"A");
  motor_control(0,"B");
  delay(ms_delay);
  motor_standby(true);
}

void motor_control(boolean direction, String Motor){ //speed from 0 to 100

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
