#include <CapacitiveSensor.h>

// Résistance de 10 mega ohms entre la PIN 4&2 
// Connecter la surface sensible type papier aluminium, cuivre, textile, à la PIN2.
CapacitiveSensor left = CapacitiveSensor(4,2);
CapacitiveSensor right = CapacitiveSensor(10,8);

// output
int output = 13;

void setup(){
  pinMode(output, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  
  long left_resultat = left.capacitiveSensor(60);
  long right_resultat = right.capacitiveSensor(60);
  
  // Dans la console 60 = Proximité; 120 = Contact; 0 = Rien;
 
  Serial.println(left_resultat - right_resultat);
  //Serial.print("\t");

  /*if(resultat > 1){digitalWrite(output, HIGH);}
  else{digitalWrite(output, LOW);}*/
  
  delay(100);
}
