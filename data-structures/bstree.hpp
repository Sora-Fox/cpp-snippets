#ifndef FTL_BSTREE_HPP
#define FTL_BSTREE_HPP

#include <cstddef>
#include <initializer_list>

namespace ftl {

template <typename T>
class Tree {
public:
    Tree();
    Tree(std::initializer_list<T>);
    Tree(const Tree &);
    Tree(Tree &&);
    ~Tree() { clear(); }

    Tree &operator=(const Tree &);
    Tree &operator=(Tree &&);

    void insert(const T &);
    void remove(const T &);

    void clear();

    size_t size() const { return m_size; }
    bool empty() const { return m_size == 0; }

private:
    struct Node;

    Node *root;
    size_t m_size;
};

}  // namespace ftl

#endif  // FTL_BSTREE_HPP
