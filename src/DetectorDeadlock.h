 //! Classe DetectorDeadlock
/*
 * File:   DetectorDeadlock.h
 * Author: Fabio Moreira
 *
 * Created on September 25, 2016, 2:08 PM
 *
 * Detecta a existência de um deadlock manipulando a lista de adjacências criada pela classe Grafo.
 * Ao detectar deadlock, ela retorna ao usuário o ciclo formado para futura correção.
 */

#ifndef DETECTORDEADLOCK_H
#define	DETECTORDEADLOCK_H

#include "grafo.h"
#include <list>
#include <vector>

using namespace std;

class DetectorDeadlock {
public:
    //! Construtor
    /*!
    * Recebe as informações necessarias para detectar um deadlock.
    *
    * \param adj um ponteiro de uma lista de inteiros. 
    * \param listaNoGrafo um vetor de ponteiros NoGrafo.
    * \param ultimoProcesso um inteiro.
    */
    DetectorDeadlock(list<int>* adj, vector<NoGrafo*> listaNoGrafo,int ultimoProcesso);
    
    //! Destrutor
    /*!
    * Destrutor da classe DetectorDeadlock.
    */
    ~DetectorDeadlock();
    
    //! Detecta a existência de Deadlock.
    /*!
    * 
    * Baseado em um algoritmo de deteccao de deadlock que possui os seguintes passos:
    * Passo 1 - Inicializar uma lista vazia (aux) e assinale os arcos como desmarcados.
    * Passo 2 - Insira o no atual no final da lista (aux) e analise se o no aparece duas vezes.
    *				Se sim: ha um ciclo (finaliza-se a execucao do algoritmo)
    *				Se nao: ainda não foi detectado ciclo (siga para o passo 3)
    * Passo 3 - No nó corrente, verificar se ha um arco de saida desmarcado.
    *				Se sim: siga para o passo 4
    *				Se nao: siga para o passo 5
    * Passo 4 - Escolha uma das saidas desmarcadas
    *				Marque-a, siga-o para obter o novo nó, e vá para o passo 2
    * Passo 5 - Se o nó é o inicial, então não há ciclo e o algoritmo pode ser encerrado
    *			Se o nó não é o inicial, então o final foi alcançado.
    *				Remova-o da lista; volte ao nó anterior; marque-o como o nó atual e volta ao passo 2
    *
    * O modo de ler uma ocorrência de deadlock é:
    *
    * Processo inicial possui recurso y -> Recurso y é requisitado pelo processo x -> Processo x possui o recurso z -> Processo inicial deseja o recurso z.
    *
    * \param vertice 
    */
    void detectar(int vertice);
private:
	list<int> *adj; /*!< Ponteiro para um array contendo as listas de adjacência. */
	vector<NoGrafo*> listaNoGrafo; /*!< Ponteiro para um array de NoGrafo.*/
	vector<int> aux; /*!< Array utilizado no passo 1 do detector de deadlock. */
	int ultimoProcesso; /*!< Indica o maior id de um processo */
};

#endif	/* DetectorDeadlock_H */