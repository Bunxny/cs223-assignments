/**
*
* This program turns a word into an exclaimation looping through each
* character. If the character is a lowercase letter, it is replaced with
* a randomly choosen character {'@', '!', '#', '*'}.
*
* @author: Anna Nguyen
* @version: February 1, 2023
*/

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

void convert(char buff[]);

int main() {
  char buff[32];
  printf("Enter a word\n");
  scanf(" %s", buff);
  convert(buff);
  return 0;
}

void convert(char buff[]) {
  srand(time(0));
  int len = strlen(buff);
  char letter;
  int randNum;
  char signs[4] = {'@', '!', '#', '*'};
  for (int i = 0; i < len; i++) {
    letter = buff[i];
    if (letter >= 'a' && letter <= 'z') {
      randNum = (rand() % (3 - 0 + 1)) + 0; //(rand % (max - min + 1 )) + min
      buff[i] = signs[randNum];
    }
  }
  printf("\nOH MY GOODNESS! %s\n",buff);
}
