#ifndef STACK_H
#define STACK_H

#include <stdio.h>

/**
 * Stack
 *
 * One of the most commonly used datastructures. It follows the
 * principle of First In Last Out (FIFO). The supported operations
 * are:
 * 1. Push: push new element on the top of the stack.
 * 2. Pop: pop element from top of the stack.
 * 3. top: peek and return the top of the stack (don't remove).
 *
 * Multiple choices can be made to hold stack elements:
 * 1. Dynamic Arrays
 * 2. Linked list.
 *
 * Both approaches has its own advantages and dis-adavantages. In this
 * solution, I have selected option 1; one time memory allocation, low
 * node memory overhead (as linklist memory overhead may be high if
 * each node stores say only int value).
 */
template <typename T>
class Stack
{
    private:
        T *s_;
        size_t size_;
        int top_;
    public:
        Stack();
        explicit Stack(size_t size);
        ~Stack();

        void push(const T &data);
        void pop();
        T top() const;

        // utility function
        bool empty() const;
        void clear();

        // purely test interface
        void display() const;
};

#endif // STACK_H
