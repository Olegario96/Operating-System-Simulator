/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileAllocator.h
 * Author: cancian
 *
 * Created on 9 de Setembro de 2016, 10:13
 */

#ifndef FILEALLOCATOR_H
#define FILEALLOCATOR_H

#include "Abstr_FileAllocationTable.h"


/**
 * Abstract class the represents an algorithm for file allocation. Possible implementations include contiguous, linked, indexed, ...
 * @param disk The disk that allocator allocates files in.
 * @param fat The file allocation table to register allocations
 */
class FileAllocator {
public:
    FileAllocator(HardDisk* disk, FileAllocationTable* fat);
    FileAllocator(const FileAllocator& orig);
public:
	/**
     * Cria pasta dentro do caminho path
     * @param path caminho para o arquivo
     */
    void createFolder(const unsigned char* path);
    /**
     * Cria arquivo e o adiciona na fat e nos registros de blocos livres
     * @param  path caminho para o arquivo
     */
    void createFile(const unsigned char* path);
    /**
     * Remove arquivo e remove entrada na fat e nos registros de blocos livres
     * @param  path caminho para o arquivo
     */
    void removeFile(const unsigned char* path);
    
    /**
     * Abre arquivo e monta a tabela baseada em inodes com os blocos do arquivo
     * @param  path caminho para o arquivo
     * @return inode do arquivo aberto
     */
    FileAllocationEntry::fileIdentifier openFile(const unsigned char* path);
    
    /**
     * Fecha arquivo e "destroi a tabela formada atraves do inode
     * @param  file inode do arquivo a ser fechado
     */
    void closeFile(const FileAllocationEntry::fileIdentifier file);
    
    /**
     * Le de um arquivo aberto
     * @param file inode do arquivo
     * @param numBytes numero de bytes a serem lidos
     * @param bufferBytes ponteiro para salvar o que for lido
     */
    unsigned int readFile(const FileAllocationEntry::fileIdentifier file, const unsigned int numBytes, unsigned char* bufferBytes);
    /**
     * Escreve em um arquivo aberto
     * @param file inode do arquivo
     * @param numBytes numero de bytes a serem escritos
     * @param bufferBytes ponteiro para os dados a serem escritos
     */
    unsigned int writeFile(const FileAllocationEntry::fileIdentifier file, const unsigned int numBytes, unsigned char* bufferBytes);
    
    /**
     * Setter
     * @param numByte posiciona o byte de inicio para leitura ou escrita
     */
    void seek(const unsigned long numByte);

	/**
     * Getter
     * @return inteiro com o numero de arquivos deletados
     */
	int getDeletados();
  	/**
     * Getter
     * @return inteiro com o numero de arquivos criados
     */
  	int getCriados();
  	/**
     * Getter
     * @return inteiro com o numero de requisicoes para criacao de arquivos
     */
	int getRequisicoes();
	/**
     * Getter
     * @return inteiro com o numero de arquivos abertos
     */
	int getNumAbertos();
	/**
     * Getter
     * @return inteiro com o espaco ainda livre no HD
     */
	int getEspacoLivreHD();
	/**
     * Getter
     * @return inteiro com o espaco total do HD
     */
	int getEspacoHD();
	/**
     * Getter
     * @return inteiro com o espaco total ocupado por arquivos
     */
	int getEspacoOcupadoPorArquivos();
private:
private:
    HardDisk* disk;
    FileAllocationTable* fat;
	int requisicoesDeCriacao, arquivosCriados, arquivosDeletados;
};

#endif /* FILEALLOCATOR_H */

