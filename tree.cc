#include "tree.h"
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <memory>
#include <boost/assign/list_of.hpp>
#include <boost/program_options.hpp>

using namespace std;
namespace po = boost::program_options;

template <class T>
Node<T>:: Node(const T &in):
    data_(in), left_(NULL), right_(NULL)
{ };

template <class T>
BinaryTree<T>::BinaryTree(const vector<T> &in):
    root_(NULL)
{
    typedef typename vector<T>::const_iterator ItrConst;
    ItrConst itr(in.begin());

    // construct the binary tree
    for (;itr != in.end(); ++itr) {
        insert(*itr);
    }
}



template <class T>
BinaryTree<T>::~BinaryTree()
{
    remove_all();
}



template <class T>
void
BinaryTree<T>::insert(const T &in)
{
    if (root_ == NULL) {
        root_ = new Node<T>(in);
    } else {
        // search for the place for new data
        Node<T> **temp = search(in);
        if (!*temp) {
            *temp = new Node<T>(in);
        } else {
            cout << "Node: " << in << " already exists!\n";
        }
    }
}



template <class T>
Node<T> **
BinaryTree<T>::search(const T &in) const
{
    if (!root_) {
        return NULL;
    }

    Node<T> **node = (Node<T>**)&root_;

    while (*node) {
        T data = (*node)->get_data();
        if (data == in) {
            cout << "Node already present: " << in << "\n";
            return NULL;
        }

        node = data > in ?
                &((*node)->left_) : &((*node)->right_);
    }

    return node;
}



template<class T>
void
BinaryTree<T>::remove_all()
{
    while (root_->left_) {
        remove(&(root_->left_));
    }

    while (root_->right_) {
        remove(&(root_->right_));
    }

    remove(&root_);
}



template<class T>
void
BinaryTree<T>::remove(const T data)
{
    Node<T> **node = search(data);
    if (!node) {
        cout << "Node not found: " << data << "\n";
        return ;
    }

    remove(node);
}



template<class T>
void
BinaryTree<T>::remove(Node<T> **node)
{
    /**
     * Three possible conditions:
     * 1. Node has no children: remove the sucker. done!
     * 2. Node has one children (left / right): replace it.
     * 3. Node has both children: this one is tricky!
     *    Look for nodes righmost successor and replace it with this node
     */
    if (!(*node)->left_ && !(*node)->right_) {
        delete *node;
        *node = NULL;
    } else if((*node)->left_ && !(*node)->right_) {
        Node<T> *to_remove = *node;
        *node = (*node)->left_;
        delete to_remove;
    } else if((*node)->right_ && !(*node)->left_) {
        Node<T> *to_remove = *node;
        *node = (*node)->right_;
        delete to_remove;
    } else {
        // scenario 3
        Node<T> **successor = &((*node)->left_);
        while ((*successor)->right_) {
            successor = &((*successor)->right_);
        }

        // swap rightmost child data
        (*node)->data_ = (*successor)->data_;
        delete *successor;
        *successor = NULL;
    }
}


template <class T>
void
BinaryTree<T>::mirror()
{
    cout << "Before mirror: ";
    preorder();
    mirror_internal(&root_);
    cout << "After mirror: ";
    preorder();
}

template <class T>
void
BinaryTree<T>::mirror_internal(Node<T> **node)
{
    /**
     * Idea is to mirror the complete tree i.e.
     * Left part goes to Right and vice-versa.
     */
    if (!*node) {
        return ;
    }
    Node<T> *temp = (*node)->left_;
    (*node)->left_ = (*node)->right_;
    (*node)->right_ = temp;

    mirror_internal(&((*node)->left_));
    mirror_internal(&((*node)->right_));
}


template<class T>
void
BinaryTree<T>::preorder() const
{
    cout << "***** Preorder Traversal ****\n";
    preorder_internal(root_);
    cout << endl;
}



template<class T>
void
BinaryTree<T>::preorder_internal(const Node<T> *node) const
{
    /**
     * Steps:
     * 1. Visit root
     * 2. Visit left child
     * 3. Visit right child
     */
    if (!node) {
        return ;
    }

    cout << node->data_ << ",";
    preorder_internal(node->left_);
    preorder_internal(node->right_);
}



