/* ----------------------------------------------------------------------------------------------------
   Chamarande, 2017
   Update: 01/10/17

   V1
   Written by Bastien DIDIER
   more info :

   ----------------------------------------------------------------------------------------------------
*/

#include <Wire.h>
#include <Adafruit_ADS1015.h>

//Adafruit ADS1115
Adafruit_ADS1115 ads;

//TB6612FNG Dual Motor Driver pololu
#define PWMA  5  // Motor A PWM
#define AIN2  6  // Motor A In 2
#define AIN1  7  // Motor A In 1
#define STBY  8  // Standby
#define BIN1  9  // Motor B In 1
#define BIN2 10  // Motor B In 2
#define PWMB 11  // Motor B PWM

/*******************************/
const int nb_adc = 2;

int16_t differentialValue[nb_adc];

//Average
float data_avg[nb_adc];
int count = 0;

//Min/Max Avg Data
float data_avg_min[nb_adc];
float data_avg_max[nb_adc];

//Process Final Data
float process[nb_adc];
float avg[nb_adc];

/*******************************/

boolean switch_state = false;
int direction_count[4]; //left = 0; right = 1; forward = 3; backward = 4;

/*******************************/

int time_betwen_move = 10; //in s
int data_resolution = 3; //how many captureted data  for process mouvement (minimum 3)

int min_move_duration = 100; //temps minimum de deplacement
int max_move_duration = 1000; //temps maximum de deplacement

