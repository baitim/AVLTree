#ifndef SRC_AVL_H
#define SRC_AVL_H

typedef struct _Tree {
    double x;
    int cl;
    int cr;
	int h;
    struct _Tree* left;
    struct _Tree* right;
} Tree ;

Tree*  tree_insert  (Tree* t, double x);
double tree_median  (Tree* tree);
void   print_tree   (Tree* tree);
void   tree_destroy (Tree* t);

#endif // SRC_AVL_H