/*
* Author: Ben Russell
* Purpose: Create a tic tac toe game
* Date: 5/21/23
*/
#include <ctype.h>
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

bool checkForWin(char gameBoard[3][3]);
void drawBoard(char gameBoard[3][3]);
void markBoard(char gameBoard[3][3], int player);
bool isValid(char gameBoard[3][3], int position);

int main() {
	char gameBoard[3][3] = {
		{'1', '2', '3'},
		{'4', '5', '6'},
		{'7', '8', '9'}
	};
	bool isGameRunning = true;
	int count = 1;
	while(isGameRunning){
		int player = count % 2;
		if(player == 0) {
			player = 2;
		}
		drawBoard(gameBoard);
		markBoard(gameBoard, player);
		if(checkForWin(gameBoard)){
			isGameRunning = false;
		}
		count++;
	}
	drawBoard(gameBoard);
	return 0;
}

bool checkForWin(char gameBoard[3][3]){
	bool doesWinnerExist = false;
	bool isBoardFull = true;
	for(int row = 0; row < 3; row++){
		if(gameBoard[row][0] == gameBoard[row][1] && gameBoard[row][1] == gameBoard[row][2]){
			doesWinnerExist = true;
		}
	}
	for(int col = 0; col < 3; col++){
		if(gameBoard[0][col] == gameBoard[1][col] && gameBoard[1][col] == gameBoard[2][col]) {
			doesWinnerExist = true;
		}
	}
	if(gameBoard[0][0] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][2]) {
		doesWinnerExist = true;
	}
	if(gameBoard[0][2] == gameBoard[1][1] && gameBoard[1][1] == gameBoard[2][0]){
		doesWinnerExist = true;
	}
	for(int row = 0; row < 3; row++){
		for(int col = 0; col < 3; col++){
			if(isdigit(gameBoard[row][col])) {
				isBoardFull = false;
			}
		}
	}
	if(isBoardFull) {
		doesWinnerExist = true;
	}
	return doesWinnerExist;
}

void drawBoard(char gameBoard[3][3]){
	system("clear");	
	printf("\n	    Tic Tac Toe	          \n");
	printf("    Player 1 - (X) Player 2 - (O)    \n\n");
	printf("              |     |     \n");
	printf("           %c  |  %c  |  %c  \n", gameBoard[0][0], gameBoard[0][1], gameBoard[0][2]);
	printf("         _____|_____|_____\n");
	printf("              |     |     \n");
	printf("       	   %c  |  %c  |  %c  \n", gameBoard[1][0], gameBoard[1][1], gameBoard[1][2]);
	printf("	 _____|_____|_____\n");
	printf("	      |     |     \n");
	printf("	   %c  |  %c  |  %c  \n", gameBoard[2][0], gameBoard[2][1], gameBoard[2][2]);
	printf("	      |     |     \n\n");
}

void markBoard(char gameBoard[3][3], int player){
	int positionInt;
	char positionChar;
	char playerChar;
	bool isPositionValid = true;

	if(player == 1) {
		playerChar = 'X';
	} else {
		playerChar = 'O';
	}

	printf("Player %i, pick a square\n", player);
	scanf("%i", &positionInt);
	isPositionValid = isValid(gameBoard, positionInt);
	while(isPositionValid == false){
		printf("Invalid Selection, pick another square\n");
		scanf("%i", &positionInt);
		isPositionValid = isValid(gameBoard, positionInt);
	}

	positionChar = positionInt + '0';
	for(int row = 0; row < 3; row++){
		for(int col = 0; col < 3; col++){
			if(gameBoard[row][col] == positionChar) {
				gameBoard[row][col] = playerChar;	
			}	
		}
	}
}

bool isValid(char gameBoard[3][3], int position){	
	int count = 0;
	if(position > 9 || position < 1){
		return false;
	}
	for(int row = 0; row < 3; row++){
		for(int col = 0; col < 3; col++){
			count++;
			if(count == position && isalpha(gameBoard[row][col])){
				return false;
			}
		}
	}
	return true;
	
}

