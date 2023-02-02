/**
*
* A program that computes an acronym from a given phrase.
*
* @author: Anna Nguyen
* @version: February 1, 2023
*/

#include <stdio.h>
#include <string.h>

void acronym(char p[]);

int main() {
  char phrase[1024] = " ";
  printf("Enter a phrase:");
  scanf("%[^\n]%*c", phrase);
  acronym(phrase);
  return 0;
}

void acronym(char p[]) {
  char acro[512];
  int count = 0;
  int len = strlen(p);
  for (int i = 0; p[i] != '\0'; i++) {
    if (p[i] >= 'A' && p[i] <= 'Z') {
      acro[count] = p[i];
      count++;
    }
  }
  printf("%s\n", acro);
}
