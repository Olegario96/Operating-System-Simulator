/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileSystem.h
 * Author: cancian
 *
 * Created on 29 de Agosto de 2016, 14:55
 */

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "Abstr_FileAllocationTable.h"
#include "Abstr_FileAllocator.h"
#include "Mediator_HardDisk.h"

/**
 * Implementa o Sistema de Arquivos em si
 */
class FileSystem {
    friend class ProblemTester;
public:
    FileSystem();
    FileSystem(const FileSystem& orig);
    virtual ~FileSystem();
public:
    // INSERT YOUR CODE HERE
    // ...
    
    /**
     * Chama o proce
     * @param path caminho da pasta
     */
    void criarDiretorio(const unsigned char* path);
    /**
     * Implementa o processo de criacao de um arquivo
     * @param path do arquivo a ser criado 
     */
    void criarArquivo(const unsigned char* path);
    /**
     * Implementa o processo de delecao de um arquivo
     * @param path do arquivo a ser removido 
     */
    void removerArquivo(const unsigned char* path);
    /**
     * Implementa o processo a ser executado para abrir um arquivo
     * @param path do arquivo a ser aberto
     */
    FileAllocationEntry::fileIdentifier abrirArquivo(const unsigned char* path);
    /**
     * Implementa o fechamento de um arquivo
     * @param file arquivo a ser fechado 
     */
    void fecharArquivo(FileAllocationEntry::fileIdentifier file);
    /**
     * Implementa o processo de escrever um dado num arquivo
     * @param inode o arquivo a ser escrito 
     * @param posicao Posicao no arquivo a receber o conteudo a ser escrito
     * @param dados ponteiro para os dados a serem escritos
     * @param tamanho Tamanho do "vetor" de char a ser escritos
     * @return inteiro 0 caso haja sucesso e 1 caso fracasse (arquivo nao esta aberto)
     */
    unsigned int escreverEmArquivo(FileAllocationEntry::fileIdentifier inode, unsigned int posicao, unsigned char* dados, unsigned int tamanho);
    /**
     * Implementa o processo de ler um dado num arquivo
     * @param inode o arquivo a ser lido
     * @param posicao Posicao no arquivo na qual os dados comecam a ser lidos
     * @param dados ponteiro para o buffer que recebe o valor lido
     * @param tamanho Tamanho do "vetor" de char a ser lido
     * @return inteiro 0 caso haja sucesso e 1 caso fracasse (arquivo nao esta aberto)
     */
    unsigned int leDoArquivo(FileAllocationEntry::fileIdentifier inode, unsigned int posicao, unsigned char* dados, unsigned int tamanho);
    /**
     * Getter
     * @return inteiro com o numero de arquivos criados
     */
    int getNumArquivosCriados();
    /**
     * Getter
     * @return inteiro com o numero de arquivos deletados
     */
    int getNumArquivosDeletados();
    /**
     * Getter
     * @return float com a porcentagem das requisicoes que foram atendidas
     */
    float getPorcentagemRequisicoesAtendidas();
    /**
     * Getter
     * @return inteiro com o espaco ainda livre no HD
     */
    int getEspacoLivreHD();
    /**
     * Getter
     * @return inteiro com o espaco total do HD
     */
    int getEspacoTotalHD();
    /**
     * Getter
     * @return inteiro com o espaco livre dentro de blocos ocupados
     */
    int getEspacoFragmentado();
    /**
     * Getter
     * @return float com a Media do tamanho dos arquivos
     */
    float getTamanhoMedioArquivos();
    /**
     * Getter
     * @return inteiro com o espaco ocupado pelos arquivos
     */
    int getEspacoOcupado();
    /**
     * Getter
     * @return inteiro com o numero de arquivos abertos
     */
    int getNumArquivosAbertos();
    
    void teste();
private:
	FileAllocationTable* fat;
	HardDisk* disk;
	FileAllocator* allocator;
	int testado;
};

#endif /* FILESYSTEM_H */

