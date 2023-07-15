#include<ncurses.h>
#include<stdlib.h>
#include<string.h>

#define CTRL_KEY(x) ((x) & 0x1f)

#define MICRO_VERSION "0.0.1"
#define WELCOME_MESSAGE "Micro Editor - version"
#define TAB_STOP 8

#define SCROLL 1

typedef struct textRow {
	int size;
	int rsize;
	char *chars;
	char *render;

} textrow;

struct programData {
	int renderX;
	int rowOffset;
	int colOffset;
	int numRows;
	textrow *rows;
};

struct programData data;

// Output
int scrollViewport();
void drawRows();
void refreshScreen();

// Input
int processInput();
void moveCursor(int key, int *scrollDirection);

// Rows Buffer
void appendRow(char *string, size_t len);
char *substring(char *string, int start, int end);

// File IO
void openFile(char *fileName);

//Initialization
void setup();

int main(int argc, char *argv[]){
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, true);

	setup();
	if(argc >= 2){	
		openFile(argv[1]);
	}
	drawRows();
	move(0, 0);
	refresh();

	while(1){
		int refreshType = processInput();
		if(refreshType == SCROLL){
			refreshScreen();
		}
		refresh();
	}

	endwin();
	return 0;
}


// Output
/* int scrollViewport(){
	int cursorY, cursorX;
	getyx(stdscr, cursorY, cursorX);

	if(cursorY < data.rowOffset){
		data.rowOffset = cursorY;
		return 1;
	}

	if(cursorY >= data.rowOffset + data.numRows){
		data.rowOffset = cursorY - data.numRows + 1;
		return 1;
	}
	return 0;
} */

void drawRows(){
	if(data.numRows < 1){
		int messageLength = strlen(WELCOME_MESSAGE) + strlen(MICRO_VERSION);
		mvprintw(LINES / 3, (COLS - messageLength) / 2, "%s %s", WELCOME_MESSAGE,  MICRO_VERSION);
	}

	for (int row = 0; row < LINES; row++){
		if(row >= data.numRows){
			mvprintw(row, 0, "~");
		} else {
			char newRow[COLS];
			strncpy(newRow, (data.rows[row + data.rowOffset].render + data.colOffset), COLS);
			mvprintw(row, 0, "%s", newRow);
		}
	}

	mvprintw(LINES - 5, COLS - 6, "%d", data.rowOffset);
	mvprintw(LINES - 4, COLS - 6, "%d", data.numRows);
	mvprintw(LINES - 3, COLS - 6, "%d", LINES);
}

void refreshScreen(){
	int cursorY, cursorX;
	getyx(stdscr, cursorY, cursorX);

	clear();
	curs_set(0);
	drawRows();
	move(cursorY, cursorX);
	curs_set(1);
	
}

// Input
void moveCursor(int c, int *scrollDirection){
	int cursorY, cursorX;
	getyx(stdscr, cursorY, cursorX);

	textrow *row = (cursorY >= data.numRows) ? NULL : &data.rows[cursorY];

	switch(c){
		case KEY_UP:
			if(cursorY > 0){
				move(--cursorY, cursorX);		
			} else if (cursorY == 0){
				if(data.rowOffset > 0){
					data.rowOffset--;
					*scrollDirection= SCROLL;
				}
			}
			break;
		case KEY_DOWN:
			if(cursorY < LINES - 1){
				move(++cursorY, cursorX);		
			} else if(cursorY == LINES - 1) {
				if(data.rowOffset < data.numRows - LINES ){
					data.rowOffset++;
					*scrollDirection = SCROLL;
				}
			}
			break;	
		case KEY_LEFT:
			if(cursorX > 0){
				move(cursorY, --cursorX);		
			}else if(cursorX == 0){
				if(data.colOffset > 0){
					data.colOffset--;
					*scrollDirection = SCROLL;
				} else if (cursorY > 0){
					--cursorY;
					move(cursorY, data.rows[cursorY].size);
				}
			}
			break;	
		case KEY_RIGHT:
			if(row && cursorX < row->size){
				move(cursorY, ++cursorX);		
			} else if(cursorX == COLS - 1){
				data.colOffset++;
				*scrollDirection = SCROLL;
			} else if(row && cursorX == row->size){
				move(++cursorY, 0);
			}
			break;	
	}
	
	getyx(stdscr, cursorY, cursorX);
	row = (cursorY >= data.numRows) ? NULL : &data.rows[cursorY];
	int rowLen = row ? row-> size : 0;
	if(cursorX > rowLen){
		move(cursorY, rowLen);
	}
}


int processInput(){
	int cursorY, cursorX;
	getyx(stdscr, cursorY, cursorX);
	int scrollDirection = 0;
	int c = getch();
	switch(c){
		case CTRL_KEY('x'):	
			endwin();
			exit(0);
			break;
			
		case KEY_END:
			move(cursorY, COLS - 1);
			break;
		case KEY_HOME:
			move(cursorY, 0);
			break;

		case KEY_PPAGE:
			move(0, cursorX);
			break;
		case KEY_NPAGE:
			move(LINES - 1, cursorX);
			break;
			
		case KEY_UP:
		case KEY_DOWN:
		case KEY_LEFT:
		case KEY_RIGHT:
			moveCursor(c, &scrollDirection);
			break;
	}
	return scrollDirection;
}

// Row Buffer

void updateRow(textrow *row){
	int tabs = 0;
	for(int i = 0; i < row->size; i++){
		if(row -> chars[i] == '\t'){
			tabs++;
		}
	}

	free(row->render);
	row->render = malloc(row->size + tabs*(TAB_STOP - 1) + 1);
	
	int numChars = 0;
	for(int i = 0; i < row->size; i++){
		if(row->chars[i] == '\t'){
			row->render[numChars++] = ' ';
			while(numChars % TAB_STOP != 0){
				row->render[numChars++] = ' ';
			}
		} else {
			row->render[numChars++] = row->chars[i];	
		}
	}

	row->render[numChars] = '\0';
	row->size = numChars;

	
}

void appendRow(char *string, size_t len){
		data.rows = realloc(data.rows, sizeof(textrow) * (data.numRows + 1));

		int rowIndex = data.numRows;
		data.rows[rowIndex].size = len;
		data.rows[rowIndex].chars = malloc(len + 1);
		memcpy(data.rows[rowIndex].chars, string, len);
		data.rows[rowIndex].chars[len] = '\0';

		data.rows[rowIndex].rsize = 0;
		data.rows[rowIndex].render = NULL;
		updateRow(&data.rows[rowIndex]);

		data.numRows++;
}

// File IO
void openFile(char *fileName){
	FILE *fp = fopen(fileName, "r");
	if(fp == NULL){
		endwin();
		perror("Could not open file");
		exit(0);
	}

	char *line = NULL;
	size_t lineSize = 0;
	ssize_t lineLen;

	while((lineLen = getline(&line, &lineSize, fp)) != -1){
		while((lineLen > 0) && (line[lineLen - 1] == '\n' ||
							    line[lineLen - 1] == '\r')){
			lineLen--;
		}
		appendRow(line, lineLen);
	}

	free(line);
	line = NULL;
	fclose(fp);
	fp = NULL;
}

// Initialization
void setup(){
	data.renderX = 0;
	data.numRows = 0;
	data.rows = NULL;
	data.rowOffset = 0;
	data.colOffset = 0;

	move(0, 0);
}
