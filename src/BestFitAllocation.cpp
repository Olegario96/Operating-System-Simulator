#include "BestFitAllocation.h"
#include <iostream>

std::list<MemoryChunk*>::iterator BestFitAllocation::allocateMemory(std::list<MemoryChunk*>* _chunks, unsigned int size){

  std::list<MemoryChunk*>::iterator it,itChunkLivre;
    itChunkLivre = _chunks->end();

    for(it = _chunks->begin(); it != _chunks->end(); it++) {

        MemoryChunk *mChunk = *it;

        if(mChunk->isFree() && mChunk->getSize() >= size) {

            if(itChunkLivre == _chunks->end()) {
                itChunkLivre = it;
            } else {
                if((*itChunkLivre)->getSize() > mChunk->getSize()) {

                    itChunkLivre = it;
                }
            }

        }

    }

    return itChunkLivre;


}