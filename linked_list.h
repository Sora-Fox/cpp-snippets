#ifndef LINKEDLIST_H
#define LINKEDLIST_H


#include <iostream>
#include <cstddef>


template <typename T>
class LinkedList
{
public:
    LinkedList() : length(0), head(nullptr), tail(nullptr) {}
    LinkedList(T data) : length(1) { this->head = this->tail = new Node (data); }
    ~LinkedList();

    size_t size() const { return this->length; }
    void clear();

    void push_back(const T&);
    void push_front(const T&);

    void pop_back();
    void pop_front();
    void pop_index(size_t);

    T& operator[](size_t) const;
    void print() const;

private:
    class Node;

    Node* head;
    Node* tail;
    size_t length;

    void checkIndex(size_t) const;
    Node* getNodeByIndex(size_t) const;


    class Node
    {
    public:
        Node(const T& data, Node* prev = nullptr, Node* next = nullptr) : data(data), prev(prev), next(next) {}

        T data;
        Node* next;
        Node* prev;
    };
};







template <typename T>
LinkedList<T>::~LinkedList()
{
    this->clear();
}

template <typename T>
void LinkedList<T>::clear()
{
    while(this->length)
    {
       this->pop_front();
    }
    this->head = this->tail = nullptr;
    this->length = 0;
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
        this->head->prev->prev = this->head;
    }
    ++this->length;
}

template <typename T>
void LinkedList<T>::pop_back()
{
    if (this->tail == nullptr)
        throw std::exception();
    
    Node* new_tail = this->tail->prev;
    delete this->tail;
    this->tail = new_tail;
    
    if (new_tail != nullptr)
        this->tail->next = nullptr;

    --this->length;
}

template <typename T>
void LinkedList<T>::pop_front()
{
    if (this->head == nullptr)
        throw std::exception();
    
    Node* new_head = this->head->next;
    delete this->head;
    this->head = new_head;
    
    if (new_head != nullptr)
        this->head->prev = nullptr;

    --this->length;
}

template <typename T>
void LinkedList<T>::pop_index(size_t index)
{
    this->checkIndex(index);

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
T& LinkedList<T>::operator[](size_t index) const
{
    this->checkIndex(index);
    return this->getNodeByIndex(index)->data;
}

template <typename T>
void LinkedList<T>::print() const
{
    if (!this->length)
    {
        std::cout << "Empty list" << std::endl;
        return;
    }

    Node* current = this->head;
    do
    {
        std::cout << current->data << ' ';
        current = current->next;
    } while (current != nullptr);
                
    std::cout << std::endl;
}

template <typename T>
void LinkedList<T>::checkIndex(size_t index) const
{
    if (this->length == 0 || index < 0 || index > this->legth - 1)
        throw std::exception();
}

template <typename T>
typename LinkedList<T>::Node* LinkedList<T>::getNodeByIndex(size_t index) const
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

#endif // LINKEDLIST_H
