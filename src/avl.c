#include <stdio.h>
#include <stdlib.h>

#include "avl.h"

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define H(a) ((!(a)) ? 0 : (a)->h)
#define R(a) ((!(a)) ? NULL : (a)->right)
#define L(a) ((!(a)) ? NULL : (a)->left)

#define LN(a) ((a) ? (a)->cl : 0)
#define RN(a) ((a) ? (a)->cr : 0)
#define IS(a) ((a) ? 1 : 0)
#define D(T)  ((T) ? (T)->x : 0)
#define F(T)  ((T) ? 1 : 0)

static Tree*  tree_balance  (Tree* n);
static Tree*  node_ctor     (Tree* l, double x, Tree* r);
static double node_median   (Tree* t, int l, int r);
static void   print_node    (Tree* tree, int dep);

static Tree* tree_balance(Tree* n)
{
	Tree* new = n;
    int bal = H(R(n)) - H(L(n));
	if (bal > 1) {
        Tree* r = R(n);
		if (H(R(r)) >= H(L(r))) {
			new = node_ctor(node_ctor(L(n), D(n), L(r)), D(r), R(r));
		}
		else {
			new = node_ctor(node_ctor(L(n), D(n), L(L(r))), D(L(r)), node_ctor(R(L(r)), D(r), R(r)));
			free(L(r));
		}
		free(r);
		free(n);
	} else if (bal < -1) {
        Tree* l = L(n);
		if (H(L(l)) >= H(R(l))) {
			new = node_ctor(L(l), D(l), node_ctor(R(l), D(n), R(n)));
		} else {
			new = node_ctor(node_ctor(L(l), D(l), L(R(l))), D(R(l)), node_ctor(R(R(l)), D(n), R(n)));
			free(R(l));
		}
		free(l);
		free(n);
	}
	return new;
}

Tree* tree_insert(Tree* t, double x)
{
    if(!t) {
        Tree* new = malloc(sizeof(Tree));
        *new = (Tree){x, 0, 0, 1, NULL, NULL};
        return new;
    }
    if (x >= t->x) { 
        t->cl++;
        t->left = tree_insert(t->left, x);
    } else {
        t->cr++;
        t->right = tree_insert(t->right, x);
    }
    t->h = MAX(H(R(t)), H(L(t))) + 1;
    return tree_balance(t);
}

static Tree *node_ctor(Tree* l, double x, Tree* r)
{
    Tree* n = (Tree*) malloc(sizeof(Tree));
    *n = (Tree){x, LN(l) + RN(l) + IS(l), LN(r) + RN(r) + IS(r), MAX(H(l), H(r))+1, l, r};
    return n;
}

static double node_median(Tree* t, int l, int r)
{
	if (RN(t) - LN(t) + r - l > 1) return node_median(t->right, l + LN(t) + 1, r);
	if (LN(t) - RN(t) + l - r > 1) return node_median(t->left, l, r + RN(t) + 1);
	return t->x;
}

double tree_median(Tree* tree)
{
	return node_median(tree, 0, 0);
}

static void print_node(Tree* tree, int dep)
{
	if (!tree)
		return;
	print_node(tree->left, dep + 1);
    int i;
	for (i = 0; i < dep; i++) printf("\t");
	printf("%.2lf\n", tree->x);
	print_node(tree->right, dep + 1);
}

void print_tree(Tree* tree)
{
    printf("Tree:\n");
	print_node(tree, 0);
}

void tree_destroy(Tree* t) 
{
    if (!t) return;
    tree_destroy(L(t));
    tree_destroy(R(t));
    free(t);
}