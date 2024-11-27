#ifndef FTL_MATRIX_HPP
#define FTL_MATRIX_HPP

#include <algorithm>
#include "matrix_buffer.hpp"
#include "matrix_iterator_base.hpp"

namespace ftl {
  template <typename T>
  class Matrix
  {
  public:
    using size_type = typename detail::MatrixBuffer<T>::size_type;
    using value_type = typename detail::MatrixBuffer<T>::value_type;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    using iterator = detail::MatrixIteratorBase<value_type, pointer, reference>;
    using const_iterator =
        detail::MatrixIteratorBase<value_type, const_pointer, const_reference>;
    static_assert(std::contiguous_iterator<iterator>);
    static_assert(std::contiguous_iterator<const_iterator>);

    Matrix() : Matrix(0, 0) { }
    Matrix(const Matrix& rhs);
    Matrix(Matrix&& rhs) noexcept;
    Matrix& operator=(const Matrix& rhs);
    Matrix& operator=(Matrix&& rhs) noexcept;
    virtual ~Matrix() = default;

    Matrix(size_type, size_type, const_reference = value_type {});
    template <std::input_iterator InputIt>
    Matrix(size_type, size_type, const InputIt&);
    template <typename U = value_type>
    Matrix(const std::initializer_list<std::initializer_list<U>>&);

    const_pointer operator[](size_type row) const
    {
      return data() + row * columns_;
    }
    pointer operator[](size_type row) { return data() + row * columns_; }

    Matrix& operator+=(const Matrix&) &;
    Matrix& operator-=(const Matrix&) &;
    bool operator==(const Matrix&) const;

    size_type rows() const noexcept { return rows_; }
    size_type columns() const noexcept { return columns_; }
    size_type size() const noexcept { return rows_ * columns_; }

    iterator begin() { return iterator(data()); }
    iterator end() { return iterator(data() + size()); }
    const_iterator begin() const { return const_iterator(data()); }
    const_iterator end() const { return const_iterator(data() + size()); }

  private:
    detail::MatrixBuffer<T> buffer_;
    size_type rows_;
    size_type columns_;

    value_type* data() const noexcept { return buffer_.data_; }
    void checkSizeTypeOverflow(size_type, size_type);
  };
}

template <typename T>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns,
                       const_reference value) :
  buffer_(rows * columns), rows_(rows), columns_(columns)
{
  buffer_.construct(data(), data() + size(), value);
}

template <typename T>
template <std::input_iterator InputIt>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns,
                       const InputIt& begin) :
  buffer_(rows * columns), rows_(rows), columns_(columns)
{
  buffer_.construct(data(), data() + size(), begin);
}

template <typename T>
template <typename U>
ftl::Matrix<T>::Matrix(
    const std::initializer_list<std::initializer_list<U>>& list) :
  buffer_(list.size() * list.begin()->size()), rows_(list.size()),
  columns_(list.begin()->size())
{
  for (pointer it = data(); const auto& row : list) {
    if (row.size() != columns_) {
      throw std::invalid_argument(
          "All rows must have the same number of columns");
    }
    buffer_.construct(it, it + columns_, row.begin());
    it += columns_;
  }
}

template <typename T>
ftl::Matrix<T>::Matrix(const Matrix& rhs) :
  buffer_(rhs.size()), rows_(rhs.rows_), columns_(rhs.columns_)
{
  buffer_.construct(data(), data() + size(), rhs.data());
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator=(const Matrix& rhs)
{
  if (buffer_.capacity() < rhs.rows_ * rhs.columns_) {
    Matrix tmp { rhs };
    std::swap(*this, tmp);
    return *this;
  }
  buffer_.destruct(data(), data() + size());
  buffer_.construct(data(), data() + rhs.size(), rhs.data());
  rows_ = rhs.rows_;
  columns_ = rhs.columns_;
  return *this;
}

template <typename T>
ftl::Matrix<T>::Matrix(Matrix&& rhs) noexcept :
  buffer_(std::move(rhs.buffer_)), rows_(rhs.rows_), columns_(rhs.columns_)
{
  rhs.rows_ = 0;
  rhs.columns_ = 0;
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator=(Matrix&& rhs) noexcept
{
  if (this != &rhs) {
    buffer_ = std::move(rhs.buffer_);
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
    data()[i] += rhs.data()[i];
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
    data()[i] -= rhs.data()[i];
  }
  return *this;
}

template <typename T>
bool ftl::Matrix<T>::operator==(const Matrix& rhs) const
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    return false;
  }
  return std::equal(begin(), end(), rhs.begin(), rhs.end());
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
