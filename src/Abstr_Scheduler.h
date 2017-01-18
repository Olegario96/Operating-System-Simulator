/*
 * File:   Scheduler.h
 * Author: cancian
 *
 * Created on September 27, 2015, 11:19 AM
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H



#include <queue>

#include "Util_Scheduling_Queue.h"
#include "Simul_Debug.h"
#include "Mediator_HardDisk.h"
#include <iostream>

template <typename T>
class Scheduler {
    friend class ProblemTester;
public:

    Scheduler() {
        _readyQueue = new Scheduling_Queue<T>();
        _choosen = nullptr;
    }

    Scheduler(const Scheduler& orig) {

    }

    virtual ~Scheduler() {

    }
private:
    Scheduling_Queue<T>* _readyQueue;
    T* _choosen;
public:
    static const bool preemptive = true;
public:

    bool getPreemptive() {
        return preemptive;
    }

    T* choose() { // choose a new schedulingItem to run
        Debug::cout(Debug::Level::trace, "Scheduler::choose()");
        // INSERT YOUR CODE HERE
        // ...
        if (_readyQueue->empty()) {
            return nullptr;
        } else {
            T* result = _readyQueue->top();
            _choosen = result;

            std::cout << "\t\t" << result << "\n";//TODO remover output?
            //_readyQueue->pop();
            return result;
        }
    }

    T* choosen() {
        return _choosen;
    }

    /**
	 * Atualiza as prioridades dos elementos na lista e reordena a lista.
	 */
    void reschedule() {
        Debug::cout(Debug::Level::trace, "Scheduler::reschedule()");
        if (!this->_readyQueue->empty()) {
            // INSERT YOUR CODE HERE
            // ...

        	_readyQueue->reschedule();
        }
    }

    void insert(T * schedulingItem) {//TODO remover trace e output?
    	Debug::cout(Debug::Level::trace, "Scheduler::insert()");
    	std::cout << "\t\t" << schedulingItem << "\n";
        this->_readyQueue->insert(schedulingItem);
    }

    T * remove(T * schedulingItem) {
        // remove a specific schedulingItem from the readyQueue
        this->_readyQueue->remove(schedulingItem);
    }

};


#endif /* SCHEDULER_H */

