#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "IOMngr.h"
//https://www.tutorialspoint.com/c_standard_library/c_function_fgets.htm
#define CONSTRAIN(x,min,max) ( ((x) < (min)) ? (min) : ( ((x) > (max)) ? (max) : (x)))
#define ESC_SEQ_LEN 7

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
void readNextLine();

bool OpenSource(const char * aFileName, bool mode) {
	FILE *sourceFile = fopen(aFileName,"r");
	echoMode = mode;
	curLine = 1;
	readNextLine();
	return sourceFile != NULL;
}

void CloseSource() {
	fclose(sourceFile);
}

/* GetSourceChar  Returns the next character in the source file. This may result
                  in the current line and associated messages being written to
                  stdout. The EOF character is returned if the source file is
                  at the end of file when this function is called.
*/
char GetSourceChar() {
	if (nextPos > bufLen) {
		display();
		readNextLine();
	}
	return buffer[nextPos++];;
}

/* PostMessage    Registers a message to be associated with the current line
                  and indicated by a marker at aColumn with aLength many
                  characters written in underline mode. The letters 'A' ... 'Z'
                  are used as the markers, consequently, a maximum of 26
                  message lines are allowed for each source line. Calls to
                  PostMessage beyond this limit are silently ignored as are calls
                  with aColumn outside the range of the current line or overlaping
                  the prior message.
*/
void PostMessage(int aColumn, int aLength, const char * aMessage) {
	if (messageCnt > 26 || aColumn > bufLen || aColumn < 0) {
		return;
	}
	struct message msg = malloc(sizeof(struct message));
	  msg->endColumn = aLength + msg->startColumn = aColumn;
	  msg->message = aMessage;
	  messages[messageCnt++] = msg;
}

int GetCurrentLine() {
	return curLine;
}

int GetCurrentColumn() {
	return nextPos-1;
}

void display() {
	int i;
	char marker = 'A';
	int nextMsg = 0;
	printf("%d: ", curLine);
	for(i = 0; i < bufLen; i++) {
		if(messages[nextMsg]->startColumn == i){
			//Print out highlighted marker
			printf(highlight);
			printf(" %c ",marker++);
			printf(reset);

			//Print out underline msg
			printf(underline);
			while(i < messages[nextMsg]->endColumn){
				printf("%c",buffer[i++]);
			}
			printf(reset);

			nextMsg++;
		} else {
			printf("%c",buffer[i]);
		}
	}

	marker = 'A';
	for(i = 0; i < messageCnt; i++) {
		printf("\t-%c %s", marker++, messages[i]->message);
	}
}

//void display() {
//	//Printf the line with char Indicator Letters
//	//inputed and highlight chars from start col to end col
//	//For Each message in list add a letter at start column and underline everything else until end column
//	//After we've build up our string
//	// Loop through all the messages again and display their messages in a list.
//	int i;
//	char marker = 'Z';
//	for(i = messageCnt; i > 0; i--) {
//
//		//Make buffer bigger and pull all other chars to the right
//		//Pull them over by
//		int j;
//		int k = messages[i]->endColumn;
//		int l = 0;
//		for(j = bufLen + ESC_SEQ_LEN; j > messages[i]->endColumn; j--) {
//			buffer[j+1] = buffer[j];
//			buffer[k++] = underline[l++];
//		}
//
//		for (j = bufLen; j > messages[i]->startColumn; j--) {
//			//TODO replace each char with a highlighted char
//			buffer[j+1] = buffer[j];
//		}
//		// Here are some words \033[7m A \033[0m\033[4mThe Rest of this line is underlined\033[0m. That was 4 esc seq
//		buffer[messages[i]->startColumn] = marker--;
//		bufLen += (4 * ESC_SEQ_LEN) + 3;
//	}
//	if (messageCnt > 0 || echoMode) {
//		printf("%d: %s\n",getCurrentLine(),buffer);
//		//Loop through and print messages inside messages list
//		marker = 'A';
//		for (i = 0; i < messageCnt; i++) {
//			printf("-%c %s",marker--,messages[i]->message);
//		}
//	}
//}
//
//void insertStringIntoBuffer(int startPos, int msgIndex, char str[]) {
//	// Move everything over starting at startPos until startPos + length
//	// Then copy everything in str into startPos until startPos + length
//	int i;
//	int j = messages[msgIndex]->endColumn;
//	int k = 0;
//	for(i = buffer + strlen(str); j > messages[msgIndex]->endColumn; i--) {
//		buffer[i+1] = buffer[i];
//		buffer[j++] = str[k++];
//	}
//}

void readNextLine() {
	fgets(buffer, MAXLINE, sourceFile);
	bufLen = strlen(buffer);
	nextPos = 0;
	curLine++;
}
