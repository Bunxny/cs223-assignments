/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Mar 22, 2023
 * Descriptia program that spawns processes
 * according to the assignment diagram
 ---------------------------------------------*/
#include <stdio.h>
#include <unistd.h>

int main() {
  printf("%d A0\n", getpid());
  int id = fork();
  if (id != 0) { //child returns 0
    printf("%d B0\n", getpid());
    id = fork();
    if (id != 0) {
      printf("%d C0\n", getpid());
    } else {
      printf("%d C1\n", getpid());
    }
  } else {
    printf("%d B1\n", getpid());
  }
  printf("%d Bye\n", getpid());
  return 0;
}
