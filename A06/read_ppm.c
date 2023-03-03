/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 18, 2023
 * Description:An implentation of read_ppm. A
 * function that reads in a PPM file and returns
 * a pointer.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "read_ppm.h"

// Choose *one* to implement (do not remove the other one!)

struct ppm_pixel** read_ppm_2d(const char* filename, int* w, int* h) {
  return NULL;
}

struct ppm_pixel *read_ppm(const char *filename, int *w, int *h) {
  FILE *infile;
  infile = fopen(filename, "rb");
  if (infile == NULL) {
    printf("Error: unable to open file %s\n", "infile");
    fclose(infile);
    return NULL;
  }

  char header[1250];
  struct ppm_pixel *arrayC;
  //read in header first get magic number
  if (!fgets(header, 1257, infile)) {
    printf("unexpected error\n");
    return NULL;
  }
  if (header[0] != 'P' || header[1] != '6') {
    printf("Please make sure the PPM is of type P6");
    fclose(infile);
    return NULL;
  }

  //ignore the comment
  char comment;
  comment = getc(infile);
  if (comment == '#') {
    while (comment != '\n') {
      comment = getc(infile);
    }
  }

  //Get the size w.h
  if (!fscanf(infile, "%d %d", w, h)) {
    printf("unexpected error for width and height\n");
    fclose(infile);
    return NULL;
  }

  //get max byte
  char maxByte[16];
  //skip blank
  fgets(header, sizeof(header), infile);
  if (!fgets(header, sizeof(header), infile)) {
    printf("unexpected error for byte size\n");
    fclose(infile);
    return NULL;
  }
  strcpy(maxByte, header);
  int size = *w * *h;
  arrayC = malloc(sizeof(struct ppm_pixel) * (size));
  if (arrayC == NULL) {
    printf("Bad malloc error\n");
    fclose(infile);
    return NULL;
  }

  fread(arrayC, sizeof(struct ppm_pixel), size, infile);
  fclose(infile);
  return arrayC;
}
