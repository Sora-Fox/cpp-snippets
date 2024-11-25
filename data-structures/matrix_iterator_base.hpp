#ifndef FTL_DETAIL_MATRIX_ITERATOR_BASE
#define FTL_DETAIL_MATRIX_ITERATOR_BASE

#include <cstddef>
#include <iterator>

namespace ftl {
  namespace detail {
    template <typename Value, typename Pointer, typename Reference>
    class MatrixIteratorBase
    {
    public:
      using iterator_category = std::random_access_iterator_tag;
      using value_type = Value;
      using difference_type = std::ptrdiff_t;
      using pointer = Pointer;
      using reference = Reference;
      using size_type = std::size_t;

      MatrixIteratorBase() = default;
      MatrixIteratorBase(const MatrixIteratorBase&) = default;
      MatrixIteratorBase& operator=(const MatrixIteratorBase&) = default;
      MatrixIteratorBase(pointer value) : p_(value) { }

      MatrixIteratorBase& operator++();
      MatrixIteratorBase operator++(int);
      MatrixIteratorBase& operator--();
      MatrixIteratorBase operator--(int);

      MatrixIteratorBase& operator+=(difference_type n);
      MatrixIteratorBase& operator-=(difference_type n);

      difference_type operator-(const MatrixIteratorBase& r) const { return p_ - r.p_; }
      MatrixIteratorBase operator-(size_type n) const { return p_ - n; }

      reference operator[](size_type n) const { return *(p_ + n); }
      reference operator*() const { return *p_; }
      pointer operator->() const { return p_; }

      bool operator==(const MatrixIteratorBase& rhs) const { return p_ == rhs.p_; }
      bool operator!=(const MatrixIteratorBase& rhs) const { return p_ != rhs.p_; }
      bool operator<(const MatrixIteratorBase& rhs) const { return p_ < rhs.p_; }
      bool operator>(const MatrixIteratorBase& rhs) const { return p_ > rhs.p_; }
      bool operator<=(const MatrixIteratorBase& rhs) const { return p_ <= rhs.p_; }
      bool operator>=(const MatrixIteratorBase& rhs) const { return p_ >= rhs.p_; }

    private:
      pointer p_ = nullptr;
    };

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference> operator+(
        const MatrixIteratorBase<Value, Pointer, Reference>& iterator,
        typename MatrixIteratorBase<Value, Pointer, Reference>::size_type n)
    {
      MatrixIteratorBase<Value, Pointer, Reference> tmp = iterator;
      tmp += n;
      return tmp;
    }

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference> operator+(
        typename MatrixIteratorBase<Value, Pointer, Reference>::size_type n,
        const MatrixIteratorBase<Value, Pointer, Reference>& iterator)
    {
      MatrixIteratorBase<Value, Pointer, Reference> tmp = iterator;
      tmp += n;
      return tmp;
    }

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference>& MatrixIteratorBase<
        Value, Pointer, Reference>::operator++()
    {
      ++p_;
      return *this;
    }

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference> MatrixIteratorBase<
        Value, Pointer, Reference>::operator++(int)
    {
      MatrixIteratorBase<Value, Pointer, Reference> tmp = *this;
      ++(*this);
      return tmp;
    }

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference>& MatrixIteratorBase<
        Value, Pointer, Reference>::operator--()
    {
      --p_;
      return *this;
    }

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference> MatrixIteratorBase<
        Value, Pointer, Reference>::operator--(int)
    {
      MatrixIteratorBase<Value, Pointer, Reference> tmp = *this;
      --(*this);
      return tmp;
    }

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference>& MatrixIteratorBase<
        Value, Pointer, Reference>::operator+=(difference_type n)
    {
      p_ += n;
      return *this;
    }

    template <typename Value, typename Pointer, typename Reference>
    MatrixIteratorBase<Value, Pointer, Reference>& MatrixIteratorBase<
        Value, Pointer, Reference>::operator-=(difference_type n)
    {
      p_ -= n;
      return *this;
    }
  }
}
#endif
