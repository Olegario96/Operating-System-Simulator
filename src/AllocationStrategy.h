#ifndef ALLOCATION_STRATEGY_H
#define ALLOCATION_STRATEGY_H

#include "Abstr_MemoryChunk.h"
#include <list>

class AllocationStrategy{
  
public:
  
  virtual std::list<MemoryChunk*>::iterator allocateMemory(std::list<MemoryChunk*>* chunks, unsigned int size) = 0;
  
  
};

#endif