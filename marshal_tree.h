#ifndef MARSHAL_TREE_H

#include <iostream>
#include <list>
#include <vector>
#include <queue>
#include <map>
#include <stdexcept>

using namespace std;
template <typename T>
class NTree
{
    private:
        template <typename Tn>
        class TNode
        {
            private:
                Tn data_;
                list<TNode<Tn> *> children_;

                // avoid copy and assignment
                TNode(const TNode<Tn> &);
                TNode<Tn>& operator=(const TNode<Tn> &);

            public:
                typedef typename list<TNode<Tn> *>::const_iterator C_ITOR;

                explicit
                    TNode(const Tn &data):
                        data_(data)
                    { }

                void
                    add_child(TNode<Tn> *child)
                    {
                        children_.push_back(child);
                    }

                bool
                    is_child(TNode<Tn> *candidate) const
                    {
                        C_ITOR itr = children_.find(candidate);

                        return itr != children_.end()? true : false;
                    }

                list<TNode<Tn> *>&
                    get_children()
                    {
                        return children_;
                    }

                T
                    get_data() const
                    {
                        return data_;
                    }
        };

        template <typename Tl>
        class LNode
        {
            private:
                Tl data_;
                int level_;
                LNode<T> *parent_;

                // avoid copy and assignment
                LNode(const LNode<Tl> &);
                LNode& operator=(const LNode<Tl> &);
            public:
                explicit LNode(Tl &data):
                    data_(data), level_(0), parent_(NULL)
                { }

                ~LNode()
                { }

                void
                    set_level(int level)
                    {
                        level_ = level;
                    }

                int
                    get_level() const
                    {
                        return level_;
                    }

                void
                    set_parent(LNode<T> *parent)
                    {
                        parent_ = parent;
                    }

                LNode<T>*
                    get_parent() const
                    {
                        return parent_;
                    }

                Tl
                    get_data()
                    {
                        return data_;
                    }
        };

        TNode<T> *root_;
        list<LNode<T> *> llist_;

    public:
        typedef typename list<TNode<T> *>::const_iterator T_CITR;
        typedef typename list<LNode<T>* >::const_iterator L_CITR;
        typedef typename list<LNode<T> *>::iterator       L_ITR;

        explicit NTree(const vector<T> &in)
        {
            /** Construct sample tree that looks like this
             *                     1
             *                    /  \
             *               2                7
             *            /  |  \          /     \
             *           3   4  5         8      9
             */
            typedef typename vector<T>::const_iterator ITR;

            ITR itr (in.begin());
            ITR end (in.end());

            root_ = new TNode<T>(*itr); ++itr;
            TNode<T> *two = new TNode<T>(*itr); ++itr;
            TNode<T> *three = new TNode<T>(*itr); ++itr;
            TNode<T> *four = new TNode<T>(*itr); ++itr;
            TNode<T> *five = new TNode<T>(*itr); ++itr;
            TNode<T> *six = new TNode<T>(*itr); ++itr;
            TNode<T> *seven = new TNode<T>(*itr); ++itr;
            TNode<T> *eight = new TNode<T>(*itr); ++itr;
            TNode<T> *nine = new TNode<T>(*itr); ++itr;

            root_->add_child(two);
            root_->add_child(seven);

            two->add_child(three);
            two->add_child(four);
            two->add_child(five);

            seven->add_child(eight);
            seven->add_child(nine);
        }

        ~NTree()
        {
            if (!root_) {
                // nothing needs to be done
                return ;
            }

            // destroy tree
            destroy_tree();


            // cleanup List nodes
            destroy_list();
        }

