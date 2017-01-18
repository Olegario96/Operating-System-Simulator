/*
 * File:   Queue.h
 * Author: cancian
 *
 * Created on September 27, 2015, 11:28 AM
 */

#ifndef QUEUE_H
#define	QUEUE_H

#include <list>

/**
 * Used by Thread as waiting queues for threads that uses void Thread::sleep(Queue* e) and void Thread::wakeup(Queue q)
 **/

// INSERT YOUR CODE HERE ; CHANGE THE QUEUE THE WAY YOU WANT

template <class T>
class Queue {
public: // constructors
    Queue(){
    	_queue = new std::list<T*>();
    }
    Queue(const Queue& orig) {
    }
    virtual ~Queue(){
    }
public: // user defined
    bool empty() {
    	return _queue->empty();
    }

    void insert(T* t) {
    	_queue->insert(_queue->end(), t);
    }

    void remove(T* t) {
        _queue->remove(t);
    }

    void pop() {
    	if (!_queue->empty()) {
            _queue->erase(_queue->begin());
        }
    }

    void order() {
    	_queue->sort([] (const T* a, const T * b) {
            return a->getWakeUpTime() <= b->getWakeUpTime();
        });
    }

    T* top() {
    	return (*_queue->begin());
    }

    T* end() {
        return (_queue->back());
    }

private:
    std::list<T*>* _queue;
};

#endif	/* QUEUE_H */

