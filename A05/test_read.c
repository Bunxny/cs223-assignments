/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 18, 2023
 * Description:A test of the funtion read_ppm
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

int main() {
  int w, h;
  char *filename = "feep-raw.ppm";
  struct ppm_pixel *pixels = read_ppm("feep-raw.ppm", &w, &h);

  // todo: print out the grid of pixels
  printf("Testing file %s: %d %d\n", filename, w, h);
  for (int i = 0; i < h; i++) { //row
    for (int j = 0; j < w; j++) { //colunm
      printf("(%d,%d,%d) ",
      pixels[(i * w) + j].red, pixels[(i * w) + j].green, pixels[(i * w) + j].blue);
    }
    printf("\n");
  }

  free(pixels);
  return 0;
}
