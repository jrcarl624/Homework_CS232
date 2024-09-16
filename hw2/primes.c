/* File: primes.c */
/* Author: Britton Wolfe */
/* Date: July 16, 2009 */
/* This program outputs all the primes in the range given */
/* by the command line arguments */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char **argv) {

  int lowerBound, upperBound;

  if (argc != 3) {
    fprintf(stderr, "USAGE: %s lowerBound upperBound\n", argv[0]);
    return -1;
  }

  lowerBound = atoi(argv[1]);
  upperBound = atoi(argv[2]);

  if (lowerBound < 1 || upperBound < 1) {
    fprintf(stderr,
            "ERROR: both the lowerBound (%d) and the upperBound (%d)"
            " must be positive.\n",
            lowerBound, upperBound);
    return -2;
  }

  if (lowerBound == 1) {
    // the upper bound
    if (upperBound == 1)
      return 0;
    // 1 is not a prime number.
    lowerBound += 1;
  }

  for (int i = lowerBound, prime = 0; i <= upperBound; ++i, prime = 0) {

    // Check if odd or not 2
    if ((!(i % 2) && i != 2) || i == 1) {
      continue;
    }

    for (int j = 2; j < i; ++j) {
      for (int k = 2; j * k <= i; ++k) {
        if (k * j == i) {
          ++prime;
        }
      }
    }

    if (prime == 0) {
      printf("%d\n", i);
    }
  }

  return 0;
}
