/*----------------------------------------------
 * Author:Anna Nguyen
 * Date:Feb 27, 2023
 * Description:a program that reads in a single
 * 64-bit unsigned integer (e.g. unsigned long)
 * and outputs it as an 8x8 1bpp sprite.
 * 1 = black 0 = white
 ---------------------------------------------*/
#include <stdio.h>

void print_bpp(int w, int h, unsigned long img);

int main() {
  unsigned long img;
  scanf(" %lx", &img);
  printf("Image (unsigned long): %lx\n", img);
  print_bpp(8, 8, img);
  return 0;
}

// todo: your code here
void print_bpp(int w, int h, unsigned long img) {
  // Use a 64-bit mask for left-most bit
  unsigned long mask = 0x1ul << 63;
  for (int i = 0; i < h; i++) { //row
    for (int j = 0; j < w; j++) { //colunm
      if((img & mask) == mask) {
        printf("@ ");
      } else {
        printf("  ");
      }
      mask = mask >> 1;
    }
    printf("\n");
  }
  printf("----\n");
}
