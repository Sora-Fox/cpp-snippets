#include <iterator>
#include "matrix_test_utils.hpp"

template <typename T>
class MatrixIteratorTest : public testing::Test
{
protected:
  void SetUp() override { matrix = CreateInitializedMatrix<T>(3, 3); }
  ftl::Matrix<T> matrix;
};

using Types = testing::Types<int, double>;
TYPED_TEST_SUITE(MatrixIteratorTest, Types);

TYPED_TEST(MatrixIteratorTest, BeginEndIterators)
{
  auto beg = this->matrix.begin();
  auto end = this->matrix.end();
  EXPECT_NE(beg, end);
}

TYPED_TEST(MatrixIteratorTest, ReadingValues)
{
  auto it = this->matrix.begin();
  for (size_t i = 0; i != this->matrix.rows(); ++i) {
    for (size_t j = 0; j != this->matrix.columns(); ++j, ++it) {
      EXPECT_EQ(*it, this->matrix[i][j])
          << "Mismatch at position (" << i << ", " << j << ")";
    }
  }
  EXPECT_EQ(it, this->matrix.end());
}

TYPED_TEST(MatrixIteratorTest, ConstBeginEndIterators)
{
  const auto& const_matrix = this->matrix;
  auto beg = const_matrix.begin();
  auto end = const_matrix.end();
  EXPECT_NE(beg, end);
}

TYPED_TEST(MatrixIteratorTest, ConstReadingValues)
{
  const ftl::Matrix<TypeParam>& const_matrix = this->matrix;
  auto it = const_matrix.begin();
  for (size_t i = 0; i != const_matrix.rows(); ++i) {
    for (size_t j = 0; j != const_matrix.columns(); ++j, ++it) {
      EXPECT_EQ(*it, const_matrix[i][j])
          << "Mismatch at position (" << i << ", " << j << ")";
    }
  }
  EXPECT_EQ(it, const_matrix.end());
}

TYPED_TEST(MatrixIteratorTest, DifferenceBetweenIterators)
{
  auto beg = this->matrix.begin();
  auto end = this->matrix.end();
  EXPECT_EQ(end - beg, this->matrix.size());
  EXPECT_EQ(std::distance(beg, end), this->matrix.size());
}

TYPED_TEST(MatrixIteratorTest, IteratorComparison)
{
  auto beg = this->matrix.begin();
  auto end = this->matrix.end();
  auto mid = beg + (end - beg) / 2;
  EXPECT_LT(beg, mid);
  EXPECT_GT(end, mid);
  EXPECT_GE(beg, beg);
  EXPECT_LE(beg, beg);
}

TYPED_TEST(MatrixIteratorTest, ModifyMatrixThroughIterator)
{
  auto it = this->matrix.begin();
  *it = 42;
  EXPECT_EQ(this->matrix[0][0], 42);
}

TYPED_TEST(MatrixIteratorTest, ArithmeticOperations)
{
  auto mtx = this->matrix;
  auto it = mtx.begin();
  EXPECT_EQ(*(it + 4), mtx[0][4]);
  it += std::distance(mtx.begin(), mtx.end());
  EXPECT_EQ(*(--it), mtx[mtx.rows() - 1][mtx.columns() - 1]);
  it -= std::distance(mtx.begin(), mtx.end());
  it++;
  EXPECT_EQ(*(it), mtx[0][0]);
  EXPECT_EQ(it[2], mtx[0][2]);
}

TYPED_TEST(MatrixIteratorTest, SelfAssignment)
{
  auto it = this->matrix.begin();
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-assign-overloaded"
  it = it;
#pragma GCC diagnostic pop
  EXPECT_EQ(*it, this->matrix[0][0]);
}

