//
// Created by Jamie Briggs on 10/07/2025.
//

#include <Arduino.h>

#define BUZZER_PIN 18
#define BUTTON_PIN 23

int buttonState = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    pinMode(BUTTON_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
    // put your main code here, to run repeatedly:
    unsigned char i;
    buttonState = digitalRead(BUTTON_PIN);

    // Check if button is pressed
    if(buttonState == HIGH){
        for(i = 0; i < 50; i++){
            digitalWrite(BUZZER_PIN, HIGH);
            delay(3);// Wait 3ms
            digitalWrite(BUZZER_PIN, LOW);
            delay(3);
        }
        for(i = 0; i < 80; i++){
            digitalWrite(BUZZER_PIN, HIGH);
            delay(5);// Wait 5ms
            digitalWrite(BUZZER_PIN, LOW);
            delay(5);
        }
    }
}