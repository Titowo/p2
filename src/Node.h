#ifndef NODE_H
#define NODE_H

template <typename T>
struct Node {
    T data;
    Node<T>* next;

    Node(T val) : data(val), next(nullptr) {}
};

#endif