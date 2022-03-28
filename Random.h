#pragma once

#include <stdint.h>

class Random 
{
private:
   uint32_t state;
public:
   Random(uint32_t state);
   Random();
   void setSeed(uint32_t state);
   uint32_t nextUINT();
   int nextInt();
   float nextFloat();
   double nextDouble();
};

