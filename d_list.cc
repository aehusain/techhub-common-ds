#include "d_link.h"
#include <iostream>
#include <vector>
#include <boost/assign/list_of.hpp>

using namespace std;

template<class T>
LinkList<T>::LinkList(const vector<T> &in):
    head_(NULL), tail_(NULL)
{
    typedef typename vector<T>::const_iterator ItrConst;
    ItrConst itr(in.begin());
    ItrConst end(in.end());

    for (; itr != end; ++itr) {
        pushback(*itr);
    }
}



template<class T>
LinkList<T>::~LinkList()
{
    while (tail_) {
        remove(tail_);
    }
}



/**
 * pushback
 *
 * Insert new node into list at the end.
 */
template<class T>
void
LinkList<T>::pushback(const T &data)
{
    Node<T>* node = new Node<T>(data);
    if (!tail_) {
        tail_ = node;
        head_ = tail_;
    } else {
        tail_->next_ = node;
        node->prv_ = tail_;
        tail_ = node;
    }
}




/**
 * insert_after
 * If NULL is provided append as tail node
 */
template<class T>
void
LinkList<T>::insert_after(const T &node_data, const T &data)
{
    Node<T> *candidate = find(node_data);
    if (!candidate) {
        cerr << "No node with data: " << node_data << ".\n";
        return ;
    }

    insert_after(candidate, data);
}



/**
 * insert_after
 * If NULL is provided append as tail node
 */
template<class T>
void
LinkList<T>::insert_after(Node<T>* node, const T &data)
{
    Node<T> *candidate = new Node<T>(data);

    if (!node) {
        if (!tail_) {
            head_ = tail_ = candidate;
        } else {
            tail_->next_ = candidate;
            candidate->prv_ = tail_;
            tail_ = candidate;
        }
    } else {
        if (node == tail_) {
            tail_->next_ = candidate;
            candidate->prv_ = tail_;
            tail_ = candidate;
        } else {
            node->next_->prv_ = candidate;
            candidate->next_ = node->next_;
            node->next_ = candidate;
            candidate->prv_ = node;
        }
    }
}


/**
 * insert_before
 * If NULL is provided, insert at head node
 */
template<class T>
void
LinkList<T>::insert_before(const T &node_data, const T &data)
{
    Node<T>* candidate = find(node_data);
    if (!candidate) {
        cerr << "No node exist with data: " << data << "\n";
        return ;
    }

    insert_before(candidate, data);
}



/**
 * insert_before
 * If NULL is passed append data as a head node
 */
template<class T>
void
LinkList<T>::insert_before(Node<T> *node, const T &data)
{
    Node<T> *candidate = new Node<T>(data);

    if (!node) {
        // make new node as head_ node
        if (!head_) {
            head_ = tail_ = candidate;
        } else {
            candidate->next_ = head_;
            head_->prv_ = candidate;
            head_ = candidate;
        }
    } else {
        if (node == head_) {
            candidate->next_ = head_;
            head_->prv_ = candidate;
            if (tail_ == head_) {
                tail_ = head_;
            }
            head_ = candidate;
        } else {
            node->prv_->next_ = candidate;
            candidate->prv_ = node->prv_;
            candidate->next_ = node;
            node->prv_ = candidate;
        }
    }
}



/**
 * remove
 *
 * Removes the node with given data.
 */
template<class T>
void
LinkList<T>::remove(const T &data)
{
    Node<T> *candidate = find(data);
    if (!candidate) {
        cerr << "Node: " << data << " does not exist!\n";
        return ;
    }
    remove(candidate);
}



/**
 * remove
 *
 * Removes the node with given data.
 */
