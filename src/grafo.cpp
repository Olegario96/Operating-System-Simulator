/*
 * File:   Grafo.cpp
 * Author: Fabio Moreira
 *
 * Created on September 25, 2016, 2:08 PM
 */

#include "grafo.h"

Grafo::Grafo(Process** listaprocessos, int* recursosRequisitados, int qtdprocessos) {
	adj = new list<int>[qtdprocessos+10];
	
	// inicializa lista NoGrafo alterando seu tamanho para qtdprocessos+10
	listaNoGrafo.resize(qtdprocessos+10);
	for (int i = 0; i < qtdprocessos; ++i)
	{
		if (listaprocessos[i] != NULL) {
			NoGrafo* v = new NoGrafo(listaprocessos[i]); // cria nó de um processo i
			listaNoGrafo.at(i) = v;
			for (int j = 0; j < 10; ++j)
			{
				int recurso = listaprocessos[i]->getRecursos()[j]; // pego um recurso que v possui
				if (recurso != -1) {
					int aux = recurso+qtdprocessos;
					NoGrafo* v1 = new NoGrafo(aux); // crio nó para o recurso
					listaNoGrafo.at(aux) = v1;
					adj[i].push_back(aux); // conecta um processo e os recursos que ele possui.	
				}
			}
		}
	}
	// conecta recursos aos processos que o desejam
	for (int i = 0; i < 10; ++i)
	{
		int aux = recursosRequisitados[i];
		if (aux != -1) { 							// se -1 aquela posição não está sendo usada
			adj[aux+qtdprocessos].push_back(i);
		}
	}
}

Grafo::~Grafo() {}

list<int>* Grafo::getAdj(){
	return adj;
}

vector<NoGrafo*> Grafo::getListaNoGrafo() {
	return listaNoGrafo;
}