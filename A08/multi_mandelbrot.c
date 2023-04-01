/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Mar 31, 2023
 * Description:a program that outputs a PPM image
  of the mandelbrot set using 4 child processes.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <sys/shm.h>
#include <sys/wait.h>

void mandelbrot(int image_size, double xmin, double xmax, double ymin,
 double ymax, int MAX, struct ppm_pixel *palette, struct ppm_pixel *image, int row_start, int col_start, int row_end,
  int col_end);

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> -b <ymin> -t <ymax>\n", argv[0]); break;
    }
  }
  printf("Generating mandelbrot with size %dx%d\n", size, size);
  printf("Num processes = 4\n");
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  int full_size = size * size;
  int half_size = size/2;
  //shared memory call
  int shmid = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * full_size, 0644|IPC_CREAT);
  int shmid2 = shmget(IPC_PRIVATE, sizeof(struct ppm_pixel) * maxIterations, 0644|IPC_CREAT);
  if (shmid == -1) {
    perror("Error: cannot initialize shared memory\n");
    exit(1);
  }
  //attach shared memory to parent process
  struct ppm_pixel *image = (struct ppm_pixel*)shmat(shmid, NULL, 0);
  if (image == (void*) -1) {
    perror("Error: cannot access shared memory\n");
    exit(1);
  }
  //pallet varable
  struct ppm_pixel *pallet = (struct ppm_pixel*)shmat(shmid2, NULL, 0);
  // generate pallet
  srand(time(0)); //seed
  int r, g, b;
  for (int i = 0; i < maxIterations; i++) {
    r = rand() % 255; //formula rand() % range + min
    g = rand() % 255;
    b = rand() % 255;
    pallet[i].red = r;
    pallet[i].blue = b;
    pallet[i].green = g;
  }
  //fork and create 4 processes
  int pid;
  //compute image and get time
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  for (int i = 0; i < 4; i++) {
    pid = fork();
    if (pid == 0) { //child process
      int start_row;
      int start_col;
      int end_row;
      int end_col;
      if (i == 0) {
        printf("Launched child process: %d\n", getpid());
        start_row = 0;
        end_row = half_size;
        start_col = 0;
        end_col = half_size;
        mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pallet, image,
         start_row, start_col, end_row, end_col);
      }
      if (i == 1) {
        printf("Launched child process: %d\n", getpid());
        start_row = 0;
        end_row = half_size;
        start_col = half_size;
        end_col = size;
        mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pallet, image,
         start_row, start_col, end_row, end_col);
      }
      if (i == 2) {
        printf("Launched child process: %d\n", getpid());
        start_row = half_size;
        end_row = size;
        start_col = 0;
        end_col = half_size;
        mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pallet, image,
         start_row, start_col, end_row, end_col);
      }
      if (i == 3) {
        printf("Launched child process: %d\n", getpid());
        start_row = half_size;
        end_row = size;
        start_col = half_size;
        end_col = size;
        mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pallet, image,
          start_row, start_col, end_row, end_col);
      }
      printf("%d) Sub-image block: cols (%d, %d) to rows (%d,%d)\n",
       getpid(), start_col, end_col, start_row, end_row);
      printf("Child process complete: %d\n", getpid());
      exit(0);
    } else if (pid == -1) {
      perror("Error: fork failed\n");
      exit(1);
    }
  }

  // Wait for child processes
  int status;
  for (int i = 0; i < 4; i++) {
    pid = wait(&status);
    if (pid == -1) {
      perror("Error: wait failed\n");
      exit(1);
    }
  }
  gettimeofday(&tend, NULL);
  double timer;
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  char title[120];
  sprintf(title, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm("t.ppm", image, size, size);
  printf("Computed mandelbrot set (%dx%d) in %f seconds\n", size, size, timer);
  printf("Writing file: %s\n", title);
  //detach and free
  if (shmdt(image) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }
  if (shmctl(shmid, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }

  if (shmdt(pallet) == -1) {
    perror("Error: cannot detatch from shared memory\n");
    exit(1);
  }
  if (shmctl(shmid2, IPC_RMID, 0) == -1) {
    perror("Error: cannot remove shared memory\n");
    exit(1);
  }
  return 0;
}

void mandelbrot(int image_size, double xmin, double xmax, double ymin, double ymax,
 int MAX, struct ppm_pixel *pallet, struct ppm_pixel  *image, int row_start, int col_start, int row_end,
  int col_end) {
  int row, col;
  double xfrac, yfrac, x0, y0, x, y, xtmp;
  struct ppm_pixel color;
  int i = 0;
  for (row = row_start; row < row_end; row++) {
    for (col = col_start; col < col_end; col++) {
      xfrac = (double) col / (double) image_size; //col
      yfrac = (double) row / (double) image_size; //row
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);
      x = 0;
      y = 0;
      while (i < MAX && x*x + y*y < 2*2) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        i++;
      }
      if (i < MAX) {// escaped
      color = pallet[i];
      } else {
      color.red = 0; //black
      color.green = 0;
      color.blue = 0;
      }
      //write color to image at location (row,col)
      image[row*image_size+col] = color;
    }
  }
}
