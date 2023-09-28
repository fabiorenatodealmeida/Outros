#ifndef _TREE_H
#define _TREE_H 1

typedef struct tree Tree;

Tree *tree_create(void);
void tree_destroy(Tree *tree);
void tree_insert(Tree *tree, int value);
void tree_remove(Tree *tree, int value);
int *tree_search(Tree *tree, int value);
void tree_print(Tree *tree);
void tree_draw(Tree *tree);

#endif
