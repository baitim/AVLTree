#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) ((a) >= (b) ? (a) : (b))

typedef struct _Tree {
    double x;
    int cl;
    int cr;
	int h;
    struct _Tree *left;
    struct _Tree *right;
} Tree ;

#define H(a) ((!(a)) ? 0 : (a)->h)
#define R(a) ((!(a)) ? NULL : (a)->right)
#define L(a) ((!(a)) ? NULL : (a)->left)

Tree *tree_balance(Tree *n);

Tree *tree_insert(Tree *t, double x)
{
    if(!t) {
        Tree *new = malloc(sizeof(Tree));
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

#define LN(a) ((a) ? (a)->cl : 0)
#define RN(a) ((a) ? (a)->cr : 0)
#define IS(a) ((a) ? 1 : 0)
#define D(T)  ((T) ? (T)->x : 0)

Tree *_Tr(Tree *l, double x, Tree *r)
{
    Tree *n = (Tree *)malloc(sizeof(Tree));
    *n = (Tree){x, LN(l) + RN(l) + IS(l), LN(r) + RN(r) + IS(r), MAX(H(l), H(r))+1, l, r};
    return n;
}

double _tree_median(Tree *t, int l, int r)
{
	if (RN(t) - LN(t) + r - l > 1) return _tree_median(t->right, l + LN(t) + 1, r);
	if (LN(t) - RN(t) + l - r > 1) return _tree_median(t->left, l, r + RN(t) + 1);
	return t->x;
}

double tree_median(Tree *tree)
{
	return _tree_median(tree, 0, 0);
}

void _print_tree(Tree *tree, int dep)
{
	if (!tree)
		return;
	_print_tree(tree->left, dep + 1);
    int i;
	for (i = 0; i < dep; i++) printf("\t");
	printf("%.2lf\n", tree->x);
	_print_tree(tree->right, dep + 1);
}

void print_tree(Tree *tree)
{
    printf("Tree:\n");
	_print_tree(tree, 0);
}

void tree_destroy(Tree *t) 
{
    if (!t) return;
    tree_destroy(L(t));
    tree_destroy(R(t));
    free(t);
}

#define F(T) ((T) ? 1 : 0)

Tree *tree_balance(Tree *n)
{
	Tree *new = n;
    int bal = H(R(n)) - H(L(n));
	if (bal > 1) {
        Tree *r = R(n);
		if (H(R(r)) >= H(L(r))) {
			new = _Tr(_Tr(L(n), D(n), L(r)), D(r), R(r));
		}
		else {
			new = _Tr(_Tr(L(n), D(n), L(L(r))), D(L(r)), _Tr(R(L(r)), D(r), R(r)));
			free(L(r));
		}
		free(r);
		free(n);
	} else if (bal < -1) {
        Tree *l = L(n);
		if (H(L(l)) >= H(R(l))) {
			new = _Tr(L(l), D(l), _Tr(R(l), D(n), R(n)));
		} else {
			new = _Tr(_Tr(L(l), D(l), L(R(l))), D(R(l)), _Tr(R(R(l)), D(n), R(n)));
			free(R(l));
		}
		free(l);
		free(n);
	}
	return new;
}

int main()
{
    Tree *T = NULL;
    double a;
	while (scanf("%lf", &a) == 1)
		T = tree_insert(T, a);

    print_tree(T);
    printf("\nmedian = %.2lf\n", tree_median(T));
    tree_destroy(T);
}