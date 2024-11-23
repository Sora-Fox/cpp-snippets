#ifndef FTL_MATRIX_HPP
#define FTL_MATRIX_HPP

#include <algorithm>
#include <stdexcept>
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

    size_type rows() const noexcept { return rows_; }
    size_type columns() const noexcept { return columns_; }
    size_type size() const noexcept { return rows_ * columns_; }

  private:
    using detail::MatrixBuffer<T>::data_;
    using detail::MatrixBuffer<T>::capacity_;
    using detail::MatrixBuffer<T>::construct;
    using detail::MatrixBuffer<T>::destruct;

    size_type rows_;
    size_type columns_;
  };
}

template <typename T>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns) :
  detail::MatrixBuffer<T>(rows * columns), rows_(rows), columns_(columns)
{
  construct(data_, data_ + size(), T {});
}

template <typename T>
template <typename IterT>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns, const IterT& begin) :
  detail::MatrixBuffer<T>(rows * columns), rows_(rows), columns_(columns)
{
  construct(data_, data_ + size(), begin);
}

template <typename T>
ftl::Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<T>>& values) :
  detail::MatrixBuffer<T>(values.size() * values.begin()->size()), rows_(values.size()),
  columns_(values.begin()->size())
{

  for (T* it = data_; const auto& row : values) {
    if (row.size() != columns_) {
      throw std::invalid_argument("All rows must have the same number of elements");
    }
    construct(it, it + columns_, row.begin());
    it += columns_;
  }
}

template <typename T>
ftl::Matrix<T>::Matrix(const Matrix& rhs) :
  detail::MatrixBuffer<T>(rhs.rows_ * rhs.columns_), rows_(rhs.rows_),
  columns_(rhs.columns_)
{
  construct(data_, data_ + size(), rhs.data_);
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator=(const Matrix& rhs)
{
  if (capacity_ < rhs.rows_ * rhs.columns_) {
    Matrix tmp { rhs };
    std::swap(*this, tmp);
    return *this;
  }
  destruct(data_, data_ + size());
  construct(data_, data_ + rhs.size(), rhs.data_);
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
    detail::MatrixBuffer<T>::operator=(std::move(rhs));
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
  std::transform(data_, data_ + size(), rhs.data_, data_, std::plus<> {});
  return *this;
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator-=(const Matrix& rhs) &
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    throw std::invalid_argument("Matrix dimensions must match");
  }
  std::transform(data_, data_ + size(), rhs.data_, data_, std::minus<> {});
  return *this;
}

template <typename T>
bool ftl::Matrix<T>::operator==(const Matrix& rhs) const
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    return false;
  }
  return std::equal(data_, data_ + size(), rhs.data_, rhs.data_ + size());
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
