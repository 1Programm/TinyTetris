#ifndef TINYTETRIS_DPAD_H
#define TINYTETRIS_DPAD_H

#include <Arduino.h>

#define KEYPAD_PIN  A0

#define LEFT_MIN 0
#define LEFT_MAX 10
#define RIGHT_MIN 500
#define RIGHT_MAX 510
#define UP_MIN 140
#define UP_MAX 150
#define DOWN_MIN 330
#define DOWN_MAX 340
#define ROTATE_MIN 740
#define ROTATE_MAX 750

namespace dpad {

    unsigned int dpad_state;

    void printState(){
        int state = analogRead(KEYPAD_PIN);
        Serial.print("DPAD-State: ");
        Serial.println(state);
    }

    void setState(){
        dpad_state = analogRead(KEYPAD_PIN);
    }

    bool getState(int min, int max){
        return dpad_state >= min && dpad_state <= max;
    }

    bool isLeft(){
        return getState(LEFT_MIN, LEFT_MAX);
    }

    bool isRight(){
        return getState(RIGHT_MIN, RIGHT_MAX);
    }

    bool isUp(){
        return getState(UP_MIN, UP_MAX);
    }

    bool isDown(){
        return getState(DOWN_MIN, DOWN_MAX);
    }

    bool isRotate(){
        return getState(ROTATE_MIN, ROTATE_MAX);
    }

}

#endif
