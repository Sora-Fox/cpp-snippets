#ifndef QUICK_SORT_HPP
#define QUICK_SORT_HPP

#include <iterator> // for size_t, std::prev, std::next, std::iter_swap, std::size

namespace sorting {

template <typename It>
void quickSort(It, It);

template <typename It>
It partition(It, It);

// for STL/custom containers and static C-style arrays
template <typename T>
void quickSort(T& container) {
    if (std::size(container) > 1)
        quickSort(std::begin(container), std::end(container));
}

// for dynamic C-style arrays
template <typename T>
void quickSort(T* arrayPtr, size_t size) {
    if (size > 1)
        quickSort(arrayPtr, arrayPtr + size);
}

template <typename It>
void quickSort(It beginIt, It endIt) {
    if (beginIt != endIt && std::next(beginIt) != endIt) {
        It middleIt = partition(beginIt, endIt);
        quickSort(beginIt, middleIt);
        quickSort(middleIt, endIt);
    }
}

template <typename It>
It partition(It beginIt, It endIt) {
    auto pivot = *std::prev(endIt);
    It leftIt = beginIt;
    It rightIt = std::prev(endIt);

    while (true) {
        while (*leftIt < pivot)
            ++leftIt;
        while (*rightIt > pivot)
            --rightIt;

        if (leftIt == rightIt || leftIt == std::next(rightIt))
            return leftIt;

        std::iter_swap(leftIt, rightIt);
        ++leftIt;
        --rightIt;
    }
}

} // namespace sorting

#endif // QUICK_SORT_HPP
