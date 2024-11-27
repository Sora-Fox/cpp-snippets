#ifndef TESTS_MATRIX_TESTS_UTILS_HPP
#define TESTS_MATRIX_TESTS_UTILS_HPP

#include <gtest/gtest.h>
#include "matrix.hpp"

template <typename T>
void AssertMatricesEqual(const ftl::Matrix<T>& first,
                         const ftl::Matrix<T>& second)
{
  ASSERT_EQ(first.rows(), second.rows()) << "Matrix rows differ";
  ASSERT_EQ(first.columns(), second.columns()) << "Matrix columns differ";
  ASSERT_EQ(first.size(), second.size()) << "Matrix sizes differ";
  for (size_t i = 0; i != first.rows(); ++i) {
    for (size_t j = 0; j != first.columns(); ++j) {
      EXPECT_EQ(first[i][j], second[i][j])
          << "Mismatch at position (" << i << ", " << j << ")";
    }
  }
}

template <typename T>
ftl::Matrix<T> CreateInitializedMatrix(size_t rows, size_t columns,
                                       T start = T(1))
{
  ftl::Matrix<T> matrix(rows, columns);
  for (size_t i = 0; i != matrix.rows(); ++i) {
    for (size_t j = 0; j != matrix.columns(); ++j, ++start) {
      matrix[i][j] = start;
    }
  }
  return matrix;
}

#endif
