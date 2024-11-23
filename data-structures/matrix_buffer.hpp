#ifndef FTL_DETAIL_MATRIX_BUFFER_HPP
#define FTL_DETAIL_MATRIX_BUFFER_HPP

#include <utility>

namespace ftl {
  namespace detail {
    template <typename T>
    class MatrixBuffer
    {
    protected:
      using value_type = T;
      using size_type = std::size_t;

      size_type capacity_;
      T* data_;

      MatrixBuffer(size_type = 0);
      MatrixBuffer(const MatrixBuffer&) = delete;
      MatrixBuffer& operator=(const MatrixBuffer&) = delete;
      MatrixBuffer(MatrixBuffer&&) noexcept;
      MatrixBuffer& operator=(MatrixBuffer&&) noexcept;
      virtual ~MatrixBuffer();

      template <typename FwdIt>
      void construct(T*, T*, FwdIt);
      void construct(T*, T*, const T& = T {});
      void destruct(T*, T*);

    private:
      size_type constructed_ = 0;
    };

    template <typename T>
    MatrixBuffer<T>::~MatrixBuffer()
    {
      destruct(data_, data_ + constructed_);
      operator delete(data_);
    }
  }
}

template <typename T>
ftl::detail::MatrixBuffer<T>::MatrixBuffer(size_type capacity) :
  capacity_(capacity),
  data_(capacity ? static_cast<T*>(operator new(sizeof(T) * capacity)) : nullptr)
{
}

template <typename T>
ftl::detail::MatrixBuffer<T>::MatrixBuffer(MatrixBuffer&& rhs) noexcept :
  capacity_(rhs.capacity_), data_(rhs.data_), constructed_(rhs.constructed_)
{
  rhs.constructed_ = 0;
  rhs.capacity_ = 0;
  rhs.data_ = nullptr;
}

template <typename T>
ftl::detail::MatrixBuffer<T>& ftl::detail::MatrixBuffer<T>::operator=(
    MatrixBuffer&& rhs) noexcept
{
  destruct(data_, data_ + constructed_);
  operator delete(data_);
  data_ = std::exchange(rhs.data_, nullptr);
  capacity_ = std::exchange(rhs.capacity_, 0);
  constructed_ = std::exchange(rhs.constructed_, 0);
  return *this;
}

template <typename T>
template <typename FwdIt>
void ftl::detail::MatrixBuffer<T>::construct(T* begin, T* end, FwdIt values)
{
  for (; begin != end; ++begin, ++values, ++constructed_) {
    new (begin) T { *values };
  }
}

template <typename T>
void ftl::detail::MatrixBuffer<T>::construct(T* begin, T* end, const T& value)
{
  for (; begin != end; ++begin, ++constructed_) {
    new (begin) T { value };
  }
}

template <typename T>
void ftl::detail::MatrixBuffer<T>::destruct(T* begin, T* end)
{
  for (; begin != end; ++begin, --constructed_) {
    begin->~T();
  }
}

#endif
