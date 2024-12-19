#ifndef FTL_DETAIL_FORWARD_ITERATOR
#define FTL_DETAIL_FORWARD_ITERATOR

#include <iterator>

namespace ftl {
  namespace detail {
    template <typename Value, typename Pointer, typename Reference>
    class ForwardIterator
    {
    public:
      using value_type = Value;
      using difference_type = std::ptrdiff_t;
      using pointer = Pointer;
      using reference = Reference;
      using iterator_category = std::forward_iterator_tag;

      ForwardIterator(pointer ptr = nullptr) : ptr_(ptr) { }
      ForwardIterator(const ForwardIterator&) = default;
      ForwardIterator& operator=(const ForwardIterator&) = default;

      ForwardIterator& operator++();
      ForwardIterator operator++(int);

      reference operator*() const { return *ptr_; }
      pointer operator->() const { return ptr_; }

      bool operator==(const ForwardIterator& rhs) const;
      bool operator!=(const ForwardIterator& rhs) const;

    protected:
      pointer ptr_;
    };

    template <typename V, typename P, typename R>
    ForwardIterator<V, P, R>& ForwardIterator<V, P, R>::operator++()
    {
      ++ptr_;
      return *this;
    }

    template <typename V, typename P, typename R>
    ForwardIterator<V, P, R> ForwardIterator<V, P, R>::operator++(int)
    {
      ForwardIterator<V, P, R> tmp = *this;
      ++(*this);
      return tmp;
    }

    template <typename V, typename P, typename R>
    bool ForwardIterator<V, P, R>::operator==(const ForwardIterator& rhs) const
    {
      return ptr_ == rhs.ptr_;
    }

    template <typename V, typename P, typename R>
    bool ForwardIterator<V, P, R>::operator!=(const ForwardIterator& rhs) const
    {
      return !(ptr_ == rhs.ptr_);
    }
  }
}

#endif

