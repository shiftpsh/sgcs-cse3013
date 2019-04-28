#ifndef __STACK__
#define __STACK__

#include "LinkedList.h"
#include <iostream>

using namespace std;

/*
    ===========
    class Stack
    ===========
*/
template <class T> class Stack : public LinkedList<T> {
  public:
    bool Delete(T &element);
};

// Deletes *first* element
template <class T> bool Stack<T>::Delete(T &element) {
    // If current size is 0, deletion fails
    if (LinkedList<T>::current_size == 0) return false;

    Node<T>* temp = LinkedList<T>::first;
    LinkedList<T>::first = LinkedList<T>::first->link;
    element = temp->data;
    delete temp;
    LinkedList<T>::current_size--;

    return true;
}

#endif