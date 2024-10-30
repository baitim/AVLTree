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
                set.insert(first_key);
                break;

            case 'q':
                std::cin >> first_key >> second_key;
                if (!std::cin.good() && !std::cin.eof()) {
                    std::cout << print_red("Error input, need key as int\n");
                    return 1;
                }
                if (first_key < second_key)
                    answer = distance(set.lower_bound(first_key), set.upper_bound(second_key));
                NO_OPT(answer);
                break;

            default:
                std::cout << print_red("Error input, need command: \"k\" or \"q\"\n");
                return 1;
        }
    }
    auto time_end = clock::now().time_since_epoch().count();
    long double time = (static_cast<long double>(time_end - time_start)) * NSEC_TO_SEC;
    std::cout << time << "\n";

    return 0;
}