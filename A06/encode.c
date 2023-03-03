/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:March 1, 2023
 * Description:A  program that reads in a
 * PPM file (raw, or binary, format) and asks
 * the user for a message to embed within it.
 ---------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "read_ppm.h"
#include "write_ppm.h"

void encode_color(struct ppm_pixel *c, char *phrase, int len) {
  int count = 0;
  unsigned char *color = (unsigned char*) c;
  for (int i = 0; i < len - 1; i++) {
    //get each char of msg/phrase
    unsigned char ch_of_msg = phrase[i];
    for (int j = 0; j < 8; j++) {
      //get each bit of char from msg
      unsigned char ch_bit = (ch_of_msg >> (7 - j)) & 0X01;
      // put the bit into the last bit code of the color
      //(color & 0XFE) makes last bit of color to 0 the or it
      //so what ever is in the last bit will be ch_cit
      color[count] = (color[count] & 0XFE) | ch_bit;
      count++;
    }
  }
  for (int k = 0; k < 8; k++) {
    color[count + k] = (color[count + k] & 0XFE) & 0;
  }
  c = (struct ppm_pixel*)color;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: encode <file.ppm>\n");
    return 0;
  }
  int w = 0;
  int h = 0;
  //unsigned char *pixels = (unsigned char*) read_ppm(argv[1], &w, &h);
  struct ppm_pixel *pixels = read_ppm(argv[1], &w, &h);
  if (pixels == NULL) {
    printf("Encode Cannot Read File\n");
    exit(1);
  }
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  int max_chars = (w * h * 3)/8;
  printf("Max number of characters in the image: %d\n", max_chars - 1);
  char *phrase = malloc(sizeof(char) * max_chars);
  printf("Enter a phrase: ");
  fgets(phrase, max_chars, stdin);
  int len = strlen(phrase);
  int args_len = strlen(argv[1]);
  char write[50];
  char append[13] = "-encoded.ppm";
  strcpy(write, argv[1]);
  write[strcspn(write, "\n")] = '\0';
  write[args_len - 4] = '\0';
  strcat(write, append);
  encode_color(pixels, phrase, len);
  printf("Writing file %s\n", write);
  write_ppm(write, pixels, w, h);
  free(phrase);
  free(pixels);
  return 0;
}
