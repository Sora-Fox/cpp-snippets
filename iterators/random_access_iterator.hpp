#ifndef FTL_DETAIL_RANDOM_ACCESS_ITERATOR
#define FTL_DETAIL_RANDOM_ACCESS_ITERATOR

#include "bidirectional_iterator.hpp"

namespace ftl {
  namespace detail {
    template <typename Value, typename Pointer, typename Reference>
    class RandomAccessIterator :
      public BidirectionalIterator<Value, Pointer, Reference>
    {
    private:
      using BaseIt = BidirectionalIterator<Value, Pointer, Reference>;

    public:
      using typename BaseIt::difference_type;
      using typename BaseIt::pointer;
      using typename BaseIt::reference;
      using typename BaseIt::value_type;
      using iterator_category = std::random_access_iterator_tag;

      RandomAccessIterator(pointer ptr = nullptr) : BaseIt(ptr) { }
      RandomAccessIterator(const RandomAccessIterator&) = default;
      RandomAccessIterator& operator=(const RandomAccessIterator&) = default;

      RandomAccessIterator& operator+=(difference_type n);
      RandomAccessIterator& operator-=(difference_type n);

      difference_type operator-(const RandomAccessIterator& rhs) const;
      RandomAccessIterator operator-(difference_type n) const;

      reference operator[](difference_type n) const { return *(ptr_ + n); }

      bool operator<(const RandomAccessIterator& rhs) const;
      bool operator>(const RandomAccessIterator& rhs) const;
      bool operator<=(const RandomAccessIterator& rhs) const;
      bool operator>=(const RandomAccessIterator& rhs) const;

    protected:
      using BidirectionalIterator<Value, Pointer, Reference>::ptr_;
    };

    template <typename V, typename P, typename R>
    RandomAccessIterator<V, P, R>& RandomAccessIterator<V, P, R>::operator+=(
        difference_type n)
    {
      ptr_ += n;
      return *this;
    }

    template <typename V, typename P, typename R>
    RandomAccessIterator<V, P, R>& RandomAccessIterator<V, P, R>::operator-=(
        difference_type n)
    {
      return (*this += -n);
    }

    template <typename V, typename P, typename R>
    typename RandomAccessIterator<V, P, R>::difference_type
    RandomAccessIterator<V, P, R>::operator-(
        const RandomAccessIterator& rhs) const
    {
      return ptr_ - rhs.ptr_;
    }

    template <typename V, typename P, typename R>
    RandomAccessIterator<V, P, R> RandomAccessIterator<V, P, R>::operator-(
        difference_type n) const
    {
      RandomAccessIterator result { ptr_ - n };
      return result;
    }

    template <typename V, typename P, typename R>
    bool RandomAccessIterator<V, P, R>::operator<(
        const RandomAccessIterator& rhs) const
    {
      return ptr_ < rhs.ptr_;
    }

    template <typename V, typename P, typename R>
    bool RandomAccessIterator<V, P, R>::operator>(
        const RandomAccessIterator& rhs) const
    {
      return ptr_ > rhs.ptr_;
    }

    template <typename V, typename P, typename R>
    bool RandomAccessIterator<V, P, R>::operator<=(
        const RandomAccessIterator& rhs) const
    {
      return ptr_ <= rhs.ptr_;
    }

    template <typename V, typename P, typename R>
    bool RandomAccessIterator<V, P, R>::operator>=(
        const RandomAccessIterator& rhs) const
    {
      return ptr_ >= rhs.ptr_;
    }
  }
}

template <typename V, typename P, typename R>
ftl::detail::RandomAccessIterator<V, P, R> operator+(
    const ftl::detail::RandomAccessIterator<V, P, R>& rhs,
    typename ftl::detail::RandomAccessIterator<V, P, R>::difference_type n)
{
  ftl::detail::RandomAccessIterator<V, P, R> tmp = rhs;
  tmp += n;
  return tmp;
}

template <typename V, typename P, typename R>
ftl::detail::RandomAccessIterator<V, P, R> operator+(
    typename ftl::detail::RandomAccessIterator<V, P, R>::difference_type n,
    const ftl::detail::RandomAccessIterator<V, P, R>& rhs)
{
  return rhs + n;
}

#endif