/*******************************/
void setup() {

  pinMode(PWMA, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(STBY, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  ads.begin();

  Serial.begin(9600);

  Serial.println("Chamarande Controller: starting...");
  // print help
  Serial.println("Help Commands: ");
  Serial.println();
  Serial.println("    'z' = move the plant forward for 100ms");
  Serial.println("    's' = move the plant backward for 100ms");
  Serial.println("    'q' = move the plant left for 100ms");
  Serial.println("    'd' = move the plant right for 100ms");
  Serial.println();
  Serial.println("Waiting for commands...");
}

void loop() {

  /*** GET DATA  ***/
  for (int i = 0; i < nb_adc; i++) {
    differentialValue[i] = i == 0 ? ads.readADC_Differential_0_1() : ads.readADC_Differential_2_3();
    data_avg[i] += differentialValue[i];
    process_min_max_avg_data(i);

    Serial.println("Differential Value :"+String(ads.readADC_Differential_0_1()));
  }
  count++;

  /*** PROCESS DATA  ***/
  if (count > data_resolution) {

    //Choose between left_right or forward_backward
    int choose = random(0, 2);
    Serial.println(choose);
    
    if (choose < 1) {

      Serial.println("Choose = Left_Right");
      
      /*** PROCESS DATA  ***/
      /*for(int i=0; i < nb_adc; i++){
        process[i] = (data_avg_min[i] - data_avg_max[i]) / 2;
        avg[i] = data_avg[i] / count;
        }*/

      /*for(int j=0; j< nb_adc; j++){
        avg[j] = map(avg[j], data_avg_min[i], process[i], min_move_duration, max_move_duration);
        right(avg_lft_rgh);
        direction_count[i]++;
        }*/

      /*** PROCESS LEFT OR RIGTH  ***/
      float process_left_right = (data_avg_min[0] - data_avg_max[0]) / 2;
      float avg_lft_rgh = data_avg[0] / count;
            
      Serial.print(avg_lft_rgh);
      Serial.print("/");
      Serial.println(process_left_right);
      
      if (avg_lft_rgh < process_left_right) {
        Serial.println("Left");
        if (switch_state == false) {
          avg_lft_rgh = map(avg_lft_rgh, data_avg_min[0], process_left_right, min_move_duration, max_move_duration);
          right(avg_lft_rgh);
          direction_count[1]++;
        } else {
          avg_lft_rgh = map(avg_lft_rgh, process_left_right, data_avg_max[0], min_move_duration, max_move_duration);
          left(avg_lft_rgh);
          direction_count[0]++;
        }
      } else if (avg_lft_rgh > process_left_right) {
        Serial.println("Right");
        if (switch_state == false) {
          avg_lft_rgh = map(avg_lft_rgh, process_left_right, data_avg_max[0], min_move_duration, max_move_duration);
          left(avg_lft_rgh);
          direction_count[0]++;
        } else {
          avg_lft_rgh = map(avg_lft_rgh, data_avg_min[0], process_left_right, min_move_duration, max_move_duration);
          right(avg_lft_rgh);
          direction_count[1]++;
        }
      } else {
        float delay_estimation = random(min_move_duration, max_move_duration);
        if (switch_state == false) {  
          left(delay_estimation);
          direction_count[0]++;
        } else {
          right(delay_estimation);
          direction_count[1]++;
        }
      }
    } else { //choose > 1
      
      Serial.println("Choose = Avant_Arrière");
      
      /*** PROCESS FORWARD OR BACKWARD  ***/
      float process_forward_backward = (data_avg_min[1] - data_avg_max[1]) / 2;
      float avg_fwd_bwd = data_avg[1] / count;

      Serial.print(avg_fwd_bwd);
      Serial.print("/");
      Serial.println(process_forward_backward);
      
      if (avg_fwd_bwd < process_forward_backward) {
        Serial.println("Avant");
        if (switch_state == false) {
          avg_fwd_bwd = map(avg_fwd_bwd, data_avg_min[1], process_forward_backward, min_move_duration, max_move_duration);
          forward(avg_fwd_bwd);
          direction_count[2]++;
        } else {
          avg_fwd_bwd = map(avg_fwd_bwd, process_forward_backward, data_avg_max[1], min_move_duration, max_move_duration);
          backward(avg_fwd_bwd);
          direction_count[3]++;
        }
      } else if (avg_fwd_bwd > process_forward_backward) {
        Serial.println("Arrière");
        if (switch_state == false) {
          avg_fwd_bwd = map(avg_fwd_bwd, process_forward_backward, data_avg_max[1], min_move_duration, max_move_duration);
          backward(avg_fwd_bwd);
          direction_count[3]++;
        } else {
          avg_fwd_bwd = map(avg_fwd_bwd, data_avg_min[1], process_forward_backward, min_move_duration, max_move_duration);
          forward(avg_fwd_bwd);
          direction_count[2]++;
        }
      } else {
        float delay_estimation = random(min_move_duration, max_move_duration);
        if (switch_state == false) {  
          forward(delay_estimation);
          direction_count[3]++;
        } else {
          backward(delay_estimation);
          direction_count[2]++;
        }  
      }
    }

    Serial.println("direction :");
    for (int i = 0; i < 4; i++) {
      if (direction_count[i] > 3) { //define nb of same move
        if (switch_state == false) {
          switch_state = true;
        } else {
          switch_state = false;
        }
      } else {
        Serial.println(direction_count[i]);
      }
    }

    //Then Reset Avg Data
    Serial.println("Reset Data!");
    reset_data();
  }

  delay((time_betwen_move / data_resolution) * 1000);
}

void serialEvent() {
  while (Serial.available()) {
    char inChar = Serial.read();
    Serial.print("Received :");
    Serial.println(inChar);

    if (inChar == 'z') {
      forward(100);
    } else if (inChar == 's') {
      backward(100);
    } else if (inChar == 'q') {
      left(100);
    } else if (inChar == 'd') {
      right(100);
    } else if (inChar == 'r') {
      reset_data();
    } else {
      Serial.println("Unknow Command");
    }
  }
}

void process_min_max_avg_data(int i) {
  if (data_avg_min[i] > data_avg[i]) {
    data_avg_min[i] = data_avg[i];
  }
  if (data_avg_max[i] < data_avg[i]) {
    data_avg_max[i] = data_avg[i];
  }
}

void reset_data() {
  for (int i = 0; i < nb_adc; i++) {
    data_avg[i]     = 0;
    data_avg_min[i] = 0;
    data_avg_max[i] = 0;
    count           = 0;
  }
}
