//! Classe NoGrafo
/*
 * File:   NoGrafo.h
 * Author: Fabio Moreira
 *
 * Created on September 24, 2016, 2:08 PM
 *
 * Nó de um grafo cujo vértices são processos ou recursos.
*/

#ifndef NOGRAFO_H
#define	NOGRAFO_H

#include "Abstr_Process.h"

class NoGrafo {
public:
    //! Construtor.
    /*!
    * \param processo como um ponteiro de Process.
    */
    NoGrafo(Process* processo) {
    	this->processo = processo;
        recurso = -1;
    	visitado = false;
    }

    //! Construtor.
    /*!
    * \param recurso como um inteiro.
    */
    NoGrafo(int recurso) {
        processo = NULL;
    	this->recurso = recurso;
    	visitado = false;
    }

    //! Destrutor.
    /*!
    * Destrutor da classe NoGrafo.
    */
    ~NoGrafo() {};

    //! Retorna a var visitado, útil para saber se o nó já foi visitado.
    /*!
    * 
    */    
    bool getVisitado() {
    	return visitado;
    }

    //! Altera o valor da var visitado.
    /*!
    * \param valor como um boolean
    */
    void setVisitado(bool valor) {
    	visitado = valor;
    }

    //! Retorna a informação do vértice.
    /*!
    * 
    */
    Process* getProcesso() {
        return processo;
    }

    //! Retorna a informação do vértice.
    /*!
    * 
    */
    int getRecurso() {
        return recurso;
    }
private:
	bool visitado; /*!< Indica se o nó já foi visitado. */
	Process* processo; /*!< O nó é um processo. */
	int recurso; /*!< O nó é um recurso. */
};

#endif	/* NOGRAFO_H */