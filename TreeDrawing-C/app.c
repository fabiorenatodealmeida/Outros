/*
* gcc -c app.c tree.c
* gcc -o app app.o tree.o
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tree.h"

#define MAX 60

int main(void) {
  srand(time(NULL));
  Tree *tree = tree_create();
  int values[MAX];
  for (int i = 0; i < MAX; i++) {
    tree_insert(tree, random() % 100);
  }
  tree_draw(tree);
  tree_destroy(tree);
  return 0;
}
