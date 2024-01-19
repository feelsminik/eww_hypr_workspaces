#include <stddef.h>
#include <stdio.h>

void printIntArray(int *given_array) {
  size_t array_size = sizeof(&given_array) / sizeof(given_array[0]);
  for (size_t i = 0; i < array_size; i++) {
    printf("%d, ", given_array[i]);
  }
}
