/*
 * File:   MemoryManager.cpp
 * Authors: Cesar Smaniotto Júnior
 * Strategy: Best fit
 *
 * Created on ...
 */


#include "Simul_Debug.h"
#include "Abstr_MemoryManager.h"
#include "Simulator.h"
#include "Traits.h"
#include <string>
#include <iostream>
#include <ctime>
#include <time.h>


MemoryManager::MemoryManager() {
    _chunks = new std::list<MemoryChunk*>();

    _memorySize = 3000;

    MemoryChunk * chunk = new MemoryChunk(0,_memorySize,false,false,false);

    _chunks->push_back(chunk);

}

MemoryManager::MemoryManager(const MemoryManager& orig) {
}

MemoryManager::~MemoryManager() {
}

//aloca espaço na memória pelo método best fit

MemoryChunk* MemoryManager::allocateMemory(unsigned int size) {
    Debug::cout(Debug::Level::trace, "MemoryManager::allocateMemory(" + std::to_string(size) + ")");


    std::list<MemoryChunk*>::iterator  itChunkLivre = _allocation->allocateMemory(_chunks, size);

        if(itChunkLivre == _chunks->end()) {

            this->defragment();

            itChunkLivre = std::prev(_chunks->end());

            std::cout << "está livre? " << (*itChunkLivre)->isFree() << std::endl;
            std::cout << "tamanho " <<  (*itChunkLivre)->getSize() << std::endl;

            if(!(*itChunkLivre)->isFree() || size > (*itChunkLivre)->getSize()) {
                return nullptr;
            }
        }

    int beginLogicalAddress = (*itChunkLivre)->getBeginLogicalAddress();

    MemoryChunk * newChunk = new MemoryChunk(beginLogicalAddress,size,true,true,true);
    newChunk->setIsFree(false);

    (*itChunkLivre)->setBeginLogicalAddress(beginLogicalAddress + size);
    (*itChunkLivre)->setSize((*itChunkLivre)->getSize() - size);

    _chunks->insert(itChunkLivre, newChunk);

    return newChunk;
}

/*
 * desaloca um chunk, seguindo o seguinte esquema para reagrupar chunks contíguos livres
 *
 * caso AmbosLivres: dois chunks adjacentes ao desalocado estão livres
 * caso NenhumLivre: nenhum chunk adjacente ao desalocado está livre
 * caso EsquerdaLivre: chunk a esquerda do desalocado está livre
 * caso DireitaLivre: chunk a direita do desalocado está livre
 */

void MemoryManager::deallocateMemory(MemoryChunk* chunk) {
    Debug::cout(Debug::Level::trace, "MemoryManager::deallocateMemory(" + std::to_string(reinterpret_cast<unsigned long> (chunk)) + ")");

    std::list<MemoryChunk*>::iterator itChunk,itNext,itPrev;

    for(itChunk = _chunks->begin(); itChunk != _chunks->end(); itChunk++) {

        if((*itChunk) == chunk) {

            break;
        }

    }

    if(itChunk == _chunks->end()) {
        return ;
    }


    itNext = std::next(itChunk);
    itPrev = std::prev(itChunk);

    neighborsType op;

    if(itChunk == _chunks->begin() && itNext ==_chunks->end()) {

        op = NoFree;

    }

    else if(itChunk == _chunks->begin() && itNext !=_chunks->end()) {

        op = (*itNext)->isFree() ? RightFree : NoFree;

    }
    else if(itChunk != _chunks->begin() && itNext ==_chunks->end()) {

        op = (*itPrev)->isFree() ? LeftFree : NoFree;
    }
    else {

        if((*itPrev)->isFree()) {

            op = (*itNext)->isFree() ? BothFree : LeftFree;

        } else {

            op = (*itNext)->isFree() ? RightFree : NoFree;

        }
    }


    switch(op) {

    case BothFree: {

        int newSize = (*itPrev)->getSize() + (*itChunk)->getSize() + (*itNext)->getSize();

        (*itPrev)->setSize(newSize);
        _chunks->erase(itChunk);
        _chunks->erase(itNext);

        break;
    }


    case NoFree: {

        (*itChunk)->setIsFree(true);

        break;
    }

    case LeftFree: {

        int newSize = (*itPrev)->getSize() + (*itChunk)->getSize();

        (*itPrev)->setSize(newSize);

        _chunks->erase(itChunk);

        break;

    }
    case RightFree: {
        int newSize = (*itChunk)->getSize() + (*itNext)->getSize();

        (*itChunk)->setSize(newSize);
        (*itChunk)->setIsFree(true);
        _chunks->erase(itNext);

        break;

    }

    }



}

//desfragmenta a memória, segundo método da desfragmentação incremental
//onde os chunks ocupados são movidos para o começo da memória, criando um único chunk livre vazio
//no fim da memória

