#ifndef LINKEDLIST_HPP
#define LINKEDLIST_HPP

#include <cstddef>
#include <utility>
#include <initializer_list>


template <typename T>
class LinkedList
{
public:
    LinkedList() : length(0), head(nullptr), tail(nullptr) {}
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
    
    iterator begin() { return iterator(this->head); }
    iterator end() {return iterator(nullptr); }
    // TODO add const iterators and reversed iterators

    LinkedList<T>& operator=(const LinkedList&);
    LinkedList<T>& operator=(LinkedList&&) noexcept;

    bool operator==(const LinkedList&) const;
    bool operator!=(const LinkedList& other) const { return !(*this == other); }

private:
    class Node;

    Node* head;
    Node* tail;
    size_t length;

    Node* getNodeByIndex(const size_t) const;
    void fillAllNodes(const T&);

    struct Node
    {
        Node(const T& data, Node* prev = nullptr, Node* next = nullptr) : data(data), prev(prev), next(next) {}

        T data;
        Node* next;
        Node* prev;
    };
};


template <typename T>
class LinkedList<T>::iterator
{
public:
    iterator(Node* ptr) : ptr(ptr) {}

    iterator& operator++() { this->ptr = this->ptr->next; return *this; }
    iterator operator++(int) { iterator tmp{ this->ptr }; ++(*this); return tmp; }

    iterator& operator--() { this->ptr = this->ptr->prev; return *this; }
    iterator operator--(int) { iterator tmp{ this->ptr }; --(*this); return tmp; }

    T& operator*() { return this->ptr->data; }
    bool operator!=(const iterator& other) const { return this->ptr != other.ptr; }
    bool operator==(const iterator& other) const { return this->ptr == other.ptr; }

private:
    Node* ptr;
};

template <typename T>
LinkedList<T>::LinkedList(std::initializer_list<T> values) : length(values.size()), head(nullptr), tail(nullptr)
{
    if (length == 0) return;
    
    this->head = new Node(*values.begin(), nullptr, nullptr);
    Node* current = this->head;

    for (auto i{ values.begin() }; i != values.end(); ++i)
    {
        if (i == values.begin()) continue;
        current->next = new Node(*i, current, nullptr);
        current = current->next;
    }
    this->tail = current;
}

template <typename T>
LinkedList<T>::LinkedList(const LinkedList& other) : length(0), head(nullptr), tail(nullptr)
{
    if (other.length)
    {    
        Node* current = other.head;
        while (current != nullptr)
        {
            this->push_back(current->data);
            current = current->next;
        }
    }
}

template <typename T>
LinkedList<T>::LinkedList(LinkedList&& other) noexcept
{
    this->head = other.head;
    this->tail = other.tail;
    this->length = other.length;

    other.head = other.tail = nullptr;
    other.length = 0;
}

template <typename T>
void LinkedList<T>::clear()
{
    while(this->length)
    {
       this->pop_front();
    }
}

template <typename T>
void LinkedList<T>::push_back(const T& data)
{
    if (!this->length)
        this->head = this->tail = new Node(data);
    else
    {
        this->tail = new Node(data, this->tail, nullptr);
        this->tail->prev->next = this->tail;
    }
    ++this->length;
}

template <typename T>
void LinkedList<T>::push_front(const T& data)
{
    if (!this->length)
        this->head = this->tail = new Node(data);
    else
    {
        this->head = new Node(data, nullptr, this->head);
        this->head->next->prev = this->head;
    }
    ++this->length;
}

template <typename T>
void LinkedList<T>::insert(const T& data,const size_t index)
{
    if (index == 0)
    {
        this->push_front(data);
    }
    else
    {
        Node* changing_node = this->getNodeByIndex(index);
        changing_node->prev = new Node(data, changing_node->prev, changing_node);
        changing_node->prev->prev->next = changing_node->prev;
        ++this->length;
    }
}

template <typename T>
void LinkedList<T>::assign(const size_t length, const T& data)
{
    if (this->length <= length)
    {
        // changing all nodes 
        this->fillAllNodes(data);
        // pushing new nodes
        for (size_t i{ 0 }; i < length - this->length; ++i)
            this->push_back(data);
    }
    else 
    {
        // removing extra nodes
        for (size_t i{ 0 }; i < this->length - length; ++i)
            this->pop_back();
        // changing all nodes
        this->fillAllNodes(data);
    }
}

template <typename T>
void LinkedList<T>::pop_back()
{
    Node* new_tail = this->tail->prev;
    delete this->tail;
    this->tail = new_tail;

    if (new_tail != nullptr)
        this->tail->next = nullptr;
    else
        this->head = nullptr;

    --this->length;
}

template <typename T>
void LinkedList<T>::pop_front()
{
    Node* new_head = this->head->next;
    delete this->head;
    this->head = new_head;

    if (new_head != nullptr)
        this->head->prev = nullptr;
    else
        this->tail = nullptr;

    --this->length;
}

template <typename T>
void LinkedList<T>::erase(const size_t index)
{
    if (index == 0)
    {
        this->pop_front();
    }
    else if (index == this->length - 1)
    {
        this->pop_back();
    }
    else
    {
        Node* rm_node = this->getNodeByIndex(index);
        rm_node->prev->next = rm_node->next;
        rm_node->next->prev = rm_node->prev;
        delete rm_node;
        --this->length;
    }
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& other)
{
    if (this == &other) return *this;
    this->clear();
    
    this->head = this->tail = nullptr;
    this->length = other.length;
    if (this->length == 0) return *this;

    Node* other_current = other.head;
    Node* this_current = this->head = new Node(other_current->data);

    while (other_current->next != nullptr)
    {
        other_current = other_current->next;
        this_current->next = new Node(other_current->data, this_current);
        this_current = this_current->next;
    }
    return *this;
}

template <typename T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList&& other) noexcept
{
    this->clear();
    if (other.length == 0) return *this;

    this->head = other.head;
    this->tail = other.tail;
    this->length = other.length;
    
    other.head = other.tail = nullptr;
    other.length = 0;

    return *this;
}

template <typename T>
bool LinkedList<T>::operator==(const LinkedList& other) const
{
    if (this->length != other.length)
        return false;
    if (!this->length && !other.length)
        return true;

    Node* this_current = this->head;
    Node* other_current = other.head;

    while (this_current != nullptr)
    {
        if (this_current->data != other_current->data)
            return false;
        this_current = this_current->next;
        other_current = other_current->next;
    }
    return true;
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::getNodeByIndex(const size_t index) const
{
    if (index == 0)
    {
        return this->head;
    }
    else if (index == this->length - 1)
    {
        return this->tail;
    }
    else if (index <= this->length / 2)
    {
        Node* result = this->head;
        for (size_t i{ 0 }; i < index; ++i)
            result = result->next;
        return result;
    }
    else
    {
        Node* result = this->tail;
        for (size_t i{ this->length - 1 }; i > index; --i)
            result = result->prev;
        return result;
    }
}

template <typename T>
void LinkedList<T>::fillAllNodes(const T& data)
{
    Node* current = this->head;
    while (current != nullptr)
    {
        current->data = data;
        current = current->next;
    } 

}


#endif // LINKEDLIST_HPP
