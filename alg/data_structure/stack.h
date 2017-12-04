#ifndef ALG_STACK_H_
#define ALG_STACK_H_

#include <cstddef>

#include "alg/common/exception.h"

namespace alg {

template <class T>
class Stack {
private:
    T *buffer;
    int top_;
    std::size_t size;

public:
    Stack(std::size_t n);
    virtual ~Stack();

    void push(T element);
    T pop();
    T top();
    std::size_t count();
};

template <class T>
Stack<T>::Stack(std::size_t n) {
    size = n;
    top_ = -1;
    buffer = new T[n];
}

template <class T>
Stack<T>::~Stack() {
    delete[] buffer;
}

template <class T>
void Stack<T>::push(T element) {
    if (top_ == size - 1) {
        throw except::BufferFull();
    }

    top_++;
    buffer[top_] = element;
}

template <class T>
T Stack<T>::pop() {
    if (top_ == -1) {
        throw except::BufferEmpty();
    }

    top_--;
    return buffer[top_ + 1];
}

template <class T>
T Stack<T>::top() {
    if (top_ == -1) {
        throw except::BufferEmpty();
    }

    return buffer[top_];
}

template <class T>
std::size_t Stack<T>::count() {
    return top_ + 1;
}

}

#endif
