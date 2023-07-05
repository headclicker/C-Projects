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
void input(WINDOW *win, struct SnakeInfo *snake);
void addSegment(struct SnakeInfo *snake);
void placeFruit(struct FruitInfo *fruit, struct SnakeInfo *snake);

bool handleCollision(struct FruitInfo *fruit, struct SnakeInfo *snake, int *score, bool *isGameRunning);

int segmentType(int row, int col, struct SnakeInfo *snake);

int main(void){
        srand(time(NULL));

        int score = 0;
        bool isGameRunning = true;
        int logSize = 0;

        initscr();
        noecho();
        curs_set(0);
        keypad(stdscr, true);
        
        struct SnakeInfo snake;
        struct FruitInfo fruit;

        WINDOW *win = newwin(HEIGHT, WIDTH, (LINES / 2) - (HEIGHT / 2), (COLS / 2) - (WIDTH / 2));
        keypad(win, true);
        wtimeout(win, 120);

        box(win, 0, 0);
        refresh();
        wrefresh(win);

        setup(&fruit, &snake);
        draw(win, &fruit, &snake, &score, &logSize);
        
        while(isGameRunning){
            input(win, &snake);

            /*clearSnake(win, &snake, &logSize); */
            
            if(handleCollision(&fruit, &snake, &score, &isGameRunning)){
                logSize++;
                addSegment(&snake);
            } else {
                mvwaddch(win, snake.body[logSize].segmentY, snake.body[logSize].segmentX, ' ');
                moveSnake(win, &fruit, &snake, &score);
            }

            draw(win, &fruit, &snake, &score, &logSize);

        }
        
        getch();
        endwin();
	return 0;
}

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
                mvwaddch(win, row, col, '0');
            }  else if(fruit->fruitY == row && fruit->fruitX == col) {
                mvwaddch(win, row, col, '*');
            } else if(*(logSize) > 1 && row == snake->body[1].segmentY && col == snake->body[1].segmentX){
                mvwaddch(win, row, col, 'o');
            }            
        }
    }
    mvprintw(1, 1, "Score: %i\n", *score);
    wrefresh(win);
    refresh();
}

/*void draw(WINDOW *win, struct FruitInfo *fruit, struct SnakeInfo *snake, int *score, int *logSize){

    for(int row = 0; row < HEIGHT; row++){
        for(int col = 0; col < WIDTH; col++){
            if(segmentType(row, col, snake) == HEAD){
                mvwaddch(win, row, col, '0');
            }  else if(fruit->fruitY == row && fruit->fruitX == col) {
                mvwaddch(win, row, col, '*');
            } else if(segmentType(row, col, snake) == BODY){
                mvwaddch(win, row, col, 'o');
            }            
        }
    }
    mvprintw(1, 1, "Score: %i\n", *score);
    wrefresh(win);
    refresh();
}*/

/* void clearSnake(WINDOW *win, struct SnakeInfo *snake, int *logSize){
	for(int row = 0; row < HEIGHT; row++){
            for(int col = 0; col < WIDTH; col++){
		if(segmentType(row, col, snake) == HEAD || segmentType(row, col, snake) == BODY){
		    mvwaddch(win, row, col, ' ');
		}
	    }
	}

        wrefresh(win);
        refresh();
}*/

void moveSnake(WINDOW *win, struct FruitInfo *fruit, struct SnakeInfo *snake, int *score){
    if(snake->state == UP || snake->state == DOWN){
		wtimeout(win, 160);
    } else {
		wtimeout(win, 80);
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

void input(WINDOW *win, struct SnakeInfo *snake){
    int c;
    switch((c = wgetch(win))){
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