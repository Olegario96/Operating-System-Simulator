/*
 * File:   TimerMediator.h
 * Author: cancian
 *
 * Created on October 9, 2015, 2:55 PM
 */

#ifndef TIMERMEDIATOR_H
#define	TIMERMEDIATOR_H

#include "GerenciadorRecurso.h"
#include "grafo.h"
#include "DetectorDeadlock.h"

//#include "ModuleInvoke_InterruptHandler.h"

class Timer {
    friend class ModuleInvoke_HardwareEvent;
    friend class ProblemTester;
public:
    Timer(unsigned int instance);

    Timer(const Timer& orig);

    virtual ~Timer();

public:

private:
    unsigned int _instance;
    // CODIGO FABIO MOREIRA
    static int count;
    static Process* processos[10];
    static int ultimoProcesso; // ultimo processo indica o espaço necessário para deixar para processos no array de grafos.
    static Gr* gerenciarRecurso;
    static Grafo* grafo;
    static DetectorDeadlock* dd;
private:
    static void interrupt_handler();
};

#endif	/* TIMERMEDIATOR_H */