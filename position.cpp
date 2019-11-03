#include "position.h"

Position::Position(){}

Position::~Position(){}

Position::Position(int y, int x){
    this->y = y;
    this->x = x;
}

void Position::setPosition(Position pos){
    this->y = pos.y;
    this->x = pos.x;
}

int Position::getX(){
    return this->x;
}

void Position::setX(int x){
    this->x = x;
}

int Position::getY(){
    return this->y;
}

void Position::setY(int y){
    this->y = y;
}