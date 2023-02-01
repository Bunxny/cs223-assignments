/***************************************************
 * mathgame.c
 * Author:Anna Nguyen
 * Implements a math game
 */

#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main() {
	int rounds;
	int firstNum;
        int secondNum;
        int answer;
	int input;
	int count = 0;
	printf("Welcome to Math Game!\nHow many rounds do you want to play?\n");
	scanf(" %d", &rounds);
	int ret[3];
	srand(time(0));
	for ( int i = 0; i < rounds; i++) {
		firstNum = rand()% 9 + 1;
		secondNum = rand()% 9 + 1;
		answer = firstNum + secondNum;
		printf("%d + %d = ? \n", firstNum, secondNum);
		scanf(" %d", &input);
		if ( input == answer) {
			printf("Correct :)\n");
			count = count + 1;
		} else {
			printf("Incorrect\n");
  	}
	}
	printf("You got %d/%d correct\n", count, rounds);
	return 0;
}
