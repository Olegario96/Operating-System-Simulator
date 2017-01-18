/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FileAllocator.cpp
 * Author: cancian
 * 
 * Created on 9 de Setembro de 2016, 10:13
 */

#include "Abstr_FileAllocator.h"
#include <vector>
#include <iostream>

#define DISK_SECTOR_SIZE 64
#define DISK_NUM_OF_BLOCKS 2000
unsigned int bytePos, diretorioAtual = 0;

struct diskBlock {
	HW_HardDisk::blockNumber blockNum;
	unsigned int blocoLivre;
};

struct hd {
	diskBlock blocks[DISK_NUM_OF_BLOCKS];
	int espacoTotal;
	int espacoAtual;
	
};	

struct openedFile {
	std::vector<HW_HardDisk::blockNumber> blocos;
	FileAttributes attr;
	FileAllocationEntry::fileIdentifier node;
};

struct componente {
	unsigned char* name;
	int posicao;
	unsigned int node;
};
struct diretorio {
	std::vector<componente> comp;
	unsigned int posicao;
	unsigned int node;
};

std::vector<diretorio> myDirectories;

hd myHd;
HW_HardDisk::blockNumber encadeamentoHD[DISK_NUM_OF_BLOCKS];
std::vector<FileAllocationEntry::fileIdentifier> usedNodes;
std::vector<openedFile> myOFs;

int FileAllocator::getNumAbertos() {
	return myOFs.size();
}

int FileAllocator::getEspacoLivreHD() {
	return myHd.espacoAtual;
}	

int FileAllocator::getEspacoHD() {
	return myHd.espacoTotal;
}

int FileAllocator::getRequisicoes() {
	return this->requisicoesDeCriacao;
}

int FileAllocator::getCriados() {
	return this->arquivosCriados;
}	

int FileAllocator::getDeletados() {
	return this->arquivosDeletados;
}	

void preparaBlocosOpenedFile(HW_HardDisk::blockNumber initial, int k) {
	myOFs[k].blocos.insert(myOFs[k].blocos.end(), initial);
	if (encadeamentoHD[initial] != DISK_NUM_OF_BLOCKS) {
		preparaBlocosOpenedFile(encadeamentoHD[initial], k);
	}
}

void removeEntradasDeEncadeamento(HW_HardDisk::blockNumber initial) {
	if (encadeamentoHD[initial] != DISK_NUM_OF_BLOCKS) {
		removeEntradasDeEncadeamento(encadeamentoHD[initial]);
	}
	encadeamentoHD[initial] = DISK_NUM_OF_BLOCKS;
	myHd.blocks[initial].blocoLivre = 1;
	myHd.espacoAtual += DISK_SECTOR_SIZE;
}	

HW_HardDisk::blockNumber localizaLivre() {
	for (int i = 0; i < DISK_NUM_OF_BLOCKS; i++) {
		if (myHd.blocks[i].blocoLivre == 1) {
			myHd.blocks[i].blocoLivre = 0;
			myHd.espacoAtual -= DISK_SECTOR_SIZE;
			return myHd.blocks[i].blockNum;
		}
	}	
	return 2000;
}

FileAllocationEntry::fileIdentifier procuraNodeLivre() {
	int usado;
	for (unsigned int i = 0; i < DISK_NUM_OF_BLOCKS; i++) {
		usado = 0;
		for (int j = 0; j < usedNodes.size(); j++) {
			if (i == usedNodes[j]) {
				usado++;
			}	
		}
		if (usado == 0) {
			usedNodes.insert(usedNodes.end(), i);
			return i;
		}	
	}
	return (2000);
		
}	

HW_HardDisk::blockNumber determinaBlocoASerUsado(openedFile myOF) {
	int n = int(bytePos/DISK_SECTOR_SIZE);
	return myOF.blocos[n];
}	

FileAllocator::FileAllocator(HardDisk* disk, FileAllocationTable* fat) {
	this->disk = disk;
	bytePos = 0;
	this->fat = fat;
	for (int i = 0; i < DISK_NUM_OF_BLOCKS; i++) {
		encadeamentoHD[i] = DISK_NUM_OF_BLOCKS;
		myHd.blocks[i].blocoLivre = 1;
		myHd.blocks[i].blockNum = i;
	}
	this->requisicoesDeCriacao = 0;
	this->arquivosCriados = 0;
	this->arquivosDeletados = 0;
	myHd.espacoTotal = this->disk->getBlockSize()*this->disk->getMaxBlocks();
	myHd.espacoAtual = myHd.espacoTotal;

	//inicializacao diretorio raiz

	diretorio* raiz = new diretorio;
	raiz->posicao = myDirectories.size();
	componente* ponto = new componente;
	unsigned char nomePonto[] = {"."};
	ponto->name = new unsigned char;
	*ponto->name = *nomePonto;
	ponto->posicao = 0;
	ponto->node = 0;
	myDirectories.insert(myDirectories.end(), *raiz);
	myDirectories[0].comp.insert(myDirectories[0].comp.end(), *ponto);
}

