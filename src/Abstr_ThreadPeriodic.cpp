#include "Abstr_ThreadPeriodic.h"
#include "Simul_Debug.h"
#include "OperatingSystem.h"
#include "Simulator.h"
#include "Mediator_CPU.h"

ThreadPeriodic::ThreadPeriodic(Process* task, int period, double cpuBurst, ThreadPeriodic::State state) {
    _id = ThreadPeriodic::getNewId();
    _process = task;
    _state = state;
    // INSERT YOUR CODE HERE
    // ...
    _accountInfo._cpuBurst = cpuBurst;
    _accountInfo._priority = period;
    _accountInfo._period = _accountInfo._priority;
    _accountInfo._cpuBurstTime = 0;
    _accountInfo._cpuTime = 0;
    _accountInfo._waitingTime = 0;
    _accountInfo._arrivalTime = Simulator::getInstance()->getTnow();
    _accountInfo._wakeupTime = 0;
    _accountInfo._dispatchedTime = _accountInfo._arrivalTime;
    _accountInfo._deadLineMiss = 0;
    _queue = new Queue<ThreadPeriodic>();
}


ThreadPeriodic::~ThreadPeriodic() = default;

int ThreadPeriodic::getDeadLineTime() {
    return _accountInfo._deadLineTime;
}

void ThreadPeriodic::sleep(Queue<ThreadPeriodic> * q) {
    Debug::cout(Debug::Level::trace, "Thread::sleep(" + std::to_string(reinterpret_cast<unsigned long> (q)) + ")");
    // INSERT YOUR CODE HERE

    auto running_thread = ThreadPeriodic::running();
    // calcula quanto tempo ficou executando
    auto executing_time = (Simulator::getInstance()->getTnow() - running_thread->getAccount()._cpuAux);
    running_thread->setCPUTime(running_thread->getAccount()._cpuTime + executing_time);
    // anota o horário que ela ficou bloqueada
    running_thread->setBlockedAux(Simulator::getInstance()->getTnow());
    // vai para o estado de espera (bloqueada)
    running_thread->setState(WAITING);
    // põe na fila de bloqueadas
    q->insert((ThreadPeriodic*)running_thread);
    // chama o escalonador
    auto scheduler = OperatingSystem::Process_Scheduler();
    // escolhe uma nova thread
    auto next_thread = scheduler->choose();
    dispatch(running_thread, next_thread);
}

void ThreadPeriodic::wakeup(Queue<ThreadPeriodic> * q) {
    Debug::cout(Debug::Level::trace, "Thread::wakeup(" + std::to_string(reinterpret_cast<unsigned long> (q)) + ")");
    // INSERT YOUR CODE HERE
    // ...
    // verifica quem é a primeira thread na lista de bloqueadas
    auto unlocked_thread = q->top();
    // tira da fila de bloqueadas
    q->pop();
    // põe no estaddo de pronta
    unlocked_thread->_state = READY;
    // calcula quanto tempo ficou bloqueada
    unlocked_thread->_accountInfo._blockedTime += (Simulator::getInstance()->getTnow() - unlocked_thread->_accountInfo._blockedAux);
    // Anota o horário que ela foi pro estado de pronto
    unlocked_thread->_accountInfo._waitAux = Simulator::getInstance()->getTnow();
    // chamar o escalonador
    auto scheduler = OperatingSystem::Process_Scheduler();
    // coloca ela na fila de prontos
    scheduler->insert(unlocked_thread);
    // escolhe uma nova thread
    auto new_running = scheduler->choose();
    dispatch(_running, new_running);
}


int ThreadPeriodic::getDLM() {
    return _accountInfo._deadLineMiss;
}

void ThreadPeriodic::incrementDLM() {
    _accountInfo._deadLineMiss++;
}

double ThreadPeriodic::getDispatchedTime() {
    return _accountInfo._dispatchedTime;
}

double ThreadPeriodic::getWaitingTime() {
    return _accountInfo._waitingTime;
}

double ThreadPeriodic::getCPUTime() {
    return _accountInfo._cpuTime;
}

double ThreadPeriodic::getArrivalTime() {
    return _accountInfo._arrivalTime;
}

double ThreadPeriodic::getWakeUpTime() const {
    return _accountInfo._wakeupTime;
}

void ThreadPeriodic::setWakeUpTime(double newWakeUp) {
    _accountInfo._wakeupTime = newWakeUp;
}

void ThreadPeriodic::setPeriod() {
    _accountInfo._period += _accountInfo._priority;
}

double ThreadPeriodic::getCPUBurst() {
    return _accountInfo._cpuBurst;
}

double ThreadPeriodic::getBurstTime() {
    return _accountInfo._cpuBurstTime;
}

bool ThreadPeriodic::isBlocked() {
    if (ThreadPeriodic::running() != nullptr)
        return (ThreadPeriodic::running()->getState() == ThreadPeriodic::State::WAITING || ThreadPeriodic::running()->getState() == ThreadPeriodic::State::FINISHING);
    else
        return true;
}

void ThreadPeriodic::setBurstTime(double time) {
    _accountInfo._cpuBurstTime = time;
}

int ThreadPeriodic::getPeriod() {
    return _accountInfo._period;
}

void ThreadPeriodic::setCPUTime() {
    _accountInfo._cpuTime += _accountInfo._cpuBurst;
    _accountInfo._cpuBurstTime = 0;
}
