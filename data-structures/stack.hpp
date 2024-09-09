#ifndef FTL_STACK_HPP
#define FTL_STACK_HPP

#include <stdexcept>

namespace ftl {

template <typename T>
class Stack {
public:
    Stack() : peek(nullptr), length(0) {}
    Stack(const Stack&) = delete;
    Stack(const Stack&&) = delete;
    // TODO new constructor with arguments
    ~Stack();

    void operator=(const Stack&) = delete;
    void operator=(const Stack&&) = delete;

    void push(const T&);
    void pop();
    T top() const;

    bool empty() const { return this->length == 0; }
    size_t size() const { return this->length; }

private:
    struct Node;

    Node* peek;
    size_t length;

    void throwIfEmpty() const;

    struct Node {
        explicit Node(const T& data, const Node* prev)
            : data(data), prev(prev){};

        T data;
        Node* prev;
    };
};


template <typename T>
Stack<T>::~Stack() {
    while (this->peek != nullptr) {
        Node* new_peek = this->peek->prev;
        delete this->peek;
        this->peek = new_peek;
    }
}

template <typename T>
T Stack<T>::top() const {
    this->throwIfEmpty();
    return this->peek->data;
}
template <typename T>
void Stack<T>::push(const T& data) {
    this->peek = new Node(data, this->peek);
    ++this->length;
}

template <typename T>
void Stack<T>::pop() {
    this->throwIfEmpty();

    Node* new_peek = this->peek->prev;
    delete this->peek;
    this->peek = new_peek;

    --this->length;
}

template <typename T>
void Stack<T>::throwIfEmpty() const {
    if (this->length == 0)
        throw std::runtime_error("Stack is empty.");
}

}  // namespace ftl

#endif  // FTL_STACK_HPP