FileAllocator::FileAllocator(const FileAllocator& orig) {

}

unsigned int verificaCaminhoValido(const unsigned char *path, int op) {
	char pchar = *(path);
	unsigned char *procurado = new unsigned char;
	int pos = 0;
	while ((pchar != ' ') && (pchar != '/')) {
		*(procurado+pos) = *(path+pos);
		pos++;
		pchar = *(path+pos);
	}
	int achei = 0;
	if (*(path + pos) == '/') {
		std::string nomeDir = std::string(reinterpret_cast<const char*>(procurado));
		std::string temp;
		for (int i = 0; i < myDirectories[diretorioAtual].comp.size(); i++) {
			temp = std::string(reinterpret_cast<const char*>(myDirectories[diretorioAtual].comp[i].name));
			if (temp == nomeDir) {
				achei = 1;
				diretorioAtual = myDirectories[diretorioAtual].comp[i].posicao;
				break;
			}
		}
	}
	if (achei == 0) {
		if ((*(path + pos) == ' ') && (op == 0)) {
			return 1;
		} else if ((*(path + pos) == ' ') && (op == 1)) {
			std::string nomeDir = std::string(reinterpret_cast<const char*>(procurado));
			std::string temp;
			for (int i = 0; i < myDirectories[diretorioAtual].comp.size(); i++) {
				temp = std::string(reinterpret_cast<const char*>(myDirectories[diretorioAtual].comp[i].name));
				if (temp == nomeDir) {
					return 1;
				}
			}
		}
	}
	if (achei == 1) {
		return verificaCaminhoValido(path+pos+1, op);
	}
	return 0;
}

unsigned char* retornaUltimo (const unsigned char *path) {
	char pchar = *(path);
	unsigned char *procurado = new unsigned char;
	int pos = 0;
	while ((pchar != ' ') && (pchar != '/')) {
		*(procurado+pos) = *(path+pos);
		pos++;
		pchar = *(path+pos);
	}
	if (*(path + pos) == '/') {
		return retornaUltimo(path+pos+1);
	}
	if (*(path + pos) == ' ') {
		unsigned char *retorno = new unsigned char;
		for (int i = 0; i < pos; i ++){
			*(retorno+i) = *(path+i);
		}
		return retorno;
	}
}

void FileAllocator::createFolder(const unsigned char* path) {
	if (*(path) == '/') {
		diretorioAtual = 0;
	}
	if (verificaCaminhoValido(path+1, 0) == 1) {
		unsigned char* aCriar = new unsigned char;
		*aCriar = *(retornaUltimo(path+1));
		componente* criando = new componente;
		criando->name = new unsigned char;
		*criando->name = *aCriar;
		criando->posicao = myDirectories.size();
		criando->node = 2000; //só serve aqui - no codigo funcao pega node livre
		myDirectories[diretorioAtual].comp.insert(myDirectories[diretorioAtual].comp.end(), *criando);
		diretorio* dirCriado = new diretorio;
		dirCriado->posicao = myDirectories.size();
		componente* ponto = new componente;
		unsigned char nomePonto[] = {"."};
		ponto->name = new unsigned char;
		*ponto->name = *nomePonto;
		ponto->posicao = diretorioAtual;
		ponto->node = 2000;
		dirCriado->comp.insert(dirCriado->comp.end(), *ponto);
		myDirectories.insert(myDirectories.end(), *dirCriado);
	}
}	

