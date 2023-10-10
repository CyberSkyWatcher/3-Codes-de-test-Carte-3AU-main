/* Code de test pour la ldr et le relais*/
/* Simple mise à 1 sous un certain seuil de luminosité*/

/* La ldr étant une réistance photo-résistance cela veut dire
que en fonction de la luminosité sa réistances diminue
C'est donc ici une simple lecture analogique*/
#include <Arduino.h>

#define LDR 14
#define switch 33

int seuil = 500;  // Seuil de "luminoisté" --> valeur analogique
int value;        // Variable destinée au stockage du  retour de la lecture analogique
 
void setup() {
  Serial.begin(9600);   // démarage de la comunication série
  pinMode(switch, OUTPUT);  
  pinMode(LDR, INPUT);
}
 
void loop() {

  value = analogRead(LDR); // Lecture analogique
/* On fait un print ici pour constater que la LDR fonctionne*/
/* Cette valeur n'a aucun sens direct, c'est une lecture analogique destinée à  être interpretée*/
  Serial.println(value);

/* En fct du  seuil --> relais switch ON/OFF*/
  if (value<seuil) digitalWrite(switch, HIGH);

  if (value>seuil) digitalWrite(switch, LOW);

  delay(200); // Un délai pour éviter d'avoir un effet "mitraillette"
}