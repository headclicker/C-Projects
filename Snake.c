/* 
* Author: Ben Russell
* Purpose: Re-create the popular video game as a command line app in C 
* Date: 6/13/23 
*/
#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>
#include<conio.h> // third party conio but it should function the same
#include<unistd.h>
#include<errno.h>

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

void addSegment(struct SnakeInfo *snake);
void move(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score);
void draw(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score);
void setup(struct FruitInfo *fruit, struct SnakeInfo *snake);
void placeFruit(struct FruitInfo *fruit, struct SnakeInfo *snake);
void input(struct SnakeInfo *snake);

bool handleCollision(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score, bool *isGameRunning);

int segmentType(int row, int col, struct SnakeInfo *snake);

long msleep(const long ms); //not mine - used in place of windows exclusive sleep() function

int main(void){
        srand(time(NULL));

        int score = 0;
        bool isGameRunning = true;

        struct SnakeInfo snake;
        struct FruitInfo fruit;

        setup(&fruit, &snake);
        draw(&fruit, &snake, &score);

        while(isGameRunning){
            input(&snake);
            
            if(handleCollision(&fruit, &snake, &score, &isGameRunning)){
                addSegment(&snake);
            } else {
                move(&fruit, &snake, &score);
            }

            draw(&fruit, &snake, &score);
        }
	return 0;
}

bool handleCollision(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score, bool *isGameRunning){
    if(snake->body[0].segmentX < 1 || snake->body[0].segmentX > WIDTH - 2 
    || snake->body[0].segmentY < 1 || snake->body[0].segmentY > HEIGHT - 2){
        *isGameRunning = false;
        return false;
    }

    for(int i = 1; i < snake->size; i++){
        if(snake->body[0].segmentX == snake->body[i].segmentX && snake->body[0].segmentY == snake->body[i].segmentY){
            *isGameRunning = false;
            return false;
        }
    }

    if(snake->body[0].segmentX == fruit->fruitX && snake->body[0].segmentY == fruit->fruitY){
        (*score)++;
        placeFruit(fruit, snake);
        return true;
    }
    return false;
}

void addSegment(struct SnakeInfo *snake){
    struct SnakeSegment temp;

    temp.segmentX = snake->body[0].segmentX;
    temp.segmentY = snake->body[0].segmentY;

    switch (snake->state){
        case UP:
            (snake->body[0].segmentY)--;
            break;
        case RIGHT:
            (snake->body[0].segmentX)++;
            break;
        case DOWN:
            (snake->body[0].segmentY)++;
            break;
        case LEFT:
            (snake->body[0].segmentX)--;
            break;
        case ENDGAME:
            break;
    }
    
    for(int i = snake->size; i > 1; i--){
        snake->body[i].isHead = false;
        snake->body[i].segmentX = snake->body[i-1].segmentX;
        snake->body[i].segmentY = snake->body[i-1].segmentY;
    }
    
    snake->body[1].isHead = false;
    snake->body[1].segmentX = temp.segmentX;
    snake->body[1].segmentY = temp.segmentY;

    (snake->size)++;
}

void move(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score){
    if(snake->state == UP || snake->state == DOWN){
		msleep(120);
    } else {
		msleep(60);
    }

    struct SnakeSegment temp1;  
    struct SnakeSegment temp2;

    temp1.segmentX = snake->body[0].segmentX;
    temp1.segmentY = snake->body[0].segmentY;

    switch (snake->state){
        case UP:
            (snake->body[0].segmentY)--;
            break;
        case RIGHT:
            (snake->body[0].segmentX)++;
            break;
        case DOWN:
            (snake->body[0].segmentY)++;
            break;
        case LEFT:
            (snake->body[0].segmentX)--;
            break;
        case ENDGAME:
            break;
    }

    for(int i = 1; i < snake->size; i++){
        temp2.segmentX = snake->body[i].segmentX;
        temp2.segmentY = snake->body[i].segmentY;

        snake->body[i].segmentX = temp1.segmentX;
        snake->body[i].segmentY = temp1.segmentY;

        temp1.segmentX = temp2.segmentX;
        temp1.segmentY = temp2.segmentY;
    }

}

int segmentType(int row, int col, struct SnakeInfo *snake){
    for(int i = 0; i < snake->size; i++){
        if(snake->body[i].segmentY == row && snake->body[i].segmentX == col){
            if(snake->body[i].isHead){
                return HEAD;
            } else {
                return BODY;
            }
        }
    }

    return EMPTY;
}

void draw(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score){
    system("clear");

    for(int row = 0; row < HEIGHT; row++){
        for(int col = 0; col < WIDTH; col++){
            if(segmentType(row, col, snake) == HEAD){
                putchar('0');
            } else if(row == 0 || row == HEIGHT - 1 || col == 0 || col == WIDTH - 1){
                putchar('#');
            } else if(fruit->fruitY == row && fruit->fruitX == col) {
                putchar('*');
            } else if(segmentType(row, col, snake) == BODY){
                putchar('o');
            } else {
                putchar(' ');
            }
        }
        putchar('\n');
    }

    printf("Score: %i\n", *score);
}

void setup(struct FruitInfo *fruit, struct SnakeInfo *snake){
    placeFruit(fruit, snake);

    snake->state = RIGHT;
    snake->size = 1;

    snake->body[0].isHead = true;
    snake->body[0].segmentX = WIDTH / 2;
    snake->body[0].segmentY = HEIGHT / 2;

}

void placeFruit(struct FruitInfo *fruit, struct SnakeInfo *snake){
        fruit->fruitY = (rand() % (HEIGHT - 2)) + 1;
	fruit->fruitX = (rand() % (WIDTH - 2)) + 1; 

        if(segmentType(fruit->fruitY, fruit->fruitX, snake)){
            placeFruit(fruit, snake);
        }
}

void input(struct SnakeInfo *snake){
	if(kbhit()){
		switch(getch()){
			case 'w':
                                if(snake->state == DOWN){
                                    break;
                                }
				snake->state = UP;
				break;
			case 'd': 
                                if(snake->state == LEFT){
                                    break;
                                }
				snake->state = RIGHT;
				break;
			case 's':
                                if(snake->state == UP){
                                    break;
                                }
				snake->state = DOWN;
				break;
			case 'a':
                                if(snake->state == RIGHT){
                                    break;
                                }
				snake->state = LEFT;
				break;
			case 'x':
				snake->state = ENDGAME;
				break;
		}
	}
}

long msleep(const long ms) //Not mine - used in place of windows exclusive sleep() function
{
    struct timespec  req, rem;

    if (ms <= 0L)
        return 0L;

    req.tv_sec = ms / 1000L;
    req.tv_nsec = (ms % 1000L) * 1000000L;
    rem.tv_sec = 0;
    rem.tv_nsec = 0;

    if (nanosleep(&req, &rem) == -1) {
        if (errno == EINTR)
            return (long)rem.tv_sec * 1000L + rem.tv_nsec / 1000000L;
        else
            return ms;
    } else
        return 0L;
}

