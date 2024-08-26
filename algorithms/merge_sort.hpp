#ifndef MERGE_SORT_HPP
#define MERGE_SORT_HPP

#include <vector>
#include <cstddef>


template <typename T>
void mergeSort(std::vector<T>&, size_t, size_t);

template <typename T>
void merge(std::vector<T>&, size_t, size_t, size_t);


template <typename T>
void mergeSort(std::vector<T>& arr)
{
    if (!arr.empty())
        mergeSort(arr, 0, arr.size() - 1);
}

template <typename T>
void mergeSort(std::vector<T>& arr, size_t left, size_t right)
{
    if (left < right)
    {
        size_t mid{ left + (right - left) / 2 };
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }

}

template <typename T>
void merge(std::vector<T>& arr, size_t left, size_t mid, size_t right)
{
    std::vector<T> left_arr(mid - left + 1), right_arr(right - mid);

    for (size_t i{ 0 }, j{ left }; i < left_arr.size(); ++i, ++j)
        left_arr[i] = arr[j];
    for (size_t i{ 0 }, j{ mid + 1 }; i < right_arr.size(); ++i, ++j)
        right_arr[i] = arr[j];

    size_t left_ind{ 0 }, right_ind{ 0 }, arr_ind{ left };

    while (left_ind < left_arr.size() && right_ind < right_arr.size())
    {
        if (left_arr[left_ind] < right_arr[right_ind])
        {
            arr[arr_ind++] = left_arr[left_ind++];
        }
        else
        {
            arr[arr_ind++] = right_arr[right_ind++];
        }
    }

    while (left_ind < left_arr.size())
    {
        arr[arr_ind++] = left_arr[left_ind++];
    }

    while (right_ind < right_arr.size())
    {
        arr[arr_ind++] = right_arr[right_ind++];
    }
}


#endif // MERGE_SORT_HPP
