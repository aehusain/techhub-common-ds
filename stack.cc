#include "stack.h"
#include <iostream>

namespace
{
    const int SIZE = 10; // incase user does not provide stack size
}

using namespace std;

template <typename T>
Stack<T>::Stack():
    s_(new T[SIZE]), size_(SIZE)
{
}

template <typename T>
Stack<T>::Stack(size_t size):
    s_(new T[size]), size_(size)
{
}

template <typename T>
Stack<T>::~Stack()
{
    // give the world everything back before u die !
    delete[] s_;
}

template <typename T>
bool
Stack<T>::empty() const
{
    return top_ == 0 ? true : false;
}

template <typename T>
void
Stack<T>::pop()
{
    if (empty()) {
        cerr << "Stack is empty!" << endl;
        return ;
    }
    cout << "Popped element: " << s_[--top_] << endl;
}

template <typename T>
void
Stack<T>::push(const T &data)
{
    if (top_+1 == size_) {
        // resize needed:
        // overhead: new memory allocation + copying contents
        T *new_s = new T[size_*2];
        for (int i = 0; i < top_; ++i) {
            new_s[i] = s_[i];
        }

        // free old memory and assign new location
        delete[] s_;
        s_ = new_s;
        size_ *= 2;
    }

    s_[top_++] = data;
}


template <typename T>
T
Stack<T>::top() const
{
    if (empty()) {
        cerr << "Stack is empty!" << endl;
        return (T)'\0';
    }

    return s_[top_-1];
}


template <typename T>
void
Stack<T>::clear()
{
    // clear the contents of the stack
    for (size_t i = 0; i < top_; ++i) {
        s_[i] = (T)'\0';
    }

    // reset the top pointer
    top_ = 0;
}

template <typename T>
void
Stack<T>::display() const
{
    for (size_t i = 0; i < top_; ++i) {
        cout << "s[" << i << "]: " << s_[i] << "\n";
    }
    cout << endl;
}

int
main(int argc, char *argv[])
{
    Stack<char> s(5);

    cout << s.top() << "\n";
    s.push('h');
    s.push('u');
    s.push('s');
    cout << s.top() << "\n";
    s.push('a');
    s.push('i');
    s.push('n');

    s.display();
    s.pop();
    cout << s.top() << "\n";
    s.clear();
    s.top();

    return 0;
}
