 /*----------------------------------------------
 * Author:Anna Nguyen
 * Date:February 5, 2023
 * Description:Reads songs from a text file into an array
 * and prints it out
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct song {
  int ms;
  int min;
  int sec;
  float energy;
  float tempo;
  float valence;
  float danceability;
  char artist[128];
  char title[128];
};

int main() {
  FILE *infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "songlist.csv");
    exit(1);
  }
  char *token;
  char *delim = ",";
  char line[2046];
  strcpy(line, fgets(line, 128, infile));
  token = strtok(line, delim);
  int len = atoi(token);
  struct song* data = malloc(sizeof(struct song) * len);
  strcpy(line, fgets(line, 128, infile));
  for (int i = 0; i < len; i++) {
    strcpy(line, fgets(line, 128, infile));
    token = strtok(line, delim);
    int count = 0;
    struct song songs;
    while (token != NULL) {
      if (count == 0) {
        strcpy(songs.title, token);
      } else if (count == 1) {
        strcpy(songs.artist, token);
      } else if (count == 2) {
        songs.ms = atoi(token);
        songs.sec = (songs.ms/1000) % 60;
        songs.min = (songs.ms/1000) / 60;
      } else if (count == 3) {
        songs.danceability = atof(token);
      } else if (count == 4) {
        songs.energy = atof(token);
      } else if (count == 5) {
        songs.tempo = atof(token);
      } else {
        songs.valence = atof(token);
      }
      count++;
      token = strtok(NULL, delim);
    }
    data[i] = songs;
  }
  for (int i = 0; i < len; i++) {
    if (data[i].sec <= 9) {
      printf("\n%d) %-20s artist: %-20s duration: %d:0%-20d danceability: %.2f\n", i,
        data[i].title, data[i].artist, data[i].min, data[i].sec,
          data[i].danceability);
    } else {
      printf("\n%d) %-20s artist: %-20s duration: %d:0%-20d danceability: %.2f\n", i,
        data[i].title, data[i].artist, data[i].min, data[i].sec,
          data[i].danceability);
    }
    printf("energy: %-20.3f tempo: %-20.3f valence: %-20.3f",
      data[i].energy, data[i].tempo, data[i].valence);
    printf("\n=======================\n");
  }
  fclose(infile);
  free(data);
  return 0;
}
