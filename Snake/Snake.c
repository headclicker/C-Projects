#include "Snake.h"
void setup(struct FruitInfo *fruit, struct SnakeInfo *snake){
    placeFruit(fruit, snake);

    snake->state = RIGHT;
    snake->size = 1;

    snake->body[0].isHead = true;
    snake->body[0].segmentX = WIDTH / 2;
    snake->body[0].segmentY = HEIGHT / 2;

}

void draw(WINDOW *win, struct FruitInfo *fruit, struct SnakeInfo *snake, int *score, int *logSize){
    for(int row = 0; row < HEIGHT; row++){
        for(int col = 0; col < WIDTH; col++){
            if(segmentType(row, col, snake) == HEAD){
                wattron(win, COLOR_PAIR(1));
                mvwaddch(win, row, col, '0');
                wattroff(win, COLOR_PAIR(1));
            }  else if(fruit->fruitY == row && fruit->fruitX == col) {
                wattron(win, COLOR_PAIR(2));   
                mvwaddch(win, row, col, ACS_DIAMOND);
                wattroff(win, COLOR_PAIR(2));
            } else if(*(logSize) > 0 && row == snake->body[1].segmentY && col == snake->body[1].segmentX){
                wattron(win, COLOR_PAIR(1));
                mvwaddch(win, row, col, 'o');
                wattroff(win, COLOR_PAIR(1));
            }            
        }
    }
    mvprintw(LINES - HEIGHT - 4, (COLS / 2) - (WIDTH / 2) + 1, "Score: %i\n", *score);
    wrefresh(win);
    refresh();
}


void moveSnake(WINDOW *win, struct FruitInfo *fruit, struct SnakeInfo *snake, int *score){

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

void input(WINDOW *win, struct SnakeInfo *snake, int c){
    switch(c){
	case KEY_UP:
            if(snake->state == DOWN){
                break;
            }
	    snake->state = UP;
	    break;
	case KEY_RIGHT: 
            if(snake->state == LEFT){
                break;
            }
	    snake->state = RIGHT;
	    break;
	case KEY_DOWN:
            if(snake->state == UP){
                break;
            }
	    snake->state = DOWN;
	    break;
	case KEY_LEFT:
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

void placeFruit(struct FruitInfo *fruit, struct SnakeInfo *snake){
        fruit->fruitY = (rand() % (HEIGHT - 2)) + 1;
		fruit->fruitX = (rand() % (WIDTH - 2)) + 1; 

        if(segmentType(fruit->fruitY, fruit->fruitX, snake)){
            placeFruit(fruit, snake);
        }
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

int waitFullTurn(WINDOW *win, float waitTime){
    wtimeout(win, waitTime); 

    int c;
    float timeOne, timeTwo;
    struct timeval currentTime;

    gettimeofday(&currentTime, NULL);
    timeOne = (float) (currentTime.tv_usec) / 1000;

    if((c = wgetch(win))){
        gettimeofday(&currentTime, NULL);
        timeTwo = (float) (currentTime.tv_usec) / 1000;
        int diff = floatAbs(timeOne - timeTwo); 

        if((waitTime - diff) < 5){
            return c;
        } else {
            waitFullTurn(win, (waitTime - diff));
        }

    }

    return c; 
}

float floatAbs(float num){
    return (num < 0) ? (num * -1) : (num);
}
