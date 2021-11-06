#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C     //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32


// Ceiling division macro
#define CEIL(x, y) (1 + ((x - 1) / y))

// Game constants
#define BLOCK_SIZE 5
#define GAME_X 8
#define GAME_Y 2
#define GAME_W ((SCREEN_WIDTH - GAME_X) / (BLOCK_SIZE + 1))
#define GAME_H ((SCREEN_HEIGHT - GAME_Y * 2) / (BLOCK_SIZE + 1))

// How many bytes to be used to capture the GAME_W
// Constant used for the game array
#define GAME_BYTE_W CEIL(GAME_W, 8)//(1 + ((GAME_W - 1) / 8))


// Init display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// The different blocks as byte arrays.
// Each blocks consists out of 2 Bytes (16 bits for the 4x4 space)
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

//Calculate game array where one BIT is a 5x5 field (1px between each tile)
//TODO: initialize array dynamically
unsigned char game[GAME_H][GAME_BYTE_W] = {
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 },
        { 0B00000000, 0B00000000, 0B00000000 }
};

//Current tile
int curBlock_id = -1;
int curBlock_x;
int curBlock_y;
int curBlock_rot;

void drawBlock(int id, int posx, int posy, int rot, bool removeOld, int posx_old, int posy_old){
//    printByte(blocks[id][0]);
//    Serial.println(blocks[id][0]);
//    printByte(blocks[id][1]);

    for(int i=0;i<2;i++){
        unsigned char b = blocks[id][i];

        for(int bi=7;bi>=0;bi--){
            int b_offx = bi % 4;
            int b_offy = bi / 4 + i * 2;

            if(b & 1){
                if(removeOld){
                    _drawTile(posx_old + b_offx, posy_old + b_offy, false);
                }

                _drawTile(posx + b_offx, posy + b_offy, true);
            }

            b = b >> 1;

            if(b == 0) break;
        }
    }
}

void _drawTile(int posx, int posy, bool state){
    setTile(posx, posy, state);
}

// Draws a tile defined by the BLOCK_SIZE at a position (posx, posy)
void drawTile(int posx, int posy){
    display.fillRect(GAME_X + posx * (BLOCK_SIZE + 1), GAME_Y + posy * (BLOCK_SIZE + 1), BLOCK_SIZE, BLOCK_SIZE, WHITE);
}

// Wrapper method to draw a white pixel on screen
void pixel(int x, int y){
    display.drawPixel(x, y, WHITE);
}

void setTile(int x, int y, bool state){
    int ix = x / 8;
    int rx = x % 8;

    unsigned char b = game[y][ix];
    unsigned char posByte = 0B10000000 >> rx;

    if(state){
        game[y][ix] = b | posByte;
    }
    else {
        if(b & posByte) {
            game[y][ix] &= ~(0B10000000 >> rx);

            //Tmp fix to clear tiles
            display.fillRect(GAME_X + x * (BLOCK_SIZE + 1), GAME_Y + y * (BLOCK_SIZE + 1), BLOCK_SIZE, BLOCK_SIZE, BLACK);
        }
    }
}

void printByte(unsigned char b){
    Serial.print("Byte[");
    while(true){
        if(b & 1){
            Serial.print("1");
        }
        else {
            Serial.print("0");
        }

        b = b >> 1;
        if(b == 0) break;
    }
    Serial.println("]");
}


// ################# SETUP METHOD #################
void setup() {
    Serial.begin(9600);
    while (!Serial);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    Serial.println("--------------");
    Serial.print("GAME_X: ");
    Serial.println(GAME_X);
    Serial.print("GAME_Y: ");
    Serial.println(GAME_Y);
    Serial.print("GAME_W: ");
    Serial.println(GAME_W);
    Serial.print("GAME_H: ");
    Serial.println(GAME_H);
    Serial.print("GAME_BYTE_W: ");
    Serial.println(GAME_BYTE_W);

    // Clear the buffer
    display.clearDisplay();
    display.display();

    delay(100);
    drawBorder();

    drawBlock(1, 2, 2, 0, false, 0, 0);
    drawGame();
    delay(1000);

    drawBlock(1, 3, 2, 0, true, 2, 2);
    drawGame();
    delay(1000);

    drawBlock(1, 4, 2, 0, true, 3, 2);
    drawGame();
    delay(1000);

    drawBlock(1, 5, 2, 0, true, 4, 2);
    drawGame();
    delay(1000);

    drawBlock(1, 6, 2, 0, true, 5, 2);
    drawGame();
    delay(1000);
}

// Draws the border around the space where the game is played
void drawBorder(){

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(6,0);
    display.println("130920");
    display.setRotation(0);

    display.drawLine(0, 0, SCREEN_WIDTH-1, 0, WHITE);
    display.drawLine(0, SCREEN_HEIGHT-2, SCREEN_WIDTH-1, SCREEN_HEIGHT-2, WHITE);
    display.drawLine(SCREEN_WIDTH-1, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-2, WHITE);
    display.drawLine(GAME_X-1, 0, GAME_X-1, SCREEN_HEIGHT-2, WHITE);

    display.display();
}

// Draws the actual game state from the game array
void drawGame(){
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

// ################# LOOP METHOD #################
void loop() {
//    if(curBlock_id != -1){
//
//    }
//    drawGame();
//
//    tx++;
//    delay(1000);
}
