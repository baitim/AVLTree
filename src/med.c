#include <stdio.h>

#include "avl.h"

int main()
{
    Tree* T = NULL;
    double a;
	while (scanf("%lf", &a) == 1)
		T = tree_insert(T, a);

    print_tree(T);
    printf("\nmedian = %.2lf\n", tree_median(T));
    tree_destroy(T);
}