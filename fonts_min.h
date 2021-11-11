//
// Created by Julian Becker on 11.11.21.
//

#ifndef TINYTETRIS_FONTS_MIN_H
#define TINYTETRIS_FONTS_MIN_H

const unsigned char TEXT_TITLE[6][5] = {
        { 0B11111010, 0B00000000, 0B00000000, 0B00000000, 0B00000000 },
        { 0B00100000, 0B00000000, 0B00000000, 0B00000000, 0B00000000 },
        { 0B00100010, 0B11110010, 0B00100000, 0B00000000, 0B00000000 },
        { 0B00100010, 0B10001010, 0B00100000, 0B00000000, 0B00000000 },
        { 0B00100010, 0B10001011, 0B11100000, 0B00000000, 0B00000000 },
        { 0B00100010, 0B10001000, 0B00100000, 0B00000000, 0B00000000 },
};

unsigned char* FONT_A(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B11111000;
    bytes_loc[3] = 0B10001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B10001000;

    return bytes_loc;
}

unsigned char* FONT_B(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B11110000;
    bytes_loc[3] = 0B10001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B11110000;

    return bytes_loc;
}

unsigned char* FONT_C(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10000000;
    bytes_loc[3] = 0B10000000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B01110000;

    return bytes_loc;
}

unsigned char* FONT_D(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10001000;
    bytes_loc[3] = 0B10001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B11110000;

    return bytes_loc;
}

unsigned char* FONT_E(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11111000;
    bytes_loc[1] = 0B10000000;
    bytes_loc[2] = 0B11110000;
    bytes_loc[3] = 0B10000000;
    bytes_loc[4] = 0B10000000;
    bytes_loc[5] = 0B11111000;

    return bytes_loc;
}

unsigned char* FONT_F(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11111000;
    bytes_loc[1] = 0B10000000;
    bytes_loc[2] = 0B11110000;
    bytes_loc[3] = 0B10000000;
    bytes_loc[4] = 0B10000000;
    bytes_loc[5] = 0B10000000;

    return bytes_loc;
}

unsigned char* FONT_G(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10000000;
    bytes_loc[3] = 0B10011000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B01110000;

    return bytes_loc;
}

unsigned char* FONT_H(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B11111000;
    bytes_loc[3] = 0B10001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B10001000;

    return bytes_loc;
}

unsigned char* FONT_I(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11111000;
    bytes_loc[1] = 0B00100000;
    bytes_loc[2] = 0B00100000;
    bytes_loc[3] = 0B00100000;
    bytes_loc[4] = 0B00100000;
    bytes_loc[5] = 0B11111000;

    return bytes_loc;
}

unsigned char* FONT_J(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11111000;
    bytes_loc[1] = 0B00001000;
    bytes_loc[2] = 0B00001000;
    bytes_loc[3] = 0B00001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B01111000;

    return bytes_loc;
}

unsigned char* FONT_K(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10010000;
    bytes_loc[1] = 0B10100000;
    bytes_loc[2] = 0B11000000;
    bytes_loc[3] = 0B10100000;
    bytes_loc[4] = 0B10010000;
    bytes_loc[5] = 0B10010000;

    return bytes_loc;
}

unsigned char* FONT_L(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10000000;
    bytes_loc[1] = 0B10000000;
    bytes_loc[2] = 0B10000000;
    bytes_loc[3] = 0B10000000;
    bytes_loc[4] = 0B10000000;
    bytes_loc[5] = 0B11111000;

    return bytes_loc;
}

unsigned char* FONT_M(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B11011000;
    bytes_loc[2] = 0B10101000;
    bytes_loc[3] = 0B10001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B10001000;

    return bytes_loc;
}

unsigned char* FONT_N(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B11001000;
    bytes_loc[2] = 0B10101000;
    bytes_loc[3] = 0B10011000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B10001000;

    return bytes_loc;
}

unsigned char* FONT_O(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10001000;
    bytes_loc[3] = 0B10001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B01110000;

    return bytes_loc;
}

unsigned char* FONT_P(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10001000;
    bytes_loc[3] = 0B11110000;
    bytes_loc[4] = 0B10000000;
    bytes_loc[5] = 0B10000000;

    return bytes_loc;
}

