#ifndef DINO_H
#define DINO_H

#include "position.h"
#include <vector>

using namespace std;

enum STATE_TABLE {
  INPUT_STATE,
  END_STATE
};

class Dino {
    private:
        int winY;
        int winX;

        Position dinoPosRun;
        Position dinoPosJump;
        Position dinoPos;

        int feetPos;
        int velocity;

        bool isjumping;
        bool isRunning;

        bool checkGameOver;

        char STATE;
        
        int dinoCanJump;
        
        double jumpPause;
        double runPause;
        double cactiPause;
        double cactiGenPause;

        int score;

        vector<Position> cactusVec;
        
        void init();
        void initDino(Position);
        void initGround(int, int, int);

        void input();
        
        void dinoRunning(Position, int);
        void dinoJumping(Position);
        void dinoClear(Position);

        void cactus(Position);
        void clearCactus(Position);
        void buildCacti();

        bool checkCollision(Position, Position);

        void gameStart();
        void gameOver();
        void clearMessage();
        
    public:
        Dino();
        ~Dino();

        void loop();
};
#endif