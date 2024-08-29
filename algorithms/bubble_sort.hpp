#ifndef BUBBLE_SORT_HPP
#define BUBBLE_SORT_HPP

#include <iterator>


template <typename T>
void bubbleSort(T& container)
{
    auto unsorted_length{ std::distance(std::begin(container), std::end(container)) };
    if (unsorted_length < 2) return;

    bool swapped;
    do 
    {
        swapped = false;

        auto iter{ std::begin(container) };

        for (size_t index{ 0 }; index < unsorted_length - 1; ++index, ++iter)
        {
            auto next_iter{ std::next(iter) };

            if (*iter > *next_iter)
            {
                std::iter_swap(iter, next_iter);
                swapped = true;
            }
        }
        
        --unsorted_length;
    } while (swapped);
}

// for dynamic C-style arrays
template <typename T>
void bubbleSort(T* array, size_t size)
{
    if (size < 2) return;

    bool swapped;
    do
    {
        swapped = false;
        for (size_t i{ 0 }; i < size - 1; ++i)
        {
            if (array[i] > array[i + 1])
            {
                std::swap(array[i], array[i + 1]);
                swapped = true;
            }
        }
        --size;
    } while (swapped);
}


#endif // BUBBLE_SORT_HPP