unsigned char* FONT_Q(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10001000;
    bytes_loc[3] = 0B10101000;
    bytes_loc[4] = 0B10010000;
    bytes_loc[5] = 0B01101000;

    return bytes_loc;
}

unsigned char* FONT_R(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11110000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10001000;
    bytes_loc[3] = 0B11110000;
    bytes_loc[4] = 0B10010000;
    bytes_loc[5] = 0B10001000;

    return bytes_loc;
}

unsigned char* FONT_S(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01111000;
    bytes_loc[1] = 0B10000000;
    bytes_loc[2] = 0B01110000;
    bytes_loc[3] = 0B00001000;
    bytes_loc[4] = 0B00001000;
    bytes_loc[5] = 0B11110000;

    return bytes_loc;
}

unsigned char* FONT_T(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11111000;
    bytes_loc[1] = 0B00100000;
    bytes_loc[2] = 0B00100000;
    bytes_loc[3] = 0B00100000;
    bytes_loc[4] = 0B00100000;
    bytes_loc[5] = 0B00100000;

    return bytes_loc;
}

unsigned char* FONT_U(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10001000;
    bytes_loc[3] = 0B10001000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B01110000;

    return bytes_loc;
}

unsigned char* FONT_V(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10001000;
    bytes_loc[3] = 0B01010000;
    bytes_loc[4] = 0B01010000;
    bytes_loc[5] = 0B00100000;

    return bytes_loc;
}

unsigned char* FONT_W(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B10101000;
    bytes_loc[3] = 0B10101000;
    bytes_loc[4] = 0B01010000;
    bytes_loc[5] = 0B01010000;

    return bytes_loc;
}

unsigned char* FONT_X(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B01010000;
    bytes_loc[2] = 0B00100000;
    bytes_loc[3] = 0B01010000;
    bytes_loc[4] = 0B10001000;
    bytes_loc[5] = 0B10001000;

    return bytes_loc;
}

unsigned char* FONT_Y(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10001000;
    bytes_loc[1] = 0B10001000;
    bytes_loc[2] = 0B01010000;
    bytes_loc[3] = 0B00100000;
    bytes_loc[4] = 0B00100000;
    bytes_loc[5] = 0B00100000;

    return bytes_loc;
}

unsigned char* FONT_Z(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11111000;
    bytes_loc[1] = 0B00010000;
    bytes_loc[2] = 0B00100000;
    bytes_loc[3] = 0B01000000;
    bytes_loc[4] = 0B10000000;
    bytes_loc[5] = 0B11111000;

    return bytes_loc;
}

unsigned char* FONT_GREATER(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B00000000;
    bytes_loc[1] = 0B11000000;
    bytes_loc[2] = 0B00110000;
    bytes_loc[3] = 0B00001000;
    bytes_loc[4] = 0B00110000;
    bytes_loc[5] = 0B11000000;

    return bytes_loc;
}

unsigned char* FONT_COLON(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B00000000;
    bytes_loc[1] = 0B01000000;
    bytes_loc[2] = 0B00000000;
    bytes_loc[3] = 0B00000000;
    bytes_loc[4] = 0B01000000;
    bytes_loc[5] = 0B00000000;

    return bytes_loc;
}

unsigned char* FONT_0(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01100000;
    bytes_loc[1] = 0B10010000;
    bytes_loc[2] = 0B10010000;
    bytes_loc[3] = 0B10010000;
    bytes_loc[4] = 0B10010000;
    bytes_loc[5] = 0B01100000;

    return bytes_loc;
}

unsigned char* FONT_1(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B00100000;
    bytes_loc[1] = 0B01100000;
    bytes_loc[2] = 0B10100000;
    bytes_loc[3] = 0B00100000;
    bytes_loc[4] = 0B00100000;
    bytes_loc[5] = 0B01110000;

    return bytes_loc;
}

unsigned char* FONT_2(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01110000;
    bytes_loc[1] = 0B10010000;
    bytes_loc[2] = 0B00100000;
    bytes_loc[3] = 0B01000000;
    bytes_loc[4] = 0B10000000;
    bytes_loc[5] = 0B11110000;

    return bytes_loc;
}

