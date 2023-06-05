/* 
* Author: Ben Russell
* Purpose: Create a rudimentary version of the popular video game C
* Date: 4/6/23
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

void placeFruit();
void input();
void takeTurn(char board[HEIGHT][WIDTH]);
void setup(char board[HEIGHT][WIDTH]);
void draw(char board[HEIGHT][WIDTH]);
long msleep(const long ms); //not mine - used in place of windows exclusive sleep() function

unsigned int snakeX, snakeY;
unsigned int fruitX, fruitY;
unsigned short snakeState;
unsigned int score;

int main(void){
	// initialize random number generator	
	srand(time(0));

	char gameBoard[HEIGHT][WIDTH];

	setup(gameBoard);
	while(snakeState){
		input();
		takeTurn(gameBoard);
		draw(gameBoard);
	}
	printf("Final Score: %d\n", score);
	return 0;
}

void setup(char board[HEIGHT][WIDTH]){
	score = 0;
	snakeState = RIGHT;
	snakeX = (WIDTH / 2);
	snakeY = (HEIGHT / 2);
	placeFruit();
	for(int row = 0; row < HEIGHT; row++){
		printf("Final Score: %d\n", score);
		for(int col = 0; col < WIDTH; col++){
			if(row == 0 || row == HEIGHT - 1 || col == 0 || col == WIDTH - 1){
				board[row][col] = '#';
			} else if(row == snakeY && col == snakeX){
				board[row][col] = '0';
			} else if(row == fruitY && col == fruitX){
				board[row][col] = '*';
			} else {
				board[row][col] = ' ';
			}
		}
     	}
}

void placeFruit(){
	fruitY = (rand() % (HEIGHT - 2)) + 1;
	fruitX = (rand() % (WIDTH - 2)) + 1;
}

void input(){
	if(kbhit()){
		switch(getch()){
			case 'w':
				snakeState = UP;
				break;
			case 'd': 
				snakeState = RIGHT;
				break;
			case 's':
				snakeState = DOWN;
				break;
			case 'a':
				snakeState = LEFT;
				break;
			case 'x':
				snakeState = ENDGAME;
				break;
		}
	}
}

void draw(char board[HEIGHT][WIDTH]){
	system("clear");
	for(int row = 0; row < HEIGHT; row++){
		for(int col = 0; col < WIDTH; col++){
			putchar(board[row][col]);
		}
		putchar('\n');
	}
	printf("Score: %d\n", score);
	// printf("\nScore: %d\nFruitX: %d, FruitY: %d\nSnakeX: %d, SnakeY: %d\n", score, fruitX, fruitY, snakeX, snakeY);
}

void takeTurn(char board[HEIGHT][WIDTH]){
	if(snakeState == UP || snakeState == DOWN){
		msleep(100);
	} else {
		msleep(50);
	}
	switch(snakeState){
		case UP:
			board[snakeY][snakeX] = ' ';
			snakeY--;
			board[snakeY][snakeX] = '0';
			break;
		case RIGHT:
			board[snakeY][snakeX] = ' ';
			snakeX++;
			board[snakeY][snakeX] = '0';
			break;
		case DOWN:
			board[snakeY][snakeX] = ' ';
			snakeY++;
			board[snakeY][snakeX] = '0';
			break;
		case LEFT: 
			board[snakeY][snakeX] = ' ';
			snakeX--;
			board[snakeY][snakeX] = '0';
			break;
	}

	

	if(snakeX == fruitX && snakeY == fruitY){
		score++;
		board[fruitY][fruitX] = ' ';
		placeFruit();
		board[fruitY][fruitX] = '*';

	} else if (snakeX < 1 || snakeX > WIDTH - 2 || snakeY < 1 || snakeY > HEIGHT - 2){
		snakeState = ENDGAME;
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
