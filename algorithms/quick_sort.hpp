#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP

#include <iterator>

template <typename It>
void quickSort(It, It);

template <typename It>
It partition(It, It);

template <typename T>
void quickSort(T& container)
{
    if (std::size(container) > 1)
        quickSort(std::begin(container), std::end(container));
}

template <typename T>
void quickSort(T* dynamic_arr, size_t size)
{
    if (size > 1)
        quickSort(dynamic_arr, dynamic_arr + size);
}

template <typename It>
void quickSort(It first, It last)
{
    if (first != last && std::next(first) != last)
    {
        It i = partition(first, last);
        quickSort(first, i);
        quickSort(i, last);
    }
}

template <typename It>
It partition(It first, It last)
{
    auto pivot = *std::prev(last);
    It left = first;
    It right = std::prev(last);

    while (true)
    {
        while (*left < pivot) ++left;
        while (*right > pivot) --right;

        if (left == right || left == std::next(right))
            return left;

        std::iter_swap(left, right);
        ++left;
        --right;
    }
}

#endif // QUICK_SORT_HPP
