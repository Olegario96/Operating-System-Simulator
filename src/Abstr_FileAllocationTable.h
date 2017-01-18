/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileAllocationTable.h
 * Author: cancian
 *
 * Created on 9 de Setembro de 2016, 10:14
 */

#ifndef FILEALLOCATIONTABLE_H
#define FILEALLOCATIONTABLE_H

#include "Mediator_HardDisk.h"


/**
 * Base class for file attributes. Includes only the name, size and type of a file.
 */
class FileAttributes {
public:
    FileAttributes();
public:
    /**
	 * enum que determina os tipos de arquivo possiveis
	 */
    enum fileType {a,d,c,b,l};
    
    /**
	 * Getter
	 * @return ponteiro para o char que identifica o nome do arquivo
	 */
    char* getFilename();
    /**
	 * Setter
	 * @param filename novo ponteiro para o nome
	 */
    void setFilename(char* filename);
    /**
	 * Getter
	 * @return tamanho do arquivo em bytes
	 */
    unsigned long getSize();
    /**
	 * Setter
	 * @param size Tamanho a ser atribuido ao arquivo em questao
	 */
    void setSize(unsigned long size);
    /**
	 * Getter
	 * @return retorna o tipo do arquivo em questao conforme o enum presente nesta mesma classe
	 */
    fileType getType();
    /**
	 * Setter
	 * @param type Novo tipo de arquivo
	 */
    void setType(fileType type);
private:
    char* filename;
    unsigned long size;
    fileType type;
};


/**
 * An entry in a FAT (@class {FileAllocationTable} ), representing a file (regular, directory or device) in the disk.
 * @param inode Unique id for the file
 * @param block The first block where the file is allocated using a @class FileAllocator algorithm.
 * @param attributes File attributes.
 */
class FileAllocationEntry {
public: 
    typedef unsigned int fileIdentifier;
public:
    FileAllocationEntry(fileIdentifier inode, HW_HardDisk::blockNumber block, FileAttributes attributes);
public:
	/**
	 * Getter
	 * @return i-node da entrada em questao
	 */
    fileIdentifier getNode();
    /**
	 * Getter
	 * @return bloco do arquivo em questao
	 */
    HW_HardDisk::blockNumber getBlock();
    /**
	 * Getter
	 * @return Atributos da entrada em questao
	 */
    FileAttributes getAttributes();
    /**
	 * Setter
	 * @param inode que sera dado ao arquivo
	 */
    void setNode(fileIdentifier inode);
    /**
	 * Setter
	 * @param block Bloco que sera atribuido a entrada do arquivo
	 */
    void setBlock(HW_HardDisk::blockNumber block);
    /**
	 * Setter
	 * @param attributes Atributos do arquivo presente na entrada
	 */
    void setAttributes(FileAttributes attributes);    
private:
    fileIdentifier inode;
    HW_HardDisk::blockNumber block;
    FileAttributes attributes;
};
/**
 * Classe que mantém controle de todos os arquivos criados
 */
class FileAllocationTable {
public:
	/**
	 * Construtor da Classe 
	 * @param disk
	 */
    FileAllocationTable(HardDisk* disk);
    FileAllocationTable(const FileAllocationTable& orig);
public:
	/**
	 * Adiciona entrada na tabela
	 * @param fatEntry entrada para a tabela 
	 */
    void addFileEntry(FileAllocationEntry fatEntry);
    /**
	 * Remove entrada da tabela
	 * @param inode Indicador unico que representa entrada de arquivo a ser removido da tabela
	 */
    void removeFileEntryByNode(FileAllocationEntry::fileIdentifier inode);
    /**
	 * Remove entrada da tabela
	 * @param rank indica posicao na tabela da entrada a ser removida
	 */
    void removeFileEntryByRank(unsigned int rank);
    /**
	 * Getter
	 * @param inode Indicador unico referente a entrada que deve ser retornada
	 * @return Entrada encontrada
	 */
    FileAllocationEntry getFileEntryByNode(FileAllocationEntry::fileIdentifier inode);
    /**
	 * Getter
	 * @param rank indica posicao da entrada a ser retornada na tabela
	 * @return Entrada encontrada
	 */
    FileAllocationEntry getFileEntryByRank(unsigned int rank);
    /**
	 * Setter
	 * @param rank indica posicao da entrada na tabela a ser substituida
	 * @param fatEntry entrada a ser posta no lugar da retirada
	 */
    void setFileEntry(unsigned int rank, FileAllocationEntry fatEntry);
    
    /**
	 * Getter
	 * @return retorna um inteiro que representa o somatorio dos tamanhos dos arquivos alocados
	 */
    int getTotalSize();
private:
    HardDisk* disk;
};


#endif /* FILEALLOCATIONTABLE_H */

