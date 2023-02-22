/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 18
 * Description:A program that identifies a common letter
 * between two given words from the command line and lists
 * one word vertically and the other word horizontally.
 * Making a cross word.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[]) { //arg count and argv includes ./a.out and null at end
  char *word1 = malloc(sizeof(char)*50);
  char *word2 = malloc(sizeof(char)*50);
  strcpy(word1, argv[1]); //vertical row
  strcpy(word2, argv[2]); //horizontal column
  int len1 = strlen(word1);
  int len2 = strlen(word2);
  char **darray = malloc(sizeof(char *) * len1); // * N pointers/rows
  if (word1 == NULL || word2 == NULL || darray == NULL) {
    printf("Bad malloc error\n");
    exit(1);
  }
  for (int i = 0; i < len1; i++) {
    darray[i] = malloc(sizeof(char) * len2); //N column
  }
  int vertical = 0;   //placement of vertical word it is based on horizontal match
  int horizontal = 0; //placement of horizontal word it is based on the vertical match
  int count = 0;
  int countt = 0;
  int found = 0; //1 if found
  char match;
  for (int i = 0; i < len1; i++) {
    if (found) {
      break;
    }
    horizontal =  i;
    for (int j = 0; j < len2; j++) {
      if (word1[i] == word2[j]) {
        match = word1[i];
        found = 1;
        break;
      }
    }
  }
  if (!found) {
    printf("NO COMMON LETTER\n");
    for(int i = 0; i < len1; i++) {
      free(darray[i]);
    }
    free(darray);
    free(word1);
    free(word2);
    exit(0);
  }
  for (int i = 0; i < len2; i++) {
    vertical = i;
    if (word2[i] == match) {
       break;
    }
  }
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
      if (j == vertical) {
        darray[i][j] = word1[count];
        count = count + 1;
      } else if (i == horizontal) {
        if (countt == vertical) {
          countt = countt + 1; //skip matched letter
        }
        darray[i][j] = word2[countt];
        countt = countt + 1;
      } else {
        darray[i][j] = '.';
      }
    }
  }
  for (int i = 0; i < len1; i++) {
    for (int j = 0; j < len2; j++) {
       printf("%c", darray[i][j]);
    }
    printf("\n");
  }
  for(int i = 0; i < len1; i++) {
    free(darray[i]);
  }
  free(darray);
  free(word1);
  free(word2);
  return 0;
}
