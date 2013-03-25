#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <vector>
#include <map>
#include <list>
#include <stack>
#include <queue>

typedef enum
{
    NOT_VISITED,
    IN_PROGRESS,
    VISITED
}STATUS;

template <class T>
class Graph
{
    private:
        template <class Ty>
        class Vertex
        {
            private:
                Ty data_;
                STATUS status_;
            public:
                explicit Vertex(const Ty in):
                    data_(in), status_(NOT_VISITED)
                { };

                ~Vertex() { };

                void
                set_status(const STATUS s)
                {
                    status_ = s;
                };

                STATUS
                get_status() const
                {
                    return status_;
                };

                Ty
                get_data() const
                {
                    return data_;
                }
        };

        typedef typename std::list<Vertex<T>*>                  Vertices;
        typedef typename std::list<Vertex<T>*>::iterator        VerticesItr;
        typedef typename std::list<Vertex<T>*>::const_iterator  VerticesItrConst;
        typedef typename std::map<Vertex<T>*, Vertices>         AdjList;
        typedef typename std::map<Vertex<T>*, Vertices>::iterator       AdjListItr;
        typedef typename std::map<Vertex<T>*, Vertices>::const_iterator AdjListItrConst;

        // define Vertex class
        Vertex<T>*
        allocVertex(const char in)
        {
            Vertex<T> *temp = new Vertex<T>(in);

            return temp;
        }

        Vertex<T>*
        find_vertex(const T in)
        {
            VerticesItrConst itr (vertices_.begin());

            for (; itr != vertices_.end(); ++itr) {
                if ((*itr)->get_data() == in) {
                    return *itr;
                }
            }

            return NULL;
        }

        Vertices        vertices_;
        AdjList         list_;
        bool            isCyclic_;

    public:
        explicit Graph(std::vector<T> &v)
        {
            typedef typename std::vector<T>::const_iterator inItr;
            inItr itr(v.begin());

            for (; itr != v.end(); ++itr) {
                vertices_.push_back(new Vertex<T>(*itr));
            }

            isCyclic_ = false;
        };

        ~Graph()
        {
            // memory leak is bAd !!
            VerticesItrConst itr(vertices_.begin());

            for (; itr != vertices_.end(); ++itr) {
                delete(*itr);
            }
        }

        bool
        hasCycles()
        {
            return isCyclic_;
        }

        void
        connectVertex(const T &s , const T &d)
        {
            Vertex<T> *src = find_vertex(s);
            Vertex<T> *dest = find_vertex(d);

            if (!s || !d) {
                std::cout << "invalid vertex" << std::endl;
                return ;
            }

            AdjListItr itr = list_.find(src);

            if (itr == list_.end()) {
                std::list<Vertex<T>*> edges;
                edges.push_back(dest);
                list_[src] = edges;
            } else {
                itr->second.push_back(dest);
            }
        }

        // traversal
        void bft()
        {
            /**
             * Breadth First Traversal
             *
             * The algorithms visits each node in the graph level by level.
             * Starting with a node, it will visit all its neighbours first
             * before visting a level down.
             */
            std::cout << "Breadth First Traversal\n";

            VerticesItr itr(vertices_.begin());
            std::queue<Vertex<T>*> q;

            for (; itr != vertices_.end() && (*itr)->get_status() != VISITED;
                 ++itr) {

                q.push(*itr);

                while (!q.empty()) {
                    Vertex<T> *cur = q.front();
                    q.pop();
                    if (cur->get_status() != VISITED) {
                        cur->set_status(VISITED);
                        std::cout << cur->get_data() << ",";
                    }

                    AdjListItrConst itr = list_.find(cur);
                    if (itr != list_.end()) {
                        Vertices v = itr->second;
                        VerticesItrConst vItr(v.begin());

                        for (; vItr != v.end(); ++vItr) {
                            if ((*vItr)->get_status() != VISITED) {
                                q.push(*vItr);
                            }
                        }
                    }
                }
            }

            std::cout << std::endl;
        }

        void dft()
        {
            /**
             * DFS: Depth First Search
             * Go down the graph looking for nodes and print it on the way
             *
             * Modified version can be used to find cycles.
             * Three shades for a possible Node:
             * 1. Not Visitied
             * 2. In progress
             * 3. Visited
             *
             * Start with a node, set its state to "Not Visitied", visit
             * all its neighbours; append them to stack and set their state
             * to "In Progress". Once the node is done set its state to "Visited".
             * During traversal of neighbours, if we encounter a node whose state
             * is "In Progress", then we found a cycle in a graph.
             */

            std::cout << "Depth First Traversal\n";

            VerticesItr itr(vertices_.begin());
            // use stack; simulates recursion with no frame creation cost.
            std::stack<Vertex<T>*> s;

            for (; itr != vertices_.end() && (*itr)->get_status() == NOT_VISITED;
                 ++itr) {
                s.push(*itr);

                while (!s.empty()) {
                    Vertex<T> *cur = s.top();
                    s.pop();
                    if (cur->get_status() != VISITED) {
                        std::cout << cur->get_data() << ",";
                        cur->set_status(IN_PROGRESS);
                    }

                    AdjListItrConst lItr = list_.find(cur);
                    if (lItr != list_.end()) {
                        Vertices v = lItr->second;
                        if (!v.empty()) {
                            VerticesItrConst vItr(v.begin());

                            for (; vItr != v.end(); ++vItr) {
                                STATUS status = (*vItr)->get_status();
                                if (status == IN_PROGRESS) {
                                    std::cout << "Graph has cycles" << std::endl;
                                    isCyclic_ = true;
                                    return ;
                                } else if (status == NOT_VISITED) {
                                    (*vItr)->set_status(IN_PROGRESS);
                                    s.push(*vItr);
                                }
                            }
                        }
                    }

                    cur->set_status(VISITED);
                }
            }

            std::cout << std::endl;
        }

        // utility function
        void reset() // set all nodes to NOT_VISITED state
        {
            VerticesItrConst itr(vertices_.begin());

            for (; itr != vertices_.end(); ++itr) {
                if ((*itr)->get_status() != NOT_VISITED) {
                    (*itr)->set_status(NOT_VISITED);
                }
            }
        }
};

#endif
