// simple code qui fait la démonstration de l'utilisation de la fct touchRead()
// Utile pour calibrer la détection
// Plus test isolé des boutons
#include <Arduino.h>

#define bptactile 14
#define bp1 12
#define bp2 36
int seuil = 20;
int valeur,oldval;
void setup() {
  // put your setup code here, to run once:
  oldval=0;
  Serial.begin(9600);
  delay(10);
  Serial.println("Test des boutons");
}

void loop() {
  // put your main code here, to run repeatedly
valeur = touchRead(bptactile);
if(digitalRead(bp1)){
    Serial.print("bp1 activé");
  }
if(digitalRead(bp2)){
    Serial.print("bp2 activé");
  }
if(valeur!=oldval){
  Serial.println(valeur);
  valeur = oldval;
  if (valeur<=seuil){
      Serial.println("bp activé");  
    } 
  }
}