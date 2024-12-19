#ifndef FTL_MATRIX_HPP
#define FTL_MATRIX_HPP

#include "buffer.hpp"
#include "random_access_iterator.hpp"

namespace ftl {
  template <typename T>
  class Matrix
  {
  public:
    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;

  private:
    template <typename V, typename P, typename R>
    using ItBase = detail::RandomAccessIterator<V, P, R>;

  public:
    using iterator = ItBase<value_type, pointer, reference>;
    using const_iterator = ItBase<value_type, const_pointer, const_reference>;

    Matrix() : Matrix(0, 0) { }
    Matrix(const Matrix& rhs);
    Matrix(Matrix&& rhs) noexcept;
    Matrix(size_type, size_type, const_reference = value_type {});
    template <typename InputIt,
              typename = detail::void_t<
                  decltype(value_type(*std::declval<InputIt&>())),
                  decltype(++std::declval<InputIt&>())>>
    Matrix(size_type, size_type, const InputIt&);
    template <typename U = value_type>
    Matrix(const std::initializer_list<std::initializer_list<U>>&);
    virtual ~Matrix() = default;

    Matrix& operator=(const Matrix& rhs);
    Matrix& operator=(Matrix&& rhs) noexcept;

    const_pointer operator[](size_type) const;
    pointer operator[](size_type);

    Matrix& operator+=(const Matrix&) &;
    Matrix& operator-=(const Matrix&) &;
    bool operator==(const Matrix&) const;
    bool operator!=(const Matrix& rhs) const { return !(*this == rhs); }

    size_type rows() const noexcept { return rows_; }
    size_type columns() const noexcept { return columns_; }
    size_type size() const noexcept { return rows_ * columns_; }

    iterator begin() { return iterator(buffer_.begin()); }
    iterator end() { return iterator(buffer_.end()); }
    const_iterator begin() const { return const_iterator(buffer_.begin()); }
    const_iterator end() const { return const_iterator(buffer_.end()); }

  private:
    Buffer<T> buffer_;
    size_type rows_;
    size_type columns_;
  };
}

template <typename T>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns,
                       const_reference value) :
  buffer_(rows * columns), rows_(rows), columns_(columns)
{
  buffer_.construct(buffer_.begin(), buffer_.begin() + size(), value);
}

template <typename T>
template <typename InputIt, typename>
ftl::Matrix<T>::Matrix(size_type rows, size_type columns,
                       const InputIt& begin) :
  buffer_(rows * columns), rows_(rows), columns_(columns)
{
  buffer_.construct(buffer_.begin(), buffer_.begin() + size(), begin);
}

template <typename T>
template <typename U>
ftl::Matrix<T>::Matrix(
    const std::initializer_list<std::initializer_list<U>>& list) :
  buffer_(list.size() * list.begin()->size()), rows_(list.size()),
  columns_(list.begin()->size())
{
  pointer it = buffer_.begin();
  for (const auto& row : list) {
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
  buffer_.construct(buffer_.begin(), buffer_.begin() + size(),
                    rhs.buffer_.begin());
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator=(const Matrix& rhs)
{
  if (buffer_.capacity() < rhs.rows_ * rhs.columns_) {
    Matrix tmp { rhs };
    std::swap(*this, tmp);
    return *this;
  }
  buffer_.destruct(buffer_.begin(), buffer_.end());
  buffer_.construct(buffer_.begin(), buffer_.begin() + rhs.size(),
                    rhs.buffer_.begin());
  rows_ = rhs.rows_;
  columns_ = rhs.columns_;
  return *this;
}

template <typename T>
ftl::Matrix<T>::Matrix(Matrix&& rhs) noexcept :
  buffer_(std::move(rhs.buffer_)), rows_(std::exchange(rhs.rows_, 0)),
  columns_(std::exchange(rhs.columns_, 0))
{
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
typename ftl::Matrix<T>::const_pointer ftl::Matrix<T>::operator[](
    size_type row) const
{
  return buffer_.begin() + row * columns_;
}

template <typename T>
typename ftl::Matrix<T>::pointer ftl::Matrix<T>::operator[](size_type row)
{
  return buffer_.begin() + row * columns_;
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator+=(const ftl::Matrix<T>& rhs) &
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    throw std::logic_error("Matrix dimensions must match");
  }
  for (size_type i = 0; i != size(); ++i) {
    (buffer_.begin())[i] += (rhs.buffer_.begin())[i];
  }
  return *this;
}

template <typename T>
ftl::Matrix<T>& ftl::Matrix<T>::operator-=(const Matrix& rhs) &
{
  if (rows_ != rhs.rows_ || columns_ != rhs.columns_) {
    throw std::logic_error("Matrix dimensions must match");
  }
  for (size_type i = 0; i != size(); ++i) {
    (buffer_.begin())[i] -= (rhs.buffer_.begin())[i];
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

