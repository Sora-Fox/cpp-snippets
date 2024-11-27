#include <limits>
#include <stdexcept>
#include "matrix_test_utils.hpp"

template <typename T>
class MatrixTest : public testing::Test
{
protected:
  void SetUp() override { matrix = CreateInitializedMatrix<T>(3, 3); }
  ftl::Matrix<T> matrix;
};

using Types = ::testing::Types<int, double>;
TYPED_TEST_SUITE(MatrixTest, Types);

TYPED_TEST(MatrixTest, DefaultConstructor)
{
  ftl::Matrix<TypeParam> matrix;
  EXPECT_EQ(matrix.rows(), 0);
  EXPECT_EQ(matrix.columns(), 0);
  EXPECT_EQ(matrix.size(), 0);
}

TYPED_TEST(MatrixTest, SizeConstructor)
{
  constexpr size_t rows = 7;
  constexpr size_t cols = 2;
  ftl::Matrix<TypeParam> matrix(rows, cols);
  EXPECT_EQ(matrix.rows(), rows);
  EXPECT_EQ(matrix.columns(), cols);
  EXPECT_EQ(matrix.size(), rows * cols);
}

TYPED_TEST(MatrixTest, SizeConstructorOverflow)
{
  constexpr size_t rows = std::numeric_limits<size_t>::max();
  constexpr size_t columns = 10;
  EXPECT_THROW(ftl::Matrix<TypeParam>(rows, columns), std::exception);
}

TYPED_TEST(MatrixTest, ElementAccess)
{
  EXPECT_EQ(this->matrix[0][0], 1);
  EXPECT_EQ(this->matrix[1][1], 5);
  EXPECT_EQ(this->matrix[2][2], 9);
  this->matrix[1][1] = 42;
  EXPECT_EQ(this->matrix[1][1], 42);
}

TYPED_TEST(MatrixTest, ValueConstructor)
{
  ftl::Matrix<TypeParam> matrix(7, 7, 7);
  EXPECT_EQ(matrix[0][0], 7);
  EXPECT_EQ(matrix[3][1], 7);
  EXPECT_EQ(matrix[6][5], 7);
}

TYPED_TEST(MatrixTest, IteratorConstructor)
{
  auto values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  ftl::Matrix<TypeParam> matrix(3, 3, values.begin());
  AssertMatricesEqual(matrix, this->matrix);
}

TYPED_TEST(MatrixTest, InitializerListConstructorValid)
{
  ftl::Matrix<TypeParam> matrix = {
    { 1, 2, 3 },
    { 4, 5, 6 },
    { 7, 8, 9 },
  };
  AssertMatricesEqual(matrix, this->matrix);
}

TYPED_TEST(MatrixTest, InitializerListConstructorInvalid)
{
  EXPECT_THROW((ftl::Matrix<TypeParam> { { 1 }, { 4, 5 } }), std::exception);
}

TYPED_TEST(MatrixTest, CopyConstructor)
{
  ftl::Matrix<TypeParam> copy(this->matrix);
  AssertMatricesEqual(copy, this->matrix);
}

TYPED_TEST(MatrixTest, CopyAssignmentInsufficientCapacity)
{
  ftl::Matrix<TypeParam> copy(1, 1);
  copy = this->matrix;
  AssertMatricesEqual(copy, this->matrix);
}

TYPED_TEST(MatrixTest, CopyAssignmentSufficientCapacity)
{
  ftl::Matrix<TypeParam> copy(this->matrix.rows() + 10,
                              this->matrix.columns() + 10);
  copy = this->matrix;
  AssertMatricesEqual(copy, this->matrix);
}

TYPED_TEST(MatrixTest, SelfCopyAssignment)
{
  ftl::Matrix<TypeParam> copy(this->matrix);
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wself-assign"
  copy = copy;
#pragma GCC diagnostic pop
  AssertMatricesEqual(copy, this->matrix);
}

