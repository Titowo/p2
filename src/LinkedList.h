#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "Node.h"

template <typename T>
class LinkedList {
public:
    LinkedList() : head(nullptr), size(0) {}
    ~LinkedList();

    bool add(T value);
    bool contains(T value) const;
    Node<T>* getHead() const { return head; }
    int getSize() const { return size; }
    void clear();

private:
    Node<T>* head;
    int size;
};


#include "LinkedList.tpp"
#endif