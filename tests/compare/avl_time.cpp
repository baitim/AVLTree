#include <iostream>
#include <ctime>
#include "avl.hpp"

int main()
{
    avl_tree::avl_t<int> avl_tree{INT32_MAX};

    clock_t time_start = clock();
    char command;
    while(std::cin >> command) {

        int first_key, second_key;
        volatile int answer;
        switch (command) {
            case 'k':
                std::cin >> first_key;
                avl_tree.insert(first_key);
                break;

            case 'q':
                std::cin >> first_key >> second_key;
                answer = avl_tree.check_range(first_key, second_key);
                break;

            default:
                std::cerr << print_lred("Error input, need key \"k\" or \"q\"\n");
                return 0;
        }

#ifdef DEBUG
        avl_tree.print();
#endif
    }
    clock_t time_end = clock();
    double time =  (static_cast<double>(time_end - time_start)) / CLOCKS_PER_SEC;
    std::cout << time << "\n";

    return 0;
}