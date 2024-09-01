#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <iterator>
#include <vector>


template <typename It>
void mergeSort(It, It);

template <typename It>
void merge(It, It, It);

template <typename T>
void mergeSort(T& container)
{
    if (std::size(container) > 1)
        mergeSort(std::begin(container), std::end(container));
}

template <typename T>
void mergeSort(T*& dynamic_arr, size_t size)
{
    if (size > 2)
        mergeSort(dynamic_arr, dynamic_arr + size);
}

template <typename It>
void mergeSort(It left, It right)
{
    if (left != right && std::next(left) != right)
    {
        It mid{ std::next(left, std::distance(left, right) / 2) };
        mergeSort(left, mid);
        mergeSort(mid, right);
        merge(left, mid, right);
    }
}

template <typename It>
void merge(It left, It mid, It right)
{
    using value_type = typename std::iterator_traits<It>::value_type;
    std::vector<value_type> left_arr(std::distance(left, mid));
    std::vector<value_type> right_arr(std::distance(mid, right));

    std::copy(left, mid, std::begin(left_arr));
    std::copy(mid, right, std::begin(right_arr));

    auto left_it{ left_arr.begin() };
    auto right_it{ right_arr.begin() };

    while (left_it != left_arr.end() && right_it != right_arr.end())
    {
        *left = (*left_it < *right_it) ? *left_it++ : *right_it++;
        ++left;
    }

    std::copy(left_it, left_arr.end(), left);
    std::copy(right_it, right_arr.end(), left);
}


#endif // MERGE_SORT_HPP
