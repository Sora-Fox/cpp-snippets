#ifndef BUBBLE_SORT_HPP
#define BUBBLE_SORT_HPP

#include <vector>
#include <cstddef>


template <typename T>
void bubbleSort(std::vector<T>& vector)
{
    bool swapped;
    size_t unsorted_length{ vector.size() };

    do 
    {
        swapped = false;

        for (size_t i{ 0 }; i < unsorted_length - 1; ++i)
        {
            if (vector[i] > vector[i + 1])
            {
                std::swap(vector[i], vector[i + 1]);
                swapped = true;
            }
        }
        
        --unsorted_length;
    } while (swapped);
}


#endif // BUBBLE_SORT_HPP