unsigned char* FONT_3(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11100000;
    bytes_loc[1] = 0B00010000;
    bytes_loc[2] = 0B01100000;
    bytes_loc[3] = 0B00010000;
    bytes_loc[4] = 0B00010000;
    bytes_loc[5] = 0B11100000;

    return bytes_loc;
}

unsigned char* FONT_4(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B10010000;
    bytes_loc[1] = 0B10010000;
    bytes_loc[2] = 0B10010000;
    bytes_loc[3] = 0B11110000;
    bytes_loc[4] = 0B00010000;
    bytes_loc[5] = 0B00010000;

    return bytes_loc;
}

unsigned char* FONT_5(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11110000;
    bytes_loc[1] = 0B10000000;
    bytes_loc[2] = 0B11100000;
    bytes_loc[3] = 0B00010000;
    bytes_loc[4] = 0B00010000;
    bytes_loc[5] = 0B11100000;

    return bytes_loc;
}

unsigned char* FONT_6(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01110000;
    bytes_loc[1] = 0B10000000;
    bytes_loc[2] = 0B11100000;
    bytes_loc[3] = 0B10010000;
    bytes_loc[4] = 0B10010000;
    bytes_loc[5] = 0B01100000;

    return bytes_loc;
}

unsigned char* FONT_7(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11110000;
    bytes_loc[1] = 0B00010000;
    bytes_loc[2] = 0B00100000;
    bytes_loc[3] = 0B01000000;
    bytes_loc[4] = 0B10000000;
    bytes_loc[5] = 0B10000000;

    return bytes_loc;
}

unsigned char* FONT_8(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B01100000;
    bytes_loc[1] = 0B10010000;
    bytes_loc[2] = 0B01100000;
    bytes_loc[3] = 0B10010000;
    bytes_loc[4] = 0B10010000;
    bytes_loc[5] = 0B01100000;

    return bytes_loc;
}

unsigned char* FONT_9(){
    unsigned char *bytes_loc = (unsigned char*) malloc(sizeof(unsigned char) * 6);

    bytes_loc[0] = 0B11110000;
    bytes_loc[1] = 0B10010000;
    bytes_loc[2] = 0B11110000;
    bytes_loc[3] = 0B00010000;
    bytes_loc[4] = 0B00010000;
    bytes_loc[5] = 0B00010000;

    return bytes_loc;
}



unsigned char* getChar(char c){

    switch (c) {
        case 'A':
            return FONT_A();
        case 'B':
            return FONT_B();
        case 'C':
            return FONT_C();
        case 'D':
            return FONT_D();
        case 'E':
            return FONT_E();
        case 'F':
            return FONT_F();
        case 'G':
            return FONT_G();
        case 'H':
            return FONT_H();
        case 'I':
            return FONT_I();
        case 'J':
            return FONT_J();
        case 'K':
            return FONT_K();
        case 'L':
            return FONT_L();
        case 'M':
            return FONT_M();
        case 'N':
            return FONT_N();
        case 'O':
            return FONT_O();
        case 'P':
            return FONT_P();
        case 'Q':
            return FONT_Q();
        case 'R':
            return FONT_R();
        case 'S':
            return FONT_S();
        case 'T':
            return FONT_T();
        case 'U':
            return FONT_U();
        case 'V':
            return FONT_V();
        case 'W':
            return FONT_W();
        case 'X':
            return FONT_X();
        case 'Y':
            return FONT_Y();
        case 'Z':
            return FONT_Z();
        case '>':
            return FONT_GREATER();
        case ':':
            return FONT_COLON();
        case '0':
            return FONT_0();
        case '1':
            return FONT_1();
        case '2':
            return FONT_2();
        case '3':
            return FONT_3();
        case '4':
            return FONT_4();
        case '5':
            return FONT_5();
        case '6':
            return FONT_6();
        case '7':
            return FONT_7();
        case '8':
            return FONT_8();
        case '9':
            return FONT_9();
    }

    Serial.print("CHAR '");
    Serial.print(c);
    Serial.println("' NOT DEFINED!!!");
    return FONT_A();
}


#endif //TINYTETRIS_FONTS_MIN_H
