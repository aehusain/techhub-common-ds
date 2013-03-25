#ifndef DOUBLE_LIST_H
#define DOUBLE_LIST_H

#include <vector>

template<class T>
class LinkList;

template <class T>
class Node
{
    private:
        T               data_;
        Node<T>         *next_;
        Node<T>         *prv_;
        friend class    LinkList<T>;
    public:
        Node(const T &data):
            data_(data), next_((Node<T>*)0), prv_((Node<T>*)0)
        { }

        ~Node()
        { }
};

template <class T>
class LinkList
{
    private:
        Node<T>         *head_;
        Node<T>         *tail_;

    public:
        LinkList(const std::vector<T> &);
        ~LinkList();

        // insertion routines
        void pushback(const T &);

        void insert_before(const T &, const T &);
        void insert_before(Node<T> *node, const T &);

        void insert_after(const T &, const T &);
        void insert_after(Node<T> *node, const T &);


        // deletion routines
        void remove(const T &);
        void remove(const Node<T>*);

        void remove_after(const T &);
        void remove_after(Node<T> *);

        void remove_before(const T &);
        void remove_before(Node<T> *);

        // utility routines
        Node<T>* find(const T &);

        void display();
};

#endif // DOUBLE_LIST_H
