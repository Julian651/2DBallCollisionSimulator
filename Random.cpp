#include "Random.h"

Random::Random(uint32_t seed) {
   state = seed;
}
Random::Random() : Random(0) {}
void Random::setSeed(uint32_t seed) {
   state = seed;
}
uint32_t Random::nextUINT() {
   state += 0xe120fc15;
   uint64_t tmp;
   tmp = (uint64_t)state * 0x4a39b70d;
   uint32_t m1 = (uint32_t) ((tmp >> 32) ^ tmp);
   tmp = (uint64_t)m1 * 0x12fad5c9;
   uint32_t m2 = (uint32_t) ((tmp >> 32) ^ tmp);
   return m2;
}
int Random::nextInt() {
   return nextUINT() - 2147483648;
}
float Random::nextFloat() {
   float f = (float)nextUINT() / (float)4294967296;
   return f;
}
double Random::nextDouble() {
   double d = (double)nextUINT() / (double)4294967296;
   return d;
}
