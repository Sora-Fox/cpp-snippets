#ifndef FTL_BSTREE_HPP
#define FTL_BSTREE_HPP

#include <cstddef>
#include <initializer_list>

namespace ftl {

template <typename T>
class Tree {
public:
    Tree() : m_root(nullptr), m_size(0) {}
    Tree(std::initializer_list<T>);
    Tree(const Tree&) = delete;
    Tree(Tree&&);
    ~Tree() { clear(); }

    Tree& operator=(const Tree&) = delete;
    Tree& operator=(Tree&&);

    void clear();

    void insert(const T& data);
    void remove(const T& data);

    const T& max() const;
    const T& min() const;

    const T& root() const { return m_root->data; }
    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

private:
    struct Node;

    size_t m_size;
    Node* m_root;

    void clear(Node*);

    Node* insert(Node*, const T&);
    Node* remove(Node*, const T&);
};

template <typename T>
struct Tree<T>::Node {
    Node(const T& data, Node* left = nullptr, Node* right = nullptr)
        : data(data), left(left), right(right) {}

    T data;
    Node* left;
    Node* right;
};

template <typename T>
Tree<T>::Tree(Tree&& other) : m_root(other.m_root), m_size(other.m_size) {
    other.m_root = nullptr;
    other.m_size = 0;
}

template <typename T>
Tree<T>& Tree<T>::operator=(Tree&& other) {
    clear();
    m_root = other.m_root;
    m_size = other.m_size;

    other.m_root = nullptr;
    other.m_size = 0;
    return *this;
}

template <typename T>
void Tree<T>::clear() {
    if (m_root != nullptr) {
        clear(m_root);
        m_size = 0;
    }
}

template <typename T>
void Tree<T>::clear(Node* root) {
    if (root == nullptr) {
        return;
    }
    clear(root->left);
    clear(root->right);
    delete root;
}

template <typename T>
void Tree<T>::insert(const T& data) {
    m_root = insert(m_root, data);
    ++m_size;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::insert(Node* root, const T& data) {
    if (root == nullptr) {
        return new Node(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else {
        root->right = insert(root->right, data);
    }
    return root;
}

template <typename T>
void Tree<T>::remove(const T& data) {
    m_root = remove(m_root, data);
    --m_size;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::remove(Node* root, const T& data) {
    if (root->data == data) {
        Node* tmp = root->right != nullptr ? root->right : root->left;
        delete root;
        return tmp;
    }
    if (data < root->data) {
        root->left = remove(root->left, data);
    } else {
        root->right = remove(root->right, data);
    }
    return root;
}

template <typename T>
const T& Tree<T>::max() const {
    Node* max = m_root;
    while (max->right != nullptr) {
        max = max->right;
    }
    return max->data;
}

template <typename T>
const T& Tree<T>::min() const {
    Node* min = m_root;
    while (min->left != nullptr) {
        min = min->left;
    }
    return min->data;
}

}  // namespace ftl

#endif  // FTL_BSTREE_HPP
