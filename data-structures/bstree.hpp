#ifndef FTL_BSTREE_HPP
#define FTL_BSTREE_HPP

#include <initializer_list>
#include <ostream>
#include <queue>
#include <string>

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

    const T* max() const;
    const T* min() const;
    const T* root() const;
    const T* find(const T&) const;

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

    void showReversed(std::ostream&, std::string = " ");
    void show(std::ostream&, std::string = " ");
    void showAsTree(std::ostream&, std::string = " ", std::string = "\n");

private:
    struct Node;

    size_t m_size;
    Node* m_root;

    void clear(Node*);

    Node* insert(Node*, const T&);
    Node* remove(Node*, const T&);

    Node* max(Node*) const;
    Node* min(Node*) const;

    Node* find(Node*, const T&);

    void showReversed(std::ostream&, Node*, std::string);
    void show(std::ostream&, Node*, std::string);
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
Tree<T>::Tree(std::initializer_list<T> values) {
    for (auto i = values.begin(); i != values.end(); ++i) {
        insert(*i);
    }
}

template <typename T>
Tree<T>::Tree(Tree&& other) : m_root(other.m_root), m_size(other.m_size) {
    other.m_root = nullptr;
    other.m_size = 0;
}

template <typename T>
Tree<T>& Tree<T>::operator=(Tree&& other) {
    if (this == &other) {
        return *this;
    }
    clear();
    m_root = other.m_root;
    m_size = other.m_size;

    other.m_root = nullptr;
    other.m_size = 0;
    return *this;
}

template <typename T>
void Tree<T>::clear() {
    clear(m_root);
    m_size = 0;
    m_root = nullptr;
}

template <typename T>
void Tree<T>::clear(Node* root) {
    if (root == nullptr) {
        return;
    }
    std::queue<Node*> nodes;
    nodes.push(root);
    while (!nodes.empty()) {
        Node* current = nodes.front();
        nodes.pop();
        if (current->left) {
            nodes.push(current->left);
        }
        if (current->right) {
            nodes.push(current->right);
        }
        delete current;
    }
}


template <typename T>
void Tree<T>::insert(const T& data) {
    m_root = insert(m_root, data);
}

template <typename T>
typename Tree<T>::Node* Tree<T>::insert(Node* root, const T& data) {
    if (root == nullptr) {
        ++m_size;
        return new Node(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

template <typename T>
void Tree<T>::remove(const T& data) {
    m_root = remove(m_root, data);
}

template <typename T>
typename Tree<T>::Node* Tree<T>::remove(Node* root, const T& data) {
    if (root == nullptr) {
        return nullptr;
    }
    if (data < root->data) {
        root->left = remove(root->left, data);
    } else if (data > root->data) {
        root->right = remove(root->right, data);
    } else if (root->left != nullptr && root->right != nullptr) {
        Node* minNode = min(root->right);
        root->data = minNode->data;
        root->right = remove(root->right, minNode->data);
    } else {
        Node* tmp = (root->left != nullptr) ? root->left : root->right;
        --m_size;
        delete root;
        return tmp;
    }
    return root;
}

template <typename T>
const T* Tree<T>::root() const {
    return m_root ? &m_root->data : nullptr;
}

template <typename T>
const T* Tree<T>::max() const {
    Node* result = max(m_root);
    return result ? &result->data : nullptr;
}

template <typename T>
const T* Tree<T>::min() const {
    Node* result = min(m_root);
    return result ? &result->data : nullptr;
}

template <typename T>
const T* Tree<T>::find(const T& data) const {
    Node* result = find(m_root, data);
    return result ? &result->data : nullptr;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::max(Node* root) const {
    if (root && root->right) {
        return max(root->right);
    }
    return root;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::min(Node* root) const {
    if (root && root->left) {
        return min(root->left);
    }
    return root;
}

template <typename T>
typename Tree<T>::Node* Tree<T>::find(Node* root, const T& data) {
    if (root == nullptr) {
        return nullptr;
    } else if (data > root->data && root->right) {
        return find(root->right, data);
    } else if (data < root->data && root->left) {
        return find(root->left, data);
    } else {
        return root;
    }
}

template <typename T>
void Tree<T>::showReversed(std::ostream& os, std::string separator) {
    if (m_root != nullptr) {
        showReversed(os, m_root, separator);
    }
}

template <typename T>
void Tree<T>::showReversed(std::ostream& os, Node* root,
                           std::string separator) {
    if (root->right != nullptr) {
        showReversed(os, root->right, separator);
    }
    os << (root == max(m_root) ? "" : separator) << root->data;
    if (root->left != nullptr) {
        showReversed(os, root->left, separator);
    }
}

template <typename T>
void Tree<T>::show(std::ostream& os, std::string separator) {
    if (m_root != nullptr) {
        show(os, m_root, separator);
    }
}

template <typename T>
void Tree<T>::show(std::ostream& os, Node* root, std::string separator) {
    if (root->left != nullptr) {
        show(os, root->left, separator);
    }
    os << (root == min(m_root) ? "" : separator) << root->data;
    if (root->right != nullptr) {
        show(os, root->right, separator);
    }
}

template <typename T>
void Tree<T>::showAsTree(std::ostream& os, std::string separator,
                         std::string end) {
    std::queue<Node*> nodes;
    nodes.push(m_root);
    while (!nodes.empty()) {
        size_t levelSize = nodes.size();
        for (size_t i = 0; i < levelSize; ++i) {
            Node* current = nodes.front();
            nodes.pop();
            os << current->data;
            if (current->left != nullptr)
                nodes.push(current->left);
            if (current->right != nullptr)
                nodes.push(current->right);
            if (i != levelSize - 1)
                os << separator;
        }
        os << end;
    }
}

}  // namespace ftl

#endif  // FTL_BSTREE_HPP
