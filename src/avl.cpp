#include <iostream>
#include <cstdint>
#include "avl.hpp"

int main()
{
    avl_tree::avl_tree_t<int> avl_tree{INT32_MAX};

    char command;
    while(std::cin >> command) {

        int first_key, second_key, answer;
        switch (command) {
            case 'k':
                std::cin >> first_key;
                avl_tree.insert(first_key);
                break;

            case 'q':
                std::cin >> first_key >> second_key;
                answer = avl_tree.check_range(first_key, second_key);
                std::cout << answer << ' ';
                break;

            default:
                std::cerr << print_lred("Error input, need key \"k\" or \"q\"\n");
                return 0;
        }

#if 1
        avl_tree.print();
#endif
    }
    std::cout << "\n";

    return 0;
}