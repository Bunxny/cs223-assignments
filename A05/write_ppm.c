/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 21, 2023
 * Description: An implementation of write_ppm.
 *
 ---------------------------------------------*/
#include "write_ppm.h"
#include <stdio.h>
#include <string.h>

// implement *one* (but do not remove the other one)!

void write_ppm_2d(const char* filename, struct ppm_pixel** pixels, int w, int h) {

}

void write_ppm(const char *filename, struct ppm_pixel *pixels, int w, int h) {
  FILE *outfile;
  outfile = fopen(filename, "wb");
  if (outfile == NULL) {
    printf("Error: unable to open file %s\n", "outfile");
    fclose(outfile);
    return;
  }

  //magic number
  fprintf(outfile, "P6\n");
  //the comment
  fprintf(outfile, "# Created by GIMP version 2.10.24 PNM plug-in\n");
  //Get the size w.h
  fprintf(outfile, "%d %d\n", w, h);
  //get max byte
  fprintf(outfile, "255\n");
  //get colors rgb
  fwrite(pixels, sizeof(struct ppm_pixel), w * h, outfile);

  fclose(outfile);
}
