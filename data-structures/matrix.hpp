#ifndef FTL_MATRIX_HPP
#define FTL_MATRIX_HPP

#include <stdexcept>
#include <utility>
#include "matrix_buffer.hpp"

namespace ftl {

  template <typename T>
  class Matrix : private detail::MatrixBuffer<T>
  {
  public:
    using typename detail::MatrixBuffer<T>::size_type;
    using typename detail::MatrixBuffer<T>::value_type;

    Matrix() : Matrix(0, 0) { }
    Matrix(const Matrix& rhs);
    Matrix(Matrix&& rhs) noexcept;
    Matrix& operator=(const Matrix& rhs);
    Matrix& operator=(Matrix&& rhs) noexcept;
    virtual ~Matrix() = default;

    Matrix(size_type, size_type);
    template <typename IterT>
    Matrix(size_type, size_type, const IterT&);
    Matrix(const std::initializer_list<std::initializer_list<T>>&);

    const T* operator[](size_type row) const { return data_ + row * columns_; }
    T* operator[](size_type row) { return data_ + row * columns_; }

    Matrix& operator+=(const Matrix&) &;
    Matrix& operator-=(const Matrix&) &;

    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix& rhs) const { return !(*this == rhs); }

    void fill(const T&);

    size_type rows() const noexcept { return rows_; }
    size_type columns() const noexcept { return columns_; }

  private:
    using detail::MatrixBuffer<T>::data_;
    using detail::MatrixBuffer<T>::size_;
    using detail::MatrixBuffer<T>::capacity_;

    size_type rows_;
    size_type columns_;
  };
}

template <typename T>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns) :
  detail::MatrixBuffer<T>(rows * columns), rows_(rows), columns_(columns)
{
  for (; size_ != rows_ * columns_; ++size_) {
    new (data_ + size_) T {};
  }
}

template <typename T>
template <typename IterT>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns, const IterT& begin) :
  detail::MatrixBuffer<T>(rows * columns), rows_(rows), columns_(columns)
{
  for (auto i = begin; size_ != rows_ * columns_; ++size_, ++i) {
    new (data_ + size_) T { *i };
  }
}
template <typename T>
ftl::Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>>& values) :
  detail::MatrixBuffer<T>(values.size() * values.begin()->size()), rows_(values.size()),
  columns_(values.begin()->size())
{
  for (auto i = values.begin(); i != values.end(); ++i) {
    for (auto j = i->begin(); j != i->end(); ++j, ++size_) {
      new (data_ + size_) T { *j };
    }
  }
}

template <typename T>
ftl::Matrix<T>::Matrix(const Matrix& rhs) :
  detail::MatrixBuffer<T>(rhs.rows_ * rhs.columns_), rows_(rhs.rows_),
  columns_(rhs.columns_)
{
  for (; size_ != rhs.size_; ++size_) {
    new (data_ + size_) T { rhs.data_[size_] };
  }
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator=(const Matrix& rhs)
{
  if (capacity_ < rhs.rows_ * rhs.columns_) {
    Matrix tmp { rhs };
    std::swap(*this, tmp);
    return *this;
  }
  for (T* i = data_; i != data_ + size_; ++i) {
    i->~T();
  }
  for (size_ = 0; size_ != rhs.size_; ++size_) {
    new (data_ + size_) T { rhs.data_[size_] };
  }
  rows_ = rhs.rows_;
  columns_ = rhs.columns_;
  return *this;
}

template <typename T>
ftl::Matrix<T>::Matrix(Matrix&& rhs) noexcept :
  detail::MatrixBuffer<T>(std::move(rhs)), rows_(rhs.rows_), columns_(rhs.columns_)
{
  rhs.rows_ = 0;
  rhs.columns_ = 0;
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator=(Matrix&& rhs) noexcept
{
  if (this != &rhs) {
    for (T* i = data_; i != data_ + size_; ++i) {
      i->~T();
    }
    operator delete(data_);
    data_ = std::exchange(rhs.data_, nullptr);
    size_ = std::exchange(rhs.size_, 0);
    capacity_ = std::exchange(rhs.capacity_, 0);
    rows_ = std::exchange(rhs.rows_, 0);
    columns_ = std::exchange(rhs.columns_, 0);
  }
  return *this;
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator+=(const ftl::Matrix<T>& rhs) &
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    throw std::invalid_argument("Matrix dimensions must match");
  }
  for (size_type i = 0; i != size_; ++i) {
    data_[i] += rhs.data_[i];
  }
  return *this;
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator-=(const Matrix& rhs) &
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    throw std::invalid_argument("Matrix dimensions must match");
  }
  for (size_type i = 0; i != size_; ++i) {
    data_[i] -= rhs.data_[i];
  }
  return *this;
}

template <typename T>
bool ftl::Matrix<T>::operator==(const Matrix& rhs) const
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    return false;
  }
  for (size_type i = 0; i != size_; ++i) {
    if (!(data_[i] == rhs.data_[i])) {
      return false;
    }
  }
  return true;
}

template <typename T>
void ftl::Matrix<T>::fill(const T& value)
{
  for (T* i = data_; i != data_ + size_; ++i) {
    i->~T();
  }
  for (size_ = 0; size_ != rows_ * columns_; ++size_) {
    new (data_ + size_) T { value };
  }
}

template <typename T>
ftl::Matrix<T> operator+(const ftl::Matrix<T>& lhs, const ftl::Matrix<T>& rhs)
{
  ftl::Matrix<T> result = lhs;
  result += rhs;
  return result;
}

template <typename T>
ftl::Matrix<T> operator-(const ftl::Matrix<T>& lhs, const ftl::Matrix<T>& rhs)
{
  ftl::Matrix<T> result = lhs;
  result -= rhs;
  return result;
}

#endif
