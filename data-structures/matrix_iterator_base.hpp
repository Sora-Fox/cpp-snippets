#ifndef FTL_DETAIL_MATRIX_ITERATOR_BASE
#define FTL_DETAIL_MATRIX_ITERATOR_BASE

#include <iterator>

namespace ftl::detail {
  template <typename Value, typename Pointer, typename Reference>
  class MatrixIteratorBase
  {
  public:
    using iterator_category = std::contiguous_iterator_tag;
    using value_type = Value;
    using difference_type = std::ptrdiff_t;
    using pointer = Pointer;
    using reference = Reference;

    MatrixIteratorBase() = default;
    MatrixIteratorBase(const MatrixIteratorBase&) = default;
    MatrixIteratorBase& operator=(const MatrixIteratorBase&) = default;
    MatrixIteratorBase(pointer value) : p_(value) { }

    MatrixIteratorBase& operator++();
    MatrixIteratorBase operator++(int);
    MatrixIteratorBase& operator--();
    MatrixIteratorBase operator--(int);

    MatrixIteratorBase& operator+=(difference_type n);
    MatrixIteratorBase& operator-=(difference_type n) { return (*this += -n); }

    difference_type operator-(const MatrixIteratorBase& r) const { return p_ - r.p_; }
    MatrixIteratorBase operator-(difference_type n) const { return p_ - n; }

    reference operator[](difference_type n) const { return *(p_ + n); }
    reference operator*() const { return *p_; }
    pointer operator->() const { return p_; }
    std::strong_ordering operator<=>(const MatrixIteratorBase&) const = default;

  private:
    pointer p_ = nullptr;
  };

  template <typename V, typename P, typename R>
  MatrixIteratorBase<V, P, R>& MatrixIteratorBase<V, P, R>::operator++()
  {
    ++p_;
    return *this;
  }

  template <typename V, typename P, typename R>
  MatrixIteratorBase<V, P, R> MatrixIteratorBase<V, P, R>::operator++(int)
  {
    MatrixIteratorBase<V, P, R> tmp = *this;
    ++(*this);
    return tmp;
  }

  template <typename V, typename P, typename R>
  MatrixIteratorBase<V, P, R>& MatrixIteratorBase<V, P, R>::operator--()
  {
    --p_;
    return *this;
  }

  template <typename V, typename P, typename R>
  MatrixIteratorBase<V, P, R> MatrixIteratorBase<V, P, R>::operator--(int)
  {
    MatrixIteratorBase<V, P, R> tmp = *this;
    --(*this);
    return tmp;
  }

  template <typename V, typename P, typename R>
  MatrixIteratorBase<V, P, R>& MatrixIteratorBase<V, P, R>::operator+=(difference_type n)
  {
    p_ += n;
    return *this;
  }

  template <typename V, typename P, typename R>
  MatrixIteratorBase<V, P, R> operator+(
      const MatrixIteratorBase<V, P, R>& iterator,
      typename MatrixIteratorBase<V, P, R>::difference_type n)
  {
    MatrixIteratorBase<V, P, R> tmp = iterator;
    tmp += n;
    return tmp;
  }

  template <typename V, typename P, typename R>
  MatrixIteratorBase<V, P, R> operator+(
      typename MatrixIteratorBase<V, P, R>::difference_type n,
      const MatrixIteratorBase<V, P, R>& iterator)
  {
    return iterator + n;
  }
}

#endif
