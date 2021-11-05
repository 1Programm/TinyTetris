#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C     //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


#define BLOCK_SIZE 5
#define GAME_X 8
#define GAME_Y 2
#define GAME_W ((SCREEN_WIDTH - GAME_X) / (BLOCK_SIZE + 1))
#define GAME_H ((SCREEN_HEIGHT - GAME_Y * 2) / (BLOCK_SIZE + 1))



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


const unsigned char blocks[5][2] = {
        /*
         * . # # .
         * . # # .
         * . . . .
         * . . . .
         */
        { 0B01100110, 0B00000000 },

        /*
         * # # # #
         * . . . .
         * . . . .
         * . . . .
         */
        { 0B11110000, 0B00000000 },

        /*
         * # # # .
         * . # . .
         * . . . .
         * . . . .
         */
        { 0B11100100, 0B00000000 },

        /*
         * # . . .
         * # . . .
         * # # . .
         * . . . .
         */
        { 0B10001000, 0B11000000 },

        /*
         * # . . .
         * # # . .
         * . # . .
         * . . . .
         */
        { 0B10001100, 0B01000000 }
};

#define GAME_BYTE_W (1 + ((GAME_W - 1) / 8))

//Calculate game array where one BIT is a 5x5 field (1px between each tile)
//TODO: initialize array dynamically
const unsigned char game[GAME_H][GAME_BYTE_W] = {
        { 0B11111111, 0B11111111, 0B11011111 },
        { 0B10100000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 },
        { 0B10000000, 0B00000000, 0B00000000 }
};






//void drawBlock(int id, int x, int y){
//    drawByteArray(id, x * (PIECE_SCALE + BLOCK_SIZE), y * (PIECE_SCALE + BLOCK_SIZE), PIECE_SCALE);
//}
//
//void drawScaledPixel(int posx, int posy, int scale){
//    for(int x=0;x<scale;x++) {
//        for (int y=0;y<scale;y++) {
//            pixel(posx + x, posy + y);
//        }
//    }
//}
//
//void drawByteArray(int id, int posx, int posy, int scale){
//    Serial.print("Draw Block: ");
//    Serial.print(id);
//    Serial.print(" at pos: [");
//    Serial.print(posx);
//    Serial.print(", ");
//    Serial.print(posy);
//    Serial.println("]");
//
//    for(int i=0;i<2;i++){
//        unsigned char b = blocks[id][i];
//
//        if(b & 0B10000000){
//            int curx = posx + 0 * scale;
//            int cury = posy + (i * 2 + 0) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//
//        if(b & 0B01000000){
//            int curx = posx + 1 * scale;
//            int cury = posy + (i * 2 + 0) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//
//        if(b & 0B00100000){
//            int curx = posx + 2 * scale;
//            int cury = posy + (i * 2 + 0) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//
//        if(b & 0B00010000){
//            int curx = posx + 3 * scale;
//            int cury = posy + (i * 2 + 0) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//
//        if(b & 0B00001000){
//            int curx = posx + 0 * scale;
//            int cury = posy + (i * 2 + 1) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//
//        if(b & 0B00000100){
//            int curx = posx + 1 * scale;
//            int cury = posy + (i * 2 + 1) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//
//        if(b & 0B00000010){
//            int curx = posx + 2 * scale;
//            int cury = posy + (i * 2 + 1) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//
//        if(b & 0B00000001){
//            int curx = posx + 3 * scale;
//            int cury = posy + (i * 2 + 1) * scale;
//            drawScaledPixel(curx, cury, scale);
//        }
//    }
//}

void drawTile(int posx, int posy){
    display.fillRect(GAME_X + posx * (BLOCK_SIZE + 1), GAME_Y + posy * (BLOCK_SIZE + 1), BLOCK_SIZE, BLOCK_SIZE, WHITE);
//    for(int x=0;x<BLOCK_SIZE;x++){
//        for(int y=0;y<BLOCK_SIZE;y++){
//
//        }
//    }
}

void pixel(int x, int y){
    display.drawPixel(x, y, WHITE);
}

void setup() {
    Serial.begin(9600);
    while (!Serial);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    Serial.print("GAME_X: ");
    Serial.println(GAME_X);
    Serial.print("GAME_Y: ");
    Serial.println(GAME_Y);
    Serial.print("GAME_W: ");
    Serial.println(GAME_W);
    Serial.print("GAME_H: ");
    Serial.println(GAME_H);

    // Clear the buffer
    display.clearDisplay();
    display.display();

    delay(100);
    drawBorder();

    delay(100);
    drawGame();
}

void drawBorder(){
    display.drawLine(0, 2, SCREEN_WIDTH-1, 2, WHITE);

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(6,0);
    display.println("130920");
    display.setRotation(0);

    display.drawLine(0, SCREEN_HEIGHT-2, SCREEN_WIDTH-1, SCREEN_HEIGHT-2, WHITE);
    display.drawLine(SCREEN_WIDTH-1, 2, SCREEN_WIDTH-1, SCREEN_HEIGHT-2, WHITE);
    display.drawLine(GAME_X, 2, GAME_X, SCREEN_HEIGHT-2, WHITE);

    display.display();
}

void drawGame(){
//    display.drawRect(GAME_X, GAME_Y, GAME_W * BLOCK_SIZE, GAME_H * BLOCK_SIZE, WHITE);

    for(int x=0;x<GAME_BYTE_W;x++){
        for(int y=0;y<GAME_H;y++){
            unsigned char b = game[y][x];

            if(b & 0B10000000){
                int ix = x * 8 + 0;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }

            if(b & 0B01000000){
                int ix = x * 8 + 1;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }

            if(b & 0B00100000){
                int ix = x * 8 + 2;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }

            if(b & 0B00010000){
                int ix = x * 8 + 3;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }

            if(b & 0B00001000){
                int ix = x * 8 + 4;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }

            if(b & 0B00000100){
                int ix = x * 8 + 5;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }

            if(b & 0B00000010){
                int ix = x * 8 + 6;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }

            if(b & 0B00000001){
                int ix = x * 8 + 7;
                if(ix < GAME_W){
                    drawTile(ix, y);
                }
            }
        }
    }

    display.display();
}

void loop() {
}
