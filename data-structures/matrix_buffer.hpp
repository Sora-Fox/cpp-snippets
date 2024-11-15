#ifndef FTL_MATRIX_BUFFER_HPP
#define FTL_MATRIX_BUFFER_HPP

#include <utility>

namespace ftl {
  template <typename T>
  class MatrixBuffer
  {
  protected:
    using value_type = T;
    using size_type = std::size_t;

    size_type size_ = 0;
    size_type capacity_;
    T* data_;

    MatrixBuffer(size_type);
    MatrixBuffer(const MatrixBuffer&) = delete;
    MatrixBuffer& operator=(const MatrixBuffer&) = delete;
    MatrixBuffer(MatrixBuffer&&) noexcept;
    MatrixBuffer& operator=(MatrixBuffer&&) noexcept;
    virtual ~MatrixBuffer();
  };
}

template <typename T>
ftl::MatrixBuffer<T>::MatrixBuffer(size_type capacity) :
  capacity_(capacity),
  data_(capacity ? static_cast<T*>(operator new(sizeof(T) * capacity))
                 : nullptr)
{
}

template <typename T>
ftl::MatrixBuffer<T>::~MatrixBuffer()
{
  for (size_type i = 0; i != size_; ++i) {
    (data_ + i)->~T();
  }
  operator delete[](data_);
}

template <typename T>
ftl::MatrixBuffer<T>::MatrixBuffer(MatrixBuffer&& rhs) noexcept :
  size_(rhs.size_), capacity_(rhs.capacity_), data_(rhs.data_)
{
  rhs.size_ = 0;
  rhs.capacity_ = 0;
  rhs.data_ = nullptr;
}

template <typename T>
ftl::MatrixBuffer<T>&
ftl::MatrixBuffer<T>::operator=(MatrixBuffer&& rhs) noexcept
{
  std::swap(size_, rhs.size_);
  std::swap(capacity_, rhs.capacity_);
  std::swap(data_, rhs.data_);
}

#endif