template<class T>
void
BinaryTree<T>::inorder() const
{
    cout << "***** Inorder Traversal ****\n";
    inorder_internal(root_);
    cout << endl;
}


template<class T>
void
BinaryTree<T>::inorder_internal(const Node<T> *node) const
{
    /**
     * Steps:
     * 1. Visit left child
     * 2. Visit root
     * 3. Visit right child
     */
    if (!node) {
        return ;
    }

    inorder_internal(node->left_);
    cout << node->data_ << ",";
    inorder_internal(node->right_);
}



template<class T>
void
BinaryTree<T>::postorder() const
{
    cout << "***** Postorder Traversal ****\n";
    postorder_internal(root_);
    cout << endl;
}


template<class T>
void
BinaryTree<T>::postorder_internal(const Node<T> *node) const
{
    /**
     * Steps:
     * 1. Visit left child
     * 2. Visit root
     * 3. Visit right child
     */
    if (!node) {
        return ;
    }

    postorder_internal(node->left_);
    postorder_internal(node->right_);
    cout << node->data_ << ",";
}



template<class T>
void
BinaryTree<T>::bft() const
{
    queue<Node<T> *> q;
    q.push(root_);

    cout << "\n***** Breadth First Traversal****\n";

    while (!q.empty()) {
        Node<T> *cur = q.front();
        q.pop();

        cout << cur->get_data() << ",";
        if (cur->left_) {
            q.push(cur->left_);
        }
        if (cur->right_) {
            q.push(cur->right_);
        }
    }
    cout << endl;
}


template<class T>
void
BinaryTree<T>::rightmost(int level)
{
    if (level < 0) {
        cerr << "Invalid argument: " << level << endl;
        return ;
    }

    /**
     * Rightmost node at level N
     * Following the concept of bft, the rightmost node
     * should be the last element of the queue if queue at a given
     * time only contains the elements pertaining to that level.
     */
    queue<Node <T> *> q;
    q.push(root_);

    int l = 0;

    while (!q.empty() && l < level) {
        // maintain a local queue per level
        queue<Node<T> *>lq;

        while (!q.empty()) {
            Node<T> *cur = q.front();
            q.pop();

            if (cur->left_) {
                lq.push(cur->left_);
            }

            if (cur->right_) {
                lq.push(cur->right_);
            }
        }

        if (lq.empty()) {
            cout << "Node does not exist at level: " << level << endl;
            return ;
        }
        q = lq;
        ++l;
    }

    cout << "Node at level " << level << ": " << q.back()->data_ << endl;
}



int
main(int argc, char *argv[])
{
    vector<int> nodes = boost::assign::list_of
                                (10)
                                (5)
                                (4)
                                (20)
                                (15)
                                (18)
                                (25)
                                (6)
                                (7)
                                (2)
                                (14)
                                ;

    BinaryTree<int> bt(nodes);

    // Use boost program options to get the user input
    try {

        po::options_description desc;
        desc.add_options()
            ("help,h", "help")
            ("inorder,i", "Inorder Traversal")
            ("preorder,p", "Preorder Traversal")
            ("postorder,o", "PostOrder Traversal")
            ("bft,b", "Breadth First Traversal")
            ("mirror,m", "Mirror")
            ("rightmost,r", po::value<int>(), "Rightmost node at given level")
            ;

        po::variables_map vm;

        try {
            po::store(po::parse_command_line(argc, argv, desc), vm);

            if (vm.count("help")) {
                cout << "Usage: \n" << desc;
                return 0;
            } else if(vm.count("inorder")) {
                bt.inorder();
                return 0;
            } else if (vm.count("preorder")) {
                bt.preorder();
                return 0;
            } else if (vm.count("postorder")) {
                bt.postorder();
                return 0;
            } else if (vm.count("bft")) {
                bt.bft();
                return 0;
            }else if (vm.count("mirror")) {
                bt.mirror();
                return 0;
            } else if(vm.count("rightmost")) {
                int level = vm["rightmost"].as<int>();
                bt.rightmost(level);
                return 0;
            }

            po::notify(vm);
        } catch (po::error &e) {
            cerr << "Error: " << e.what();
            cerr << desc << endl;
            return -2;
        }

    } catch (std::exception &e) {
        cerr << "Exception caught: " << e.what() << ". Terminating program";
        return -1;
    }

    return 0;
}

