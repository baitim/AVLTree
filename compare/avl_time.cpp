#include <iostream>
#include "avl.hpp"
#include "benchmark.hpp"

int main()
{
    using clock = std::chrono::high_resolution_clock;
    avl_tree::avl_tree_t<int> avl_tree{INT32_MAX};
    
    auto time_start = clock::now().time_since_epoch().count();
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
                if (first_key >= second_key)
                    answer = 0;
                else
                    answer = distance(avl_tree.lower_bound(first_key), avl_tree.upper_bound(second_key));
                NO_OPT(answer);
                break;

            default:
                std::cout << print_red("Error input, need command: \"k\" or \"q\"\n");
                return 1;
        }

#ifdef DEBUG
        std::cout << avl_tree << "\n";
#endif
    }
    auto time_end = clock::now().time_since_epoch().count();
    long double time = (static_cast<long double>(time_end - time_start)) * NSEC_TO_SEC;
    std::cout << time << "\n";

    return 0;
}