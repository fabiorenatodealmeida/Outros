#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tree.h"

#define MAX 512

struct tree_node {
  int value;
  struct tree_node *left;
  struct tree_node *right;
  int len;
  int L_, R_;
};

typedef struct tree_node Node;

struct tree {
  Node *root;
  Node *bottom[MAX];
  int pos;
  int ignore_spc;
};

Tree *tree_create(void) {
  Tree *tree = (Tree *)malloc(sizeof(Tree));
  tree->root = NULL;
  return tree;
}

static void tree_destroy_(Node *node) {
  if (node != NULL) {
    tree_destroy_(node->left);
    tree_destroy_(node->right);
    free(node);
  }
}

void tree_destroy(Tree *tree) {
  tree_destroy_(tree->root);
}

static void tree_insert_(Node *parent, Node *node) {
  if (node->value == parent->value) {
    free(node);
  } else if (node->value < parent->value) {
    if (parent->left == NULL) {
      parent->left = node;
    } else {
      tree_insert_(parent->left, node);
    }
  } else {
    if (parent->right == NULL) {
      parent->right = node;
    } else {
      tree_insert_(parent->right, node);
    }
  }
}

void tree_insert(Tree *tree, int value) {
  Node *node = (Node *)malloc(sizeof(Node));
  node->value = value;
  node->left = node->right = NULL;
  char buffer[15];
  sprintf(buffer, "%d", value);
  node->len = strlen(buffer);
  node->L_ = node->R_ = 0;
  if (tree->root == NULL) {
    tree->root = node;
  } else {
    tree_insert_(tree->root, node);
  }
}

static Node *tree_remove_(Node *node, int value) {
  if (node == NULL) {
    return NULL;
  } else if (value < node->value) {
    node->left = tree_remove_(node->left, value);
  } else if (value > node->value) {
    node->right = tree_remove_(node->right, value);
  } else {
    if (node->left == NULL && node->right == NULL) {
      free(node);
      node = NULL;
    } else if (node->left == NULL) {
      Node *hold = node;
      node = node->right;
      free(hold);
    } else if (node->right == NULL) {
      Node *hold = node;
      node = node->left;
      free(hold);
    } else {
      Node *n = node->left;
      while (n->right != NULL) {
        n = n->right;
      }
      node->value = n->value;
      n->value = value;
      node->left = tree_remove_(node->left, value);
    }
  }
  return node;
}

void tree_remove(Tree *tree, int value) {
  tree->root = tree_remove_(tree->root, value);
}

static int *tree_search_(Node *node, int value) {
  if (node == NULL) return NULL;
  else if (value < node->value) return tree_search_(node->left, value);
  else if (value > node->value) return tree_search_(node->right, value);
  else return &node->value;
}

int *tree_search(Tree *tree, int value) {
  return tree_search_(tree->root, value);
}

static void tree_print_(Node *node) {
  if (node != NULL) {
    tree_print_(node->left);
    printf("%d ", node->value);
    tree_print_(node->right);
  }
}

void tree_print(Tree *tree) {
  tree_print_(tree->root);
  printf("\n");
}

static int size(Node *node) {
  if (node == NULL) return 0;
  node->L_ = node->R_ = 0;
  int sz = node->len;
  if (node->left != NULL) {
    if (node->left->right != NULL) node->L_ = size(node->left->right) + 1;
    sz += size(node->left) + 1;
  }
  if (node->right != NULL) {
    if (node->right->left != NULL) node->R_ = size(node->right->left) + 1;
    sz += size(node->right) + 1;
  }
  return sz;
}

static void fill_bottom(Tree *tree, Node *node) {
  if (node != NULL) {
    if (node->left != NULL) fill_bottom(tree, node->left);
    tree->bottom[tree->pos++] = node;
    if (node->right != NULL) fill_bottom(tree, node->right);
  }
}

static void printNode(Node *node) {
  if (node->left != NULL) printf(" ");
  printf("%d", node->value);
  if (node->right != NULL) printf(" ");
}

static void printSpace(Tree *tree, Node *node) {
  if (node->left != NULL) {
    if (tree->ignore_spc > 0) {
      tree->ignore_spc--;
    } else {
      printf(" ");
    }
  }
  for (int i = 0; i < node->len; i++) {
    if (tree->ignore_spc > 0) {
      tree->ignore_spc--;
    } else {
      printf(" ");
    }
  }
  if (node->right != NULL) {
    if (tree->ignore_spc > 0) {
      tree->ignore_spc--;
    } else {
      printf(" ");
    }
  }
}

static void printLine(Tree *tree, Node *node) {
  if (node->left != NULL) {
    for (int i = 0; i < node->L_; i++) printf("\b");
  }
  if (node->left != NULL) {
    for (int i = 0; i < node->L_; i++) printf("_");
    printf("/");
  }
  for (int i = 0; i < node->len; i++) printf(" ");
  if (node->right != NULL) {
    printf("\\");
    for (int i = 0; i < node->R_; i++) printf("_");
    tree->ignore_spc += node->R_;
  }
}

static void print(Tree *tree, Node *nodes[]) {
  int i, pos;
  Node *node;
  tree->ignore_spc = 0;
  node = nodes[0];
  i = pos = 0;
  while (node != NULL) {
    while (1) {
      if (node == tree->bottom[i]) {
        printNode(node);
        i++;
        break;
      } else {
        printSpace(tree, tree->bottom[i]);
        i++;
      }
    }
    node = nodes[++pos];
  }
  printf("\n");
  tree->ignore_spc = 0;
  node = nodes[0];
  i = pos = 0;
  while (node != NULL) {
    while (1) {
      if (node == tree->bottom[i]) {
        printLine(tree, node);
        i++;
        break;
      } else {
        printSpace(tree, tree->bottom[i]);
        i++;
      }
    }
    node = nodes[++pos];
  }
  printf("\n");
  Node *next[MAX] = { NULL };
  node = nodes[0];
  i = pos = 0;
  while (node != NULL) {
    if (node->left != NULL) next[i++] = node->left;
    if (node->right != NULL) next[i++] = node->right;
    node = nodes[++pos];
  }
  if (i > 0) print(tree, next);
}

void tree_draw(Tree *tree) {
  size(tree->root);
  fill_bottom(tree, tree->root);
  Node *nodes[MAX] = { NULL };
  nodes[0] = tree->root;
  print(tree, nodes);
}
