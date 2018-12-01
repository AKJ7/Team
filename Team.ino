#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         5          // Configurable, see typical pin layout above
#define SS_PIN          15

class dataParser : public MFRC522{
    private:
        int anzahlVonStudenten = 0;
        Uid *gespeicherteKarten = nullptr;
    public:
        Uid testStruct;
        dataParser(byte chipSelectPin, byte resetPowerDownPin) : MFRC522(chipSelectPin, resetPowerDownPin) {};
        bool karteExistiert(Uid kartenInfos);
        bool karteSpeichern(Uid kartenInfos);
        bool structCompare(Uid struct1, Uid struct2);
        void printKartenId(Uid kartenInfos);
        ~dataParser();
};

bool dataParser::structCompare(Uid struct1, Uid struct2){
    int counter;   
    while (counter < 10){
        if (struct1.uidByte[counter] != struct2.uidByte[counter]){
            return false;
        }
        counter++;
    }
    return true;
}



//bool dataParser::structCompare(Uid struct1, Uid struct2){
//    int counter1 = 0;
//    while (counter1 < 10){
//        int counter2 = 0;
//        while (counter2 < 10){
//            Serial.print("Comparing: ");
//            Serial.print(struct1.uidByte[counter1], HEX);
//            Serial.print(" and ");
//            Serial.println(struct2.uidByte[counter1], HEX);
//            if (struct1.uidByte[counter1] != struct2.uidByte[counter2]){
//                return false;
//            }
//            counter2++;
//        }
//      counter1++;
//    }
//    return true;
//}

bool dataParser::karteExistiert(Uid kartenInfos){
    int counter = 0;
    while (counter < anzahlVonStudenten){
        printKartenId(kartenInfos);
        Serial.print("Checking ");
        printKartenId(gespeicherteKarten[counter]);
        if (structCompare(kartenInfos, gespeicherteKarten[counter])){
            return true;    
        }
        counter++; 
    }
    return false;
}


bool dataParser::karteSpeichern(Uid kartenInfos){
    if (!karteExistiert(kartenInfos)){
        gespeicherteKarten = (Uid*)realloc(gespeicherteKarten, (anzahlVonStudenten + 1)*sizeof(Uid));
        gespeicherteKarten[anzahlVonStudenten] = kartenInfos;
        Serial.print("Neu Karte gespeichert. Totale gespeicherte karten: ");
        Serial.println(anzahlVonStudenten + 1);
        printKartenId(kartenInfos);
        anzahlVonStudenten++;
        return true; 
    }
    anzahlVonStudenten--;
    Serial.print("Karte existiert schon! Anzahl von Studenten: ");
    Serial.println(anzahlVonStudenten);
    return false;
}

void dataParser::printKartenId(Uid kartenInfos){
  return;
    Serial.print(kartenInfos.uidByte[0], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[1], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[2], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[3], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[4], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[5], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[6], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[7], HEX);
    Serial.print(" ");
    Serial.print(kartenInfos.uidByte[8]), HEX;
    Serial.print(" ");
    Serial.println(kartenInfos.uidByte[9], HEX);  
}

dataParser::~dataParser(){
    free(gespeicherteKarten);
}

dataParser mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
	Serial.begin(9600);		// Initialize serial communications with the PC
	while (!Serial);		// Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
	SPI.begin();			// Init SPI bus
	mfrc522.PCD_Init();		// Init MFRC522
	mfrc522.PCD_DumpVersionToSerial();	// Show details of PCD - MFRC522 Card Reader details
	Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
	if ( ! mfrc522.PICC_IsNewCardPresent()) {
		return;
	}

	// Select one of the cards
	if ( ! mfrc522.PICC_ReadCardSerial()) {
		return;
	}
  mfrc522.karteSpeichern(mfrc522.uid);
  delay(1000);
	// Dump debug info about the card; PICC_HaltA() is automatically called
	// mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

}