void FileAllocator::createFile(const unsigned char* path) {
	this->requisicoesDeCriacao++;
	if (*(path) == '/') {
		diretorioAtual = 0;
	}
	if (verificaCaminhoValido(path+1, 0) == 1) {
		HW_HardDisk::blockNumber blocoEscolhido = localizaLivre();
		if (blocoEscolhido < DISK_NUM_OF_BLOCKS) {
			FileAllocationEntry::fileIdentifier newNode = procuraNodeLivre();
			if (newNode < DISK_NUM_OF_BLOCKS) {
				//pegando nome
				unsigned char* aCriar = new unsigned char;
				*aCriar = *(retornaUltimo(path+1));
				//inserindo tabela diretorios
				componente* criando = new componente;
				criando->name = new unsigned char;
				*criando->name = *aCriar;
				criando->posicao = myDirectories.size();
				criando->node = newNode;
				myDirectories[diretorioAtual].comp.insert(myDirectories[diretorioAtual].comp.end(), *criando);
				//inserindo fat
				FileAttributes *attr = new FileAttributes();
				attr->setSize(0);
				attr->setType(FileAttributes::fileType::a);
				attr->setFilename((char *)(aCriar));
				//std::cout<<"BLOCO ESCOLHIDO PARA CRIAR"<<blocoEscolhido<<std::endl;
				FileAllocationEntry *entrada = new FileAllocationEntry(newNode, blocoEscolhido, *attr);
				this->fat->addFileEntry(*entrada);
				std::cout<<"Node escolhido: "<<newNode<<std::endl;
			} else {
				myHd.blocks[blocoEscolhido].blocoLivre = 1;
			}
			this->arquivosCriados++;		 
		}
	}
	/*for (int i = 0; i < myDirectories[0].comp.size(); i++) {
		std::cout<<"\n"<<myDirectories[0].comp[i].name<<std::endl;
	}
	int posicaoProximo = myDirectories[0].comp[1].posicao;

	for (int i = 0; i < myDirectories[posicaoProximo].comp.size(); i++) {
		std::cout<<myDirectories[posicaoProximo].comp[i].name<<std::endl;
	}*/
}

void FileAllocator::removeFile(const unsigned char* path) {
	if (*(path) == '/') {
		diretorioAtual = 0;
	}
	if (verificaCaminhoValido(path+1, 1) == 1) {
		diretorioAtual = 0;
		verificaCaminhoValido(path+1, 0);
		unsigned char* name = new unsigned char;
		*name = *(retornaUltimo(path+1));
		unsigned int nodeToRemove = 2000;
		//removendo da tabela diretorios
		for (int i = 0; i < myDirectories[diretorioAtual].comp.size(); i++) {
			if (*(myDirectories[diretorioAtual].comp[i].name) == *name) {
				nodeToRemove = myDirectories[diretorioAtual].comp[i].node;
				myDirectories[diretorioAtual].comp.erase(myDirectories[diretorioAtual].comp.begin()+i);
				diretorioAtual = 0;
			}
		}
		//removendo fat
		//for (int i = 0; i < usedNodes.size(); i++) {
		//entrada = this->fat->getFileEntryByNode(usedNodes[i]);
		if (nodeToRemove != 2000) {
			FileAllocationEntry entrada = this->fat->getFileEntryByNode(nodeToRemove);
			//FileAttributes attr;
			HW_HardDisk::blockNumber blocoInicial;
			//attr = entrada.getAttributes();
			if (nodeToRemove == entrada.getNode()) {
				blocoInicial = entrada.getBlock();
				this->fat->removeFileEntryByNode(nodeToRemove);
				removeEntradasDeEncadeamento(blocoInicial);
				for (int k = 0; k < myOFs.size(); k++) {
					if (myOFs[k].node == nodeToRemove) {
						myOFs.erase(myOFs.begin()+k);
						break;
					}
				}
				for (int i = 0; i < usedNodes.size(); i++) {
					if (usedNodes[i] == nodeToRemove) {
						usedNodes.erase(usedNodes.begin()+i);
						break;
					}
				}
				this->arquivosDeletados++;
			}	
		}	
		/*for (int i = 0; i < myDirectories[0].comp.size(); i++) {
			std::cout<<"\n"<<myDirectories[0].comp[i].name<<std::endl;
		}
		int posicaoProximo = myDirectories[0].comp[1].posicao;

		for (int i = 0; i < myDirectories[posicaoProximo].comp.size(); i++) {
			std::cout<<myDirectories[posicaoProximo].comp[i].name<<std::endl;
		}*/
	}
}

DiskAccessRequest* montaRequestDeAcesso(unsigned int op, HW_HardDisk::blockNumber block) {
	DiskAccessRequest* myReq;
	HW_HardDisk::DiskSector* diskSector = new HW_HardDisk::DiskSector;
	diskSector->surface = 0;
	diskSector->track = (int)(block/2);
	diskSector->sector = block % 2;
	if (op == 0) {
		myReq = new DiskAccessRequest(DiskAccessRequest::READ, block, diskSector);
	} else {
		myReq = new DiskAccessRequest(DiskAccessRequest::WRITE, block, diskSector);
	}
	return myReq;	
}
 
