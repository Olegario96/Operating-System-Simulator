/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileSystem.cpp
 * Author: cancian
 * 
 * Created on 29 de Agosto de 2016, 14:56
 */

#include "Abstr_FileSystem.h"
#include "Abstr_FileAllocator.h"
#include <vector>
#include <iostream>


FileSystem::FileSystem() {
	this->disk = new HardDisk(0);
	this->fat = new FileAllocationTable(disk);
	this->allocator =  new FileAllocator(disk, fat);
	this->testado = 0;
}

FileSystem::FileSystem(const FileSystem& orig) {

}

FileSystem::~FileSystem() {

}
void FileSystem::criarDiretorio(const unsigned char* path) {
	//unsigned char newName = (nome);
	this->allocator->createFolder(path);
}

void FileSystem::criarArquivo(const unsigned char* path) {
	//unsigned char newName = (nome);
	this->allocator->createFile(path);
}	

void FileSystem::removerArquivo(const unsigned char* path) {
	//unsigned char newName = ((unsigned char)nome);
	this->allocator->removeFile(path);
}

FileAllocationEntry::fileIdentifier FileSystem::abrirArquivo(const unsigned char* path) {
	//unsigned char newName = (unsigned char)nome;
	return this->allocator->openFile(path);
}	

void FileSystem::fecharArquivo(FileAllocationEntry::fileIdentifier file) {
	this->allocator->closeFile(file);
}	

unsigned int FileSystem::escreverEmArquivo(FileAllocationEntry::fileIdentifier inode, unsigned int posicao, unsigned char* dados, unsigned int tamanho) {
	this->allocator->seek(posicao);
	return	this->allocator->writeFile(inode, tamanho, dados);
}	

unsigned int FileSystem::leDoArquivo(FileAllocationEntry::fileIdentifier inode, unsigned int posicao, unsigned char* dados, unsigned int tamanho) {
	this->allocator->seek(posicao);
	return this->allocator->readFile(inode, tamanho, dados);
}	

int FileSystem::getNumArquivosCriados() {
	return this->allocator->getCriados();
}

int FileSystem::getNumArquivosDeletados() {
	return this->allocator->getDeletados();
}

float FileSystem::getPorcentagemRequisicoesAtendidas() {
	return (FileSystem::getNumArquivosCriados()/this->allocator->getRequisicoes())*100;
}

int FileSystem::getEspacoLivreHD() {
	this->allocator->getEspacoLivreHD();
}

int FileSystem::getEspacoTotalHD() {
	this->allocator->getEspacoHD();
}	

int FileSystem::getEspacoFragmentado() {
	int espacoReal = this->fat->getTotalSize(); 
	int frag = FileSystem::getEspacoOcupado();
	frag -= espacoReal;
	return frag;
}

float FileSystem::getTamanhoMedioArquivos() {
	int espacoReal = this->fat->getTotalSize(); 
	int numArquivos = FileSystem::getNumArquivosCriados();
	numArquivos -= FileSystem::getNumArquivosDeletados();
	return (espacoReal / numArquivos);
}	

int FileSystem::getEspacoOcupado() {
	return (FileSystem::getEspacoTotalHD() - FileSystem::getEspacoLivreHD());
}

int FileSystem::getNumArquivosAbertos() {
	return this->allocator->getNumAbertos();
}

void FileSystem::teste() {
	if (testado == 0) {
		unsigned char newPalavra[] = {"/o "};
		const unsigned char* sameWord = newPalavra;
		criarDiretorio(sameWord);
		unsigned char newPalavra2[] = {"/o/t "};
		const unsigned char* sameWord2 = newPalavra2;
		criarArquivo(sameWord2);
		unsigned char newPalavra3[] = {"/o/p "};
		const unsigned char* sameWord3 = newPalavra3;
		criarArquivo(sameWord3);
		std::cout<<"Tentando abrir arquivo não criado -- caminho nao existe"<<std::endl;
		unsigned char newPalavra4[] = {"/o/s "};
		const unsigned char* sameWord4 = newPalavra4;
		std::cout<<abrirArquivo(sameWord4)<<std::endl;
		std::cout<<"Tentando abrir arquivo criado -- caminho existe"<<std::endl;
		unsigned int node1 = abrirArquivo(sameWord3);
		std::cout<<node1<<std::endl;
		int size = 64;
		unsigned char vector[size];
		unsigned char* buff;
		for (int i = 0; i < size; i++) {
			vector[i] = 'T';
		}
		vector[63] = 'F';
		buff = vector;
		std::cout<<"escrevendo em arquivo: "<<escreverEmArquivo(node1, 0, buff, size)<<std::endl;
		unsigned char* buff2;
		unsigned char vector2[size];
		buff2 = vector2;
		std::cout<<"lendo de arquivos "<<leDoArquivo(node1, 0, buff2, size)<<std::endl;
		int ok = 0;
		for (int i = 0; i < size; i++) {
			if (!(*(buff2+i) == *(buff+i))) {
				ok++;
				std::cout<<"arq 1 = "<<*(buff2+i)<<" arq 2 = "<<*(buff+i)<<std::endl;
			}		
		}
		std::cout<<"Resultado: "<< ok << " erros."<<std::endl;
		unsigned int node2 = abrirArquivo(sameWord2);
		std::cout<<"Aberto segundo arquivo: "<<node2<<std::endl;
		std::cout<<"escrevendo em arquivo: "<<escreverEmArquivo(node2, 5, buff, size)<<std::endl;
		unsigned char* buff3;
		unsigned char vector3[size];
		buff3 = vector3;
		std::cout<<"lendo de arquivos "<<leDoArquivo(node2, 5, buff3, size)<<std::endl;
		ok = 0;
		for (int i = 0; i < size; i++) {
			if (!(*(buff3+i) == *(buff+i))) {
				ok++;
				std::cout<<"arq 1 = "<<*(buff3+i)<<" arq 2 = "<<*(buff+i)<<" AT POSITION: "<<i<<std::endl;
			}		
		}
		std::cout<<"Resultado: "<< ok << " erros."<< std::endl;
		removerArquivo(sameWord2);
		std::cout<<"verificando arquivos deletados "<<getNumArquivosDeletados()<<std::endl;
		std::cout<<"tentando abrir arquivo deletado: "<<abrirArquivo(sameWord2)<<std::endl;
		
	}
	testado++;
	
}
