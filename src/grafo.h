//! Classe Grafo
/*
 * File:   Grafo.h
 * Author: Fabio Moreira
 *
 * Created on September 24, 2016, 2:08 PM
 *
 * Grafo utilizado para a detecção de um deadlock.
*/

#ifndef GRAFO_H
#define	GRAFO_H

#include "NoGrafo.h"
#include <list>
#include <vector>

using namespace std;

class Grafo {
public:
    //! Construtor.
    /*!
    *
    * Inicialização e construção da lista de adjacência e lista de referências.
    *
    * \param listaprocessos como um vetor de ponteiros de Process.
    * \param recursosRequisitados como um vetor de inteiros.
    * \param qtdprocess como um inteiro.
    */
    Grafo(Process** listaprocessos, int* recursosRequisitados,int qtdprocessos);

    //! Destrutor.
    /*!
	* Destrutor da classe Grafo.
    */
    ~Grafo();

    //! Retorna lista de adjacência.
    /*!
    *
    */
    list<int>* getAdj();

    //! Retorna lista de referências de nós.
    /*!
    *
    */
    vector<NoGrafo*> getListaNoGrafo();

private:
	list<int> *adj; /*!< ponteiro para um array contendo as listas de adjacência. */
	vector<NoGrafo*> listaNoGrafo; /*!< ponteiro para um array de NoGrafo, quando precisar marcar como visitado utiliza essa lista. */
};

#endif	/* GRAFO_H */