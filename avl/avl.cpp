#include <iostream>
#include "avl.hpp"

int main()
{
    avl_tree::avl_t<int> avl_tree;

    char command;
    while(std::cin >> command) {

        int elem, left_border, right_border, count;
        switch (command) {
            case 'k':
                std::cin >> elem;
                avl_tree.insert(elem);
                break;

            case 'q':
                std::cin >> left_border >> right_border;
                count = avl_tree.check_range(left_border, right_border);
                std::cout << count << ' ';
                break;

            default:
                std::cout << print_lred("Error input, need key \"k\" or \"q\"");
                return 0;
        }

#ifdef DEBUG
        avl_tree.print();
#endif
    }
    std::cout << '\n';

    return 0;
}