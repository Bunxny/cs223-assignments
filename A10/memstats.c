/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:April 19, 2023
 * Description:an implementataion of the function memstat
  that displays memory and block information.
 ---------------------------------------------*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
  int total_blocks_used = 0;
  int total_blocks_free = 0;
  size_t total_memory_allocated = 0;
  size_t total_memory_used = 0;
  size_t total_used = 0;

  // count blocks and memory
  for (int i = 0; i < len; i++) {
    if (buffer[i] != NULL) {
      struct chunk* cnk = (struct chunk*)((struct chunk*)buffer[i] - 1);
      total_memory_allocated = total_memory_allocated + cnk->size;
      total_memory_used = total_memory_used + cnk->size;
      total_used = total_used + cnk->used;
      total_blocks_used++;
    }
  }

  // free blocks
  struct chunk* curr = freelist;
  while (curr != NULL) {
    total_blocks_free++;
    total_memory_allocated = total_memory_allocated + curr->size;
    curr = curr->next;
  }

  size_t underutilized_memory = total_memory_allocated - total_used - (total_memory_allocated - total_memory_used);
  printf("%ld", total_used);
  printf("\nTotal blocks: %d", total_blocks_used + total_blocks_free);
  printf("  Used blocks: %d", total_blocks_used);
  printf("  Free blocks: %d", total_blocks_free);
  printf("\nTotal memory allocated: %zu bytes", total_memory_allocated);
  printf("  Used memory: %zu bytes", total_memory_used);
  printf("  Free memory: %zu bytes", total_memory_allocated - total_memory_used);
  printf("\nUnderutilised memory: %.2f\n",
   (double)underutilized_memory / (double)total_memory_used);
}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }
  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      }
      else {
        size_t size = (size_t) randExp(8, 4000);
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        }
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]);
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
