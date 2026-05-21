#include <Wire.h>
#include "MAX30100.h"
#include <LiquidCrystal_I2C.h>

#define INTERVAL_ZI 60000      
#define INTERVAL_NOAPTE 20000   
#define PRAG_LUMINA 500         

const int pinLDR = A0;
const int pinBuzzer = 8;
const int pinButon = 2;

MAX30100 sensor;
LiquidCrystal_I2C lcd(0x27, 16, 2); 

unsigned long timpUltimaVerificare = 0;
unsigned long ultimulUpdateLCD = 0;

void setup() {
    Serial.begin(9600);
    pinMode(pinBuzzer, OUTPUT);
    pinMode(pinButon, INPUT_PULLUP);
    
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Sistem Tirist");

    sensor.begin();

    timpUltimaVerificare = millis();
    delay(2000);
    lcd.clear();
}

void loop() {
    sensor.readSensor(); 

    int lumina = analogRead(pinLDR);
    unsigned long intervalCurent = (lumina > PRAG_LUMINA) ? INTERVAL_ZI : INTERVAL_NOAPTE;

    if (millis() - ultimulUpdateLCD > 1000) {
        lcd.setCursor(0, 0);
        lcd.print("Senzor: ");
        if(sensor.IR > 100) lcd.print("OK    "); 
        else lcd.print("CHECK ");

        lcd.setCursor(0, 1);
        if (lumina > PRAG_LUMINA) lcd.print("ZI (60s)      ");
        else lcd.print("NOAPTE (20s)   ");
        
        ultimulUpdateLCD = millis();
    }

    if (millis() - timpUltimaVerificare > intervalCurent) {
        lcd.clear();
        lcd.print("!!! TREZIREA !!!");
        lcd.setCursor(0, 1);
        lcd.print("APASA BUTONUL");

        while (digitalRead(pinButon) == HIGH) {
            digitalWrite(pinBuzzer, HIGH);
            delay(100);
            digitalWrite(pinBuzzer, LOW);
            delay(100);
        }

        lcd.clear();
        lcd.print("Drum bun!");
        delay(1000);
        timpUltimaVerificare = millis();
    }
}