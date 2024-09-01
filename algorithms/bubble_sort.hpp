#ifndef BUBBLE_SORT_HPP
#define BUBBLE_SORT_HPP

#include <iterator>


template <typename It>
void bubbleSort(It left, It right);

template <typename T>
void bubbleSort(T& container)
{
    if (std::size(container) > 1)
        bubbleSort(std::begin(container), std::end(container));
}

// for dynamic C-style arrays
template <typename T>
void bubbleSort(T*& dynamic_arr , size_t size)
{
    if (size > 1)
        bubbleSort(dynamic_arr, dynamic_arr + size);
}

template <typename It>
void bubbleSort(It left, It right)
{
    It unsorted_right{ right };
    bool swapped;
    do 
    {
        swapped = false;

        for (It iter{ left }; std::next(iter) != unsorted_right; ++iter)
        {
            auto next_iter{ std::next(iter) };
            if (*iter > *next_iter)
            {
                std::iter_swap(iter, next_iter);
                swapped = true;
            }
        }
        --unsorted_right;
    } while (swapped);
}


#endif // BUBBLE_SORT_HPP
