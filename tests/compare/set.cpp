#include <iostream>
#include <set>
#include <ctime>
#include "ANSI_colors.hpp"

int main()
{
    std::set<int> set;

    clock_t time_start = clock();
    char command;
    while(std::cin >> command) {

        int first_key, second_key;
        volatile int answer;
        switch (command) {
            case 'k':
                std::cin >> first_key;
                set.insert(first_key);
                break;

            case 'q':
                std::cin >> first_key >> second_key;
                if (first_key >= second_key)
                    answer = 0;
                else 
                    answer = std::distance(set.lower_bound(first_key), set.upper_bound(second_key));
                
                break;

            default:
                std::cerr << print_lred("Error input, need key \"k\" or \"q\"\n");
                return 0;
        }
    }
    clock_t time_end = clock();
    double time =  (static_cast<double>(time_end - time_start)) / CLOCKS_PER_SEC;
    std::cout << time << "\n";

    return 0;
}