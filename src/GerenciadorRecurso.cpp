//! Classe GerenciadorRecurso
/*
 * File:   GerenciadorRecurso.cpp
 * Author: Fabio Moreira
 *
 * Created on September 24, 2016, 2:08 PM
 *
 * Gerencia os recursos que podem ser requisitados por processos.
 * Permite aos processos alocarem e desalocarem recursos, processos
 * que pedem determinado recurso indisponível são bloqueados e não
 * podem requisitar novos recursos até obter aquele que o bloqueou.
 */
#include <iostream>
#include "GerenciadorRecurso.h"

using namespace std;

Gr::Gr() {
	qtdRecursos = 10;
	for (int i = 0; i < qtdRecursos; ++i) {
		recursos[i] = 1;
		processosBloqueados[i] = -1;
	}	
}

bool Gr::alocacaoRecursos(int recurso, int iDprocesso) {
	if (recurso > qtdRecursos || recurso < 0)
		throw 20;
	if (processosBloqueados[iDprocesso] == -1) { // processo bloqueado não pode alocar.
		if (recursos[recurso]) { //se o recurso estiver disponível, pode alocar
			recursos[recurso] = 0;
			return true;
		} else { // se não, é bloqueado e não pode mais fazer pedidos de alocação.
			processosBloqueados[iDprocesso] = recurso;
			return false;
		}
	} else {
		return false;
	}
}

int Gr::desalocaoRecursos(int recurso) {
	if (!recursos[recurso]) {
		recursos[recurso] = 1;
		for (int i = 0; i < qtdRecursos; ++i)
		{
			if (processosBloqueados[i] == recurso) {
				processosBloqueados[i] = -1;
				recursos[recurso] = 0;
				return i;
			}
		}
		return -1;
	} else {
		return -1;
	}
}

int* Gr::getRecursos() {
	return recursos;
}

int* Gr::getProcessosBloqueados() {
	return processosBloqueados;
}