void MemoryManager::defragment() {

    clock_t begin = std::clock();

    std::list<MemoryChunk*> *defragmented = new std::list<MemoryChunk*>;
    std::list<MemoryChunk*>::iterator itFragmented, itDefragmented;
    int newBeginAddress;
    for(itFragmented = _chunks->begin(); itFragmented != _chunks->end(); itFragmented++) {

        if(!(*itFragmented)->isFree()) {

            if(defragmented->empty()) {

                newBeginAddress = 0;

            } else {

                itDefragmented = std::prev(defragmented->end());
                newBeginAddress = (*itDefragmented)->getBeginLogicalAddress() + (*itDefragmented)->getSize();
            }
            (*itFragmented)->setBeginLogicalAddress(newBeginAddress);
            defragmented->push_back(*itFragmented);

        }



    }
    itDefragmented = std::prev(defragmented->end());
    newBeginAddress = (*itDefragmented)->getBeginLogicalAddress() + (*itDefragmented)->getSize();
    MemoryChunk *freeChunk = new MemoryChunk(newBeginAddress, _memorySize - newBeginAddress, false, false, false);
    freeChunk->setIsFree(true);
    defragmented->push_back(freeChunk);

    _chunks = defragmented;

    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    _accountInformation._countNumberOfDefragmentations++;
    _accountInformation.timeSpentWithDefragmentation += elapsed_secs;


}


unsigned int MemoryManager::getNumMemoryChunks() {

    return _chunks->size();
}

MemoryChunk* MemoryManager::getMemoryChunk(unsigned int index) {

    if(index > this->getNumMemoryChunks() - 1) {
        return nullptr;
    }

    std::list<MemoryChunk*>::iterator it = _chunks->begin();
    std::advance(it, index);
    return *it;

}

void MemoryManager::showMemory() {
    // INSERT YOUR CODE TO SHOW THE MEMORY MAP, IN THE FOLLOWING FORMAT
    // <beginAddress>-<endAddress>: <FREE|ALLOCATED> <size> <process id>

    // Exemplo:
    /*
      0-1499:FREE 1500 0
      1500-1999:ALLOCATED 500 2
      2000-2999:ALLOCATED 1000 3
      3000-9999:FREE 7000 0
      10000-19999:ALLOCATED 10000 7
      20000-1000000:FREE 800000 0
    */
    std::cout << "Memory Map:" << std::endl;

    std::list<MemoryChunk*>::iterator it;

    for(it = _chunks->begin(); it != _chunks->end(); it++) {

        int beginAddress = (*it)->getBeginLogicalAddress();
        int size = (*it)->getSize();
        std::string allocated = (*it)->isFree() ? "FREE" : "ALLOCATED";

        std::cout << beginAddress << "-" << (beginAddress + size) -1 << " " + allocated + " " << size << std::endl;

    }



}

void MemoryManager::showStatistics()
{
    std::cout << "Statistics:" << std::endl;

    std::cout << "Fragmentation rate: " << getFragmentationRate() << std::endl;
    std::cout << "Occupancy rate: " << getOccupancyRate() << std::endl;
    std::cout << "Average time Defragmentation: " << getAverageTimeDefragmentation() << std::endl;

}


//taxa de fragmentação: tamanho dos chunks vazios - maior chunk vazio / tamanho dos chunks vazios * 100

double MemoryManager::getFragmentationRate() {

    double sizeLargestChunkFree = 0.0;
    std::list<MemoryChunk*>::iterator it;
    double sizeFreeMemory = 0.0;

    for(it = _chunks->begin(); it != _chunks->end(); it++) {

        if((*it)->isFree()) {

            int tamanhoChunkIter = (*it)->getSize();

            if(sizeFreeMemory == 0) {
                sizeLargestChunkFree = tamanhoChunkIter;
            } else if(tamanhoChunkIter > sizeLargestChunkFree) {
                sizeLargestChunkFree = tamanhoChunkIter;
            }
            sizeFreeMemory += tamanhoChunkIter;
        }

    }


    return ((sizeFreeMemory - sizeLargestChunkFree) / sizeFreeMemory) * 100;

}

//taxa de ocupação: tamanho dos chunks ocupados / tamanho total da memória * 100

double MemoryManager::getOccupancyRate() {


    std::list<MemoryChunk*>::iterator it;
    double allocatedMemorySize = 0.0;

    for(it = _chunks->begin(); it != _chunks->end(); it++) {

        if(!(*it)->isFree()) {

            allocatedMemorySize +=  (*it)->getSize();
        }

    }

    return ( allocatedMemorySize / _memorySize) * 100;

}

//tempo médio para desfragmentação: somatório do tempo gasto com desfragmentação / quantidade de desfragmentações realizadas

double MemoryManager::getAverageTimeDefragmentation() {

    return _accountInformation.timeSpentWithDefragmentation / (double) _accountInformation._countNumberOfDefragmentations;
}