/*
 * ADS1115 16-bit version
 * 
 * Getting differential reading from AIN0 (P) and AIN1 (N) and AIN2 (P) and AIN3 (N)
 * ADC Range: +/- 6.144V (1 bit = 0.1875mV/ADS1115)
 * 
 */

#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;

//int16_t lft_rgt,fwd_bwd;
int16_t differentialValue[2];

//Min/Max
float data_min[2];
float data_max[2];

//Average
float data_avg[2];
int count = 0;

void setup(){  
  Serial.begin(9600);

  // The ADC input range (or gain) can be changed via the following
  // functions, but be careful never to exceed VDD +0.3V max, or to
  // exceed the upper and lower limits if you adjust the input range!
  // Setting these values incorrectly may destroy your ADC!
  //                                                                ADS1015  ADS1115
  //                                                                -------  -------
  // ads.setGain(GAIN_TWOTHIRDS);  // 2/3x gain +/- 6.144V  1 bit = 3mV      0.1875mV (default)
  // ads.setGain(GAIN_ONE);        // 1x gain   +/- 4.096V  1 bit = 2mV      0.125mV
  // ads.setGain(GAIN_TWO);        // 2x gain   +/- 2.048V  1 bit = 1mV      0.0625mV
  // ads.setGain(GAIN_FOUR);       // 4x gain   +/- 1.024V  1 bit = 0.5mV    0.03125mV
  // ads.setGain(GAIN_EIGHT);      // 8x gain   +/- 0.512V  1 bit = 0.25mV   0.015625mV
  // ads.setGain(GAIN_SIXTEEN);    // 16x gain  +/- 0.256V  1 bit = 0.125mV  0.0078125mV
  
  ads.begin();
}

void loop(){
  
  /* Be sure to update this value based on the IC and the gain settings! */
  //float   multiplier = 3.0F;    /* ADS1015 @ +/- 6.144V gain (12-bit results) */
  //float multiplier = 0.1875F; /* ADS1115  @ +/- 6.144V gain (16-bit results) */

  for (int i = 0; i < 2; i++) {

    differentialValue[i] = i==0 ? ads.readADC_Differential_0_1() : ads.readADC_Differential_2_3();
    
    if(data_min[i] > differentialValue[i]){
      data_min[i] = differentialValue[i];
    } 
    if(data_max[i] < differentialValue[i]){
      data_max[i] = differentialValue[i];
    }
        
    data_avg[i] += differentialValue[i];
    count++;
        
    //Serial.println("#"+String(i)+" Average value :"+data_avg[i]/count);
        
    differentialValue[i] = map(differentialValue[i],data_min[i],data_max[i],-10,10);
  }
      
  //Serial.print(results * multiplier); Serial.println("mV)");
  
  Serial.print(differentialValue[0]);
  Serial.print(",");
  Serial.print(differentialValue[1]);
  Serial.println();
  
  delay(1000);
}

void reset_data(int i){
  data_min[i] = 0;
  data_max[i] = 0;
  data_avg[i] = 0;
  count       = 0;
}
