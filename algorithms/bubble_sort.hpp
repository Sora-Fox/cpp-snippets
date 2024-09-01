#ifndef BUBBLE_SORT_HPP
#define BUBBLE_SORT_HPP

#include <iterator> // for size_t, std::begin/end, std::iter_swap, std::next

namespace sorting {

template <typename It>
void bubbleSort(It, It);

// for STL/custom containers and static C-style arrays
template <typename T>
void bubbleSort(T& container) {
    if (std::size(container) > 1)
        bubbleSort(std::begin(container), std::end(container));
}

// for dynamic C-style arrays
template <typename T>
void bubbleSort(T*& arrayPtr, size_t size) {
    if (size > 1)
        bubbleSort(arrayPtr, arrayPtr + size);
}

template <typename It>
void bubbleSort(It beginIt, It rightIt) {
    It lastUnsorted = rightIt;
    bool swapped;
    do {
        swapped = false;

        for (It iter = beginIt; std::next(iter) != lastUnsorted; ++iter) {
            It nextIt = std::next(iter);
            if (*iter > *nextIt) {
                std::iter_swap(iter, nextIt);
                swapped = true;
            }
        }
        --lastUnsorted;
    } while (swapped);
}

} // namespace sorting

#endif // BUBBLE_SORT_HPP
