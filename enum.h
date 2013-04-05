#ifndef ENUM_H

#include <iostream>
#include <set>
#include <algorithm>

template <typename T>
class Enum
{
    protected:
        // unary predicate helps in finding value in enum
        struct find_predicate: public std::unary_function
                               <const Enum<T>*, bool> {
                find_predicate(int value): p_value_(value)
                { }
                bool operator()(const Enum<T> *E)
                {
                    return E->get_value() == p_value_;
                }
            private:
                int p_value_;
        };

        // binary predicate helps in creating set in ascending order
        struct less_predicate: public std::binary_function
                               <Enum<T>*, Enum<T>*, bool> {
            bool operator()(const Enum<T> *e1, const Enum<T>* e2)
            {
                return e1->get_value() < e2->get_value();
            }
        };

    public:
        
        typedef typename std::set<Enum<T>*, less_predicate> instances;
        typedef typename instances::const_iterator      c_iterator;

        explicit Enum(int value);
        // copy constructor and = are OK.
        int
            get_value() const
            {
                return value_;
            }

        // iteration routines
        static c_iterator
            begin()
            {
                return s_instances.begin();
            }
        static c_iterator
            end()
            {
                return s_instances.end();
            }
        static c_iterator
            find(int value)
            {
                c_iterator itr = find_if(s_instances.begin(),
                                         s_instances.end(),
                                         find_predicate(value));
                return itr;
            }
        // utility routines
        static bool
            is_valid(int value)
            {
                c_iterator itr = find(value);
                return itr != end() ? true: false;
            }

    protected:

        static instances s_instances;
        int value_;
};

template <typename T>
Enum<T>::Enum(int value):
    value_(value)
{
    s_instances.insert(this);
}

#endif // ENUM_H
