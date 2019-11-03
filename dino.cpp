#include <iostream>
#include <curses.h>
#include <random>

#include "position.h"
#include "dino.h"

using namespace std;

Dino::Dino() {
    init();

    isRunning = false;
    dinoCanJump = 0;
    changeThisLater = 1;
    velocity = 5;

    jumpPause = 0;
    runPause = 0;
    cactiPause = 0;
    cactiGenPause = 0;

    dinoPosRun = Position(13, 7);
    dinoPosJump = Position(2, 7);
    dinoPos = dinoPosRun;
    
    checkGameOver = false;
}

Dino::~Dino() {
    endwin();
}

int main() {
    setlocale(LC_ALL, "");

    Dino *dino = new Dino();
    dino -> loop();

    delete dino;

    return 0;
}

void Dino::loop() {
    char input;
    string scoreString;
    initGround(20, 0, winX);
    while(true){
        input = getch();
        switch (input) {
            case ' ':
                if(checkGameOver){
                    clearMessage();
                }
                dinoJumping(dinoPosJump);
                break;
            default:
                if(!checkGameOver && jumpPause <= 0){
                    if(!isRunning){
                        dinoClear(dinoPosJump);
                        initDino(dinoPosRun);
                        isRunning = true;
                    } else{
                        dinoRunning(dinoPosRun, velocity);
                    }
                }
                break;
        }
        buildCacti();
        jumpPause -= 1;
        runPause -= 1;
        cactiPause -= 1;
        cactiGenPause -= 1;
        dinoCanJump -= 1;
    }
}

void Dino::init() {
    initscr();
    noecho();
    cbreak();
    nodelay(stdscr, true);
    getmaxyx(stdscr, winY, winX);
    keypad(stdscr, true);
    curs_set(0);
}

void Dino::initGround(int groundLine, int begin, int end){
    for(int i = begin; i < end; i++){
        mvprintw(groundLine, i, "_");
    }
}

void Dino::initDino(Position pos){
    mvprintw(pos.getY(), pos.getX()+7, "▄█████▄");
    mvprintw(pos.getY()+1, pos.getX()+7, "█▄█████");
    mvprintw(pos.getY()+2, pos.getX()+7, "████▄▄ ");
    mvprintw(pos.getY()+3, pos.getX()+6, "█████");
    mvprintw(pos.getY()+4, pos.getX(), "▐    ██████▄▄");
    mvprintw(pos.getY()+5, pos.getX()+1, "█▄ ███████ ▀");
    mvprintw(pos.getY()+6, pos.getX()+2, "████████");
}

// for testing purposes
void rectangle(int y1, int x1, int y2, int x2)
{
    mvhline(y1, x1, 0, x2-x1);
    mvhline(y2, x1, 0, x2-x1);
    mvvline(y1, x1, 0, y2-y1);
    mvvline(y1, x2, 0, y2-y1);
    mvaddch(y1, x1, ACS_ULCORNER);
    mvaddch(y2, x1, ACS_LLCORNER);
    mvaddch(y1, x2, ACS_URCORNER);
    mvaddch(y2, x2, ACS_LRCORNER);
}

void Dino::dinoRunning(Position pos, int v){
    if(runPause <= 0){
        mvprintw(pos.getY()+7, pos.getX()+3, "        ");
        mvprintw(pos.getY()+8, pos.getX()+3, "         ");  
        if (changeThisLater == 1) {
            mvprintw(pos.getY()+7, pos.getX()+2, " █    █▄ ");
            mvprintw(pos.getY()+8, pos.getX()+2, " ▀▀");         
        } else {
            mvprintw(pos.getY()+7, pos.getX()+2, " █▄   █");
            mvprintw(pos.getY()+8, pos.getX()+7, " ▀▀");
        }
        changeThisLater *= -1;
        refresh();
        dinoPos = dinoPosRun;
        runPause = 50000;
    }
}

void Dino::dinoJumping(Position pos){
    if(dinoCanJump <= 0 && jumpPause <= 0){
        dinoClear(dinoPosRun);
        initGround(20, dinoPosRun.getX(), dinoPosRun.getX()+17);
        initDino(dinoPosJump);
        mvprintw(pos.getY()+7, pos.getX(), "  █▄    █▄");
        refresh();
        dinoPos = dinoPosJump;
        isRunning = false;
        dinoCanJump = 10000;
        jumpPause = 150000;
    }
}

void Dino::dinoClear(Position pos){
    for(int i = 0; i < 9; i++){
        mvprintw(pos.getY()+i, pos.getX(), "              ");
    }
}

void Dino::cactus(Position pos){
    mvprintw(pos.getY()-1, pos.getX()-5, "██");
    mvprintw(pos.getY(), pos.getX()-5, "██");
    refresh();
}

void Dino::clearCactus(Position pos){
    for(int i = 0; i < 2; i++){
        mvprintw(pos.getY()-i, pos.getX()-5, "  "); 
    }
    initGround(20, pos.getX()-5, pos.getX()+5);
    refresh();
}

void Dino::buildCacti(){
    if(cactiPause <= 0){
        if(!cactusVec.empty()){
            for(int i = 0; i < cactusVec.size(); i++){
                Position cactiPos = cactusVec.at(i);
                if(checkCollision(dinoPos, cactiPos)){
                    gameOver();
                    return;
                }
                if(cactiPos.getX() <= 0){
                    cactusVec.erase(cactusVec.begin());
                } else {
                    cactusVec.at(i) = Position(20, cactiPos.getX()-1);
                }
                clearCactus(cactiPos);
                cactiPos = cactusVec.at(i);
                cactus(cactiPos);
            }
            cactiPause = 6000;
        }
        if((cactiGenPause <= 0) && (cactusVec.size() <= 3) && (rand() % 100) > 80) {
            cactusVec.push_back(Position(20, winX));
            cactiGenPause = 500000;
        }
    }   
}

void Dino::gameStart(){
    mvprintw((winY / 2) + 1, (winX / 2) - 9, "PRESS SPACE TO START");
}

void Dino::gameOver(){
    checkGameOver = true;
    mvprintw((winY / 2) - 1, (winX / 2) - 4, "GAME OVER");
    refresh();
    napms(100);
    gameStart();
    napms(300);
}

void Dino::clearMessage(){
    mvprintw((winY / 2) - 1, (winX / 2) - 4, "         ");
    mvprintw((winY / 2) + 1, (winX / 2) - 9, "                    ");
    checkGameOver = false;
}
bool valueInRange(int value, int min, int max)
{ return (value >= min) && (value <= max); }

bool Dino::checkCollision(Position dino, Position cacti) {
    bool xOverlap = valueInRange(dino.getX(), cacti.getX()-5, cacti.getX()-5) ||
                    valueInRange(cacti.getX()-5, dino.getX(), dino.getX() + 13);

    bool yOverlap = valueInRange(dino.getY()-1, cacti.getY(), cacti.getY() + 2) ||
                    valueInRange(cacti.getY(), dino.getY()-1, dino.getY()-1 + 9);

    return xOverlap && yOverlap;
}