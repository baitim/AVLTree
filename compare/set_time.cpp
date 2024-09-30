#include <iostream>
#include <set>
#include "ANSI_colors.hpp"
#include "benchmark.hpp"

int main()
{
    using clock = std::chrono::high_resolution_clock;
    std::set<int> set;
    
    auto time_start = clock::now().time_since_epoch().count();
    char command;
    while(std::cin >> command) {

        int first_key, second_key, answer;
        switch (command) {
            case 'k':
                std::cin >> first_key;
                set.insert(first_key);
                break;

            case 'q':
                std::cin >> first_key >> second_key;
                if (first_key < second_key)
                    answer = std::distance(set.lower_bound(first_key), set.upper_bound(second_key));
                NO_OPT(answer);
                
                break;

            default:
                std::cerr << print_lred("Error input, need key \"k\" or \"q\"\n");
                return 0;
        }
    }
    auto time_end = clock::now().time_since_epoch().count();
    long double time = (static_cast<long double>(time_end - time_start)) * NSEC_TO_SEC;
    std::cout << time << "\n";

    return 0;
}