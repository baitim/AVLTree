#include <iostream>
#include <cstdint>
#include "avl.hpp"

int main()
{
    avl_tree::avl_tree_t<int> avl_tree{INT32_MAX};

    char command;
    while(std::cin >> command) {
        if (!std::cin.good()) {
            std::cout << print_red("Error input, need command as char\n");
            return 1;
        }

        int first_key, second_key, answer;
        switch (command) {
            case 'k':
                std::cin >> first_key;
                if (!std::cin.good() && !std::cin.eof()) {
                    std::cout << print_red("Error input, need key as int\n");
                    return 1;
                }
                avl_tree.insert(first_key);
                break;

            case 'q':
                std::cin >> first_key >> second_key;
                if (!std::cin.good() && !std::cin.eof()) {
                    std::cout << print_red("Error input, need key as int\n");
                    return 1;
                }
                answer = distance<int>(avl_tree.lower_bound(first_key), avl_tree.upper_bound(second_key));
                std::cout << answer << ' ';
                break;

            default:
                std::cout << print_red("Error input, need command: \"k\" or \"q\"\n");
                return 1;
        }

#ifdef DEBUG
        std::cout << avl_tree << "\n";
#endif
    }
    std::cout << "\n";

    return 0;
}