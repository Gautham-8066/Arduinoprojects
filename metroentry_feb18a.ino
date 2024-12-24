


#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
#define BUZZER 5
Servo myservo;
int pos = 0; 
 
MFRC522 mfrc522(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key; 
const int maxCards = 10;
byte allowedCards[maxCards][7]; 
int numAllowedCards = 0;

void setup() {

  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(BUZZER, OUTPUT); 
  myservo.attach(8);
  Serial.begin(9600);         
    while (!Serial);           
    SPI.begin();               
    mfrc522.PCD_Init();         
} 
boolean compareUID(byte uid1[], byte uid2[], int uidLength) {
  
  for (int i = 0; i < uidLength; i++) {
    if (uid1[i] != uid2[i]) {
      return false;
    }
  }
  return true;
}

boolean isCardAllowed(byte uid[], int uidLength) {
 
  for (int i = 0; i < numAllowedCards; i++) {
    if (compareUID(uid, allowedCards[i], uidLength)) {
      return true;
    }
  }
  return false;
}
void addAllowedCard(byte uid[], int uidLength) {
 
  if (numAllowedCards < maxCards) {
    memcpy(allowedCards[numAllowedCards], uid, uidLength);
    numAllowedCards++;
  } else {
    Serial.println("Cannot add more cards. Maximum limit reached.");
  }
}
    



void loop() {
  digitalWrite(7, HIGH); 
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    Serial.println("Card detected!");

    // Check if the card is allowed
    if (!isCardAllowed(mfrc522.uid.uidByte, mfrc522.uid.size)) {
      Serial.println("Access granted!");
       digitalWrite(BUZZER, HIGH); 
            delay(100);
            digitalWrite(BUZZER, LOW);  
            delay(100);
            digitalWrite(7, LOW);   
            delay(100);
            digitalWrite(6, HIGH); 
            for (pos = 90; pos >= 0; pos -= 5) {
             
              myservo.write(pos);              
            }
            delay(3000);  
            for (pos = 0; pos <= 90; pos += 5) { 
              myservo.write(pos);              
              delay(15);                       
            digitalWrite(6, LOW);   
            delay(100);
            digitalWrite(7, HIGH);
            
      addAllowedCard(mfrc522.uid.uidByte, mfrc522.uid.size);  // Add the card to the allowed list
    } 
    }
    
    else {
      Serial.println("Access denied! Card has already been used.");
    }

    
    delay(1000);
  }
 
                   
}
