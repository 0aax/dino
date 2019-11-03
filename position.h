#ifndef POSITION_H
#define POSITION_H
class Position {
    private:
        int y;
        int x;

    public:
        Position();
        ~Position();

        Position(int, int);

        void setPosition(Position);
        void setX(int);
        void setY(int);
        int getY();
        int getX();

};
#endif