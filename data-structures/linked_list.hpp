#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstddef>
#include <initializer_list>
#include <iterator>
#include <utility>

template <typename T>
class LinkedList {
  public:
    LinkedList() : length(0), head(nullptr), tail(nullptr), sentinel(nullptr) {}
    LinkedList(std::initializer_list<T>);
    LinkedList(const LinkedList&);
    LinkedList(LinkedList&& other) noexcept;
    ~LinkedList() { this->clear(); }

    void clear();
    class iterator;

    size_t size() const { return this->length; }
    size_t memory() const { return sizeof(Node) * this->length; }

    void push_back(const T&); // performance tested
    void push_front(const T&);
    void insert(const T&, const size_t);
    void assign(const size_t, const T&);

    void pop_back(); // performance tested
    void pop_front();
    void erase(const size_t);
    iterator erase(iterator);

    iterator begin() const { return iterator(this->head); }
    iterator end() const { return iterator(nullptr); }
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

    Node* getNodeByIndex(const size_t) const;
    void fillAllNodes(const T&);

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
        this->ptr = this->ptr->next;
        return *this;
    }

    iterator operator++(int) {
        iterator tmp {this->ptr};
        ++(*this);
        return tmp;
    }

    iterator& operator--() {
        if (this->ptr != nullptr)
            this->ptr = this->ptr->prev;
        return *this;
    }

    iterator operator--(int) {
        iterator tmp {this->ptr};
        --(*this);
        return tmp;
    }

    T& operator*() { return this->ptr->data; }

    bool operator!=(const iterator& other) const {
        return this->ptr != other.ptr;
    }
    bool operator==(const iterator& other) const {
        return this->ptr == other.ptr;
    }

  private:
    Node* ptr;
};

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> values)
    : length(values.size()), head(nullptr), tail(nullptr), sentinel(nullptr) {
    if (length == 0)
        return;

    this->head = new Node(*values.begin(), nullptr, nullptr);
    Node* current = this->head;

    for (auto i = values.begin(); i != values.end(); ++i) {
        if (i == values.begin())
            continue;
        current->next = new Node(*i, current, nullptr);
        current = current->next;
    }
    tail = current;
    sentinel = new Node(0, tail);
    tail->next = sentinel;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other)
    : length(0), head(nullptr), tail(nullptr), sentinel(nullptr) {
    if (other.length) {
        Node* current = other.head;
        while (current != other.sentinel) {
            this->push_back(current->data);
            current = current->next;
        }
        sentinel = new Node(0, tail);
        tail->next = sentinel;
    }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept {
    this->head = other.head;
    this->tail = other.tail;
    this->sentinel = other.sentinel;
    this->length = other.length;

    other.head = other.tail = other.sentinel = nullptr;
    other.length = 0;
}

template <typename T>
void LinkedList<T>::clear() {
    while (this->length) {
        this->pop_front();
    }
    delete sentinel;
    sentinel = nullptr;
}

template <typename T>
void LinkedList<T>::push_back(const T& data) {
    if (!this->length) {
        this->head = this->tail = new Node(data);
        sentinel = new Node(0, tail);
        tail->next = sentinel;
    } else {
        this->tail = new Node(data, this->tail, sentinel);
        this->tail->prev->next = this->tail;
        sentinel->prev = tail;
    }
    ++this->length;
}

template <typename T>
void LinkedList<T>::push_front(const T& data) {
    if (!this->length) {
        this->head = this->tail = new Node(data);
        sentinel = new Node(0, tail);
        tail->next = sentinel;
    } else {
        this->head = new Node(data, nullptr, this->head);
        this->head->next->prev = this->head;
    }
    ++this->length;
}
/*
template <typename T>
void LinkedList<T>::insert(const T& data, const size_t index) {
    if (index == 0) {
        this->push_front(data);
    } else {
        Node* changing_node = this->getNodeByIndex(index);
        changing_node->prev =
            new Node(data, changing_node->prev, changing_node);
        changing_node->prev->prev->next = changing_node->prev;
        ++this->length;
    }
}

template <typename T>
void LinkedList<T>::assign(const size_t length, const T& data) {
    if (this->length <= length) {
        this->fillAllNodes(data);
        for (size_t i {0}; i < length - this->length; ++i)
            this->push_back(data);
    } else {
        for (size_t i {0}; i < this->length - length; ++i)
            this->pop_back();
        this->fillAllNodes(data);
    }
}
*/
template <typename T>
void LinkedList<T>::pop_back() {
    Node* new_tail = this->tail->prev;
    delete this->tail;
    this->tail = new_tail;

    if (new_tail != nullptr) {
        this->tail->next = sentinel;
        sentinel->prev = tail;
    } else {
        this->head = sentinel = nullptr;
    }

    --this->length;
}

template <typename T>
void LinkedList<T>::pop_front() {
    Node* new_head = this->head->next;
    delete this->head;
    this->head = new_head;

    if (new_head != nullptr)
        this->head->prev = nullptr;
    else {
        this->tail = sentinel = nullptr;
    }

    --this->length;
}
/*
template <typename T>
void LinkedList<T>::erase(const size_t index) {
    if (index == 0) {
        this->pop_front();
    } else if (index == this->length - 1) {
        this->pop_back();
    } else {
        Node* rm_node = this->getNodeByIndex(index);
        rm_node->prev->next = rm_node->next;
        rm_node->next->prev = rm_node->prev;
        delete rm_node;
        --this->length;
    }
}
*/

template <typename T>
typename LinkedList<T>::iterator
LinkedList<T>::erase(LinkedList<T>::iterator where) {
    if (where.ptr == this->head) {
        this->pop_front();
        return this->begin();
    } else if (where.ptr == this->tail->prev) {
        this->pop_back();
        return --this->end();
    } else if (where != this->end()) {
        for (auto it = this->begin(); it != this->end(); ++it) {
            if (it == where) {
                it.ptr->next->prev = it.ptr->prev;
                it.ptr->prev->next = it.ptr->next;
                auto tmp = LinkedList<T>::iterator(it.ptr->prev);
                delete it.ptr;
                --length;
                return tmp;
            }
        }
    }
}
// TODO here
template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other) {
    if (this == &other)
        return *this;
    this->clear();

    this->head = this->tail = nullptr; // TODO check if it necessary
    this->length = other.length;
    if (this->length == 0)
        return *this;

    Node* other_current = other.head;
    Node* this_current = this->head = new Node(other_current->data);

    while (other_current->next != nullptr) {
        other_current = other_current->next;
        this_current->next = new Node(other_current->data, this_current);
        this_current = this_current->next;
    }
    // TODO where is tail?
    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept {
    this->clear();
    if (other.length == 0)
        return *this;

    this->head = other.head;
    this->tail = other.tail;
    this->length = other.length;

    other.head = other.tail = nullptr;
    other.length = 0;

    return *this;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList& other) const {
    if (this->length != other.length)
        return false;
    if (!this->length && !other.length)
        return true;

    Node* this_current = this->head;
    Node* other_current = other.head;

    while (this_current != nullptr) {
        if (this_current->data != other_current->data)
            return false;
        this_current = this_current->next;
        other_current = other_current->next;
    }
    return true;
}

template <typename T>
typename LinkedList<T>::Node*
LinkedList<T>::getNodeByIndex(const size_t index) const {
    if (index == 0) {
        return this->head;
    } else if (index == this->length - 1) {
        return this->tail;
    } else if (index <= this->length / 2) {
        Node* result = this->head;
        for (size_t i {0}; i < index; ++i)
            result = result->next;
        return result;
    } else {
        Node* result = this->tail;
        for (size_t i {this->length - 1}; i > index; --i)
            result = result->prev;
        return result;
    }
}

template <typename T>
void LinkedList<T>::fillAllNodes(const T& data) {
    Node* current = this->head;
    while (current != nullptr) {
        current->data = data;
        current = current->next;
    }
}

#endif // LINKEDLIST_HPP
