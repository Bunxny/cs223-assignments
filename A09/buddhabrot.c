#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <sys/time.h>
#include <pthread.h>
#include "read_ppm.h"
#include "write_ppm.h"
#include <string.h>

struct ppm_pixel *pallet;
struct ppm_pixel *image;

struct data {
  int *count;
  int *membership;
  int id;
  int size;
  float xmin;
  float xmax;
  float ymin;
  float ymax;
  int maxIterations;
  int start_row;
  int start_col;
  int end_row;
  int end_col;
};

pthread_barrier_t barrier;
pthread_mutex_t mutex;

void * buddhabrot(void *d);

int main(int argc, char* argv[]) {
  int size = 480;
  float xmin = -2.0;
  float xmax = 0.47;
  float ymin = -1.12;
  float ymax = 1.12;
  int maxIterations = 1000;
  int numProcesses = 4;

  int opt;
  while ((opt = getopt(argc, argv, ":s:l:r:t:b:p:")) != -1) {
    switch (opt) {
      case 's': size = atoi(optarg); break;
      case 'l': xmin = atof(optarg); break;
      case 'r': xmax = atof(optarg); break;
      case 't': ymax = atof(optarg); break;
      case 'b': ymin = atof(optarg); break;
      case '?': printf("usage: %s -s <size> -l <xmin> -r <xmax> "
        "-b <ymin> -t <ymax> -p <numProcesses>\n", argv[0]); break;
    }
  }
  printf("Generating buddhabrot with size %dx%d\n", size, size);
  printf("  Num processes = %d\n", numProcesses);
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your code here
  // compute image
  int full_size = size * size;
  int half_size = size/2;
  image = malloc(sizeof(struct ppm_pixel) * full_size);
  if (image == NULL) {
    perror("Malloc Error");
    exit(1);
  }
  //pallet varable
  pallet = malloc(sizeof(struct ppm_pixel) * maxIterations);
  if (pallet == NULL) {
    perror("Malloc Error");
    exit(1);
  }
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
  struct timeval tend;
  struct timeval tstart;
  gettimeofday(&tstart, NULL);
  struct data *data = malloc(sizeof(struct data) * 4);
  pthread_t th[4];
  //addedmutex barrier varables
  int *membership = (int*)malloc(full_size * sizeof(int));
  int *count = (int*)malloc(full_size * sizeof(int));
  memset(count, 0, size * size * sizeof(int));
  pthread_mutex_t mutex;
  pthread_mutex_init(&mutex, NULL);
  if (pthread_barrier_init(&barrier, NULL, 4) != 0) {
    perror("pthread_barrier_init Error");
    exit(1);
  }
  for (int i = 0; i < 4; i++) {
    data[i].id = i;
    data[i].size = size;
    data[i].xmin = xmin;
    data[i].xmax = xmax;
    data[i].ymin = ymin;
    data[i].ymax = ymax;
    data[i].maxIterations = maxIterations;
    data[i].count = count;
    data[i].membership = membership;
    if (i == 0) {
      data[i].start_row = 0;
      data[i].end_row = half_size;
      data[i].start_col = 0;
      data[i].end_col = half_size;
    }
    if (i == 1) {
      data[i].start_row = 0;
      data[i].end_row = half_size;
      data[i].start_col = half_size;
      data[i].end_col = size;
    }
    if (i == 2) {
      data[i].start_row = half_size;
      data[i].end_row = size;
      data[i].start_col = 0;
      data[i].end_col = half_size;
    }
    if (i == 3) {
      data[i].start_row = half_size;
      data[i].end_row = size;
      data[i].start_col = half_size;
      data[i].end_col = size;
    }
    if (pthread_create(&th[i], NULL, buddhabrot, &data[i]) != 0) {
      perror("Create Thread Failed");
      exit(1);
    }
    printf("Thread %d) Sub-image block: cols (%d, %d) to rows (%d,%d)\n",
    data[i].id, data[i].start_col, data[i].end_col, data[i].start_row, data[i].end_row);
    printf("Thread %d) finished\n", data[i].id);
  }
  for (int i = 0;  i < 4; i++) {
    if (pthread_join(th[i], NULL) != 0) {
      perror("Thread Join Failed");
    }
  }
  gettimeofday(&tend, NULL);
  double timer;
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  char title[120];
  sprintf(title, "buddabrot-%d-%ld.ppm", size, time(0));
  write_ppm(title, image, size, size);
  printf("Computed buddhabrot set (%dx%d) in %f seconds\n", size, size, timer);
  printf("Writing file: %s\n", title);
  pthread_mutex_destroy(&mutex);
  pthread_barrier_destroy(&barrier);
  //detach and free
  free(image);
  free(count);
  free(membership);
  free(pallet);
  free(data);
  return 0;
}

