#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstddef>
#include <initializer_list>
#include <iterator>

template <typename T>
class LinkedList {
   public:
    LinkedList() : length(0), head(nullptr), tail(nullptr), sentinel(nullptr) {}
    LinkedList(std::initializer_list<T>);
    LinkedList(const LinkedList&);
    LinkedList(LinkedList&& other) noexcept;
    ~LinkedList() { clear(); }

    void clear();
    class iterator;

    size_t size() const { return length; }
    size_t memory() const { return sizeof(Node) * length; }

    void push_back(const T&);  // performance tested
    void push_front(const T&);

    void pop_back();  // performance tested
    void pop_front();

    iterator erase(iterator);
    iterator erase(iterator, iterator);

    iterator begin() const { return iterator(head); }
    iterator end() const { return iterator(sentinel); }
    // TODO add const iterators and reversed iterators

    LinkedList<T>& operator=(const LinkedList&);
    LinkedList<T>& operator=(LinkedList&&) noexcept;

    bool operator==(const LinkedList&) const;
    bool operator!=(const LinkedList& other) const { return !(*this == other); }

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
class LinkedList<T>::iterator {
   public:
    friend class LinkedList;

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
LinkedList<T>::LinkedList(std::initializer_list<T> values)
    : length(values.size()), head(nullptr), tail(nullptr), sentinel(nullptr) {
    if (length == 0) return;

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
LinkedList<T>::LinkedList(const LinkedList& other)
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
LinkedList<T>::LinkedList(LinkedList&& other) noexcept {
    head = other.head;
    tail = other.tail;
    sentinel = other.sentinel;
    length = other.length;

    other.head = other.tail = other.sentinel = nullptr;
    other.length = 0;
}

template <typename T>
void LinkedList<T>::clear() {
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
void LinkedList<T>::push_back(const T& data) {
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
void LinkedList<T>::push_front(const T& data) {
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
void LinkedList<T>::pop_back() {
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
void LinkedList<T>::pop_front() {
    Node* new_head = head->next;
    delete head;
    head = new_head;

    if (head != nullptr)
        head->prev = nullptr;
    else
        tail = sentinel = nullptr;
    --length;
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::erase(
    LinkedList<T>::iterator where) {
    if (where.ptr == head) {
        pop_front();
        return begin();
    } else if (where.ptr == tail->prev) {
        pop_back();
        return --end();
    } else if (where != end()) {
        for (auto it = begin(); it != end(); ++it) {
            if (it == where) {
                it.ptr->prev->next = it.ptr->next;
                it.ptr->next->prev = it.ptr->prev;
                --length;
                auto tmp = LinkedList<T>::iterator(it.ptr->next);
                delete it.ptr;
                return tmp;
            }
        }
    }
}

template <typename T>
typename LinkedList<T>::iterator LinkedList<T>::erase(
    LinkedList<T>::iterator first, LinkedList<T>::iterator last) {
    if (first == last) return first;

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
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this == &other) return *this;

    clear();
    length = other.length;
    if (length == 0) return *this;

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
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept {
    clear();
    if (other.length == 0) return *this;

    head = other.head;
    tail = other.tail;
    sentinel = other.sentinel;
    length = other.length;

    other.head = other.tail = nullptr;
    other.length = 0;
    return *this;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList& other) const {
    if (length != other.length) return false;
    if (!length && !other.length) return true;

    Node* current = head;
    Node* other_current = other.head;

    while (current != tail) {
        if (current->data != other_current->data) return false;
        current = current->next;
        other_current = other_current->next;
    }
    return true;
}

#endif  // LINKEDLIST_HPP
