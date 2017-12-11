#include <Wire.h>
#include <Adafruit_ADS1015.h>

 Adafruit_ADS1115 ads;  /* Use this for the 16-bit version */
//Adafruit_ADS1015 ads;     /* Use thi for the 12-bit version */

int16_t lft_rgt,fwd_bwd;
/*float[] differentialValue = new float[2];

//Min/Max
float[] data_min = new float[2];
float[] data_max = new float[2];

//Average
float[] data_avg = new float[2];
int count = 0;*/

void setup(){
  
  Serial.begin(9600);
  Serial.println("Hello!");
  
  Serial.println("Getting differential reading from AIN0 (P) and AIN1 (N)");
  Serial.println("ADC Range: +/- 6.144V (1 bit = 3mV/ADS1015, 0.1875mV/ADS1115)");
  
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

  lft_rgt = ads.readADC_Differential_0_1();  
  fwd_bwd = ads.readADC_Differential_2_3();
  
  //Serial.print("Differential: "); Serial.print(results); Serial.print("("); Serial.print(results * multiplier); Serial.println("mV)");
  //Serial.println(lft_rgt+","+fwd_bwd);
  
  Serial.print(lft_rgt);
  Serial.print(",");
  Serial.print(fwd_bwd);
  Serial.println();
  
  delay(10);
}
