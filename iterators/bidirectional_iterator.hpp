#ifndef FTL_DETAIL_BIDIRECTIONAL_ITERATOR
#define FTL_DETAIL_BIDIRECTIONAL_ITERATOR

#include "forward_iterator.hpp"

namespace ftl {
  namespace detail {
    template <typename Value, typename Pointer, typename Reference>
    class BidirectionalIterator :
      public ForwardIterator<Value, Pointer, Reference>
    {
    private:
      using BaseIt = ForwardIterator<Value, Pointer, Reference>;

    public:
      using typename BaseIt::difference_type;
      using typename BaseIt::pointer;
      using typename BaseIt::reference;
      using typename BaseIt::value_type;
      using iterator_category = std::bidirectional_iterator_tag;

      BidirectionalIterator(pointer ptr = nullptr) : BaseIt(ptr) { }
      BidirectionalIterator(const BidirectionalIterator&) = default;
      BidirectionalIterator& operator=(const BidirectionalIterator&) = default;

      BidirectionalIterator& operator--();
      BidirectionalIterator operator--(int);

    protected:
      using ForwardIterator<Value, Pointer, Reference>::ptr_;
    };

    template <typename V, typename P, typename R>
    BidirectionalIterator<V, P, R>& BidirectionalIterator<V, P, R>::operator--()
    {
      --ptr_;
      return *this;
    }

    template <typename V, typename P, typename R>
    BidirectionalIterator<V, P, R> BidirectionalIterator<V, P, R>::operator--(
        int)
    {
      BidirectionalIterator<V, P, R> tmp = *this;
      --(*this);
      return tmp;
    }
  }
}

#endif

