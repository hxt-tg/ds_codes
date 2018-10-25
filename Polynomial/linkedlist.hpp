#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>
#include "err.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))


/**
 * Node part.
 */
template <class T>
class LinkedNode {
public:
    LinkedNode<T> *prev;
    T data;
    LinkedNode<T> *next;
};

/**
* List part.
*/
template <class T>
class LinkedList {
private:
    LinkedNode<T> _head;
    unsigned int length;
public:
    LinkedList() {
        _head.next = &_head;
        _head.prev = &_head;  /* Tail pointer */
        //head.data = nullptr;
        length = 0;
    }

    LinkedList(const LinkedList<T> &list) {
        _head.next = &_head;
        _head.prev = &_head;  /* Tail pointer */
                              //head.data = nullptr;
        length = 0;
        LinkedNode<T> *tp = list.head()->next;
        for (unsigned int i = 0; i < list.size(); i++, tp = tp->next)
            LinkedList<T>::insert(tp->data, i + 1);
    }

    ~LinkedList() {
        LinkedNode<T> *p = _head.next;
        for (unsigned int i = 0; i < length; i++) {
            p = p->next;
            free(p->prev);
        }
        _head.next = &_head;
        _head.prev = &_head;
        length = 0;
    }

    /* Insert item **before** pos. (If index >= length then append to the end.) */
    void insert(T &data, unsigned int index) {
        LinkedNode<T> *hp = &_head;
        for (int i = 0; i < (int)MIN(index, length); i++)
            hp = hp->next;
        LinkedNode<T> *e = new LinkedNode<T>();
        e->data = data;
        e->next = hp->next;
        hp->next->prev = e;
        hp->next = e;
        e->prev = hp;
        length++;
    }

    T &operator[](unsigned int index) {
        if ((int)index > (int)length - 1) err("Index out of range.");
        LinkedNode<T> *hp = _head.next;
        for (int i = 0; i < (int)index; i++)
            hp = hp->next;
        return hp->data;
    }

    LinkedList<T> &operator=(LinkedList<T> list) {
        LinkedList<T>::~LinkedList();
        LinkedNode<T> *tp = list.head()->next;
        for (unsigned int i = 0; i < list.size(); i++, tp = tp->next)
            LinkedList<T>::insert(tp->data, i + 1);
        return *this;
    }

    void remove(unsigned int index) {
        if ((int)index >(int)length - 1) err("Index out of range.");
        if (length == 0) err("Linked list is empty.");
        LinkedNode<T> *hp = _head.next;
        for (int i = 0; i < (int)index; i++)
            hp = hp->next;
        remove(hp);
    }

    void remove(LinkedNode<T> *hp) {
        hp->prev->next = hp->next;
        hp->next->prev = hp->prev;
        delete hp;
        length--;
    }

    unsigned int size() const { return length; }
    const LinkedNode<T> *head() const { return &_head; }
};

#endif
