//
// Created by Julian Becker on 09.11.21.
//

#ifndef TINYTETRIS_DISPLAY_H
#define TINYTETRIS_DISPLAY_H

#define OLED_COMMAND	                            0x80
#define OLED_DATA	                                0x40

#define OLED_SETCONTRAST                            0x81
#define OLED_DISPLAY_ALL_ON_RESUME                  0xA4
#define OLED_DISPLAY_ALL_ON_IGNORE                  0xA5
#define OLED_DISPLAY_NORMAL                         0xA6
#define OLED_DISPLAY_INVERT                         0xA7
#define OLED_DISPLAY_OFF                            0xAE
#define OLED_DISPLAY_ON                             0xAF

#define OLED_SETMEMORY_MODE                         0x20
#define OLED_SETMEMORY_MODE_HORIZONTAL              0x00
#define OLED_SETMEMORY_MODE_VERTICAL                0x01
#define OLED_SETMEMORY_MODE_PAGE                    0x02
#define OLED_SETPAGE_ADDR                           0x21
#define OLED_SETCOLUMN_ADDR                         0x22

#define OLED_SETSTARTLINE_ZERO                      0x40
#define OLED_SEGREMAPNORMAL                         0xA0
#define OLED_SEGREMAPINV                            0xA1
#define OLED_SETMULTIPLEX                           0xA8
#define OLED_COMSCANINC                             0xC8
#define OLED_COMSCANDEC                             0xC0
#define OLED_SETDISPLAY_OFFSET                      0xD3
#define OLED_SETCOMPINS                             0xDA

#define OLED_SETDISPLAY_CLOCKDIV                     0xD5
#define OLED_SETPRECHARGE                           0xD9
#define OLED_SETVCOMDESELECT                        0xDB
#define OLED_NOP                                    0xE3

#define OLED_CHARGEPUMP                             0x8D
#define OLED_CHARGEPUMP_ON                          0x14
#define OLED_CHARGEPUMP_OFF                         0x10

/*

#define OLED_ADDRESS	        	    0x3C //you may need to change this, this is the OLED I2C address.
#define OLED_COMMAND	                0x80
#define OLED_DATA	                    0x40
#define OLED_DISPLAY_OFF	            0xAE
#define OLED_DISPLAY_ON	                0xAF
#define OLED_NORMAL_DISPLAY	    	    0xA6
#define OLED_INVERSE_DISPLAY     	    0xA7
#define OLED_SET_BRIGHTNESS	            0x81
#define OLED_SET_ADDRESSING	            0x20
#define OLED_HORIZONTAL_ADDRESSING	    0x00
#define OLED_VERTICAL_ADDRESSING	    0x01
#define OLED_PAGE_ADDRESSING	        0x02
#define OLED_SET_COLUMN                 0x21
#define OLED_SET_PAGE	                0x22

 */


#define WHITE 0xff
#define BLACK 0x00
#define SCREEN_WIDTH  64         // OLED display width, in pixels
#define SCREEN_HEIGHT 128        // OLED display height, in pixels
#define SCREEN_BYTE_WIDTH (SCREEN_WIDTH / 8)

class Display {
private:
    char screenAddress;
    unsigned char pixels[SCREEN_HEIGHT][SCREEN_BYTE_WIDTH];
    unsigned char textColor = WHITE;

    void OLEDCommand(unsigned char command) {
        Wire.beginTransmission(screenAddress);
        Wire.write(OLED_COMMAND);
        Wire.write(command);
        Wire.endTransmission();
    }

    void OLEDData(unsigned char data) {
        Wire.beginTransmission(screenAddress);
        Wire.write(OLED_DATA);
        Wire.write(data);
        Wire.endTransmission();
    }

public:
    Display(int address){
        screenAddress = address;
    }

    bool begin(){
        unsigned char initCommands[] = {
                OLED_DISPLAY_OFF,
                OLED_DISPLAY_NORMAL,

                OLED_SETMEMORY_MODE,
                OLED_SETMEMORY_MODE_PAGE,

                OLED_SEGREMAPINV,
                OLED_SETMULTIPLEX,
                0x3F,
                OLED_COMSCANDEC,
                OLED_SETDISPLAY_OFFSET,
                0x00,

                OLED_SETDISPLAY_CLOCKDIV,
                0x80,
                OLED_SETPRECHARGE,
                0x22,
                OLED_SETVCOMDESELECT,
                0x20,

                OLED_CHARGEPUMP,
                OLED_CHARGEPUMP_ON,

                OLED_DISPLAY_ALL_ON_RESUME,
                OLED_DISPLAY_ON,
        };

        for(int i=0;i<sizeof(initCommands);i++){
            OLEDCommand(initCommands[i]);
        }

        clearDisplay();

        return true;
    }

