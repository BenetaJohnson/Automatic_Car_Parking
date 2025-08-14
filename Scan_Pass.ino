#include <Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal.h>
#define sensor_pin A0 
#define RST_PIN         9          // Reset pin (not used!)
#define SS_PIN          10         // Slave select pin
#define MAX_CARDS       4          // Maximum number of cards to track
float adcValue=0, val=0, mgL=0, amount=0;

//LiquidCrystal lcd(12,11,7,4,3,2);
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance

struct Card {
  byte uid[10];
  unsigned long firstScanTime;
  unsigned long secondScanTime;
  bool firstScanDone;
};

Card cards[MAX_CARDS];

Servo servo1;
Servo servo2;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  servo1.attach(6);
  servo2.attach(5);
  pinMode(sensor_pin, INPUT);
  // lcd.begin(16,2);
  // lcd.clear();
  // lcd.setCursor(0,0);
  // lcd.print("Scan RFID...");
  mfrc522.PCD_Init();
  Serial.println("Scan RFID tags...");
}

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent() || ! mfrc522.PICC_ReadCardSerial() ) {
    return;
  }

  // Check if the card is already being tracked
  int cardIndex = -1;
  for (int i = 0; i < MAX_CARDS; i++) {
    if (memcmp(cards[i].uid, mfrc522.uid.uidByte, mfrc522.uid.size) == 0) {
      cardIndex = i;
      break;
    }
  }

  // If not tracked, find an empty slot
  if (cardIndex == -1) {
    for (int i = 0; i < MAX_CARDS; i++) {
      if (cards[i].uid[0] == 0) {
        cardIndex = i;
        memcpy(cards[i].uid, mfrc522.uid.uidByte, mfrc522.uid.size);
        break;
      }
    }
  }

  if (cardIndex != -1) {
    // If first scan is not done, record the time
    // lcd.clear();
    // lcd.setCursor(0,0);
    // lcd.print("Scan RFID...");
    if (!cards[cardIndex].firstScanDone) {
      cards[cardIndex].firstScanTime = millis();
      Serial.println("First scan recorded.");
      cards[cardIndex].firstScanDone = true;
      servo1.write(90);
      // lcd.clear();
      // lcd.setCursor(0,0);
      // lcd.print("Entry");
      // lcd.setCursor(0,1);
      // lcd.print("Scanned");
      delay(4000);
      servo1.write(180);
    } else {
      adcValue=0;
      for(int i=0;i<10;i++){
        adcValue+= analogRead(sensor_pin);
        delay(10);
      }
      val = (adcValue/10) * (5.0/1024.0);
      mgL = 0.67 * val;
      Serial.println(mgL);
      if (mgL<0.8){
        cards[cardIndex].secondScanTime = millis();
        Serial.println("Second scan recorded.");
        amount = ((cards[cardIndex].secondScanTime - cards[cardIndex].firstScanTime) / 1000);
        // lcd.clear();
        // lcd.setCursor(0,0);
        // lcd.print("Can Exit");
        // lcd.setCursor(0,1);
        // lcd.print(String("Amount: ")+ String(amount));
        servo2.write(90);
        delay(4000);
        servo2.write(0);
        // Calculate and print the time difference
        Serial.print("Time between scans for card ");
        for (int i = 0; i < mfrc522.uid.size; i++) {
          Serial.print(cards[cardIndex].uid[i] < 0x10 ? " 0" : " ");
          Serial.print(cards[cardIndex].uid[i], HEX);
        }
        Serial.print(": ");
        Serial.print(cards[cardIndex].secondScanTime - cards[cardIndex].firstScanTime);
        Serial.println(" ms");

        // Reset variables for next scan
        memset(cards[cardIndex].uid, 0, sizeof(cards[cardIndex].uid));
        cards[cardIndex].firstScanTime = 0;
        cards[cardIndex].secondScanTime = 0;
        cards[cardIndex].firstScanDone = false;
      }
      else {
        Serial.print("Alcohol Detected");
        // lcd.clear();
        // lcd.setCursor(0,0);
        // lcd.print("Alcohol Detected");
        // lcd.setCursor(0,1);
        // lcd.print("Cannot Exit");
        delay(4000);
      }
    }
  }

  delay(1000); // Delay to avoid reading the same card multiple times
}
