/**
*
* This program fills an array with three songs and
* then allows the user to edit the data of the song.
*
* @author: Anna Nguyen
* @version: February 1, 2023
*/

#include <stdio.h>
#include <string.h>

struct song {
  int minutes;
  int seconds;
  float danceability;
  char artist[32];
  char title[32];
};

void printSongs(struct song data[]);
void editSong(int id, char attr[], struct song data[]);

int main() {
  printf("Welcome to the song list\n");
  struct song s1, s2, s3;
  s1.minutes = 4;
  s1.seconds = 11;
  s1.danceability = 0.50;
  strcpy(s1.artist, "Tears for Fears");
  strcpy(s1.title, "Shout");
  s2.minutes = 3;
  s2.seconds = 10;
  s2.danceability = 0.2;
  strcpy(s2.artist, "Happy Clown");
  strcpy(s2.title, "The Circus");
  s3.minutes = 2;
  s3.seconds = 12;
  s3.danceability = 0.10;
  strcpy(s3.artist, "Sad Clown");
  strcpy(s3.title, "Crying Children");
  struct song data[3];
  data[0] = s1;
  data[1] = s2;
  data[2] = s3;
  printSongs(data);
  printf("Enter a song id to edit [0,1,2]: ");
  int id;
  char attr[30];
  scanf(" %d", &id);
  if (id >= 3 || id < 0) {
    printf("Invalid choice\n");
  } else {
    printf("Which attribute do you wish to edit? [artist, title, duration, danceability]: ");
    scanf(" %s", attr);
    editSong(id, attr, data);
    printSongs(data);
  }
}

void printSongs(struct song data[]) {
  for (int i = 0; i < 3; i++) {
    printf("\n%d) %-20s artist: %-20s duration: %d:%-20d danceability: %.2f\n", i,
      data[i].title, data[i].artist, data[i].minutes, data[i].seconds,
        data[i].danceability);
  }
  printf("\n=======================\n");
}

void editSong(int id, char attr[], struct song data[]) {
  char art[32];
  char tit[32];
  int min;
  int sec;
  float dan;

  if (strcmp("artist", attr) == 0) {
    printf("Enter a artist: ");
    scanf(" %[^\n]%*c", art);
    strcpy(data[id].artist, art);
  } else if (strcmp("title", attr) == 0) {
    printf("Enter a title: ");
    scanf(" %[^\n]%*c", tit);
    strcpy(data[id].title, tit);
  } else if (strcmp("duration", attr) == 0) {
    printf("Enter minutes: ");
    scanf(" %d", &min);
    data[id].minutes = min;
    printf("Enter seconds: ");
    scanf(" %d", &sec);
    data[id].seconds = sec;
  } else if (strcmp("danceability", attr) == 0) {
    printf("Enter danceability: ");
    scanf(" %f", &dan);
    data[id].danceability = dan;
  } else {
    printf("Invalid choice\n");
  }
}
