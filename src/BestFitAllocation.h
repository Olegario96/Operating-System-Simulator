#ifndef BEST_FIT_ALLOCATION_H
#define BEST_FIT_ALLOCATION_H

#include "AllocationStrategy.h"
#include "Abstr_MemoryChunk.h"
#include <list>

class BestFitAllocation : public AllocationStrategy{
   
public :
  std::list<MemoryChunk*>::iterator allocateMemory(std::list<MemoryChunk*>* _chunks, unsigned int size);
  
};

#endif