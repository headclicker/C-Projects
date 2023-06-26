/* 
* Author: Ben Russell
* Purpose: Create a simple cli habit tracker to practice file i/o
* Date: 6/25/23
*/
#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>

#define MAXLEN 50
#define MAXDIGITS 10
#define PROGRESSFILE "progress.txt"
#define GOALSFILE "goals.txt"

void viewProgress();
void addGoal();
void markGoal();
void removeGoal();
void printOptions();
int countLines(FILE *fp);

int main(void){
	int action;
	bool isRunning = true;

	while(isRunning){
		printf("----------Habit Tracker----------\n1.) View Progress\n2.) Add Goal\n3.) Mark Goal\n4.) Remove Goal\n5.) Quit\n");
		scanf("%d", &action);
		getchar(); //consume leftover "enter"

		switch(action){
			case 1:
				viewProgress();
				break;	
			case 2:
				addGoal();
				break;
			case 3:
				markGoal();
				break;
			case 4:
				removeGoal();
				break;
			default:
				isRunning = false;
				break;
		}
	}

	return 0;
}

/* void viewProgress(){
	int numLines;
	char *goal = (char *) malloc(MAXLEN * sizeof(char));
	char *streak = (char *) malloc(MAXDIGITS * sizeof(char));

	FILE *goalsFile = fopen(GOALSFILE, "r");
	if(goalsFile == NULL){
		perror("Could not open file");
		exit(0);
	}

	FILE *progressFile = fopen(PROGRESSFILE, "r");
	if(progressFile == NULL){
		perror("Could not open file");
		exit(0);
	}

	numLines = countLines(goalsFile);	

	for(int i = 0; i < numLines; i++){
		fgets(goal, MAXLEN, goalsFile);
		fgets(streak, MAXDIGITS, progressFile);
		printf("%s%s\n", goal, streak);
	}
	
	free(goal);
	goal = NULL;
	free(streak);
	streak = NULL;

	fclose(goalsFile);
	goalsFile = NULL;

	fclose(progressFile);
	progressFile = NULL;
} */

void viewProgress(){
	time_t t = time(NULL);
	printf("%ld\n", t);

	struct tm *time = gmtime(&t);

	printf("%d/%d\n", (time->tm_mon) + 1, time->tm_mday);
}

void addGoal(){
	char *goal = (char *) malloc(MAXLEN * sizeof(char));

	FILE *fp = fopen(GOALSFILE, "a");
	if(fp == NULL){
		perror("Could not open file");
		exit(0);
	}

	printf("Enter a new goal: ");
	fgets(goal, MAXLEN, stdin);

	fputs(goal, fp);

	free(goal);
	goal = NULL;

	fclose(fp);
	fp = fopen(PROGRESSFILE, "a");
	if(fp == NULL){
		perror("Could not open file");
		exit(0);
	}

	fputs("0\n", fp);

	fclose(fp);
	fp = NULL;
}

void markGoal(){
	int numLines, selection;
	FILE *fp = fopen(PROGRESSFILE, "r");
	if(fp == NULL){
		perror("Could not open file");
		exit(0);
	}	

	numLines = countLines(fp);
	char *currentLine = (char *) malloc(MAXDIGITS * sizeof(char));
	int *buffer = (int *) malloc(numLines * sizeof(int));

	for(int i = 0; i < numLines; i++){
		fgets(currentLine, MAXDIGITS, fp);
		*(buffer + i) = atoi(currentLine);	
	}
	
	free(currentLine);
	currentLine = NULL;

	fclose(fp);
	fp = fopen(PROGRESSFILE, "w");

	printOptions();
	printf("Which goal would you like to mark? (1-%d)", numLines);
	scanf("%d", &selection);

	(*(buffer + selection - 1))++;

	for(int i = 0; i < numLines; i++){
		fprintf(fp, "%d\n", *(buffer + i));	
	}

	free(buffer);
	buffer = NULL;

	fclose(fp);
	fp = NULL;
}

void removeGoal(){
	int numLines, selection;
	FILE *fp = fopen(GOALSFILE, "r");
	if(fp == NULL){
		perror("Could not open file");
		exit(0);
	}
	numLines = countLines(fp);
	fclose(fp);
	fp = NULL;

	printOptions();
	printf("Which goal would you like to remove? (1-%d)", numLines);
	scanf("%d", &selection);

	fp = fopen(GOALSFILE, "r");
	if(fp == NULL){
		perror("Could not open file");
		exit(0);
	}

	char *currentLine = (char *) malloc(MAXLEN * sizeof(char));
	char goalBuffer[numLines][MAXLEN]; 

	for(int i = 0; i < numLines; i++){
		fgets(currentLine, MAXLEN, fp);
		strncpy(goalBuffer[i], currentLine, MAXLEN);	
	}

	free(currentLine);
	currentLine = NULL;

	fclose(fp);
	fp = fopen(GOALSFILE, "w");

	for(int i = 0; i < numLines; i++){
		if(i != selection - 1){
			fputs(goalBuffer[i], fp);
		}
	}

	fclose(fp);
	fp = fopen(PROGRESSFILE, "r");

	currentLine = (char *) malloc(MAXDIGITS * sizeof(char));
	int *progressBuffer = (int *) malloc(numLines * sizeof(int));

	for(int i = 0; i < numLines; i++){
		fgets(currentLine, MAXDIGITS, fp);
		*(progressBuffer + i) = atoi(currentLine);	
	}

	free(currentLine);
	currentLine = NULL;

	fclose(fp);
	fp = fopen(PROGRESSFILE, "w");
	
	for(int i = 0; i < numLines; i++){
		if(i != selection - 1){
			fprintf(fp, "%d\n", progressBuffer[i]);
		}
	}	

	fclose(fp);
	fp = NULL;
}

void printOptions(){
	FILE *fp = fopen(GOALSFILE, "r");	
	if(fp == NULL){
		perror("Could not open file");
		exit(0);
	}

	int numLines = countLines(fp);
	char *currentLine = (char *) malloc(MAXLEN * sizeof(char));

	for(int i = 0; i < numLines; i++){
		fgets(currentLine, MAXLEN, fp);
		printf("%d.) %s\n", i + 1, currentLine);
	}

	free(currentLine);
	currentLine = NULL;

	fclose(fp);
	fp = NULL;
}

int countLines(FILE *fp){
	int c, numLines = 0;

	while((c = fgetc(fp)) != EOF){
		if(c == '\n'){
			numLines++;
		}
	}

	rewind(fp);
	return numLines;
}

