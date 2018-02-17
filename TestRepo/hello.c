/*
 * hello.c
 *
 *  Created on: Jan 23, 2018
 *      Author: Austin Klum
 */
#include <stdio.h>

int main(){
	printf("Hello World!\n");
	printf("Here are some words \033[7m A \033[0m\033[4mThe Rest of this line is underlined\033[0m. That was 4 esc seq");

	return 0;
}
