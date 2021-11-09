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
#define OLED_COMSCANINC                             0xC0
#define OLED_COMSCANDEC                             0xC8
#define OLED_SETDISPLAYOFFSET                       0xD3
#define OLED_SETCOMPINS                             0xDA

#define OLED_SETDISPLAYCLOCKDIV                     0xD5
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


#define WHITE 0x00
#define BLACK 0xff
#define SSD1306_SWITCHCAPVCC 0

class Display {
private:
    char screenAddress;

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
    Display(int screen_w, int screen_h){

    }

    bool begin(int a, char address){
        screenAddress = address;

        unsigned char initCommands[] = {
                OLED_DISPLAY_OFF,
                OLED_DISPLAY_ON,
                OLED_DISPLAY_NORMAL,

                OLED_CHARGEPUMP,
                OLED_CHARGEPUMP_ON,

                OLED_DISPLAY_NORMAL
        };

        for(int i=0;i<sizeof(initCommands);i++){
            OLEDCommand(initCommands[i]);
        }

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
    }

    void test(){
        OLEDCommand(OLED_SETMEMORY_MODE);
        OLEDCommand(OLED_SETMEMORY_MODE_VERTICAL);
        OLEDCommand(OLED_SETCOLUMN_ADDR);
        OLEDCommand(0);
        OLEDCommand(7);
        OLEDCommand(OLED_SETPAGE_ADDR);
        OLEDCommand(0);
        OLEDCommand(63);

        OLEDData(0xff);
        OLEDData(0x00);
        OLEDData(0xff);
//        for(int i=0;i<(8 * 128);i++){
//            OLEDData(0xff);
//        }
    }

    void display(){}

    void setRotation(int i){}
    void setTextSize(int i){}
    void setTextColor(int c){}
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

    void drawLine(int x1, int y1, int x2, int y2, int c){}
    void drawRect(int x, int y, int w, int h, int c){}
    void fillRect(int x, int y, int w, int h, int c){}

};

#endif //TINYTETRIS_DISPLAY_H