        void
            marshal()
            {
                /* Steps involved in marshalling:
                 * Perform BFS and prepare list nodes with following info:
                 * 1. Data
                 * 2. Level
                 * 3. Parent
                 */
                if (!root_) {
                    // do nothing
                    return ;
                }

                queue<TNode<T> *> q;
                q.push(root_);
                T data = root_->get_data();
                llist_.push_back(new LNode<T>(data));

                while (!q.empty()) {
                    TNode<T> *tCandidate = q.front();
                    q.pop();
                    data = tCandidate->get_data();

                    list<TNode<T> *> children = tCandidate->get_children();

                    T_CITR t_itr (children.begin());
                    T_CITR t_end (children.end());
                    for (; t_itr != t_end; ++t_itr) {
                        q.push(*t_itr);
                        T c_data = (*t_itr)->get_data();
                        LNode<T> *lNode = new LNode<T>(c_data);
                        LNode<T> *parent = find_list_node(data);
                        if (!parent) {
                            throw logic_error("We should never get here!");
                        }
                        lNode->set_parent(parent);
                        lNode->set_level(parent->get_level() + 1);
                        llist_.push_back(lNode);
                    }
                }

                destroy_tree();
                display_list();
            }

        void
            demarshal()
            {
                /* Iterate list node and reconstruct tree using
                 * 1. Data
                 * 2. Parent node information
                 * 3. level information
                 */
                L_ITR itr (llist_.begin());
                L_ITR end (llist_.end());

                map<T, TNode<T>* > hash;
                typedef typename map<T, TNode<T> *>::iterator M_ITR;

                for (; itr != end; ++itr) {
                    T child_data = (*itr)->get_data();
                    TNode<T> *child = new TNode<T>(child_data);
                    hash[child_data] = child;

                    LNode<T> *parent = (*itr)->get_parent();
                    if (!parent) {
                        // root is orphan
                        hash[child_data] = child;
                        root_ = child;
                        continue ;
                    }
                    T parent_data = parent->get_data();
                    M_ITR m_itr = hash.find(parent_data);
                    m_itr->second->add_child(child);
                }

                destroy_list();
                display_tree();
            }

        // display routines
        void
            display_tree()
            {
                cout << "Display Tree\n";
                //perform BFS and print nodes
                queue<TNode<T> *>q;
                q.push(root_);

                while (!q.empty()) {
                    TNode<T> *candidate = q.front();
                    q.pop();

                    cout << candidate->get_data() << ", ";

                    list<TNode<T> *> children = candidate->get_children();

                    T_CITR t_itr (children.begin());
                    T_CITR t_end (children.end());
                    for (; t_itr != t_end; ++t_itr) {
                        q.push(*t_itr);
                    }
                }

                cout << endl;
            }

        void
            display_list()
            {
                cout << "Display List\n";
                L_CITR itr (llist_.begin());
                L_CITR end (llist_.end());

                for (; itr != end; ++itr) {
                    cout << (*itr)->get_data() << ", ";
                }

                cout << endl;
            }

    private:

        LNode<T>*
            find_list_node(T &data)
            {

                L_CITR itr (llist_.begin());
                L_CITR end (llist_.end());

                for (; itr != end; ++itr) {
                    if ((*itr)->get_data() == data) {
                        return *itr;
                    }
                }

                // not found
                return NULL;
            }

        void
            destroy_tree()
            {
                cout << "Destroying Tree!\n";

                // perform BFT and delete the tree node
                queue<TNode<T> *> q;

                q.push(root_);

                while (!q.empty()) {
                    TNode<T> *candidate = q.front();
                    q.pop();

                    list<TNode<T> *> children = candidate->get_children();

                    T_CITR t_itr (children.begin());
                    T_CITR t_end (children.end());
                    for (; t_itr != t_end; ++t_itr) {
                        q.push(*t_itr);
                    }

                    cout << candidate->get_data() << ", ";
                    delete candidate;
                }
                cout << endl;
                root_ = NULL;
            }

        void
            destroy_list()
            {
                cout << "Destroying list\n";

                L_CITR l_itr (llist_.begin());
                L_CITR l_end (llist_.end());
                for (; l_itr != l_end; ++l_itr) {
                    cout << (*l_itr)->get_data() << ", ";
                    delete (*l_itr);
                }
                // clear list contents
                llist_.clear();
                cout << endl;
            }

};

#endif // MARSHAL_TREE_H
