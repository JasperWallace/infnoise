// This is a simple program to read from a binary file of random bits and generate
// dice rolls.

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Role a single die.
static uint32_t rollDie(uint32_t sides, FILE *file) {
  uint32_t randVal;
  do {
    uint32_t numBytes = 0;
    randVal = 0;
    while (1 << (8*numBytes) <= sides) {
      numBytes++;
      int c = getc(file);
      if (c == EOF) {
        printf("Ran out of random data\n");
        exit(1);
      }
      randVal = (randVal << 8) | getc(file);
    }
  } while(randVal >= sides);
  return randVal;
}

int main(int argc, char **argv) {
  if (argc != 3) {
    printf("Usage: passgen randFile numKeys\n");
    return 1;
  }
  FILE *file = fopen(argv[1], "r");
  uint32_t keys = atoi(argv[2]);
  uint32_t i;
  printf("password:");
  for (i = 0; i < keys; i++) {
    uint32_t randVal = rollDie(26, file);
    putchar('a' + randVal);
  }
  printf("\nThis password has %.2f bits of entropy\n", log(pow(26.0, keys))/log(2));
  fclose(file);
  return 0;
}
