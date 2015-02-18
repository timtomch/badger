/*
 * Timtom BADGEr code v1
 * Version 1.0 - Feb 17, 2015
 *
 * Displays name tag images on the BADGEr
 * Button 1 (mode) switches between badges
 * Button 2 (up) displays contact information
 */

#include <SPI.h>
#include <SD.h>
#include "EPD.h"
#include "S5813A.h"
#include "EReader.h"
#include <EEPROM.h>

const int BADGEDISPLAYED_ADDR = 0;   // EEPROM address of byte where currently displayed badge # is stored    
const int DISPLAYMODE_ADDR = 1;      // EEPROM address of the byte where current display mode is stored

short int displaymode = 0;                 // Modes: 0: badge display   1: contact display
short int badge = 0;                       // Which badge to display

char badgepaths[][50] = {"/BADGES/OCLS_01.WIF", "/BADGES/SWC_01.WIF","/BADGES/HELLO_01.WIF"};
int nrofbadges = 3;
char contact[] = "/BADGES/CONTACT.WIF";

// I/O setup
void setup() {
  // Pin addresses for those constants are defined in EReader.h
  pinMode(UP_PIN, INPUT);
  pinMode(DOWN_PIN, INPUT);
  pinMode(SEL_PIN, INPUT);
  pinMode(MODE_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  ereader.setup(EPD_2_7);
  digitalWrite(LED_PIN, LOW);
  
  // Load values stored in EEPROM:
  badge = EEPROM.read(BADGEDISPLAYED_ADDR);
  displaymode = EEPROM.read(DISPLAYMODE_ADDR);
}

// main loop

void loop() {
  
  if(analogRead(MODE_PIN) > 512){
    // The MODE button is connected to an analog input
    // If this button is pressed    
    Serial.println("Display badge");
    //Serial.print("Current EEPROM value: ");
    //Serial.print(badge, DEC);
    //Serial.println();
    Serial.println(badge);
    Serial.println(displaymode);
    
    if (displaymode == 0){
      if (badge < nrofbadges-1){
        badge++;
      }
      else{
        badge = 0;
      }
    }
    else {
      displaymode = 0;
      EEPROM.write(DISPLAYMODE_ADDR, displaymode);
    }
    
    //Serial.print("New EEPROM value: ");
    //Serial.print(badge, DEC);
    //Serial.println();
    Serial.println(badgepaths[badge]);
    
    digitalWrite(LED_PIN, HIGH);
    EEPROM.write(BADGEDISPLAYED_ADDR, badge);
    ereader.display_wif(badgepaths[badge], 0, 0);
    ereader.show();
    ereader.sleep(4000); // allows EPD to power off gracefully
    digitalWrite(LED_PIN, LOW);
  }
  if(digitalRead(UP_PIN)){
    Serial.println("Display contact information");
    
    if (displaymode != 1){
      displaymode = 1;
      Serial.println(contact);
      digitalWrite(LED_PIN, HIGH);
      EEPROM.write(DISPLAYMODE_ADDR, displaymode);
      ereader.display_wif(contact, 0, 0);
      ereader.show();
      ereader.sleep(4000); // allows EPD to power off gracefully
      digitalWrite(LED_PIN, LOW);
    }
    else{
      delay(200); // Leave time for finger to leave button before rerunning loop
    }
  }
}