void * buddhabrot(void *d) {
  struct data *data = (struct data*)d;
  int *count = data->count;
  int row, col;
  double xfrac, yfrac, x0, y0, x, y, xtmp;
  struct ppm_pixel color;
  for (row = data->start_row; row < data->end_row; row++) {
    for (col = data->start_col; col < data->end_col; col++) {
      int i = 0;
      xfrac = (double) col / (double) data->size; //col
      yfrac = (double) row / (double) data->size; //row
      x0 = data->xmin + xfrac * (data->xmax - data->xmin);
      y0 = data->ymin + yfrac * (data->ymax - data->ymin);
      x = 0;
      y = 0;
      while (i < data->maxIterations && x*x + y*y < 2*2) {
        xtmp = x*x - y*y + x0;
        y = 2*x*y + y0;
        x = xtmp;
        i++;
      }
      if (i < data->maxIterations) {// escaped
        data->membership[row * data->size + col] = 0; //false
      } else {
        data->membership[row * data->size + col] = 1; //true
      }
    }
  }
  int max_count = 0;
  for (row = data->start_row; row < data->end_row; row++) {
    for (col = data->start_col; col < data->end_col; col++) {
      if (data->membership[row * data->size + col] == 1) {
        continue;
      } else {
        xfrac = (double) col / (double) data->size; //col
        yfrac = (double) row / (double) data->size; //row
        x0 = data->xmin + xfrac * (data->xmax - data->xmin);
        y0 = data->ymin + yfrac * (data->ymax - data->ymin);
        x = 0;
        y = 0;
        while (x*x + y*y < 2*2) {
          xtmp = x*x - y*y + x0;
          y = 2*x*y + y0;
          x = xtmp;
          int yrow = round(data->size * (y - data->ymin)/(data->ymax - data->ymin));
          int xcol = round(data->size * (x - data->xmin)/(data->xmax - data->xmin));
          if (yrow < 0 || yrow >= data->size) { // out of range
           continue;
          }
          if (xcol < 0 || xcol >= data->size) { // out of range
            continue;
          }
          pthread_mutex_lock(&mutex);
          //increment count at (yrow, xcol)
          count[yrow * data->size + xcol]++;
          //update max count
          if (count[yrow * data->size + xcol] > max_count) {
            max_count = count[yrow * data->size + xcol];
          }
          pthread_mutex_unlock(&mutex);
        }
      }
    }
  }
  pthread_barrier_wait(&barrier);
  //generate color
  double gamma = 0.681;
  double factor = 1.0/gamma;
  double value;
  for (row = data->start_row; row < data->end_row; row++) {
    for (col = data->start_col; col < data->end_col; col++) {
      value = 0;
      if (count[row * data->size + col] > 0) {
        value = log((double)count[row * data->size + col]) / log((double)max_count);
        value = pow(value, factor);
      }
      color.red = value * 255;
      color.green = value * 255;
      color.blue = value * 255;
      //write color to image at location (row,col)
      image[row * data->size + col] = color;
    }
  }
  return NULL;
}
