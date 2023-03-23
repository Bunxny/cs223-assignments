/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Mar 22, 2023
 * Description:a program that outputs a PPM image
  of the mandelbrot set.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "read_ppm.h"
#include "write_ppm.h"

void mandelbrot(int image_size, double xmin, double xmax, double ymin,
 double ymax, int MAX, struct ppm_pixel *palette, struct ppm_pixel *image);

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
  printf("  X range = [%.4f,%.4f]\n", xmin, xmax);
  printf("  Y range = [%.4f,%.4f]\n", ymin, ymax);

  // todo: your work here
  int full_size = size * size;
  struct ppm_pixel *image = malloc(sizeof(struct ppm_pixel) * full_size);
  struct ppm_pixel *pallet = malloc(sizeof(struct ppm_pixel) * maxIterations);
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
  // compute image and get time
  struct timeval tstart, tend;
  gettimeofday(&tstart, NULL);
  mandelbrot(size, xmin, xmax, ymin, ymax, maxIterations, pallet, image);
  gettimeofday(&tend, NULL);
  double timer;
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  char title[120];
  sprintf(title, "mandelbrot-%d-%ld.ppm", size, time(0));
  write_ppm(title, image, size, size);
  printf("Computed mandelbrot set (480x480) in %f seconds\n", timer);
  printf("Writing file: %s\n", title);
  free(pallet);
  free(image);
}

void mandelbrot(int image_size, double xmin, double xmax, double ymin, double ymax,
 int MAX, struct ppm_pixel *pallet, struct ppm_pixel  *image) {
  int row, col, iter;
  double xfrac, yfrac, x0, y0, x, y, xtmp;
  struct ppm_pixel color;
  for (row = 0; row < image_size; row++) {
    for (col = 0; col < image_size; col++) {
      xfrac = (double) col / (double) image_size; //col
      yfrac = (double) row / (double) image_size; //row
      x0 = xmin + xfrac * (xmax - xmin);
      y0 = ymin + yfrac * (ymax - ymin);
      x = 0;
      y = 0;
      int i = 0;
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
