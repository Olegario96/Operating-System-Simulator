/*
 * File:   TimerMediator.cpp
 * Author: cancian
 *
 * Created on October 9, 2015, 2:55 PM
 */

#include "Mediator_Timer.h"
#include "Simulator.h"
#include "Simul_Debug.h"
#include "OperatingSystem.h"
#include "Traits.h"
#include "Abstr_Thread.h"
#include <time.h> // Adicionado
#include <stdlib.h> //Adicionado


Process* Timer::processos[];
int Timer::ultimoProcesso;
Gr* Timer::gerenciarRecurso;
Grafo* Timer::grafo;
DetectorDeadlock* Timer::dd;
int Timer::count;


Timer::Timer(unsigned int instance) {
    _instance = instance;
    count = 0;
    srand (time(NULL));
}

Timer::Timer(const Timer& orig) {
}

Timer::~Timer() {
}

/**
 * O sistema simulado possui um timer que gera interrupções periodicamente. O timer foi inicialmente configurado
 * para gerar interrupções a cada Traits<Timer>::timer_interrupt_period unidades de tempo. 
 * Sempre que uma interrupção do timer ocorre, automaticamente é
 * invocado o método static void TimerMediator::interrupt_handler(), que precisa ser implementado. Esse método trata uma
 * interrupção do timer, e sua implementação depende do algoritmo de escalonamento. Ele pode ser usado para preemptar
 * uma thread caso seu time slice tenha sido atingido, no caso do algoritmo round-robin, por exemplo.
 */
void Timer::interrupt_handler() {
    Debug::cout(Debug::Level::trace, "Timer::interrupt_handler()");
        // INSERT YOUR CODE HERE
	    // ...
        count++;
        bool deadlock = true;
        

        /* Etapa 1 - Criacao de processos e do Gerenciador de Recursos */
        
        ultimoProcesso = 10; // Max 20 processos. Para permitir mais aumentar array de processos no Mediator_Timer.h
        for (int i = 0; i < ultimoProcesso; ++i)
            processos[i] = new Process(i);
        gerenciarRecurso = new Gr();
        
        /* ------------------------------------------------------------ */
        
        /* Etapa 2 - alocacao/desalocao de recursos */

        // Caso aleatoriamente não ocorra Deadlocks, esse exemplo garante a ocorrência no terceiro "tick".
        if (count == 3) {
            ultimoProcesso = 3;
        	processos[0]->alocarRecurso(4,gerenciarRecurso->alocacaoRecursos(4,0));
            processos[0]->alocarRecurso(6,gerenciarRecurso->alocacaoRecursos(6,0));
        	processos[0]->alocarRecurso(9,gerenciarRecurso->alocacaoRecursos(9,0));
        	processos[1]->alocarRecurso(1,gerenciarRecurso->alocacaoRecursos(1,1));
        	processos[1]->alocarRecurso(0,gerenciarRecurso->alocacaoRecursos(0,1));
        	processos[2]->alocarRecurso(7,gerenciarRecurso->alocacaoRecursos(7,2));
            processos[1]->alocarRecurso(7,gerenciarRecurso->alocacaoRecursos(7,1));
        	processos[2]->alocarRecurso(1,gerenciarRecurso->alocacaoRecursos(1,2));
        } else {
            int acao = 0;
            int recurso = 0;
            // Gera alocações/desalocações, permite 5 ações por processo.
            for (int i = 0; i < 5; ++i)
                for (int i = 0; i < ultimoProcesso; ++i) // para cada processo, uma ação é permitida.
                {
                    acao = processos[i]->acao();
                    switch(acao) {
                        case 0: // processo nao deseja alocar nem desalocar recursos
                            break;
                        case 1: // processo deseja alocar recursos
                            while (true){
                                recurso = rand() % 10;
                                if (processos[i]->getRecursos()[recurso] != recurso) { // Não irá alocar se já possui o processo.
                                    processos[i]->alocarRecurso(recurso,gerenciarRecurso->alocacaoRecursos(recurso,i));
                                    break;
                                }
                            }
                            break;
                        case 2: // desalocao de recursos e verificacao de processos na fila de requisicao do recurso
                            recurso = processos[i]->desalocarRecurso();
                            int tmp = gerenciarRecurso->desalocaoRecursos(recurso);
                            if (tmp != -1)
                                processos[tmp]->alocarRecurso(recurso,true);
                            break;
                    }
                }
        }
        /* --------------------------------------------------------------------------------------------------------------- */

        /* Etapa 3 - Criação do Grafo e Deteccao de Deadlock */
        grafo = new Grafo(processos,gerenciarRecurso->getProcessosBloqueados(),ultimoProcesso);
        list<int> *adj = grafo->getAdj();
        vector<NoGrafo*> listaNoGrafo = grafo->getListaNoGrafo();
        dd = new DetectorDeadlock(adj,listaNoGrafo,ultimoProcesso);
        
        // Exibe ligação de processos - recursos, recursos - processos
        list<int>::iterator outputIt;
        for (int i = 0; i < ultimoProcesso+10; ++i)
        {
            if (i < ultimoProcesso) {
                cout << "Processo " << i << " possui os recursos: ";
                for (outputIt = adj[i].begin(); outputIt != adj[i].end(); ++outputIt)
                {
                    cout << *outputIt - ultimoProcesso << " ";
                }
                cout << endl;
            } else {
                cout << "Recurso " << i-ultimoProcesso << " desejado por: ";
                for (outputIt = adj[i].begin(); outputIt != adj[i].end(); ++outputIt)
                {
                    cout << *outputIt << " ";
                }
                cout << endl;
            }
        }

        // Checa se ocorreu deadlock em relacao a todos os processos, processos já visitados não precisam ser revisitados.
        for (int i = 0; i < ultimoProcesso; ++i)
            if (!listaNoGrafo[i]->getVisitado())
                try {
                    dd->detectar(i);   
                } catch (int e) {
                    deadlock = false;
                    break;
                }

        if (deadlock)
            printf("        Não Ocorreu Deadlock\n"); // Se ele chegou aqui não ocorreu deadlock
        /* -------------------------------------------------- */

        /* Etapa 4 - Finalização de processos e demais objetos */
        for (int i = 0; i < ultimoProcesso; ++i)
            delete processos[i];
        delete dd;
        delete grafo;
        for (int i = 0; i < listaNoGrafo.size(); ++i)
            if (listaNoGrafo[i] != NULL)
                delete listaNoGrafo[i];
        /* --------------------------------------------------- */
}