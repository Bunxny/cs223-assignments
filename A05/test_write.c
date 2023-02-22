/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 22, 2023
 * Description:A test of the funtion write_ppm
 ---------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

int main(int argc, char** argv) {
  int w, h;
  struct ppm_pixel* pixels = read_ppm("feep-raw.ppm", &w, &h);

  //test writing the file to test.ppm, reload it, and print the contents
  write_ppm("test.ppm", pixels, w, h);
  struct ppm_pixel* test = read_ppm("test.ppm", &w, &h);
  printf("Testing file %s: %d %d\n", "test.ppm", w, h);
  for (int i = 0; i < h; i++) { //row
    for (int j = 0; j < w; j++) { //colunm
      printf("(%d,%d,%d) ",
      test[(i * w) + j].red, test[(i * w) + j].green, test[(i * w) + j].blue);
    }
    printf("\n");
  }

  free(pixels);
  free(test);
  return 0;
}
