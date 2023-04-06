/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:April 4, 2023
 * Description:a program that uses N threads
   specified by camand line to search for a
   value in a list. Thread count starts at 0.
 ---------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int *list;
int len;
int find;
int n;
int found = 0;

void *finder(void *d);

int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <NumThreads>\n", argv[0]);
    exit(0);
  }
  FILE* fp = fopen("data.bin", "rb");
  fread(&len, sizeof(int), 1, fp);
  list = malloc(sizeof(int) * len);
  if (list == NULL) {
    perror("Malloc Error");
    exit(1);
  }
  fread(list, sizeof(int), len, fp);
  printf("Enter a value to search: ");
  scanf(" %d", &find);
  n = atoi(argv[1]); //n threads
  //creat threads
  pthread_t *th = (pthread_t *)malloc(sizeof(pthread_t) * n);
  if (th == NULL) {
    perror("Malloc Error");
    exit(1);
  }
  //thread ids
  int *thread_ids = malloc(n * sizeof(int));
  if (thread_ids == NULL) {
    perror("Malloc Error");
    exit(1);
  }
  for (int i = 0; i < atoi(argv[1]); i++) {
     //thread id values
     thread_ids[i] = i;
    if (pthread_create(&th[i], NULL, finder, (void *)&thread_ids[i]) != 0) {
      perror("Create Thread Failed\n");
      exit(1);
   }
  }
  for (int i = 0; i < atoi(argv[1]); i++) {
    if (pthread_join(th[i], NULL) != 0) {
      perror("Thread Join Failed\n");
      exit(1);
    }
  }
  if (!found) {
    printf("Not Found\n");
  }
  //free
  free(thread_ids);
  free(th);
  free(list);
  fclose(fp);
  return 0;
}
//void func
void *finder(void *d) {
  int *id = (int *)d;
  int start = *id * (len / n);
  int end = (*id + 1) * (len / n);
  for (int i = start; i < end; i++) {
    if (list[i] == find) {
      printf("Thread %d found %d at index %d\n", *id, find, i);
      found = 1;
    }
  }
  return NULL;
}
