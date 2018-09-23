#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOMngr.h"

/*  Author:   Austin Klum
    Created:  02/11.18
    Resources: https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
               Kevin Urban
               Evan Fisher
*/

#define CONSTRAIN(x,min,max) ( ((x) < (min)) ? (min) : ( ((x) > (max)) ? (max) : (x)))


FILE *sourceFile;
char buffer[MAXLINE];
int bufLen, nextPos;
int curLine;
bool echoMode;
bool needsDisplay;
char highlight[] = "\033[7m";
char underline[] = "\033[4m";
char reset[] = "\033[0m";

struct message {
  int startColumn;
  int endColumn;
  char * message;
};
#define MAX_MESSAGES 26
struct message messages[MAX_MESSAGES];
int messageCnt;

void display();
bool readNextLine();

bool OpenSource(const char * aFileName, bool mode) {
	sourceFile = fopen(aFileName,"r");
  if (!sourceFile) printf("Can't open input file.\n");
	echoMode = mode;
	curLine = 0;
	readNextLine();
	return sourceFile != NULL;
}

void CloseSource() {
  display();
	fclose(sourceFile);
}

char GetSourceChar() {
	if (nextPos >= bufLen) {
		display();
		if(!readNextLine()){
			return EOF;
		}
	}
	return buffer[nextPos++];
}

void PostMessage(int aColumn, int aLength, const char * aMessage) {
  // Make sure inputs is valid
	if (messageCnt > 26 || aColumn > bufLen || aColumn < 0) {
		return;
	}
  messages[messageCnt].startColumn = aColumn;
  messages[messageCnt].endColumn = aLength + aColumn;
  messages[messageCnt].message = strdup(aMessage);
  messageCnt++;
}

int GetCurrentLine() {
	return curLine;
}

int GetCurrentColumn() {
	return nextPos-1;
}
/*
    Prints out the output string with highlighted markers, underlined messages,
    and message list to stdout.
*/
void display() {
  //This is a little ugly, but I was having some issues with the display.
	if(!needsDisplay){
    return;
  }
if(messageCnt == 0) {
  if (echoMode){
    return;
  }
}

	int i;
	char marker = 'A';
	int nextMsg = 0;

  printf("%5d: ", curLine);
	for(i = 0; i < bufLen; i++) {
		if(messageCnt > nextMsg && messages[nextMsg].startColumn == i){
			//Print out highlighted marker
			printf("%s",highlight);
			printf(" %c ",marker++);
			printf("%s",reset);

			//Print out underline msg
			printf("%s",underline);
			while(i < messages[nextMsg].endColumn){
				printf("%c",buffer[i++]);
			}
			printf("%s",reset);
			printf("%c",buffer[i]);


			nextMsg++;
		} else {
			printf("%c",buffer[i]);
		}
	}

	marker = 'A';
	for(i = 0; i < messageCnt; i++) {
		printf("       -%c %s\n", marker++, messages[i].message);
	}
  messageCnt = 0;
  bufLen = 0;
}
/*
  Reads in the next line of the file and resets other vars
  returns bool on weather the fgets() returned a null pointer or not
*/
bool readNextLine() {
  // A little verbose, but again I wanted to be explicit in my code.
	needsDisplay = fgets(buffer, MAXLINE, sourceFile) ? true : false;

  // Reset and set other vars
	bufLen = strlen(buffer);

	nextPos = 0;
	curLine++;
 	messageCnt = 0;

	return needsDisplay;
}
