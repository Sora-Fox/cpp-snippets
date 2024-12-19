#ifndef FTL_BUFFER_HPP
#define FTL_BUFFER_HPP

#include <utility>

namespace ftl {
  namespace detail {
    template <typename...>
    using void_t = void;
  }
}

namespace ftl {
  template <typename T>
  class Buffer final
  {
  public:
    using value_type = T;
    using size_type = std::size_t;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;

    Buffer(size_type = 0);
    Buffer(const Buffer&) = delete;
    Buffer(Buffer&&) noexcept;
    Buffer& operator=(const Buffer&) = delete;
    Buffer& operator=(Buffer&&) noexcept;
    ~Buffer();

    template <typename InputIt,
              typename = detail::void_t<
                  decltype(value_type(*std::declval<InputIt&>())),
                  decltype(++std::declval<InputIt&>())>>
    void construct(pointer, pointer, InputIt);
    void construct(pointer, pointer, const_reference = value_type {});
    void destruct(pointer, pointer);

    size_type capacity() const noexcept { return capacity_; }
    size_type size() const noexcept { return constructed_; }
    bool empty() const noexcept { return capacity_; }

    pointer begin() { return data_; }
    pointer end() { return data_ + constructed_; }
    const_pointer begin() const { return data_; }
    const_pointer end() const { return data_ + constructed_; }

  private:
    pointer data_;
    size_type capacity_;
    size_type constructed_ = 0;
  };

  template <typename T>
  Buffer<T>::~Buffer()
  {
    destruct(data_, data_ + constructed_);
    operator delete(data_);
  }

  template <typename T>
  Buffer<T>::Buffer(size_type capacity) :
    data_(static_cast<pointer>(operator new(sizeof(T) * capacity))),
    capacity_(capacity)
  {
  }

  template <typename T>
  Buffer<T>::Buffer(Buffer&& rhs) noexcept :
    data_(std::exchange(rhs.data_, nullptr)),
    capacity_(std::exchange(rhs.capacity_, 0)),
    constructed_(std::exchange(rhs.constructed_, 0))
  {
  }

  template <typename T>
  Buffer<T>& Buffer<T>::operator=(Buffer&& rhs) noexcept
  {
    destruct(data_, data_ + constructed_);
    operator delete(data_);
    data_ = std::exchange(rhs.data_, nullptr);
    capacity_ = std::exchange(rhs.capacity_, 0);
    constructed_ = std::exchange(rhs.constructed_, 0);
    return *this;
  }

  template <typename T>
  template <typename InputIt, typename>
  void Buffer<T>::construct(pointer begin, pointer end, InputIt values)
  {
    for (; begin != end; ++begin, ++values, ++constructed_) {
      new (begin) T(*values);
    }
  }

  template <typename T>
  void Buffer<T>::construct(pointer begin, pointer end, const_reference value)
  {
    for (; begin != end; ++begin, ++constructed_) {
      new (begin) T(value);
    }
  }

  template <typename T>
  void Buffer<T>::destruct(pointer begin, pointer end)
  {
    for (; begin != end; ++begin, --constructed_) {
      begin->~T();
    }
  }
}

#endif