template<class T>
void
LinkList<T>::remove(const Node<T> *node)
{
    if (!node) {
        cerr << "Invalid input: NULL\n";
        return ;
    }

    if (node == head_) {
        head_ = head_->next_;
        if (head_) {
            head_->prv_ = NULL;
        } else {
            // no more elements left, reset tail_ too
            tail_ = NULL;
        }
    } else if (node == tail_) {
        tail_ = node->prv_;
        if (tail_) {
            tail_->next_ = NULL;
        } else {
            // no elements left
            head_ = NULL;
        }
    } else {
        node->next_->prv_ = node->prv_;
        node->prv_->next_ = node->next_;
    }

    delete node;
}

/*
 * remove_after
 *
 * Removes a node after the given node.
 * if NULL is passed, it will remove the tail node
 */
template<class T>
void
LinkList<T>::remove_after(const T &data)
{
    Node<T> *candidate = find(data);
    if (!candidate) {
        cerr << "Node: " << data << " does not exist!\n";
        return ;
    }

    remove_after(candidate);
}

/**
 * remove_after
 *
 * Removes a node after the given node.
 * if NULL is passed, it will remove the tail node
 */
template<class T>
void
LinkList<T>::remove_after(Node<T> *node)
{
    Node<T> *candidate = NULL;
    if (!node) {
        // remove tail node
        candidate = tail_;
        tail_ = tail_->prv_;
        if (tail_) {
            // may be this is the last node
            tail_->next_ = NULL;
        }
    } else {
        candidate = node->next_;
        if (node == tail_) {
            cerr << "Node: " << node->data_ << " is a tail node!\n";
            return ;
        } else if (candidate == tail_) {
            // adjust tail if needed
            tail_ = node;
            tail_->next_ = NULL;
        } else {
            node->next_ = candidate->next_;
            candidate->prv_ = node;
        }

        // adjust head just incase
        if (candidate == head_) {
            head_ = node->next_;
        }
    }

    delete candidate;
}



/*
 * remove_before
 *
 * Removes a node after the given node.
 * if NULL is passed, it will remove the tail node
 */
template<class T>
void
LinkList<T>::remove_before(const T &data)
{
    Node<T> *candidate = find(data);
    if (!candidate) {
        cerr << "Node: " << data << " does not exist!\n";
        return ;
    }

    remove_before(candidate);
}


/**
 * remove_before
 *
 * Removes a node after the given node.
 * if NULL is passed, it will remove the head node
 */
template<class T>
void
LinkList<T>::remove_before(Node<T> *node)
{
    Node<T> *candidate = NULL;
    if (!node) {
        // remove head node
        candidate = head_;
        head_ = head_->next_;
        if (head_) {
            // may be this is the last node
            head_->prv_ = NULL;
        }

    } else {
        candidate = node->prv_;
        if(node == head_) {
            cerr << "Node " << node->data_ << " is a head node!\n";
            return ;
        } else if (node->prv_ == head_) {
            head_ = node;
            head_->prv_ = NULL;
        } else {
            candidate->prv_->next_ = node;
            node->prv_ = candidate->prv_;
        }

        if (candidate == tail_) {
            tail_ = node->prv_;
        }
    }

    delete candidate;
}



/**
 * find
 *
 * return pointer to node input
 * if not found, return NULL
 */
template<class T>
Node<T>*
LinkList<T>::find(const T &data)
{
    Node<T> *candidate = head_;

    for (; candidate != NULL && candidate->data_ != data;
         candidate = candidate->next_);

    return candidate;
}



/**
 * display
 *
 * Display the list elements on console
 * TODO: enhance it to take outstream?
 */
template<class T>
void
LinkList<T>::display()
{
    int i = 0;
    for (Node<T> *candidate = head_;
         candidate != NULL; candidate = candidate->next_, ++i) {
        cout << "Node[" << i << "]: " << candidate->data_ << "\n";
    }
}



int
main(int argc, char *argv[])
{
    vector<char> input = boost::assign::list_of
                                ('a')
                                ('b')
                                ('c')
                                ('d')
                                ('e')
                                ('f')
                                ;
    LinkList<char> ll(input);

    ll.insert_after('c', '0');
    ll.display();

    return 0;
}

