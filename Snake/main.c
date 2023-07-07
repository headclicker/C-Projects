#include "Snake.h"
int main(void){
        srand(time(NULL));

        int score = 0;
        int logSize = 0;
        int c;
        short turn = 1;
        bool isGameRunning = true;

        initscr();
        noecho();
        curs_set(0);
        start_color();
        keypad(stdscr, true);
        init_color(COLOR_GREEN, 0, 1000, 0); 
        init_color(COLOR_RED, 1000, 0, 0);
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_RED, COLOR_BLACK);


        
        struct SnakeInfo snake;
        struct FruitInfo fruit;

        WINDOW *win = newwin(HEIGHT, WIDTH, LINES - HEIGHT - 3, (COLS / 2) - (WIDTH / 2));
        keypad(win, true);
        wtimeout(win, 120);

        box(win, 0, 0);
        refresh();
        wrefresh(win);

        setup(&fruit, &snake);
        draw(win, &fruit, &snake, &score, &logSize);
        
        while(isGameRunning){
            turn *= -1;

            if(snake.state == UP || snake.state == DOWN){
                c = waitFullTurn(win, 140);
            } else {
                c = waitFullTurn(win, 70);
            }
            
            input(win, &snake, c);
            
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
