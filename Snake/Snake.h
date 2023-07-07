#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED

#include<ncurses.h>
#include<stdbool.h>
#include<time.h>
#include<stdlib.h>
#include<sys/time.h>

#define HEIGHT 20 
#define WIDTH 40 

#define UP 1
#define RIGHT 2
#define DOWN 3
#define LEFT 4
#define ENDGAME 0

#define MAXSEGMENTS 684

#define EMPTY 0
#define HEAD 1
#define BODY 2

struct FruitInfo {
    unsigned short fruitX;
    unsigned short fruitY;
};

struct SnakeSegment{
    bool isHead;
    unsigned short segmentX;
    unsigned short segmentY;
};

struct SnakeInfo{
    char state;
    short size;
    struct SnakeSegment body[MAXSEGMENTS];
};

void setup(struct FruitInfo *fruit, struct SnakeInfo *snake);
void draw(WINDOW *win, struct FruitInfo *fruit, struct SnakeInfo *snake, int *score, int *logSize);
void clearSnake(WINDOW *win, struct SnakeInfo *snake, int *logSize);
void moveSnake(WINDOW *win,struct FruitInfo *fruit, struct SnakeInfo *snake, int *score);
void input(WINDOW *win, struct SnakeInfo *snake, int c);
void addSegment(struct SnakeInfo *snake);
void placeFruit(struct FruitInfo *fruit, struct SnakeInfo *snake);

bool handleCollision(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score, bool *isGameRunning);

int segmentType(int row, int col, struct SnakeInfo *snake);
int waitFullTurn(WINDOW *win, float time);

float floatAbs(float num);

#endif
