#include <gtest/gtest.h>
#include <initializer_list>
#include "../data-structures/matrix.hpp"

template <typename T>
class MatrixTest : public ::testing::Test
{
protected:
  ftl::Matrix<T> mat;

  static ftl::Matrix<T> CreateInitializedMatrix()
  {
    ftl::Matrix<T> matrix(3, 3);
    T value = 1;
    for (size_t i = 0; i < 3; ++i) {
      for (size_t j = 0; j < 3; ++j) {
        matrix[i][j] = value++;
      }
    }
    return matrix;
  }

  void SetUp() override { mat = CreateInitializedMatrix(); }

  void TearDown() override { }
};

template <typename T>
void AssertMatricesEqual(const ftl::Matrix<T>& mat1, const ftl::Matrix<T>& mat2)
{
  ASSERT_EQ(mat1.rows(), mat2.rows());
  ASSERT_EQ(mat1.columns(), mat2.columns());
  for (size_t i = 0; i < mat1.rows(); ++i) {
    for (size_t j = 0; j < mat1.columns(); ++j) {
      EXPECT_EQ(mat1[i][j], mat2[i][j]) << "Mismatch at (" << i << ", " << j << ")";
    }
  }
}

using MyTypes = ::testing::Types<int, double, float>;
TYPED_TEST_SUITE(MatrixTest, MyTypes);

TYPED_TEST(MatrixTest, DefaultConstructor)
{
  ftl::Matrix<TypeParam> matrix;
  EXPECT_EQ(matrix.rows(), 0);
  EXPECT_EQ(matrix.columns(), 0);
}

TYPED_TEST(MatrixTest, SizeConstructor)
{
  ftl::Matrix<TypeParam> matrix(7, 2);
  EXPECT_EQ(matrix.rows(), 7);
  EXPECT_EQ(matrix.columns(), 2);
}

TYPED_TEST(MatrixTest, ElementAccess)
{
  EXPECT_EQ(this->mat[0][0], 1);
  EXPECT_EQ(this->mat[1][1], 5);
  EXPECT_EQ(this->mat[2][2], 9);
  this->mat[1][1] = 42;
  EXPECT_EQ(this->mat[1][1], 42);
}

TYPED_TEST(MatrixTest, IteratorConstructor)
{
  std::initializer_list<TypeParam> values = { 3, 6, 6, 2, 5, 0 };
  ftl::Matrix<TypeParam> matrix(2, 3, values.begin());
  AssertMatricesEqual(matrix, ftl::Matrix<TypeParam>(2, 3, values.begin()));
}

TYPED_TEST(MatrixTest, CopyConstructor)
{
  ftl::Matrix<TypeParam> matrix(this->mat);
  AssertMatricesEqual(matrix, this->mat);
}

TYPED_TEST(MatrixTest, CopyAssignmentOperator)
{
  ftl::Matrix<TypeParam> first(1, 0);
  first = this->mat;
  AssertMatricesEqual(first, this->mat);

  ftl::Matrix<TypeParam> second(10, 10);
  second = this->mat;
  AssertMatricesEqual(second, this->mat);

  second = second;
  AssertMatricesEqual(second, this->mat);
}

TYPED_TEST(MatrixTest, MoveConstructor)
{
  ftl::Matrix<TypeParam> moved_mat(std::move(this->mat));
  AssertMatricesEqual(moved_mat, MatrixTest<TypeParam>::CreateInitializedMatrix());
  EXPECT_EQ(this->mat.rows(), 0);
  EXPECT_EQ(this->mat.columns(), 0);
}

TYPED_TEST(MatrixTest, MoveAssignmentOperator)
{
  ftl::Matrix<TypeParam> matrix(2, 2);
  matrix = std::move(this->mat);
  AssertMatricesEqual(matrix, MatrixTest<TypeParam>::CreateInitializedMatrix());
  EXPECT_EQ(this->mat.rows(), 0);
  EXPECT_EQ(this->mat.columns(), 0);

  matrix = std::move(matrix);
  AssertMatricesEqual(matrix, MatrixTest<TypeParam>::CreateInitializedMatrix());
}

