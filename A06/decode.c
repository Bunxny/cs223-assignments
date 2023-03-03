/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 27, 2023
 * Description:a program that reads in a PPM file
 * and then outputs a message that might be stored
 * in the least significant bits of each color.
 ---------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "read_ppm.h"

//gets least significant bit
int small_bit(unsigned char pixel, int mask) {
  if ((pixel & mask) == mask) {
    return 1;
  }
    return 0;
}

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: decode <file.ppm>\n");
    return 0;
  }
  int w = 0;
  int h = 0;
  struct ppm_pixel *pixels = read_ppm(argv[1], &w, &h);
  if (pixels == NULL) {
    printf("Decode Cannot Read File\n");
    exit(1);
  }
  printf("Reading %s with width %d and height %d\n", argv[1], w, h);
  int max_char = (w * h * 3)/8;
  printf("Max number of characters in image: %d\n", max_char);
  unsigned char *message = malloc((max_char * sizeof(char)) + 1);
  if (message == NULL) {
    printf("Bad malloc error\n");
    exit(1);
  }
  int mask = 0x01;
  int tracker = 7; //tracks bits
  char ascii_char = 0; //8 bits
  int message_count = 0;
  for (int i = 0; i < w * h; i++) {
    int red = small_bit(pixels[i].red, mask);
    int green = small_bit(pixels[i].green, mask);
    int blue = small_bit(pixels[i].blue, mask);
    //appends bits to tracker by shifting and
    // operand or left side to shifted color
    ascii_char |= red << (tracker);
    if (tracker == 0) {
      message[message_count] = ascii_char;
      message_count = message_count + 1;
      if (ascii_char  == '\0' || message_count > max_char) {
        break;
      }
      tracker = 8;
      ascii_char = 0;
    }
    tracker--;
    ascii_char |= green << (tracker);
    if (tracker == 0) {
      message[message_count] = ascii_char;
      message_count = message_count + 1;
      if (ascii_char  == '\0' || message_count > max_char) {
        break;
      }
      tracker = 8;
      ascii_char = 0;
    }
    tracker--;
    ascii_char |= blue << (tracker);
    if (tracker == 0) {
      message[message_count] = ascii_char;
      message_count = message_count + 1;
      if (ascii_char  == '\0' || message_count > max_char) {
        break;
      }
      tracker = 8;
      ascii_char = 0;
    }
    tracker--;
  }
  printf("message: %s \n", message);
  free(message);
  free(pixels);
  return 0;
}
