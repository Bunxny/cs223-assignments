/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 12, 2023
 * Description: A program that reads in a song file storing
  the songs in a linked list. The file prints out the songs
  and can find the song with the highest dancability.
  When found it deletes it from the linked list and the user
  can request highest dancability songs till the list runs out
  of songs to go through.
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

struct node {
  struct song value;
  struct node* next;
};

struct node *newNode(struct song value) {
  struct node *ret = malloc(sizeof(struct node));
  ret->value = value;
  ret->next = NULL;
  return ret;
}

void insert (struct node **head, struct song insert) {
  struct node *tmp, *ins;
  ins = newNode(insert);
  if (*head == NULL) {
    *head = ins;
  } else {
    tmp = *head;
    while(tmp->next != NULL){
      tmp = tmp->next;
    }
    tmp->next = ins;
  }
}

void print(struct node *head) {
  struct node *tmp = head;
  int i = 0;
  while (tmp != NULL) {
    if (tmp->value.sec <= 9) {
      printf("\n%d) %-20s artist: %-20s duration: %d:0%-20d danceability: %-20.2f", i,
        tmp->value.title, tmp->value.artist, tmp->value.min,
          tmp->value.sec, tmp->value.danceability);
    } else {
      printf("\n%d) %-20s artist: %-20s duration: %d:0%-20d danceability: %-20.2f", i,
        tmp->value.title, tmp->value.artist, tmp->value.min, tmp->value.sec,
          tmp->value.danceability);
    }
    printf("energy: %-20.3f tempo: %-20.3f valence: %-20.3f",
      tmp->value.energy, tmp->value.tempo, tmp->value.valence);
    tmp = tmp->next;
    i++;
  }
  printf("\n\nData set contains %d songs\n", i);
  printf("=======================\n");
}

struct node *findMax(struct node *head) {
  struct node *max = head;
  struct node *tmp = head;
  while (tmp != NULL) {
    if (tmp->value.danceability >= max->value.danceability) {
      max = tmp;
    }
    tmp = tmp->next;
  }
  return max;
}

int rmv(struct node **head, struct node *find) {
  struct node *tmp = *head;
  struct node *prev = *head;
  if (head == NULL) {
    printf("empty");
    return 1;
  } else {
    int count = 0;
    while (tmp != NULL) {
      count++;
      if (strcmp(tmp->value.title, find->value.title) == 0 &&
        strcmp(tmp->value.artist, find->value.artist) == 0) {
        int prevNum = count - 1;
        if (strcmp(head[0]->value.title, tmp->value.title) == 0) {
          *head = tmp->next;
          free(tmp);
          tmp = NULL;
          return 0; //found node to remove
        } else {
          for (int i = 1; i < prevNum; i++) {
            prev = prev->next;
            printf("%d", prevNum);
          }
          prev->next = tmp->next;
          free(tmp);
          tmp = NULL;
          return 0;
        }
      }
      tmp = tmp->next;
    }
  }
  return 1; //for not found node to remove
}

void clear(struct node **head) {
  struct node *tmp = *head;
  struct node *next = *head;
  while (tmp != NULL) {
    next = tmp->next;
    free(tmp);
    tmp = next;
  }
  *head = NULL;
}

int main() {
  struct node *head = NULL;
  FILE *infile;
  infile = fopen("songlist.csv", "r");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "songlist.csv");
    exit(1);
  }
  char *token;
  char *delim = ",";
  char line[2046];
  fgets(line, 2046, infile);
  while (fgets(line, 2046, infile) != NULL) {
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
    insert(&head, songs);
  }
  for (;;) {
    if (head == NULL) {
      printf("Dataset contains 0 songs\n");
      printf("\n=======================\n");
    } else {
      print(head);
    }
    char input = ' ';
    printf("Press 'd' to show the most danceable song (any other key to quit): \n");
    scanf(" %c", &input);
    if (input == 'd') {
      if (head != NULL) {
        struct node *max = findMax(head);
        printf("\n------------------------ Most danceable ------------------------");
        if (max->value.sec <= 9) {
          printf("\n %-20s artist: %-20s duration: %d:0%-20d danceability: %.2f",
            max->value.title, max->value.artist, max->value.min,
              max->value.sec, max->value.danceability);
        } else {
          printf("\n %-20s artist: %-20s duration: %d:0%-20d danceability: %.2f",
            max->value.title, max->value.artist, max->value.min, max->value.sec,
              max->value.danceability);
        }
        printf("energy: %-16.3f tempo: %-21.3f valence: %-20.3f",
          max->value.energy, max->value.tempo, max->value.valence);
        printf("\n----------------------------------------------------------------\n");
        rmv(&head, max);
      }
    } else {
      break;
    }
  }
  clear(&head);
  fclose(infile);
  return 0;
}
