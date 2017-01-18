/*
 * File:   Thread.h
 * Author: cancian
 *
 * Created on September 27, 2015, 10:30 AM
 */

#ifndef THREAD_H
#define	THREAD_H

#include <list>
#include <stdlib.h>
#include "Util_Queue.h"
#include "Abstr_Process.h"
#include "Mediator_Timer.h"
//#include "ModuleInvoke_ThreadExec.h"
//

#define DEAD_LINE rand()%100

class Thread {
    //friend class ModuleInvoke_ThreadExec; // simulator use only
    friend class Timer;
    friend class Process;
    friend class ProblemTester; // for simulation purposes only

    enum State {
        RUNNING,
        READY,
        WAITING,
        FINISHING
    };

    enum Criterion {
        HIGH,
        NORMAL,
        LOW,
        MAIN,
        IDLE
    };

    /**
     *  struct for grouping threads' attributes
     **/
    struct AccountInformation {
        // do not change
        int _priority; // this attribute MUST BE the one used to sort the Schelung_Queue, no matter the scheduler algorithm
        double _arrivalTime;
        double _cpuBurst;
        double _cpuBurstTime;
        double _cpuTime;
        double _waitingTime;
        double _blockedTime;
        double _cpuAux;
        double _blockedAux;
        double _waitAux;
        // add here another information you need
        // INSERT YOUR CODE HERE
        int _period;
        double _wakeupTime;
        double _dispatchedTime;
        double _deadLineTime;
        int _deadLineMiss;
    };

public: // constructors
    Thread(Process* task, Thread::State state = Thread::State::READY);
    Thread(Process* task, int period, double cpuBurst, Thread::State state = Thread::State::READY);
    Thread(const Thread& orig);
    virtual ~Thread();
public: // statics
    static void sleep(Queue<Thread> * q);
    static void wakeup(Queue<Thread> * q);
    static Thread* thread_create(Process* parent);

    static std::list<Thread*>* getThreadsList() {
        static std::list<Thread*>* _threadsList;
        if (_threadsList == nullptr) {
            _threadsList = new std::list<Thread*>();
        }
        return _threadsList;
    }
protected: // statics
    static Thread* _running;
    static void yield();
    static void exit(int status = 0);
    static Thread* running();
    static void dispatch(Thread* previous, Thread* next);

    static unsigned int getNewId() {
        static unsigned int _lastId = 0;
        return ++_lastId;
    }
public: //
    int join();
    Process* getProcess();
    int getPriority() const;
    int getDeadLineTime();
    int getDLM();
    void incrementDLM();
    double getDispatchedTime();
    double getWaitingTime();
    double getCPUTime();
    double getArrivalTime();
    double getWakeUpTime() const;
    void setWakeUpTime(double newWakeUp);
    void setPeriod();
    double getCPUBurst();
    double getBurstTime();
    unsigned int getId();
    bool isBlocked();
    void setBurstTime(double time);
    int getPeriod();
    void setCPUTime();
protected:
    Process * _process;
    State _state;
    unsigned int _id;
    AccountInformation _accountInfo;


private: // USER DEFINED
    // INSERT YOUR CODE HERE
    // ...
    Queue<Thread>* _queue;
};

#endif	/* THREAD_H */

