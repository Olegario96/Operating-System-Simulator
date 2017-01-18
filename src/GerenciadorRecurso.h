//! Classe GerenciadorRecurso
/*
 * File:   GerenciadorRecurso.h
 * Author: Fabio Moreira
 *
 * Created on September 24, 2016, 2:08 PM
 *
 * Gerencia os recursos que podem ser requisitados por processos.
 * Permite aos processos alocarem e desalocarem recursos, processos
 * que pedem determinado recurso indisponível são bloqueados e não
 * podem requisitar novos recursos até obter aquele que o bloqueou.
 */

#ifndef GERENCIADORRECURSO_H
#define	GERENCIADORRECURSO_H

class Gr {
public:
    //! Construtor
    /*!
    * Inicializa os recursos disponíves e a lista de processos bloqueados.
    */
    Gr();

    //! Destrutor
    /*!
    * Destrutor.
    */
    /*~Gr();*/

    //! Gerencia a alocação de recursos para processos.
    /*!
    * \param recurso um inteiro.
    * \param iDprocesso um inteiro.
    * \return Alocação foi realizada ou não.
    */
    bool alocacaoRecursos(int recurso, int iDprocesso);

    //! Gerencia a desalocação de recursos para processos.
    /*!
    * \param recurso um inteiro.
    * \return Desalocação foi realizada com ou sem necessidade de alocar para outro processo ou gerou um erro.
    */
    int desalocaoRecursos(int recurso);

    //! Retorna uma lista de recursos existentes.
    /*!
    * \return Lista de recursos.
    */
    int* getRecursos();
    
    //! Retorna uma lista de processos bloqueados.
    /*!
    * \return Lista de processos bloqueados.
    */
    int* getProcessosBloqueados();

private:
    int qtdRecursos; /*!< Quantidade de recursos disponíveis.*/
    int recursos[10];	/*!< Recursos disponiveis para serem alocados pelos processos. 1 = disponivel, 0 = indisponivel*/
    int processosBloqueados[10]; //!< É necessário manter uma lista de processos para que o Gr desbloqueie o processo após alocar um recurso que ele estava esperando.
    							 //!< Indice - processo bloqueado, Valor - numero do recurso
};

#endif	/* GERENCIADORRECURSO_H */