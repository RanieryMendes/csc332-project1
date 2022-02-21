

#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define SS_PIN    10
#define RST_PIN   9
#define SERVO_PIN A5

MFRC522 rfid(SS_PIN, RST_PIN);
Servo servo;

byte authorizedUID[4] = {0x23, 0x4C, 0x52, 0x9B};
int angle = 0; // the current angle of servo motor

int ledRedPin = 7; 
int ledGreenPin = 6; 

void setup() {
 // Serial.begin(9600);  use it only when connected directly to the PC
  SPI.begin(); // init SPI bus
  rfid.PCD_Init(); // init MFRC522
  servo.attach(SERVO_PIN);
  servo.write(angle); // rotate servo motor to 0°
  digitalWrite(ledRedPin, HIGH);
  //Serial.println("Tap RFID/NFC Tag on reader"); use it only when connected directly to the PC
}

void loop() {
 
  
  
  if (rfid.PICC_IsNewCardPresent()) { // new tag is available
    if (rfid.PICC_ReadCardSerial()) { // NUID has been readed
      MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
      Serial.println("My tag is running");

      if (rfid.uid.uidByte[0] == authorizedUID[0] &&
          rfid.uid.uidByte[1] == authorizedUID[1] &&
          rfid.uid.uidByte[2] == authorizedUID[2] &&
          rfid.uid.uidByte[3] == authorizedUID[3] ) {
        Serial.println("Authorized Tag");

        //LOCKs the box so red LED is on and the green one is off
        // change angle of servo motor 
        if (angle == 0) 
        {  angle = 90;
          digitalWrite(ledRedPin, HIGH);
          digitalWrite(ledGreenPin, LOW);
          }

          //UNLOCK the box, so red light turns off and the green light turns on
        else {//if(angle == 90){
          angle = 0;
          digitalWrite(ledRedPin, LOW);
          digitalWrite(ledGreenPin, HIGH);
          }

        // control servo motor arccoding to the angle, that is, unlock or lock the box
        servo.write(angle);
       
      } else {
        /*
         * use this block only when connected directly to the PC, otherwise it's not needed once we don't have a display that could inform the user about an unauthorized tag
         * 
        Serial.print("Unauthorized Tag with UID:");  
        for (int i = 0; i < rfid.uid.size; i++) {
          Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
          Serial.print(rfid.uid.uidByte[i], HEX);
        }
        Serial.println();*/
      }

     rfid.PICC_HaltA(); // halt PICC
     delay(300);
     
    }
  }
}
