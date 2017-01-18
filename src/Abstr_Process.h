/*
 * File:   Process.h
 * Author: cancian
 *
 * Created on August 31, 2015, 7:49 AM
 */

#ifndef PROCESS_H
#define	PROCESS_H

#include <list>
#include <queue>

#include "Abstr_MemoryManager.h"
#include "Abstr_MemoryChunk.h"
#include "Simul_Entity.h"
//#include "ModuleInvoke_ProcessExec.h"

class Process {
    friend class Thread;
    //friend class ModuleInvoke_ProcessExec; // for simulation purposes only
    friend class ProblemTester; // for simulation purposes only
    friend class Simulator;

    // scructs for grouping process' attributes

    struct IdentificationInformation {
        unsigned int _processId;
        unsigned int _parentId;
        int _priority;
        // ...
    };

    struct AccountInformation {
        double _CPUTime;
        double _waitingTime;
        double _dispatchedTime;
        double _blockedTime;
        // ...
    };

    struct ChunkTable {
        //MemoryChunk* _chunk;
        std::list<MemoryChunk*>* _chunks;
    };


public: // constrs
    Process(unsigned int parentId);
    Process(const Process& orig);
    virtual ~Process();
public: // id info
    unsigned int getParentId() const;
    unsigned int getId() const;
    Entity* getEntity() const;

    MemoryChunk * getChunk(){
      return _chunk;
    }

    void setChunk(MemoryChunk * chunk){
      this->_chunk =  chunk;
    }

public: // statics

    static unsigned int getNewId() {
        static unsigned int _lastId = 0;
        return ++_lastId;
    }

    static std::list<Process*>* getProcessesList() { // return the listr of all processes in the system
        static std::list<Process*>* _ProcessesList;
        if (_ProcessesList == nullptr) {
            _ProcessesList = new std::list<Process*>();
        }
        return _ProcessesList;
    }

    static MemoryManager* getMemoryManager(){
        static MemoryManager *memManager;
        if (memManager == nullptr){
            memManager = new MemoryManager();
        }
        return memManager;
    }

    static Process* exec(); // execute a new process
    static void exit(int status = 0); // finishes the running process
private:
    IdentificationInformation _idInfo;
    ChunkTable _memInfo;
    AccountInformation _cpuTime;
    MemoryChunk *_chunk;
public: // For simulation use only. Do not change

    Entity* getEntity() {
        return _entity;
    }

private: // for simulator use only. Do not change
    Entity* _entity; //used only by the simulator
public: // USER DEFINED
    // INSERT YOUR CODE HERE
    AccountInformation getCpuTime() {
        return _cpuTime;
    }

     //! Aloca recursos para esse processo.
    /*!
    * \param recurso um inteiro.
    * \param permissao um boolean.
    */
    void alocarRecurso(int recurso, bool permissao);

    //! Desaloca recursos desse processo.
    /*!
    */
    int desalocarRecurso();

    //! Retorna quantos recursos esse processo possui.
    /*!
    */
    int getQtdRecursos();

    //! Retorna a lista de recursos que esse processo possui.
    /*!
    */
    int* getRecursos();

    //! Escolhe aleatoriamente se o processo vai alocar/desalocar recursos ou não vai fazer nada.
    /*!
    */
    int acao();

    //! Gera o aleatório para acao e desalocar Recursos.
    /*!
    */
    int num_random(int op);

private: // USER DEFINED
    // INSERT YOUR CODE HERE
    int recurso[10]; /*!< Indica os recursos alocados por um processo. Valor - indica o número do recurso em posse. */
    bool bloqueado; /*!< Indica se o processo está bloqueado. Processos são bloqueados quando requisitam um recurso já alocado a outro processo.*/
    int qtdRecursos; /*!< Quantidade de recursos que um processo possui. */

private: // USER DEFINED
    // INSERT YOUR CODE HERE
};

#endif	/* PROCESS_H */
