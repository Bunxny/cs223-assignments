/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:March 28, 2023
 * Description:a program that uses two processes
   to search for a value in a list.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
  FILE* fp = fopen("data.bin", "rb");
  int len;
  int find;
  fread(&len, sizeof(int), 1, fp);
  int *list = malloc(sizeof(int) * len);
  fread(list, sizeof(int), len, fp);
  printf("Enter a value to search:");
  scanf(" %d", &find);
  //fork
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    fprintf(stderr, "Fork Failed");
    return 1;
  }
  if (pid > 0) { //parent
    for (int i = 0; i < len/2; i++) {
      if (list[i] == find) {
        printf("Parent found %d at index %d\n", find, i);
        wait(NULL);
        free(list);
        fclose(fp);
        exit(0);
      }
    }
    wait(NULL);
    printf("Not found (parent) \n");
    free(list);
    fclose(fp);
    exit(1);
  } else { //child
    for (int i = len/2; i <= len; i++) {
      if (list[i] == find) {
        printf("Child found %d at index %d\n", find, i);
        free(list);
        fclose(fp);
        exit(0);
      }
    }
    printf("Not found (child) \n");
    free(list);
    fclose(fp);
    exit(1);
  }
  return 0;
}
