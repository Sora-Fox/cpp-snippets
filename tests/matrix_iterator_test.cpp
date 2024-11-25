#include <gtest/gtest.h>
#include <iterator>
#include <vector>
#include "../data-structures/matrix.hpp"

template <typename T>
class MatrixIteratorTest : public ::testing::Test
{
protected:
  ftl::Matrix<T> mat;

  void SetUp() override { mat = CreateInitializedMatrix(); }

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
};

using MatrixTypes = ::testing::Types<int, double>;
TYPED_TEST_SUITE(MatrixIteratorTest, MatrixTypes);

TYPED_TEST(MatrixIteratorTest, BeginEndCorrectness)
{
  auto begin = this->mat.begin();
  auto end = this->mat.end();
  EXPECT_EQ(std::distance(begin, end), 9);

  std::vector<TypeParam> values;
  for (auto it = begin; it != end; ++it) {
    values.push_back(*it);
  }

  TypeParam expected_value = 1;
  for (const auto& value : values) {
    EXPECT_EQ(value, expected_value++);
  }
}

TYPED_TEST(MatrixIteratorTest, RandomAccess)
{
  auto begin = this->mat.begin();
  auto it = begin;
  std::advance(it, 3);
  EXPECT_EQ(*(begin + 4), 5);
  EXPECT_EQ(*(begin + 8), 9);
  EXPECT_EQ(*it, 4);
  EXPECT_EQ(*(this->mat.end() - 1), 9);
  EXPECT_EQ(*(this->mat.end() - 3), 7);
}

TYPED_TEST(MatrixIteratorTest, IteratorComparison)
{
  auto begin = this->mat.begin();
  auto end = this->mat.end();
  EXPECT_TRUE(begin < end);
  EXPECT_TRUE(end > begin);
  EXPECT_TRUE(begin + 5 == end - 4);
  EXPECT_TRUE(begin + 4 != begin + 3);
}

TYPED_TEST(MatrixIteratorTest, IteratorArithmetic)
{
  auto begin = this->mat.begin();
  auto it1 = begin;
  auto it2 = begin + 4;
  it1 += 4;
  it2 -= 2;
  auto diff = this->mat.end() - begin;
  EXPECT_EQ(*it1, 5);
  EXPECT_EQ(*it2, 3);
  EXPECT_EQ(diff, 9);
}

TYPED_TEST(MatrixIteratorTest, ConstIteratorSupport)
{
  const ftl::Matrix<TypeParam>& const_mat = this->mat;
  auto begin = const_mat.begin();
  auto end = const_mat.end();

  std::vector<TypeParam> values;
  for (auto it = begin; it != end; ++it) {
    values.push_back(*it);
  }

  TypeParam expected_value = 1;
  for (const auto& value : values) {
    EXPECT_EQ(value, expected_value++);
  }
}

TYPED_TEST(MatrixIteratorTest, IteratorWriteAccess)
{
  auto begin = this->mat.begin();
  *(begin + 4) = 42;
  EXPECT_EQ(*(begin + 4), 42);
  EXPECT_EQ(this->mat[1][1], 42);
}

