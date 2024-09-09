#ifndef FTL_MERGE_SORT_HPP
#define FTL_MERGE_SORT_HPP

#include <iterator>  // for std::begin/end, std::iterator_traits
#include <vector>    // for std::vector

namespace ftl {

template <typename It>
void mergeSort(It, It);

template <typename It>
void merge(It, It, It);

// for STL/custom containers and static C-style arrays
template <typename T>
void mergeSort(T& container) {
    if (std::size(container) > 1)
        mergeSort(std::begin(container), std::end(container));
}

// for dynamic C-style arrays
template <typename T>
void mergeSort(T*& arrayPtr, size_t size) {
    if (size > 2)
        mergeSort(arrayPtr, arrayPtr + size);
}

template <typename It>
void mergeSort(It beginIt, It endIt) {
    if (beginIt != endIt && std::next(beginIt) != endIt) {
        It midIt = std::next(beginIt, std::distance(beginIt, endIt) / 2);
        mergeSort(beginIt, midIt);
        mergeSort(midIt, endIt);
        merge(beginIt, midIt, endIt);
    }
}

template <typename It>
void merge(It beginIt, It midIt, It endIt) {
    using ValueType = typename std::iterator_traits<It>::value_type;
    std::vector<ValueType> beginIt_arr(std::distance(beginIt, midIt));
    std::vector<ValueType> endIt_arr(std::distance(midIt, endIt));

    std::copy(beginIt, midIt, std::begin(beginIt_arr));
    std::copy(midIt, endIt, std::begin(endIt_arr));

    auto leftIt = beginIt_arr.begin();
    auto rightIt = endIt_arr.begin();

    while (leftIt != beginIt_arr.end() && rightIt != endIt_arr.end()) {
        *beginIt = (*leftIt < *rightIt) ? *leftIt++ : *rightIt++;
        ++beginIt;
    }

    std::copy(leftIt, beginIt_arr.end(), beginIt);
    std::copy(rightIt, endIt_arr.end(), beginIt);
}

}  // namespace ftl

#endif  // FTL_MERGE_SORT_HPP
