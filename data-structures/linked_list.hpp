#ifndef FTL_LIST_HPP
#define FTL_LIST_HPP

#include <initializer_list>
#include <iterator>

namespace ftl {

template <typename T>
class List {
public:
    List() : length(0), head(nullptr), tail(nullptr), sentinel(nullptr) {}
    List(std::initializer_list<T>);
    List(const List&);
    List(List&&) noexcept;
    ~List() { clear(); }

    class iterator;

    void clear();
    size_t size() const { return length; }
    bool empty() const { return length == 0; }

    void push_back(const T&);
    void push_front(const T&);

    void pop_back();
    void pop_front();

    iterator erase(iterator);
    iterator erase(iterator, iterator);

    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(sentinel); }
    // TODO add const iterators and reversed iterators

    List<T>& operator=(const List&);
    List<T>& operator=(List&&) noexcept;

    bool operator==(const List&) const;
    bool operator!=(const List& other) const { return !(*this == other); }

private:
    struct Node;

    Node* head;
    Node* tail;
    Node* sentinel;
    size_t length;

    struct Node {
        Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
            : data(data), prev(prev), next(next) {}

        T data;
        Node* next;
        Node* prev;
    };
};

template <typename T>
class List<T>::iterator {
public:
    friend class List;

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T*;
    using reference = T&;

    iterator(Node* ptr) : ptr(ptr) {}

    iterator& operator++() {
        ptr = ptr->next;
        return *this;
    }

    iterator operator++(int) {
        iterator tmp(ptr);
        ptr = ptr->next;
        return tmp;
    }

    iterator& operator--() {
        ptr = ptr->prev;
        return *this;
    }

    iterator operator--(int) {
        iterator tmp(ptr);
        ptr = ptr->prev;
        return tmp;
    }

    T& operator*() { return ptr->data; }

    bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    bool operator==(const iterator& other) const { return ptr == other.ptr; }

private:
    Node* ptr;
};

template <typename T>
List<T>::List(std::initializer_list<T> values)
    : length(values.size()), head(nullptr), tail(nullptr), sentinel(nullptr) {
    if (length == 0)
        return;

    auto iter = values.begin();
    head = new Node(*iter);
    Node* current = head;
    ++iter;

    for (; iter != values.end(); ++iter) {
        current->next = new Node(*iter, current);
        current = current->next;
    }
    tail = current;
    sentinel = new Node(0, tail);
    tail->next = sentinel;
}

template <typename T>
List<T>::List(const List& other)
    : length(other.length), head(nullptr), tail(nullptr), sentinel(nullptr) {
    if (length != 0) {
        head = new Node(other.head->data);
        Node* current = head;
        Node* other_current = other.head->next;
        while (other_current != other.sentinel) {
            current->next = new Node(other_current->data, current);
            current = current->next;
            other_current = other_current->next;
        }
        tail = current;
        sentinel = new Node(0, tail);
        tail->next = sentinel;
    }
}

template <typename T>
List<T>::List(List&& other) noexcept {
    head = other.head;
    tail = other.tail;
    sentinel = other.sentinel;
    length = other.length;

    other.head = other.tail = other.sentinel = nullptr;
    other.length = 0;
}

template <typename T>
void List<T>::clear() {
    Node* current = head;
    while (current != sentinel) {
        current = current->next;
        delete current->prev;
    }
    delete current;
    head = tail = sentinel = nullptr;
    length = 0;
}

template <typename T>
void List<T>::push_back(const T& data) {
    if (length != 0) {
        tail = new Node(data, tail, sentinel);
        tail->prev->next = tail;
        sentinel->prev = tail;
    } else {
        head = tail = new Node(data);
        sentinel = new Node(0, tail);
        tail->next = sentinel;
    }
    ++length;
}

template <typename T>
void List<T>::push_front(const T& data) {
    if (length != 0) {
        head = new Node(data, nullptr, head);
        head->next->prev = head;
    } else {
        head = tail = new Node(data);
        sentinel = new Node(0, tail);
        tail->next = sentinel;
    }
    ++length;
}

template <typename T>
void List<T>::pop_back() {
    Node* new_tail = tail->prev;
    delete tail;
    tail = new_tail;

    if (tail != nullptr) {
        tail->next = sentinel;
        sentinel->prev = tail;
    } else {
        head = sentinel = nullptr;
    }
    --length;
}

template <typename T>
void List<T>::pop_front() {
    Node* new_head = head->next;
    delete head;
    head = new_head;

    if (head != sentinel)
        head->prev = nullptr;
    else
        tail = sentinel = nullptr;
    --length;
}

template <typename T>
typename List<T>::iterator List<T>::erase(List<T>::iterator where) {
    if (where.ptr == head) {
        pop_front();
        return begin();
    }
    if (where.ptr == tail->prev) {
        pop_back();
        return --end();
    }
    for (auto it = begin(); it != end(); ++it) {
        if (it != where)
            continue;
        it.ptr->prev->next = it.ptr->next;
        it.ptr->next->prev = it.ptr->prev;
        --length;
        auto tmp = List<T>::iterator(it.ptr->next);
        delete it.ptr;
        return tmp;
    }
}

template <typename T>
typename List<T>::iterator List<T>::erase(List<T>::iterator first,
                                          List<T>::iterator last) {
    if (first == last)
        return first;

    while (first != last) {
        auto current = first.ptr;
        ++first;

        if (current->prev != nullptr) {
            current->prev->next = current->next;
        } else {
            head = current->next;
        }

        if (current->next != sentinel) {
            current->next->prev = current->prev;
        } else {
            tail = head = sentinel = nullptr;
            length = 0;
            return nullptr;
        }

        delete current;
        --length;
    }
    return first;
}

template <typename T>
List<T>& List<T>::operator=(const List& other) {
    if (this == &other)
        return *this;

    clear();
    length = other.length;
    if (length == 0)
        return *this;

    head = new Node(other.head->data);
    Node* current = head;
    Node* other_current = other.head->next;

    while (other_current != other.sentinel) {
        current->next = new Node(other_current->data, current);
        other_current = other_current->next;
        current = current->next;
    }
    tail = current;
    sentinel = new Node(0, tail);
    tail->next = sentinel;
    return *this;
}

template <typename T>
List<T>& List<T>::operator=(List&& other) noexcept {
    clear();
    if (other.length == 0)
        return *this;

    head = other.head;
    tail = other.tail;
    sentinel = other.sentinel;
    length = other.length;

    other.head = other.tail = nullptr;
    other.length = 0;
    return *this;
}

template <typename T>
bool List<T>::operator==(const List& other) const {
    if (length != other.length)
        return false;
    if (!length && !other.length)
        return true;

    Node* current = head;
    Node* other_current = other.head;

    while (current != tail) {
        if (current->data != other_current->data)
            return false;
        current = current->next;
        other_current = other_current->next;
    }
    return true;
}

}  // namespace ftl

#endif  // FTL_LIST_HPP
