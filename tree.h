#ifndef _TREE_H

#include <vector>

/**
 * Tree: datastructure representing Tree
 *
 * This is Abstract Datatype
 */

template <class T>
class BinaryTree;

template <class T>
class Node {
    private:
        T data_;
        Node<T> *left_;
        Node<T> *right_;

        friend class BinaryTree<T>;
    public:
        explicit Node(const T &in);

        ~Node()
        { };

        T
            get_data()
            {
                return data_;
            }
};



template <class T>
class BinaryTree
{
    private:
        Node<T> *root_;

        void mirror_internal(Node<T> **node);
        void inorder_internal(const Node<T> *node) const;
        void postorder_internal(const Node<T> *node) const;
        void preorder_internal(const Node<T> *node) const;

    public:
        BinaryTree(const std::vector<T> &in);
        ~BinaryTree();

        // helper routines
        Node<T>** search(const T&) const;

        // traversal
        void insert(const T &);
        void remove_all();
        void remove(const T data);
        void remove(Node<T> **node);

        // Depth First Traversal methods
        void postorder() const;
        void inorder() const;
        void preorder() const;

        // Breadth First Traversal
        void bft() const;

        // mirror left and right subtrees.
        void mirror();

        // find the rightmost node at given level
        void rightmost(int level);
};

#endif //TREE_H
