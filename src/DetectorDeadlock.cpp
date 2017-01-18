/*
 * File:   DetectorDeadlock.cpp
 * Author: Fabio Moreira
 *
 * Created on September 25, 2015, 2:55 PM
 */

#include "DetectorDeadlock.h"

DetectorDeadlock::DetectorDeadlock(list<int>* adj, vector<NoGrafo*> listaNoGrafo,int ultimoProcesso) {
	this->adj = adj;
	this->listaNoGrafo = listaNoGrafo;
	this->ultimoProcesso = ultimoProcesso;
}

DetectorDeadlock::~DetectorDeadlock() {}

void DetectorDeadlock::detectar(int vertice) {
	//Passo 2 - Verificar se o nó atual aparece duas vezes em aux (há ciclo)
	aux.push_back(vertice); 					
	int count = 0;							
	for (int i = 0; i < aux.size(); ++i)	 
		if (aux[i] == vertice) count++;		
	if (count == 2) {
		printf("	Ocorreu Deadlock entre: -");
		for (int i = 0; i < aux.size(); ++i)
		{
			int val = aux[i];
			if (val < ultimoProcesso)
				printf("Processo %d - ",val );
			else
				printf("Recurso %d - ",val-ultimoProcesso);
		}
		printf("\n");
		throw 20;
	}

	if (!adj[vertice].empty()) { // Quando nós são folhas
		// Passo 3 - verificar se há nós desmarcados para usá-los
		list<int>::iterator outputIt;
		for (outputIt = adj[vertice].begin(); outputIt != adj[vertice].end(); ++outputIt) // se esquecer sobre isso, veja arquivo teste.cpp
		{
			int tmp = *outputIt;
			if (!listaNoGrafo[tmp]->getVisitado()) {
				// Passo 4
				//int arcoDesmarcado = tmp;
				listaNoGrafo[tmp]->setVisitado(true);
				detectar(tmp); // Volto para o passo 2
			}
		}
	}

	// Passo 5
	aux.pop_back();
}