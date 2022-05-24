//Mohammad MUTTAQI - 17050241030
//Summer project-1
//Smart door

#include <FaBoLCD_PCF8574.h>
#include <Wire.h>
#include <MFRC522.h>
#include <Servo.h>
#include <SPI.h>

int RST_PIN = 9;                          //defining the reset pin for the RC522 module.
int SS_PIN = 10;                          //defining the chip select pin for the RC522 module.
int servoPin = 8;                         //defining the Servo motor pin.
int leds[]={2,3,4,5};                     //our LEDs.
int exit_buttonPIN = 6;                   //the exit button.

FaBoLCD_PCF8574 lcd;
Servo motor;                              //Creating a variable fot the Servo motor.
MFRC522 rfid(SS_PIN, RST_PIN);            //setting up the RC522 module.

//Defining the ID number of the authorized cards.
byte ID1[4] = {244, 110, 154, 42};           
byte ID2[4] = {192, 174, 198, 50};         
byte ID3[4] = {233, 229, 102, 194};        
byte ID4[4] = {217, 190, 211, 162};        

int max_p = 3;                            // maximum number of people allowed
int people = 0;                           // number of people inside
int exit_buttonState = 0;                 // current state of the exit button
int exit_lastButtonState = 0;             // previous state of the exit button
 
void setup() {
  
  lcd.begin(16,2);
  lcd.setCursor(0,0);
  lcd.print("max sayi = ");
  lcd.print(max_p);
  lcd.setCursor(0,1);      
  lcd.print("mevcut sayi= ");
  lcd.print(people);
  delay(500);
  
  motor.attach(servoPin);                 
  Serial.begin(9600);                     
  SPI.begin();                            
  rfid.PCD_Init();                        

  pinMode (exit_buttonPIN, INPUT_PULLUP);
  for (int i=0; i<4; i++){
    pinMode(leds[i], OUTPUT);
    }
}

void loop() {

  checkDown();

  if (people>= max_p){
    digitalWrite (5, LOW);
    digitalWrite (4, HIGH); 
  }
  else {
    digitalWrite (4, LOW);
    digitalWrite(5, HIGH);
  }
  
  if ( ! rfid.PICC_IsNewCardPresent())    // waiting for the new card to be scanned.
    return;

  if ( ! rfid.PICC_ReadCardSerial())      // waiting if the card is not scanned.
    return;

    
    if ((rfid.uid.uidByte[0] == ID1[0] &&     // comparing the scanned card with the ID1 variable.
    rfid.uid.uidByte[1] == ID1[1] && 
    rfid.uid.uidByte[2] == ID1[2] && 
    rfid.uid.uidByte[3] == ID1[3])|| 
    (rfid.uid.uidByte[0] == ID2[0] &&         // comparing the scanned card with the ID2 variable.
    rfid.uid.uidByte[1] == ID2[1] && 
    rfid.uid.uidByte[2] == ID2[2] && 
    rfid.uid.uidByte[3] == ID2[3]) || 
    (rfid.uid.uidByte[0] == ID3[0] &&        // comparing the scanned card with the ID3 variable.
    rfid.uid.uidByte[1] == ID3[1] && 
    rfid.uid.uidByte[2] == ID3[2] && 
    rfid.uid.uidByte[3] == ID3[3]) || 
    (rfid.uid.uidByte[0] == ID4[0] &&       // comparing the scanned card with the ID4 variable.
    rfid.uid.uidByte[1] == ID4[1] && 
    rfid.uid.uidByte[2] == ID4[2] && 
    rfid.uid.uidByte[3] == ID4[3])) {
        
        if (people < max_p){
        digitalWrite(5, LOW);
        digitalWrite(3, HIGH);
        Serial.println("Door has opened");
        print_on_screen();
        motor.write(180);                 
        delay(2000);
        digitalWrite(3, LOW);
        motor.write(0);                   
        delay(1000);

        people++;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("max sayi = ");
        lcd.print(max_p);

        lcd.setCursor(0,1);
        lcd.print("mevcut sayi=");
        lcd.print(people);
        }
        else {                          // works in case we reach the maximum number.
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("-max sayi dolu-");

        lcd.setCursor(0,1);
        lcd.print("max sayi = ");
        lcd.print(max_p);
        }
          }
    else{                                 // works in case of an unauthorized attempt.
      Serial.println("Unauthorized Card");
      print_on_screen();
      digitalWrite(5, LOW);
      digitalWrite(2, HIGH);
      delay(2000);
      digitalWrite(2, LOW);
    }

  
  rfid.PICC_HaltA();
}

void print_on_screen()
{
  Serial.print("ID number: ");
  for(int j = 0; j < 4; j++){
    Serial.print(rfid.uid.uidByte[j]);
    Serial.print(" ");
  }
  Serial.println("");
}


void checkDown(){

  exit_buttonState = digitalRead(exit_buttonPIN);

  if (exit_buttonState != exit_lastButtonState){
    if (exit_buttonState == LOW){
        
        people --;
        if (people < 0){
          people = 0;
        }
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("max sayi = ");
        lcd.print(max_p);

        lcd.setCursor(0,1);
        lcd.print("mevcut sayi=");
        lcd.print(people);
    
    }
    delay(50);
  }
  exit_lastButtonState = exit_buttonState;
}
