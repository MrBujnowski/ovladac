#include <Arduino.h>
#define IR_USE_AVR_TIMER 1
#include <IRremote.hpp>
#define ovladac 2
#define SAVE 4127850240
#define RED 3091726080  //Odesílácí tlačítko pro červenou ledku
#define BLUE 3208707840  //Odesílácí tlačítko pro modrou ledku
#define GREEN 3158572800  //Odesílácí tlačítko pro zelenou ledku
#define GND 8
#define bluepin 9 //Port na kterém je modrá ledka
#define redpin 6 //Port na kterém je červená ledka
#define greenpin 5 //Port na kterém je zelená ledka

int ulozenaLedka = 0;
int bar[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; //Ulozené kody pro tlačítka na ovladači
unsigned long codes[] = {
    3910598400,  // 0
    4077715200,  // 1
    3877175040,  // 2
    2707357440,  // 3
    4144561920,  // 4
    3810328320,  // 5
    2774204160,  // 6
    3175284480,  // 7
    2907897600,  // 8
    3041591040   // 9
};
int rgb[] = {9, 5, 6};
String stringNum = "";

void setup() {
  for (int i = 0; i < 3; i++) {
    pinMode(rgb[i], OUTPUT);
  }
  IrReceiver.begin(ovladac, ENABLE_LED_FEEDBACK); // Zapnutí přijímače
  Serial.begin(9600);
}

void handleColorChange(int pin, String colorName) {
    int intNum = stringNum.toInt();
    stringNum = "";
    if (intNum >= 0 && intNum <= 255) {
      Serial.print("Zadaná hodnota ");
      Serial.print(colorName);
      Serial.print(" ledky: ");
      Serial.println(intNum);
      analogWrite(pin, intNum);
    } else {
      Serial.println("Zadane cislo neni v intervalu 0 - 255.");
    }
  
}

void loop() {
  if (IrReceiver.decode()) {
    // Přicházející IR code
    unsigned long receivedValue = IrReceiver.decodedIRData.decodedRawData;
    //Serial.println(receivedValue);

    // Zkontroluje jestli přicházející kod je stejný jako čísla která jsem definoval
    for (int i = 0; i < 10; i++) {
      if (receivedValue == codes[i]) {
        stringNum += String(i);
        Serial.print("Přijaté číslo: ");
        Serial.println(i);
      }
    }

    // testovací tlačítko save, nakonec nefunugje
    /*if (receivedValue == SAVE) {
      ulozenaLedka = stringNum.toInt();
      Serial.print("Uložená hodnota je: ");
      Serial.println(ulozenaLedka);
      stringNum = "";
    }
*/
    
    if (receivedValue == RED) {
      handleColorChange(redpin, "červené");
    }


    if (receivedValue == GREEN) {
      handleColorChange(greenpin, "zelené");
    }

    if (receivedValue == BLUE) {
      handleColorChange(bluepin, "modré");
    }

    IrReceiver.resume();
  }
}


