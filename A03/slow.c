/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:February 5, 2023
 * Description: This programs puts n numbers of '.'
 * between each letter in a phrase.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main() {
  char buff[32];
  int pause;
  printf("Pause length: ");
  scanf(" %d", &pause);
  printf("\nText: ");
  scanf(" %s", buff);
  int len = strlen(buff);
  char *text;
  text = malloc((sizeof(char) * (pause * len)) + len);
  int lenT = (sizeof(char) * (pause * len)) + len;
  //(('.' * pauseLength) * numberOfCharacters) + numberOfCharacters + 1
  for (int i = 0; i < len; i++) {
    text[i + (i * pause)] = buff[i];
    for (int j = 1; j <= pause; j++) {
      text[i + (i * pause) + j] = '.';
    }
  }
  printf("\nEnt-phrase is ");
  for (int k = 0; k < lenT; k++) {
    printf("%c", text[k]);
  }
  printf("\n");
  free(text);
  return 0;
}

