#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP

#include <vector>
#include <cstddef>


template <typename T>
void quickSort(std::vector<T>&, size_t, size_t);

template <typename T>
void quickSort(std::vector<T>& arr)
{
    if (!arr.empty())
        quickSort(arr, 0, arr.size() - 1);
}

template <typename T>
void quickSort(std::vector<T>& arr, size_t left, size_t right)
{
    if (left < right)
    {
        T pivot{ arr[left + (right - left) / 2] };
        size_t left_ind{ left }, right_ind{ right };

        while(left_ind <= right_ind)
        {
            while (arr[left_ind] < pivot && left_ind < right) ++left_ind;
            while (arr[right_ind] > pivot && right_ind > left) --right_ind;

            if (left_ind <= right_ind)
            {
                std::swap(arr[left_ind], arr[right_ind]);
                ++left_ind;
                if (right_ind > left) --right_ind;
            }
        }
        
        if (left < right_ind)
            quickSort(arr, left, right_ind);
        if (left_ind < right)
            quickSort(arr, left_ind, right);
    }
}


#endif // QUICK_SORT_HPP
