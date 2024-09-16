#ifndef FTL_LIST_HPP
#define FTL_LIST_HPP

#include <initializer_list>
#include <iterator>  // for std::bidirectional_iterator_tag

namespace ftl {

template <typename T>
class List {
public:
    List() : length(0), head(nullptr), tail(nullptr), sentinel(nullptr) {}
    List(std::initializer_list<T>);
    List(const List &);
    List(List &&) noexcept;
    ~List() { clear(); }

    class const_iterator;
    class iterator;

    void clear();
    void sort();

    size_t size() const { return length; }
    bool empty() const { return length == 0; }

    void push_back(const T &);
    void push_front(const T &);

    void pop_back();
    void pop_front();

    iterator insert(iterator, const T &);
    iterator insert(iterator, std::initializer_list<T>);

    iterator erase(iterator);
    iterator erase(iterator, iterator);

    const_iterator cbegin() const { return const_iterator(head, 0); }
    const_iterator cend() const { return const_iterator(sentinel, length + 1); }

    iterator begin() const { return iterator(head, 0); }
    iterator end() const { return iterator(sentinel, length + 1); }
    // TODO reversed iterators

    List<T> &operator=(const List &);
    List<T> &operator=(List &&) noexcept;

    bool operator==(const List &) const;
    bool operator!=(const List &other) const { return !(*this == other); }

private:
    struct Node;

    Node *head;
    Node *tail;
    Node *sentinel;
    size_t length;

    void quickSort(iterator, iterator);
    iterator partition(iterator, iterator);

    struct Node {
        Node(const T &data, Node *prev = nullptr, Node *next = nullptr)
            : data(data), prev(prev), next(next) {}

        T data;
        Node *next;
        Node *prev;
    };
};

template <typename T>
class List<T>::const_iterator {
public:
    friend class List;

    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;

    const_iterator &operator++() {
        ptr = ptr->next;
        ++position;
        return *this;
    }
    const_iterator operator++(int) {
        const_iterator tmp(ptr);
        ptr = ptr->next;
        ++position;
        return tmp;
    }

    const_iterator &operator--() {
        ptr = ptr->prev;
        --position;
        return *this;
    }
    const_iterator operator--(int) {
        const_iterator tmp(ptr);
        ptr = ptr->prev;
        --position;
        return tmp;
    }

    const T &operator*() { return ptr->data; }

    size_t operator-(const const_iterator &other) const {
        return position - other.position;
    }

    bool operator!=(const const_iterator &other) const {
        return ptr != other.ptr;
    }
    bool operator==(const const_iterator &other) const {
        return ptr == other.ptr;
    }
    bool operator<(const const_iterator &other) const {
        return position < other.position;
    }
    bool operator>(const const_iterator &other) const {
        return other.position < position;
    }

protected:
    Node *ptr;
    size_t position;

    const_iterator(Node *ptr = nullptr, size_t position = 0)
        : ptr(ptr), position(position) {}
};

template <typename T>
class List<T>::iterator : public List<T>::const_iterator {
public:
    friend class List;

    T &operator*() { return List<T>::const_iterator::ptr->data; }

private:
    iterator(Node *ptr = nullptr, size_t position = 0)
        : List<T>::const_iterator(ptr, position) {}
};

template <typename T>
List<T>::List(std::initializer_list<T> values)
    : length(values.size()), head(nullptr), tail(nullptr), sentinel(nullptr) {
    if (length == 0)
        return;

    auto iter = values.begin();
    head = new Node(*iter);
    Node *current = head;
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
List<T>::List(const List &other)
    : length(other.length), head(nullptr), tail(nullptr), sentinel(nullptr) {
    if (length != 0) {
        head = new Node(other.head->data);
        Node *current = head;
        Node *other_current = other.head->next;
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
List<T>::List(List &&other) noexcept {
    head = other.head;
    tail = other.tail;
    sentinel = other.sentinel;
    length = other.length;

    other.head = other.tail = other.sentinel = nullptr;
    other.length = 0;
}

template <typename T>
void List<T>::clear() {
    Node *current = head;
    while (current != sentinel) {
        current = current->next;
        delete current->prev;
    }
    delete current;
    head = tail = sentinel = nullptr;
    length = 0;
}

template <typename T>
void List<T>::push_back(const T &data) {
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
void List<T>::push_front(const T &data) {
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
    Node *new_tail = tail->prev;
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
    Node *new_head = head->next;
    delete head;
    head = new_head;

    if (head != sentinel)
        head->prev = nullptr;
    else
        tail = sentinel = nullptr;
    --length;
}

template <typename T>
void List<T>::sort() {
    if (length > 1)
        quickSort(begin(), end());
}

template <typename T>
void List<T>::quickSort(iterator left, iterator right) {
    if (left != right && left.ptr->next != right.ptr) {
        iterator middle = partition(left, right);
        quickSort(left, middle);
        quickSort(middle, right);
    }
}

template <typename T>
typename List<T>::iterator List<T>::partition(iterator left, iterator right) {
    T pivot = right.ptr->prev->data;
    --right;
    while (true) {
        while (*left < pivot)
            ++left;
        while (*right > pivot)
            --right;
        if (left == right || left.ptr == right.ptr->next)
            return left;
        std::swap(left.ptr->data, right.ptr->data);
        ++left;
        --right;
    }
}

template <typename T>
typename List<T>::iterator List<T>::insert(List<T>::iterator where,
                                           const T &value) {
    auto current = where.ptr;

    if (current == nullptr) {
        head = tail = new Node(value);
        sentinel = tail->next = new Node(0, tail);
        where = iterator(head);
    } else if (current->prev == nullptr) {
        head = new Node(value, nullptr, head);
        head->next->prev = head;
    } else if (current->next == nullptr) {
        tail = new Node(value, tail, sentinel);
        sentinel->prev = tail;
        tail->prev->next = tail;
    } else {
        current->prev->next = new Node(value, current->prev, current);
        current->prev = current->prev->next;
    }
    ++length;
    return iterator(current->prev);
}

template <typename T>
typename List<T>::iterator List<T>::insert(List<T>::iterator where,
                                           std::initializer_list<T> values) {
    length += values.size();
    auto current = where.ptr;

    if (current != head) {
        for (const T &value : values) {
            current->prev->next = new Node(value, current->prev, current);
            current->prev = current->prev->next;
        }
        if (current == sentinel)
            tail = current->prev;
    } else {
        head = new Node(*values.begin(), nullptr, current);
        current->prev = head;
        auto it = values.begin();
        ++it;
        for (; it != values.end(); ++it) {
            current->prev->next = new Node(*it, current->prev, current);
            current->prev = current->prev->next;
        }
    }
    return iterator(current->prev);
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
List<T> &List<T>::operator=(const List &other) {
    if (this == &other)
        return *this;

    clear();
    length = other.length;
    if (length == 0)
        return *this;

    head = new Node(other.head->data);
    Node *current = head;
    Node *other_current = other.head->next;

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
List<T> &List<T>::operator=(List &&other) noexcept {
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
bool List<T>::operator==(const List &other) const {
    if (length != other.length)
        return false;
    if (!length && !other.length)
        return true;

    Node *current = head;
    Node *other_current = other.head;

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
