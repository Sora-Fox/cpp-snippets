#include <iostream>
#include <string>
#include <algorithm>
#include <limits>

void stringToLower(std::string& data)
{
    std::transform(data.begin(), data.end(), data.begin(),
    [](unsigned char c){ return std::tolower(c); });
}

void finder()
{
    std::string text, text_to_find;

    std::cout << "Enter the text: " << std::flush;
    std::getline(std::cin, text);

    std::cout << "Enter a character or substring to find: " << std::flush;
    std::getline(std::cin, text_to_find);

    std::cout << "Consider text case (y/n): " << std::flush;
    char respond;
    std::cin >> respond; 

    if (respond == 'N' || respond == 'n')
    {
        stringToLower(text);
        stringToLower(text_to_find);
    }


    for (size_t index{ 0 }, matches{ 0 }; index < text.length(); ++matches)
    {
        size_t found_index = text.find(text_to_find, index);

        if (found_index == std::string::npos && matches > 0)
        {
            std::cout << "Total matches: " << matches << std::endl;
            return;
        }
        else if (found_index == std::string::npos)
        {
            std::cout << "No matches found." << std::endl;
            return;
        }

        std::cout << "\tMatch at index: " << found_index << std::endl;
        index = ++found_index;
    }
}

int main()
{
    finder();
    return 0;
}
