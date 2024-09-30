#include <iostream>
#include "avl.hpp"
#include "benchmark.hpp"

int main()
{
    using clock = std::chrono::high_resolution_clock;
    avl_tree::avl_t<int> avl_tree{INT32_MAX};
    
    auto time_start = clock::now().time_since_epoch().count();
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
                NO_OPT(answer);
                break;

            default:
                std::cerr << print_lred("Error input, need key \"k\" or \"q\"\n");
                return 0;
        }

#ifdef DEBUG
        avl_tree.print();
#endif
    }
    auto time_end = clock::now().time_since_epoch().count();
    long double time = (static_cast<long double>(time_end - time_start)) * NSEC_TO_SEC;
    std::cout << time << "\n";

    return 0;
}