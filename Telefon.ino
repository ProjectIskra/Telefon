#include "SD.h"
#include "TMRpcm.h"
#include "SPI.h"

#define SD_ChipSelectPin 4
uint8_t pinDIAL  = 5;
uint8_t pinPULSE = 6;
uint8_t cntPULSE;
int newline = true;
String str = "";
unsigned long lastdigit = 0;

#define ROTARY_DIAL_TIMEOUT 3000
TMRpcm tmrpcm;

void setup(){
    pinMode(pinDIAL,  INPUT);
    pinMode(pinPULSE, INPUT);
    digitalWrite(pinDIAL,  HIGH);
    digitalWrite(pinPULSE, HIGH);
    Serial.begin(9600);
    tmrpcm.speakerPin = 9;
    if (!SD.begin(SD_ChipSelectPin)) {
        Serial.println("SD fail");
        return;
    }
    else{
      Serial.println("hi");
    }
    tmrpcm.setVolume(4);
}
void loop (){
    if(!digitalRead(pinDIAL)){
        delay(20);
        cntPULSE=0;
        delay(30);
        while(!digitalRead(pinDIAL)){
            if(digitalRead(pinPULSE)){
//              Фронт импульса:                         //
                delay(5);
                while(digitalRead(pinPULSE)){delay(5);}
//              Спад импульса:                          //
                delay(5);
                cntPULSE++;
            }
        }
        delay(20);
        if(cntPULSE){
            if(cntPULSE>=10){cntPULSE=0;}
            str += cntPULSE;
            lastdigit = millis();
            newline = false;
        }
    }
    if (!newline && millis() - lastdigit > ROTARY_DIAL_TIMEOUT) {
        newline = true;
        String file_name = str + ".wav";
        Serial.println(file_name);
        if (SD.exists(file_name) == 1) {
            tmrpcm.play(file_name.c_str());
        }
        if (SD.exists(file_name) == 0) {
            tmrpcm.play("1.wav");
        }
        str = "";
    }
}