#include <SPI.h>
#include <Wire.h>
#include <EEPROM.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

#include "dpad.h"
#include "Display.h"


// Ceiling division macro
#define CEIL(x, y) (1 + ((x - 1) / y))



#define SCREEN_WIDTH 128        // OLED display width, in pixels
#define SCREEN_HEIGHT 64        // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     4        // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C     //< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

// RANDOM NUMBER GENERATION
#define OUT_UNCONNECTED 1       // Must be an output which is unconnected as it will generate random noise on read

// MEMORY ADDRESS
#define MEM_SCORE_SET 0         //WIDTH: 1 byte
#define MEM_SCORE 1             //WIDTH: 3 byte

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
//Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
Display display(SCREEN_WIDTH, SCREEN_HEIGHT);

// The different blocks as byte arrays.
// Each blocks consists out of 2 Bytes (16 bits for the 4x4 space)
const unsigned char blocks[BLOCKS_COUNT][2] = {
        /*
         * . . . .
         * . # # .
         * . # # .
         * . . . .
         */
        { 0B00000110, 0B01100000 },

        /*
         * . # . .
         * . # . .
         * . # . .
         * . # . .
         */
        { 0B00000000, 0B11110000 },

        /*
         * . . . .
         * . # # #
         * . . # .
         * . . . .
         */
        { 0B01000110, 0B01000000 },

        /*
         * . . . .
         * . # # #
         * . # . .
         * . . . .
         */
        { 0B01000100, 0B01100000 },

        /*
         * . # . .
         * . # # .
         * . . # .
         * . . . .
         */
        { 0B00000110, 0B11000000 },
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

//0 = Title, 1 = Game, 2 = Dead, 3 = Options, 4 = Credits
unsigned short game_state = 0;
unsigned short state_title_option;
unsigned short state_dead_option;
bool dead_isNewHighScore = false;
unsigned long oldHighScore;
unsigned short state_options_option;
bool option_music = false;
bool option_speedup = false;
unsigned long score;

// --- In game globals: ---

// Current tile
unsigned short curBlock_id = -1;
unsigned short curBlock_x;
unsigned short curBlock_y;
unsigned short curBlock_rot;

// Timer for game loop
unsigned int timer_wait;
unsigned int timer;

bool drawChange = false;




// ################# UI METHODS #################

void changeStateTitle(){
    game_state = 0;
    state_title_option = 0;
    drawStateTitle();
}

void drawStateTitle(){
    display.clearDisplay();
    drawUiBorder();

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(4, 10);
    display.println("Tiny");
    display.setCursor(4, 20);
    display.println("Tetris");
    display.drawLine(1, 29, SCREEN_HEIGHT - 1, 29, WHITE);

    if(state_title_option == 0) {
        display.setCursor(4, 40);
        display.println("> Start");
        display.setCursor(3, 50);
        display.println("  Options");
        display.setCursor(3, 60);
        display.println("  Credits");
    }
    else if(state_title_option == 1) {
        display.setCursor(3, 40);
        display.println("  Start");
        display.setCursor(4, 50);
        display.println("> Options");
        display.setCursor(3, 60);
        display.println("  Credits");
    }
    else if(state_title_option == 2) {
        display.setCursor(3, 40);
        display.println("  Start");
        display.setCursor(3, 50);
        display.println("  Options");
        display.setCursor(4, 60);
        display.println("> Credits");
    }

    display.setRotation(0);
    display.display();
}

void changeStateGame(){
    display.clearDisplay();

    score = 0;
    drawBorder();

    generateCurBlock();
    timer_wait = 1000;
    timer = millis();
    game_state = 1;
}

void changeStateDead(){
    game_state = 2;
    for(int x=0;x<GAME_W;x++){
        for(int y=0;y<GAME_H;y++){
            drawTile(x, y, false);
        }

        display.display();
        delay(10);
    }

    //Clean up game board
    for(int y=0;y<GAME_H;y++){
        for(int i=0;i<GAME_BYTE_W;i++){
            game[y][i] = 0B00000000;
        }
    }

    unsigned char highscore_set = EEPROM.read(MEM_SCORE_SET);

    //If set = 255 then the highscore was never set before
    if(highscore_set == 255){
        dead_isNewHighScore = true;
        writeHighScore(score);
        EEPROM.write(MEM_SCORE_SET, 0);
    }
    else {
        oldHighScore = readHighScore();
        if (score > oldHighScore) {
            dead_isNewHighScore = true;
            writeHighScore(score);
        }
    }

    state_dead_option = 0;
    drawStateDead();
}

void drawStateDead(){
    display.clearDisplay();
    drawUiBorder();

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    if(dead_isNewHighScore){
        display.setCursor(4, 10);
        display.println("HIGH SCORE!");

        display.drawLine(1, 19, SCREEN_HEIGHT - 2, 19, WHITE);

        display.setCursor(4, 30);
        display.println("Score: ");
        display.setCursor(4, 40);
        display.println(score);
    }
    else {
        display.setCursor(4, 10);
        display.println("GAME OVER");

        display.drawLine(1, 19, SCREEN_HEIGHT - 2, 19, WHITE);

        display.setCursor(4, 30);
        display.println("Score: ");
        display.setCursor(4, 40);
        display.println(score);
        display.setCursor(4, 55);
        display.println("High: ");
        display.setCursor(3, 65);
        display.println(oldHighScore);
    }

    if (state_dead_option == 0) {
        display.setCursor(4, SCREEN_WIDTH - 26);
        display.println("> Restart");
        display.setCursor(3, SCREEN_WIDTH - 16);
        display.println("  Title");
    } else if (state_dead_option == 1) {
        display.setCursor(3, SCREEN_WIDTH - 26);
        display.println("  Restart");
        display.setCursor(4, SCREEN_WIDTH - 16);
        display.println("> Title");
    }

    display.setRotation(0);
    display.display();
}

void changeStateOptions(){
    game_state = 3;
    state_options_option = 0;

    drawStateOptions();
}

void drawStateOptions(){
    display.clearDisplay();
    drawUiBorder();

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(4, 10);
    display.println("Options");
    display.drawLine(1, 19, SCREEN_HEIGHT - 1, 19, WHITE);

    if(state_options_option == 0) {
        display.setCursor(4, 30);
        display.println("> Music:");
        display.setCursor(4, 40);
        if(option_music){ display.println("  [On ]"); }
        else { display.println("  [Off]"); }

        display.setCursor(3, 50);
        display.println("  Speedup:");
        display.setCursor(3, 60);
        if(option_speedup){ display.println("  [On ]"); }
        else { display.println("  [Off]"); }

        display.setCursor(3, SCREEN_WIDTH - 16);
        display.println("  Back");
    }
    else if(state_options_option == 1){
        display.setCursor(3, 30);
        display.println("  Music:");
        display.setCursor(3, 40);
        if(option_music){ display.println("  [On ]"); }
        else { display.println("  [Off]"); }

        display.setCursor(4, 50);
        display.println("> Speedup:");
        display.setCursor(4, 60);
        if(option_speedup){ display.println("  [On ]"); }
        else { display.println("  [Off]"); }

        display.setCursor(3, SCREEN_WIDTH - 16);
        display.println("  Back");
    }
    else if(state_options_option == 2){
        display.setCursor(3, 30);
        display.println("  Music:");
        display.setCursor(3, 40);
        if(option_music){ display.println("  [On ]"); }
        else { display.println("  [Off]"); }

        display.setCursor(3, 50);
        display.println("  Speedup:");
        display.setCursor(3, 60);
        if(option_speedup){ display.println("  [On ]"); }
        else { display.println("  [Off]"); }

        display.setCursor(4, SCREEN_WIDTH - 16);
        display.println("> Back");
    }

    display.setRotation(0);
    display.display();
}

void changeStateCredits(){
    game_state = 4;
    drawStateCredits();
}

void drawStateCredits(){
    display.clearDisplay();
    drawUiBorder();

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(WHITE);

    display.setCursor(4, 10);
    display.println("Credits");
    display.drawLine(1, 19, SCREEN_HEIGHT - 1, 19, WHITE);

    display.setCursor(3, 30);
    display.println("Idea from:");
    display.setCursor(3, 40);
    display.println("TinyTetris");
    display.setCursor(3, 50);
    display.println("by:");
    display.setCursor(3, 60);
    display.println("AJRussell");

    display.setCursor(4, SCREEN_WIDTH - 16);
    display.println("> Back");

    display.setRotation(0);
    display.display();
}

void drawUiBorder(){
    display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
}

unsigned long readHighScore(){
    long highScore = EEPROM.read(MEM_SCORE);
    highScore = (highScore << 8) + EEPROM.read(MEM_SCORE + 1);
    highScore = (highScore << 8) + EEPROM.read(MEM_SCORE + 2);

    return highScore;
}

void writeHighScore(long highScore){
    unsigned char byte1 = highScore & 255;
    unsigned char byte2 = (highScore >> 8) & 255;
    unsigned char byte3 = (highScore >> 16) & 255;

    //Reverse order so we can read it later more easily
    EEPROM.write(MEM_SCORE + 2, byte1);
    EEPROM.write(MEM_SCORE + 1, byte2);
    EEPROM.write(MEM_SCORE + 0, byte3);
}

// ################# GAME METHODS #################

void generateCurBlock(){
    curBlock_id = randomBlock();
    curBlock_x = -2;
    curBlock_y = 3;
    curBlock_rot = 0;


    if(!isSpaceForBlock(curBlock_id, curBlock_x, curBlock_y, curBlock_rot)) {
        curBlock_id = -1;
        changeStateDead();
    }
    else {
        setBlock(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, true);
    }
}

void rotateCurBlock(){
    int newRotation = curBlock_rot + 1;
    if(newRotation == 4){
        newRotation = 0;
    }

    setBlockWithoutDrawing(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, false);
    if(isSpaceForBlock(curBlock_id, curBlock_x, curBlock_y, newRotation)) {
        setBlock(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, false);

        curBlock_rot = newRotation;
        setBlock(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, true);
    }
    else {
        setBlockWithoutDrawing(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, true);
    }
}

void moveCurBlock(int vel_x, int vel_y){
    if(curBlock_id == -1) return;

    int futurePos_x = curBlock_x + vel_x;
    int futurePos_y = curBlock_y + vel_y;

    setBlockWithoutDrawing(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, false);
    if(isSpaceForBlock(curBlock_id, futurePos_x, futurePos_y, curBlock_rot)){
        setBlock(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, false);
        setBlock(curBlock_id, futurePos_x, futurePos_y, curBlock_rot, true);
        curBlock_x = futurePos_x;
        curBlock_y = futurePos_y;
    }
    else {
        setBlockWithoutDrawing(curBlock_id, curBlock_x, curBlock_y, curBlock_rot, true);

        // Only if the block moved downwards it will be stopped
        if(vel_x != 0){
            curBlock_id = -1;
            checkFullLine();
        }
    }
}

void checkFullLine(){
    for(int ox=0;ox<4;ox++) {
        int x = curBlock_x + ox;
        bool isFull = true;

        for (int y=0;y<GAME_H;y++){
            int ix = x / 8;
            int rx = x % 8;

            unsigned char b = game[y][ix];
            unsigned char posByte = 0B10000000 >> rx;

            if(!(b & posByte)){
                isFull = false;
                break;
            }
        }

        if(isFull){
            scoreFullLine(x);
        }
    }
}

void scoreFullLine(int x){
    for (int y=0;y<GAME_H;y++){
        drawTile(x, y, false);
        display.display();
        delay(30);
    }

    for(int i=x;i>0;i--){
        for (int y=0;y<GAME_H;y++){
            bool t = getTile(i - 1, y);
            setTile(i, y, t, true);
        }
    }

    for (int y=0;y<GAME_H;y++){
        setTile(0, y, false, true);
    }

    score += 1000;

    if(option_speedup){
        if(timer_wait > 200){
            timer_wait -= 20;
        }
    }

    drawBorder();
}

void setBlockWithoutDrawing(int id, int pos_x, int pos_y, int rot, bool state){
    for(int i=0;i<2;i++){
        unsigned char b = blocks[id][i];

        for(int bi=7;bi>=0;bi--){
            int b_off_x = bi % 4;
            int b_off_y = bi / 4 + i * 2;

            if(b & 1){
                for(int r=0;r<rot;r++){
                    int tmp = b_off_x;
                    b_off_x = 3 - b_off_y;
                    b_off_y = tmp;
                }

                int cur_x = pos_x + b_off_x;
                int cur_y = pos_y + b_off_y;

                if(cur_x < 0 || cur_y < 0 || cur_x >= GAME_W || cur_y >= GAME_H) continue;

                setTile(cur_x, cur_y, state, false);
            }

            b = b >> 1;

            if(b == 0) break;
        }
    }
}

bool isSpaceForBlock(int id, int pos_x, int pos_y, int rot){
    for(int i=0;i<2;i++){
        unsigned char b = blocks[id][i];

        for(int bi=7;bi>=0;bi--){
            int b_off_x = bi % 4;
            int b_off_y = bi / 4 + i * 2;

            if(b & 1){
                for(int r=0;r<rot;r++){
                    int tmp = b_off_x;
                    b_off_x = 3 - b_off_y;
                    b_off_y = tmp;
                }

                int cur_x = pos_x + b_off_x;
                int cur_y = pos_y + b_off_y;

                if(cur_x >= 0) {
                    if (/*cur_x < 0 ||*/ cur_y < 0 || cur_x >= GAME_W || cur_y >= GAME_H) {
                        return false;
                    }

                    if (getTile(cur_x, cur_y)) {
                        return false;
                    }
                }
            }

            b = b >> 1;

            if(b == 0) break;
        }
    }

    return true;
}

void setBlock(int id, int pos_x, int pos_y, int rot, bool state){
    for(int i=0;i<2;i++){
        unsigned char b = blocks[id][i];

        for(int bi=7;bi>=0;bi--){
            int b_off_x = bi % 4;
            int b_off_y = bi / 4 + i * 2;

            if(b & 1){
                for(int r=0;r<rot;r++){
                    int tmp = b_off_x;
                    b_off_x = 3 - b_off_y;
                    b_off_y = tmp;
                }

                setTile(pos_x + b_off_x, pos_y + b_off_y, state, true);
            }

            b = b >> 1;

            if(b == 0) break;
        }
    }
}

// Draws a tile defined by the TILE_SIZE at a position (posx, posy)
void drawTile(int x, int y, bool state){
    display.fillRect(GAME_X + x * (TILE_SIZE + 1), GAME_Y + y * (TILE_SIZE + 1), TILE_SIZE, TILE_SIZE, state ? WHITE : BLACK);
    drawChange = true;
}

void setTile(int x, int y, bool state, bool draw){
    if(x < 0) return;

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

/* ---- [RANDOM BLOCK GENERATION] ----
 * To select a random Block we:
 *
 * 1. Read from an output-pin which is not connected to anything
 * 2. Module that read number by the amount of possible blocks to get a value between 0 - (BLOCKS_COUNT-1)
 * 3. A little check so that 2 same blocks do not spawn as often in succession
 * 3.1. Check if randomBlock was already called and if lastRandomBlock would be the same as the new one
 * 3.2. Read another random value from the unconnected output pin
 * 3.3. check if that new random number 'rr' is uneven
 * 3.4. if it is: raise the id by one and set it to zero if it exceeds the possible values
 */
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

//    display.clearDisplay();

    Serial.println(F("--------------"));
    Serial.print(F("GAME_X: "));
    Serial.println(GAME_X);
    Serial.print(F("GAME_Y: "));
    Serial.println(GAME_Y);
    Serial.print(F("GAME_W: "));
    Serial.println(GAME_W);
    Serial.print(F("GAME_H: "));
    Serial.println(GAME_H);
    Serial.print(F("GAME_BYTE_W: "));
    Serial.println(GAME_BYTE_W);


    display.fillDisplay();

    delay(2000);

    display.clearDisplay();

    delay(2000);

    display.test();

    return;

    changeStateTitle();
}

// Draws the border around the space where the game is played
void drawBorder(){
    display.fillRect(0, 0, GAME_X, SCREEN_HEIGHT, BLACK);

    display.setRotation(3);
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(6,0);
    display.println(score);
    display.setRotation(0);

    display.drawLine(0, 0, SCREEN_WIDTH-1, 0, WHITE);
    display.drawLine(0, SCREEN_HEIGHT-2, SCREEN_WIDTH-1, SCREEN_HEIGHT-2, WHITE);
    display.drawLine(SCREEN_WIDTH-1, 0, SCREEN_WIDTH-1, SCREEN_HEIGHT-2, WHITE);
    display.drawLine(GAME_X-1, 0, GAME_X-1, SCREEN_HEIGHT-2, WHITE);

    display.display();
}

// Displays new changes
void drawGame(){
    if(drawChange) {
        drawChange = false;
        display.display();
    }
}

// ################# LOOP METHOD #################
void loop() {
    return;
    if(game_state == 0){
        loopTitle();
    }
    else if(game_state == 1){
        loopGame();
    }
    else if(game_state == 2){
        loopDead();
    }
    else if(game_state == 3){
        loopOptions();
    }
    else if(game_state == 4){
        loopCredits();
    }
}

void loopTitle(){
    dpad::setState();

    if(dpad::isUp()){
        if(state_title_option == 0) {
            state_title_option = 2;
        }
        else {
            state_title_option--;
        }

        drawStateTitle();
        delay(200);
    }
    else if(dpad::isDown()){
        state_title_option++;
        if(state_title_option == 3) state_title_option = 0;
        drawStateTitle();
        delay(200);
    }
    else if(dpad::isRotate()){
        if(state_title_option == 0) {
            changeStateGame();
            delay(1000);
        }
        else if(state_title_option == 1) {
            changeStateOptions();
            delay(1000);
        }
        else if(state_title_option == 2) {
            changeStateCredits();
            delay(1000);
        }
    }
}

void loopGame(){
    if(curBlock_id != -1){
        dpad::setState();

        if(dpad::isLeft()){
            moveCurBlock(0, 1);
        }
        else if(dpad::isRight()){
            moveCurBlock(0, -1);
        }
        else if(dpad::isDown()){
            moveCurBlock(1, 0);

            drawGame();
            delay(150);
            return;
        }
        else if(dpad::isRotate()){
            rotateCurBlock();
        }

        int now = millis();

        if(now - timer >= timer_wait) {
            timer = now;

            // Moving current block down
            moveCurBlock(1, 0);
        }
    }

    if(curBlock_id == -1) {
        generateCurBlock();
    }

    drawGame();
    delay(100);
}

void loopDead(){
    dpad::setState();

    if(dpad::isUp()){
        if(state_dead_option == 0) {
            state_dead_option = 1;
        }
        else {
            state_dead_option--;
        }
        drawStateDead();
        delay(200);
    }
    else if(dpad::isDown()){
        state_dead_option++;
        if(state_dead_option == 2) state_dead_option = 0;
        drawStateDead();
        delay(200);
    }
    else if(dpad::isRotate()){
        if(state_dead_option == 0) {
            changeStateGame();
            delay(1000);
        }
        else if(state_dead_option == 1){
            changeStateTitle();
            delay(1000);
        }
    }
}

void loopOptions(){
    dpad::setState();

    if(dpad::isUp()){
        if(state_options_option == 0) {
            state_options_option = 2;
        }
        else {
            state_options_option--;
        }
        drawStateOptions();
        delay(200);
    }
    else if(dpad::isDown()){
        state_options_option++;
        if(state_options_option == 3) state_options_option = 0;
        drawStateOptions();
        delay(200);
    }
    else if(dpad::isRotate()){
        if(state_options_option == 0) {
            option_music = !option_music;
            drawStateOptions();
            delay(200);
        }
        else if(state_options_option == 1){
            option_speedup = !option_speedup;
            drawStateOptions();
            delay(200);
        }
        else if(state_options_option == 2){
            changeStateTitle();
            delay(1000);
        }
    }
}

void loopCredits(){
    dpad::setState();

    if(dpad::isRotate()){
        changeStateTitle();
        delay(1000);
    }
}







// TMP UTIL METHODS

//void printCurBlockInfo(){
//    Serial.println("Cur Block:");
//    Serial.print("ID: ");
//    Serial.println(curBlock_id);
//    Serial.print("X: ");
//    Serial.println(curBlock_x);
//    Serial.print("Y: ");
//    Serial.println(curBlock_y);
////    Serial.print("Rot: ");
////    Serial.println(curBlock_rot);
//}
//
//void printByte(unsigned char b){
//    Serial.print("Byte[");
//    while(true){
//        if(b & 1){
//            Serial.print("1");
//        }
//        else {
//            Serial.print("0");
//        }
//
//        b = b >> 1;
//        if(b == 0) break;
//    }
//    Serial.println("]");
//}
//
//void printGame(){
//
//    Serial.println();
//    Serial.println("##########");
//
//    for(int y=0;y<GAME_H;y++){
//        for(int x=0;x<GAME_BYTE_W;x++){
//            unsigned char b = game[y][x];
//
//            Serial.print("[");
//            for(int i=0;i<8;i++){
//                if(b & 1){
//                    Serial.print("1");
//                }
//                else {
//                    Serial.print("0");
//                }
//
//                b = b >> 1;
//            }
//            Serial.print("]");
//        }
//
//        Serial.println();
//    }
//}
