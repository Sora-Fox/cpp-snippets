#include <gtest/gtest.h>
#include "bidirectional_iterator.hpp"
#include "forward_iterator.hpp"
#include "random_access_iterator.hpp"

namespace {
  template <typename IteratorType>
  class IteratorTest : public ::testing::Test
  {
  protected:
    size_t size = 5;
    int data[5] = { 1, 2, 3, 4, 5 };
    using Iterator = IteratorType;
  };

  using TestingTypes =
      ::testing::Types<ftl::detail::ForwardIterator<int, int*, int&>,
                       ftl::detail::BidirectionalIterator<int, int*, int&>,
                       ftl::detail::RandomAccessIterator<int, int*, int&>>;

  TYPED_TEST_SUITE(IteratorTest, TestingTypes);

  TYPED_TEST(IteratorTest, DefaultConstructor)
  {
    using Iterator = typename TestFixture::Iterator;
    Iterator it;
    EXPECT_EQ(it.operator->(), nullptr);
  }

  TYPED_TEST(IteratorTest, CopyConstructor)
  {
    using Iterator = typename TestFixture::Iterator;
    Iterator it1(this->data);
    Iterator it2(it1);
    EXPECT_EQ(*it1, *it2);
    EXPECT_TRUE(it1 == it2);
  }

  TYPED_TEST(IteratorTest, IncrementOperators)
  {
    using Iterator = typename TestFixture::Iterator;
    Iterator it(this->data);
    EXPECT_EQ(*it, this->data[0]);
    ++it;
    EXPECT_EQ(*it, this->data[1]);
    it++;
    EXPECT_EQ(*it, this->data[2]);
  }

  TYPED_TEST(IteratorTest, EqualityOperators)
  {
    using Iterator = typename TestFixture::Iterator;
    Iterator it1(this->data);
    Iterator it2(this->data);
    Iterator it3(this->data + 1);
    EXPECT_TRUE(it1 == it2);
    EXPECT_FALSE(it1 == it3);
    EXPECT_TRUE(it1 != it3);
  }

  TYPED_TEST(IteratorTest, CompatibilityWithSTLDistance)
  {
    using Iterator = typename TestFixture::Iterator;
    Iterator begin(this->data);
    Iterator end(this->data + this->size);
    EXPECT_EQ(std::distance(begin, end), this->size);
  }

  TYPED_TEST(IteratorTest, CompatibilityWithSTLAdvance)
  {
    using Iterator = typename TestFixture::Iterator;
    Iterator it(this->data);
    std::advance(it, 2);
    EXPECT_EQ(*it, this->data[2]);
  }

  TYPED_TEST(IteratorTest, IteratorTraits)
  {
    using Iterator = typename TestFixture::Iterator;
    using Traits = std::iterator_traits<Iterator>;
    EXPECT_TRUE((std::is_same<typename Traits::value_type, int>::value));
    EXPECT_TRUE((std::is_same<typename Traits::pointer, int*>::value));
    EXPECT_TRUE((std::is_same<typename Traits::reference, int&>::value));
    EXPECT_TRUE((
        std::is_same<typename Traits::difference_type, std::ptrdiff_t>::value));
  }

  TYPED_TEST(IteratorTest, DecrementOperators)
  {
    using Iterator = typename TestFixture::Iterator;
    if constexpr (std::is_base_of<std::bidirectional_iterator_tag,
                                  typename std::iterator_traits<
                                      Iterator>::iterator_category>::value) {
      Iterator it(this->data);
      Iterator it_end(this->data + this->size);
      --it_end;
      EXPECT_EQ(*it_end, this->data[this->size - 1]);
      it_end--;
      EXPECT_EQ(*it_end, this->data[this->size - 2]);
    }
  }

  TYPED_TEST(IteratorTest, SubtractionOperator)
  {
    using Iterator = typename TestFixture::Iterator;
    if constexpr (std::is_base_of<std::random_access_iterator_tag,
                                  typename std::iterator_traits<
                                      Iterator>::iterator_category>::value) {
      Iterator it1(this->data);
      Iterator it2(this->data + 3);
      EXPECT_EQ(it2 - it1, 3);
    }
  }

  TYPED_TEST(IteratorTest, ComparisonOperators)
  {
    using Iterator = typename TestFixture::Iterator;
    if constexpr (std::is_base_of<std::random_access_iterator_tag,
                                  typename std::iterator_traits<
                                      Iterator>::iterator_category>::value) {
      Iterator it1(this->data);
      Iterator it2(this->data + 3);
      EXPECT_TRUE(it1 < it2);
      EXPECT_TRUE(it2 > it1);
      EXPECT_TRUE(it1 <= it2);
      EXPECT_TRUE(it2 >= it1);
    }
  }

  TYPED_TEST(IteratorTest, SelfArithmeticOperators)
  {
    using Iterator = typename TestFixture::Iterator;
    if constexpr (std::is_base_of<std::random_access_iterator_tag,
                                  typename std::iterator_traits<
                                      Iterator>::iterator_category>::value) {
      Iterator it(this->data);
      it += 2;
      EXPECT_EQ(*it, this->data[2]);
      it -= 1;
      EXPECT_EQ(*it, this->data[1]);
    }
  }
}
