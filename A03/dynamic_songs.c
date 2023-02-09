/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:February 5, 2023
 * Description:Reads songs from a text file into an array
 * and prints it out
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  FILE *infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "songlist.csv");
    exit(1);
  }
  char *token;
  char *delim = ",";
  char *line;
  char **data;
  line = malloc(128 * sizeof(char));
  if (line == NULL) {
    printf("Error: malloc failed\n");
    exit(1);
  }
  line = fgets(line, 128, infile);
  token = strtok(line, delim);
  int len = atoi(token);
  data = malloc(sizeof(char *) * len);
  for (int i = 0; i < len; i++) {
    data[i] = malloc(sizeof(char) * 7);
  }
  line = fgets(line, 128, infile);
  for (int i = 0; i < len; i++) {
    line = fgets(line, 128, infile);
    token = strtok(line, delim);
    int count=0;
    while (token != NULL) {
      data[i][count] = *token;
      printf("%s\n", data[count]);
      count++;
      token = strtok(NULL, delim);
    }
  }
  for (int j = 0; j < len; j++) {
    for (int k = 0; k < 7; k++) {
      printf("%s\n", data[j][k]);
    }
  }
  free(data);
  free(line);
  return 0;
}
