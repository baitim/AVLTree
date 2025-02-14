#include "AVLTree/ANSI_colors.hpp"
#include <iostream>
#include <set>

int main()
{
    std::set<int> set;
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
                if (first_key >= second_key)
                    answer = 0;
                else 
                    answer = distance(set.lower_bound(first_key), set.upper_bound(second_key));
                std::cout << answer << ' ';
                
                break;

            default:
                std::cout << print_red("Error input, need command: \"k\" or \"q\"\n");
                return 1;
        }
    }

    std::cout << "\n";

    return 0;
}