FileAllocationEntry::fileIdentifier FileAllocator::openFile(const unsigned char* path) {
	if (*(path) == '/') {
		diretorioAtual = 0;
	}
	if (verificaCaminhoValido(path+1, 1) == 1) {
		//std::cout<<"ENTROU"<<std::endl;
		diretorioAtual = 0;
		//usado para colocar no diretorio onde o arquivo esta
		//não importa o retorno
		verificaCaminhoValido(path+1, 0);
		unsigned char* name = new unsigned char;
		*name = *(retornaUltimo(path+1));
		unsigned int nodeToOpen = 2000;
		//encontrando nodeToOpen na estrutura dos diretorios
		for (int i = 0; i < myDirectories[diretorioAtual].comp.size(); i++) {
			if (*(myDirectories[diretorioAtual].comp[i].name) == *name) {
				nodeToOpen = myDirectories[diretorioAtual].comp[i].node;
				diretorioAtual = 0;
			}
		}
		/*
		unsigned char* nome = new unsigned char;
		*nome = *retornaUltimo(path+1);
		*/
		bytePos = 0;
		FileAllocationEntry entrada = this->fat->getFileEntryByNode(nodeToOpen);
		//FileAttributes attr;
		HW_HardDisk::blockNumber blocoInicial;
		//for (int i = 0; i < usedNodes.size(); i++) {
		if (nodeToOpen != 2000) {
			//entrada = this->fat->getFileEntryByNode(usedNodes[i]);
			FileAttributes attr;
			attr = entrada.getAttributes();
			if (nodeToOpen == entrada.getNode()) {
				blocoInicial = entrada.getBlock();
				openedFile myOF;
				int k = myOFs.size();
				myOFs.insert(myOFs.end(), myOF);
				preparaBlocosOpenedFile(blocoInicial, k);
				myOFs[k].attr = attr;
				myOFs[k].node = nodeToOpen;
				return nodeToOpen;
			}	
		}
	}
	return 2000;
}
 
void FileAllocator::closeFile(const FileAllocationEntry::fileIdentifier file) {
	for (int i = 0; i < myOFs.size(); i++) {
		if (myOFs[i].node == file) {
			myOFs.erase(myOFs.begin() + i);
			break;
		}	
	}	
}
    
unsigned int FileAllocator::readFile(const FileAllocationEntry::fileIdentifier file, const unsigned int numBytes, unsigned char* bufferBytes) {
	openedFile of;
	int j;
	for (j = 0; j < myOFs.size(); j++) {
		if (myOFs[j].node == file) {
			of = myOFs[j];
			break;
		}
	}
	//std::cout<<"Valor de j = "<<j<<" vs myOFs.size = "<<myOFs.size()<<std::endl;
	if (j < myOFs.size()) {
		HW_HardDisk::blockNumber blocoASerUtilizado = determinaBlocoASerUsado(of);
		//std::cout<<"bloco lido"<<blocoASerUtilizado<<std::endl;
		int initial = (bytePos%DISK_SECTOR_SIZE);
		DiskAccessRequest* diskAccess = montaRequestDeAcesso(0, blocoASerUtilizado);
		//std::cout<<"VALOR ESCOLHIDO PARA BLOCO "<<diskAccess->GetBlockNumber()<<std::endl;
		if ((numBytes + initial) <= this->disk->getBlockSize()) {
			this->disk->accessBlock(diskAccess);
			for (int i = initial; i < numBytes+initial; i++) {
				//std::cout<<"O que eu to copiando "<<diskAccess->GetDiskSector()->data[i]<<std::endl;
				*(bufferBytes + (i-initial)) = diskAccess->GetDiskSector()->data[i];
			}
		} else {
			//std::cout<<"antes verificacao num blocks"<<std::endl;
			if (((bytePos+numBytes)/this->disk->getBlockSize()) < myOFs[j].blocos.size()) {
				//std::cout<<"passou verificacao num blocks"<<std::endl;
				int numberOfBytes = numBytes;
				int copiados = 0;
				while (numberOfBytes > 0) {
					if (numberOfBytes+initial <= this->disk->getBlockSize()) {
						//std::cout<<"iniciando copia parte 2: copiados = "<<copiados<<std::endl;
						this->disk->accessBlock(diskAccess);
						for (int i = initial; i < numBytes+initial; i++) {
							//std::cout<<"O que eu to copiando "<<diskAccess->GetDiskSector()->data[i]<<std::endl;
							*(bufferBytes + (copiados)) = diskAccess->GetDiskSector()->data[i];
							copiados++;
						}
						numberOfBytes = numberOfBytes - this->disk->getBlockSize() + initial;
						initial = 0;
					} else {
						//std::cout<<"iniciando copia parte 1: copiados = "<<copiados<<std::endl;
						this->disk->accessBlock(diskAccess);
						for (int i = initial; i < this->disk->getBlockSize(); i++) {
							*(bufferBytes + (copiados)) = diskAccess->GetDiskSector()->data[i];
							copiados++;
						}
						for (int i = 0; i < myOFs[j].blocos.size(); i++) {
							if (myOFs[j].blocos[i] == blocoASerUtilizado) {
								blocoASerUtilizado = myOFs[j].blocos[i+1];
								break;
							}
						}
						diskAccess = montaRequestDeAcesso(0, blocoASerUtilizado);
						//std::cout<<"VALOR ESCOLHIDO PARA BLOCO "<<diskAccess->GetBlockNumber()<<std::endl;
						numberOfBytes = numberOfBytes - this->disk->getBlockSize() + initial;
						initial = 0;
					}			
				}
			} else {
				return 1;
			}	
		}
		return 0;	
	} else {
		return 1;
	}
}
 
