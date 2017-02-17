#ifndef THREADPERIODIC_H
#define THREADPERIODIC_H

#include "Abstr_Thread.h"
#include <list>
#include <stdlib.h>
#include "Util_Queue.h"
#include "Abstr_Process.h"
#include "Mediator_Timer.h"

class ThreadPeriodic : public Thread
{


public:
	ThreadPeriodic(Process* task, int period, double cpuBurst, ThreadPeriodic::State state = ThreadPeriodic::State::READY);
	ThreadPeriodic();
	~ThreadPeriodic();
public: // statics
    static void sleep(Queue<ThreadPeriodic> * q);
    static void wakeup(Queue<ThreadPeriodic> * q);
    static ThreadPeriodic* thread_create(Process* parent);

    static std::list<ThreadPeriodic*>* getThreadsList() {
        static std::list<ThreadPeriodic*>* _threadsList;
        if (_threadsList == nullptr) {
            _threadsList = new std::list<ThreadPeriodic*>();
        }
        return _threadsList;
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
private: // USER DEFINED
    // INSERT YOUR CODE HERE
    // ...
    Queue<ThreadPeriodic>* _queue;
};

#endif
