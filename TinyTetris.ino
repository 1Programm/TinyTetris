#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// Ceiling division macro
#define CEIL(x, y) (1 + ((x - 1) / y))



#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C     //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// RANDOM NUMBER GENERATION
#define OUT_UNCONNECTED 1       // Must be an output which is unconnected as it will generate random noise on read


// Game constants
#define BLOCKS_COUNT 5
#define TILE_SIZE 5 //How many pixels one tile is
#define GAME_X 8
#define GAME_Y 2
#define GAME_W ((SCREEN_WIDTH - GAME_X) / (TILE_SIZE + 1))
#define GAME_H ((SCREEN_HEIGHT - GAME_Y * 2) / (TILE_SIZE + 1))

// How many bytes to be used to capture the GAME_W
// Constant used for the game array
#define GAME_BYTE_W CEIL(GAME_W, 8)//(1 + ((GAME_W - 1) / 8))


// Init display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// The different blocks as byte arrays.
// Each blocks consists out of 2 Bytes (16 bits for the 4x4 space)
const unsigned char blocks[BLOCKS_COUNT][2] = {
        /*
         * # # . .
         * # # . .
         * . . . .
         * . . . .
         */
        { 0B00000000, 0B11001100 },

        /*
         * # . . .
         * # . . .
         * # . . .
         * # . . .
         */
        { 0B00000000, 0B00001111 },

        /*
         * # # # .
         * . # . .
         * . . . .
         * . . . .
         */
        { 0B00001000, 0B11001000 },

        /*
         * # # # #
         * # . . .
         * . . . .
         * . . . .
         */
        { 0B10001000, 0B11000000 },

        /*
         * # . . .
         * # # . .
         * . # . .
         * . . . .
         */
        { 0B00000000, 0B01101100 }
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

void generateCurBlock(){
    curBlock_id = randomBlock();
    curBlock_x = 0;
    curBlock_y = 3;
    curBlock_rot = 0;

    setBlock(curBlock_id, curBlock_x, curBlock_y, curBlock_rot);
}

void moveCurBlock(int vel_x, int vel_y){
    if(curBlock_id == -1) return;

    int futurePos_x = curBlock_x + vel_x;
    int futurePos_y = curBlock_y + vel_y;

    setBlockWithoutDrawing(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, false);
    if(isSpaceForBlock(curBlock_id, futurePos_x, futurePos_y)){
        setBlock(curBlock_id, futurePos_x, futurePos_y, curBlock_rot, true, curBlock_x, curBlock_y);
        curBlock_x = futurePos_x;
        curBlock_y = futurePos_y;
    }
    else {
        setBlockWithoutDrawing(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, true);
        curBlock_id = -1;
    }
}

void setBlockWithoutDrawing(int id, int pos_x, int pos_y, int rot, bool state){
    for(int i=0;i<2;i++){
        unsigned char b = blocks[id][i];

        for(int bi=7;bi>=0;bi--){
            int b_offx = bi % 4;
            int b_offy = bi / 4 + i * 2;

            if(b & 1){
                int cur_x = pos_x + b_offx;
                int cur_y = pos_y + b_offy;

                if(cur_x < 0 || cur_y < 0 || cur_x >= GAME_W || cur_y >= GAME_H) continue;

                setTile(cur_x, cur_y, state, false);
            }

            b = b >> 1;

            if(b == 0) break;
        }
    }
}

bool isSpaceForBlock(int id, int pos_x, int pos_y){
    for(int i=0;i<2;i++){
        unsigned char b = blocks[id][i];

        for(int bi=7;bi>=0;bi--){
            int b_offx = bi % 4;
            int b_offy = bi / 4 + i * 2;

            if(b & 1){
                int cur_x = pos_x + b_offx;
                int cur_y = pos_y + b_offy;

                if(cur_x < 0 || cur_y < 0 || cur_x >= GAME_W || cur_y >= GAME_H) {
                    return false;
                }

                if(getTile(cur_x, cur_y)){
                    return false;
                }
            }

            b = b >> 1;

            if(b == 0) break;
        }
    }

    return true;
}

void setBlock(int id, int pos_x, int pos_y, int rot){
    setBlock(id, pos_x, pos_y, rot, false, 0, 0);
}

void setBlock(int id, int pos_x, int pos_y, int rot, bool removeOld, int pos_x_old, int pos_y_old){
    for(int i=0;i<2;i++){
        unsigned char b = blocks[id][i];

        for(int bi=7;bi>=0;bi--){
            int b_off_x = bi % 4;
            int b_off_y = bi / 4 + i * 2;

            if(b & 1){
                if(removeOld){
                    setTile(pos_x_old + b_off_x, pos_y_old + b_off_y, false, true);
                }

                setTile(pos_x + b_off_x, pos_y + b_off_y, true, true);
            }

            b = b >> 1;

            if(b == 0) break;
        }
    }
}

// Draws a tile defined by the TILE_SIZE at a position (posx, posy)
void drawTile(int x, int y, bool state){
    display.fillRect(GAME_X + x * (TILE_SIZE + 1), GAME_Y + y * (TILE_SIZE + 1), TILE_SIZE, TILE_SIZE, state ? WHITE : BLACK);
}

void setTile(int x, int y, bool state, bool draw){
    int ix = x / 8;
    int rx = x % 8;

    unsigned char b = game[y][ix];
    unsigned char posByte = 0B10000000 >> rx;

    if(state){
        game[y][ix] = b | posByte;
    }
    else {
        game[y][ix] &= ~(posByte);
    }

    if(draw) {
        drawTile(x, y, state);
    }
}

bool getTile(int x, int y){
    int ix = x / 8;
    int rx = x % 8;

    unsigned char b = game[y][ix];
    unsigned char posByte = 0B10000000 >> rx;

    return b & posByte;
}

unsigned char lastRandomBlock = BLOCKS_COUNT;
int randomBlock(){
    int r = analogRead(OUT_UNCONNECTED);
    int rBlock = r % BLOCKS_COUNT;

    if(lastRandomBlock != BLOCKS_COUNT && lastRandomBlock == rBlock){
        int rr = analogRead(OUT_UNCONNECTED);

        if(rr % 2){
            rBlock++;
            if(rBlock >= BLOCKS_COUNT){
                rBlock = 0;
            }
        }
    }

    lastRandomBlock = rBlock;
    return rBlock;
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
//    Serial.print("GAME_X: ");
//    Serial.println(GAME_X);
//    Serial.print("GAME_Y: ");
//    Serial.println(GAME_Y);
//    Serial.print("GAME_W: ");
//    Serial.println(GAME_W);
//    Serial.print("GAME_H: ");
//    Serial.println(GAME_H);
//    Serial.print("GAME_BYTE_W: ");
//    Serial.println(GAME_BYTE_W);

    // Clear the buffer
    display.clearDisplay();
    display.display();

    delay(100);
    drawBorder();


    generateCurBlock();
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
//    for(int x=0;x<GAME_BYTE_W;x++) {
//        for (int y=0;y<GAME_H;y++) {
//            unsigned char b = game[y][x];
//
//            for (int bi = 7; bi >= 0; bi--) {
//                if (b & 1) {
//                    drawTile(x * 8 + bi, y);
//                }
//
//                b = b >> 1;
//
//                if (b == 0) break;
//            }
//        }
//    }

    display.display();
}

// ################# LOOP METHOD #################
void loop() {
    if(curBlock_id != -1){
        // Moving current block down
        moveCurBlock(1, 0);

        if(curBlock_id == -1){
            generateCurBlock();
        }
    }
    else {
        generateCurBlock();
    }

    drawGame();
    delay(1000);
}








// TMP UTIL METHODS

void printCurBlockInfo(){
    Serial.println("Cur Block:");
    Serial.print("ID: ");
    Serial.println(curBlock_id);
    Serial.print("X: ");
    Serial.println(curBlock_x);
    Serial.print("Y: ");
    Serial.println(curBlock_y);
//    Serial.print("Rot: ");
//    Serial.println(curBlock_rot);
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