TYPED_TEST(MatrixTest, MoveConstructor)
{
  ftl::Matrix<TypeParam> moved(std::move(this->matrix));
  AssertMatricesEqual(moved, CreateInitializedMatrix<TypeParam>(3, 3));
  EXPECT_EQ(this->matrix.rows(), 0);
  EXPECT_EQ(this->matrix.columns(), 0);
  EXPECT_EQ(this->matrix.size(), 0);
}

TYPED_TEST(MatrixTest, MoveAssignment)
{
  ftl::Matrix<TypeParam> moved;
  moved = std::move(this->matrix);
  AssertMatricesEqual(moved, CreateInitializedMatrix<TypeParam>(3, 3));
  EXPECT_EQ(this->matrix.rows(), 0);
  EXPECT_EQ(this->matrix.columns(), 0);
  EXPECT_EQ(this->matrix.size(), 0);
}

TYPED_TEST(MatrixTest, SelfMoveAssignment)
{
  this->matrix = std::move(this->matrix);
  AssertMatricesEqual(this->matrix, CreateInitializedMatrix<TypeParam>(3, 3));
}

TYPED_TEST(MatrixTest, EqualOperator)
{
  ftl::Matrix<TypeParam> same = this->matrix;
  ftl::Matrix<TypeParam> diff_size(2, 3);
  ftl::Matrix<TypeParam> diff_values = this->matrix;
  diff_values[0][0] = 42;
  EXPECT_TRUE(this->matrix == same);
  EXPECT_FALSE(this->matrix == diff_size);
  EXPECT_FALSE(this->matrix == diff_values);
}

TYPED_TEST(MatrixTest, NotEqualOperator)
{
  ftl::Matrix<TypeParam> same = this->matrix;
  ftl::Matrix<TypeParam> diff_size(2, 3);
  ftl::Matrix<TypeParam> diff_values = this->matrix;
  diff_values[0][0] = 42;
  EXPECT_FALSE(this->matrix != same);
  EXPECT_TRUE(this->matrix != diff_size);
  EXPECT_TRUE(this->matrix != diff_values);
}

TYPED_TEST(MatrixTest, AdditionAssignmentOperatorValid)
{
  ftl::Matrix<TypeParam> expected = CreateInitializedMatrix<TypeParam>(3, 3, 2);
  this->matrix += ftl::Matrix<TypeParam>(3, 3, 1);
  AssertMatricesEqual(this->matrix, expected);
}

TYPED_TEST(MatrixTest, AdditionAssignmentOperatorInvalid)
{
  ftl::Matrix<TypeParam> diff_size(2, 2);
  EXPECT_THROW(this->matrix += diff_size, std::invalid_argument);
}

TYPED_TEST(MatrixTest, SubtractionAssignmentOperatorValid)
{
  ftl::Matrix<TypeParam> empty(3, 3, 0);
  this->matrix -= CreateInitializedMatrix<TypeParam>(3, 3);
  AssertMatricesEqual(this->matrix, empty);
}

TYPED_TEST(MatrixTest, SubtractionAssignmentOperatorInvalid)
{
  ftl::Matrix<TypeParam> diff_size(2, 2);
  EXPECT_THROW(this->matrix -= diff_size, std::invalid_argument);
}

TYPED_TEST(MatrixTest, SubtractionOperatorValid)
{
  ftl::Matrix<TypeParam> empty(3, 3, 0);
  this->matrix = this->matrix - this->matrix;
  AssertMatricesEqual(this->matrix, empty);
}

TYPED_TEST(MatrixTest, SubtractionOperatorInvalid)
{
  ftl::Matrix<TypeParam> diff_size(2, 2);
  EXPECT_THROW(this->matrix - diff_size, std::invalid_argument);
}

TYPED_TEST(MatrixTest, AdditionOperatorValid)
{
  ftl::Matrix<TypeParam> expected = CreateInitializedMatrix<TypeParam>(3, 3, 2);
  AssertMatricesEqual(this->matrix + ftl::Matrix<TypeParam>(3, 3, 1), expected);
}

TYPED_TEST(MatrixTest, AdditionOperatorInvalid)
{
  ftl::Matrix<TypeParam> diff_size(2, 2);
  EXPECT_THROW(this->matrix + diff_size, std::invalid_argument);
}

