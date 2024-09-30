#include <iostream>
#include <set>
#include "ANSI_colors.hpp"

int main()
{
    std::set<int> set;
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
                if (first_key >= second_key)
                    answer = 0;
                else 
                    answer = std::distance(set.lower_bound(first_key), set.upper_bound(second_key));
                std::cout << answer << ' ';
                
                break;

            default:
                std::cerr << print_lred("Error input, need key \"k\" or \"q\"\n");
                return 0;
        }
    }

    std::cout << "\n";

    return 0;
}