    void clearDisplay(){
        OLEDCommand(OLED_SETMEMORY_MODE);
        OLEDCommand(OLED_SETMEMORY_MODE_VERTICAL);
        OLEDCommand(OLED_SETCOLUMN_ADDR);
        OLEDCommand(0);
        OLEDCommand(7);
        OLEDCommand(OLED_SETPAGE_ADDR);
        OLEDCommand(0);
        OLEDCommand(127);

        for(int i=0;i<(16 * 128);i++){
            OLEDData(0x00);
        }

        for(int y=0;y<SCREEN_HEIGHT;y++){
            for(int x=0;x<SCREEN_BYTE_WIDTH;x++){
                pixels[y][x] = 0x00;
            }
        }
    }

    void fillDisplay(){
        OLEDCommand(OLED_SETMEMORY_MODE);
        OLEDCommand(OLED_SETMEMORY_MODE_VERTICAL);
        OLEDCommand(OLED_SETCOLUMN_ADDR);
        OLEDCommand(0);
        OLEDCommand(7);
        OLEDCommand(OLED_SETPAGE_ADDR);
        OLEDCommand(0);
        OLEDCommand(127);

        for(int i=0;i<(16 * 128);i++){
            OLEDData(0xff);
        }

        for(int y=0;y<SCREEN_HEIGHT;y++){
            for(int x=0;x<SCREEN_BYTE_WIDTH;x++){
                pixels[y][x] = 0xff;
            }
        }
    }

    void test(){
        pixels[0][0] = 0B10000001;
    }

    void display(){
        OLEDCommand(OLED_SETMEMORY_MODE);
        OLEDCommand(OLED_SETMEMORY_MODE_VERTICAL);
        OLEDCommand(OLED_SETCOLUMN_ADDR);
        OLEDCommand(0);
        OLEDCommand(7);
        OLEDCommand(OLED_SETPAGE_ADDR);
        OLEDCommand(0);
        OLEDCommand(127);

        for(int y=0;y<128;y++){
            for(int x=0;x<8;x++){
                OLEDData(pixels[y][x]);
            }
        }
    }

    void setRotation(int i){}
    void setTextSize(int i){}
    void setTextColor(unsigned char c){
        textColor = c;
    }
    void setCursor(int x, int y){}

    void println(char const* s){}
    void print(char const* s){}
    void println(int s){}
    void print(int s){}
    void println(unsigned int s){}
    void print(unsigned int s){}
    void println(long s){}
    void print(long s){}
    void println(unsigned long s){}
    void print(unsigned long s){}

    void printStr(int x, int y, char const* str){
        Serial.print("Str length: ");
        Serial.println(strlen(str));
        for(int i=0;i<strlen(str);i++){
            Serial.println(str[i]);
        }
    }

    void drawHorizontalLine(int x, int y, int x2, unsigned char c){
        if(y < 0 || y >= SCREEN_HEIGHT) return;
        if(x < 0 || x2 >= SCREEN_WIDTH) return;

        int ix = x / 8;
        int rx = x % 8;

        int ix2 = x2 / 8;
        int rx2 = x2 % 8;

        if(rx != 0){
            unsigned char b = pixels[y][ix];
            unsigned char addB = 0xff;
            unsigned char subB = 1;
            for(int i=0;i<rx;i++){
                subB *= 2;
            }
            addB -= (subB - 1);

            if(c == WHITE){
                pixels[y][ix] = b | addB;
            }
            else {
                pixels[y][ix] = b & addB;
            }
        }
        else {
            pixels[y][ix] = c;
        }

        for(int i=ix+1;i<ix2;i++){
            pixels[y][i] = c;
        }

        if(rx2 != 0){
            unsigned char b = pixels[y][ix2];
            unsigned char addB = 1;
            for(int i=0;i<rx2;i++){
                addB *= 2;
            }
            addB--;

            if(c == WHITE){
                pixels[y][ix2] = b | addB;
            }
            else {
                pixels[y][ix2] = b & addB;
            }
        }
    }

    void drawVerticalLine(int x, int y, int y2, unsigned char c){
        if(y < 0 || y2 >= SCREEN_HEIGHT) return;
        if(x < 0 || x >= SCREEN_WIDTH) return;

        int ix = x / 8;
        int rx = x % 8;

        unsigned char posByte = 0B00000001 << rx;

        for(int i=y;i<y2;i++){
            pixels[i][ix] |= posByte;
        }
    }

    void drawRect(int x, int y, int w, int h, unsigned char c){
        drawHorizontalLine(x, y, w, c);
        drawHorizontalLine(x, y + h, w, c);
        drawVerticalLine(x, y, h, c);
        drawVerticalLine(x + w, y, h, c);
    }

    void fillRect(int x, int y, int w, int h, unsigned char c){
        if(x < 0 || x + w >= SCREEN_WIDTH) return;
        if(y < 0 || y + h >= SCREEN_HEIGHT) return;

        for(int i=0;i<h;i++){
            drawHorizontalLine(x, y + i, x + w, c);
        }
    }


    void printByte(unsigned char b){
        Serial.print(F("Byte["));
        while(true){
            if(b & 1){
                Serial.print(F("1"));
            }
            else {
                Serial.print(F("0"));
            }

            b = b >> 1;
            if(b == 0) break;
        }
        Serial.println(F("]"));
    }
};

#endif //TINYTETRIS_DISPLAY_H