unsigned int FileAllocator::writeFile(const FileAllocationEntry::fileIdentifier file, const unsigned int numBytes, unsigned char* bufferBytes) {
	openedFile of;
	int j;
	for (j = 0; j < myOFs.size(); j++) {
		if (myOFs[j].node == file) {
			of = myOFs[j];
			break;
		}
	}
	if (j < myOFs.size()) {
		HW_HardDisk::blockNumber blocoASerUtilizado = determinaBlocoASerUsado(of);
		//std::cout<<"bloco escrito"<<blocoASerUtilizado<<std::endl;
		int initial = (bytePos%DISK_SECTOR_SIZE);	
		DiskAccessRequest* diskAccess = montaRequestDeAcesso(1, blocoASerUtilizado);
		if ((numBytes + initial) <= 64) {	
			for (int i = initial; i < numBytes+initial; i++) {
				diskAccess->GetDiskSector()->data[i] = *(bufferBytes + (i-initial));
			}
			this->disk->accessBlock(diskAccess);
			//of.attr.setSize(numBytes + initial);
			FileAllocationEntry entrada = this->fat->getFileEntryByNode(file);
			FileAttributes attr = entrada.getAttributes();
			attr.setSize(numBytes+initial);
			entrada.setAttributes(attr);
			this->fat->removeFileEntryByNode(entrada.getNode());
			this->fat->addFileEntry(entrada);
		} else {
			int numberOfBytes = numBytes;
			int copiados = 0;
			while (numberOfBytes > 0) {
				//std::cout<<"bloco escrito"<<blocoASerUtilizado<<std::endl;
				if (numberOfBytes+initial <= 64) {
					for (int i = initial; i < numberOfBytes+initial; i++) {
						diskAccess->GetDiskSector()->data[i] = *(bufferBytes + (copiados));
						copiados++;
					}
					this->disk->accessBlock(diskAccess);
					numberOfBytes = numberOfBytes - 64 + initial;
					initial = 0;
				} else {							
					for (int i = initial; i < 64; i++) {
						diskAccess->GetDiskSector()->data[i] = *(bufferBytes + (copiados));
						copiados++;
					}
					this->disk->accessBlock(diskAccess);
					encadeamentoHD[blocoASerUtilizado] = localizaLivre();
					blocoASerUtilizado = encadeamentoHD[blocoASerUtilizado];
					myOFs[j].blocos.insert(myOFs[j].blocos.end(), blocoASerUtilizado);
					diskAccess = montaRequestDeAcesso(1, blocoASerUtilizado);
					numberOfBytes = numberOfBytes - 64 + initial;
					initial = 0;
				}
			}
			FileAllocationEntry entrada = this->fat->getFileEntryByNode(file);
			FileAttributes attr = entrada.getAttributes();
			attr.setSize(numBytes+initial);
			entrada.setAttributes(attr);
			this->fat->removeFileEntryByNode(entrada.getNode());
			this->fat->addFileEntry(entrada);	
		}
		return 0;
	} else {
		return 1;
	}	
}
   
int FileAllocator::getEspacoOcupadoPorArquivos() {
	int usado;
	int valorAretornar = 0;
	for (unsigned int i = 0; i < DISK_NUM_OF_BLOCKS; i++) {
		usado = 0;
		for (int j = 0; j < usedNodes.size(); j++) {
			if (i == usedNodes[j]) {
				usado++;
			}	
		}
		if (usado > 0) {
			FileAllocationEntry entrada = this->fat->getFileEntryByNode(i);
			FileAttributes attr = entrada.getAttributes();
			valorAretornar += attr.getSize();
		}	
	}
	return valorAretornar;
}	
 
void FileAllocator::seek(const unsigned long numByte) {
	bytePos = (unsigned int)numByte;
}
