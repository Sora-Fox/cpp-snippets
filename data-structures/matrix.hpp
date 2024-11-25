#ifndef FTL_MATRIX_HPP
#define FTL_MATRIX_HPP

#include <algorithm>
#include <limits>
#include "matrix_buffer.hpp"
#include "matrix_iterator_base.hpp"

namespace ftl {
  template <typename T>
  class Matrix : private detail::MatrixBuffer<T>
  {
  public:
    using typename detail::MatrixBuffer<T>::size_type;
    using typename detail::MatrixBuffer<T>::value_type;
    using iterator = detail::MatrixIteratorBase<T, T*, T&>;
    using const_iterator = detail::MatrixIteratorBase<T, const T*, const T&>;
    static_assert(std::contiguous_iterator<iterator>);
    static_assert(std::contiguous_iterator<const_iterator>);

    Matrix() : Matrix(0, 0) { }
    Matrix(const Matrix& rhs);
    Matrix(Matrix&& rhs) noexcept;
    Matrix& operator=(const Matrix& rhs);
    Matrix& operator=(Matrix&& rhs) noexcept;

    Matrix(size_type, size_type, const T& = T {});
    template <std::input_iterator InputIt>
    Matrix(size_type, size_type, const InputIt&);
    template <typename U = T>
    Matrix(const std::initializer_list<std::initializer_list<U>>&);

    const T* operator[](size_type row) const { return data_ + row * columns_; }
    T* operator[](size_type row) { return data_ + row * columns_; }

    Matrix& operator+=(const Matrix&) &;
    Matrix& operator-=(const Matrix&) &;
    bool operator==(const Matrix&) const;

    size_type rows() const noexcept { return rows_; }
    size_type columns() const noexcept { return columns_; }
    size_type size() const noexcept { return rows_ * columns_; }

    iterator begin() { return iterator(data_); }
    iterator end() { return iterator(data_ + size()); }
    const_iterator begin() const { return const_iterator(data_); }
    const_iterator end() const { return const_iterator(data_ + size()); }

  private:
    using detail::MatrixBuffer<T>::data_;
    using detail::MatrixBuffer<T>::capacity_;
    using detail::MatrixBuffer<T>::construct;
    using detail::MatrixBuffer<T>::destruct;

    size_type rows_;
    size_type columns_;

    void checkSizeTypeOverflow(size_type, size_type);
  };
}

template <typename T>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns, const T& value) :
  detail::MatrixBuffer<T>(rows * columns), rows_(rows), columns_(columns)
{
  checkSizeTypeOverflow(rows_, columns_);
  construct(data_, data_ + size(), value);
}

template <typename T>
template <std::input_iterator InputIt>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns, const InputIt& begin) :
  detail::MatrixBuffer<T>(rows * columns), rows_(rows), columns_(columns)
{
  checkSizeTypeOverflow(rows_, columns_);
  construct(data_, data_ + size(), begin);
}

template <typename T>
template <typename U>
ftl::Matrix<T>::Matrix(const std::initializer_list<std::initializer_list<U>>& list) :
  detail::MatrixBuffer<T>(list.size() * list.begin()->size()), rows_(list.size()),
  columns_(list.begin()->size())
{
  checkSizeTypeOverflow(rows_, columns_);
  for (T* it = data_; const auto& row : list) {
    if (row.size() != columns_) {
      throw std::invalid_argument("All rows must have the same number of columns");
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
  for (size_type i = 0; i != size(); ++i) {
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
  for (size_type i = 0; i != size(); ++i) {
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
  return std::equal(data_, data_ + size(), rhs.data_, rhs.data_ + size());
}

template <typename T>
void ftl::Matrix<T>::checkSizeTypeOverflow(size_type first, size_type second)
{
  if (first != 0 && second > std::numeric_limits<size_type>::max() / first) {
    throw std::overflow_error("Matrix size_type overflow");
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
