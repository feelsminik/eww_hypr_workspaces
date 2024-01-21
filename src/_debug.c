#include <stddef.h>
#include <stdio.h>

#include "../include/_hypr_structs.h"
#include "../include/_macros.h"

void printIntArray(int* given_array) {
  size_t array_size = sizeof(&given_array) / sizeof(given_array[0]);
  printf("IntArray: {");
  for (size_t i = 0; i < array_size; i++) {
    printf("%d, ", given_array[i]);
  }
  printf("}");
}

void printWorkspaceTiles(workspaceTile* tiles) {
  for (size_t i = 0; i <= MAX_NUM_OF_WORKSPACES; i++) {
    workspaceTile tile = tiles[i];
    printf("[%d, %s, %s]\n", tile.index, tile.icon, tile.css_class);
  }
}
