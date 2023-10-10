/*IOabstration propose de nombreuses applications --> dont cet exemple qui permet
d'utiliser une mémoire EEPROM
L'utilisation des différentes fonctions nécéssaires pour vérifier si l'EEPROM fonctionne y sont*/
/* On peut voir l'adresse des différents registres lecteure /écriture sur la datasheet*/ 
/*Une version "maison" sera disponible sur le gitHub prochainement*/

#include <Arduino.h>
#include <Wire.h>

#include <EepromAbstractionWire.h>
#include <TaskManagerIO.h>

const unsigned int romStart = 800;

I2cAt24Eeprom anEeprom(0x50, PAGESIZE_AT24C128);

const char strData[100] = { "On place ici un string --> dont l'objectif est de montrer qu'on peut tout autant écrire des array"};

void setup() {
  Serial.begin(115200);
  while(!Serial);
  delay(1000);

  Wire.begin();

  Serial.println("EEPROM example starting");

  // clear the ROM first.. flush --> write '0' on evryline from rom start to reomstart +100
  for(int i=romStart;i<(romStart+100);i++) anEeprom.write8(i, 0);
  Serial.println(anEeprom.hasErrorOccurred() ? "Write failure" : "Write success");
  Serial.write('\n');
  Serial.write('\n');
  

  // now write the values to the rom. 8, 16 and 32 bit
  Serial.println("Ecriture différents types de données");
  anEeprom.write8(romStart, (byte)42);
  anEeprom.write16(romStart + 1, 0xface);
  anEeprom.write32(romStart + 3, 0xf00dface);
  Serial.println(anEeprom.hasErrorOccurred() ? "Write failure" : "Write success");
  Serial.write('\n');
  Serial.write('\n');

  //write an array to the rom.
  Serial.println("Ecriture array");
  anEeprom.writeArrayToRom(romStart + 30, (const unsigned char*)strData, sizeof strData);

  Serial.println(anEeprom.hasErrorOccurred() ? "Write failure" : "Write success");
  Serial.write('\n');
  Serial.write('\n');

  Serial.println("Ecriture du nom");

  /*************************************************************/
  anEeprom.write8(romStart+14,0x4a);
  anEeprom.write8(romStart+15,0x75);
  anEeprom.write8(romStart+16,0x61);
  anEeprom.write8(romStart+17,0x6e);
  Serial.println(anEeprom.hasErrorOccurred() ? "Write failure" : "Write success");
  Serial.write('\n');
  Serial.write('\n');

  Serial.println("Fin de phase d'ecriture");
  Serial.write('\n');
  Serial.write('\n');

}

void loop() {

  Serial.println("Debut de lecture");
  Serial.write('\n');
  Serial.write('\n');

  Serial.print("Reading back byte: ");
  Serial.println(anEeprom.read8(romStart));

  Serial.print("Reading back word: 0x");
  Serial.println(anEeprom.read16(romStart + 1), HEX);

  Serial.print("Reading back long: 0x");
  Serial.println(anEeprom.read32(romStart + 3), HEX);

  // finally we'll do hard comparisons against the array, as it's hard to check by hand.
  char readBuffer[100];
  anEeprom.readIntoMemArray((unsigned char*)readBuffer, romStart + 30, sizeof readBuffer);
  Serial.print("Rom Array: ");
  Serial.println(readBuffer);
  Serial.print("String is same: ");
  Serial.println(strcmp(readBuffer, strData)==0 ? "YES":"NO");
  Serial.write('\n');
  Serial.write('\n');

/*******************************************************************************/

  Serial.print("Fin du test par : ");
  Serial.write(anEeprom.read8(romStart+14));
  Serial.write(anEeprom.read8(romStart+15));
  Serial.write(anEeprom.read8(romStart+16));
  Serial.write(anEeprom.read8(romStart+17));
  Serial.write('\n');

  Serial.println(anEeprom.hasErrorOccurred() ? "Read error" : "Successfully");
  Serial.write('\n');
  Serial.write('\n');
  delay(10000);
}