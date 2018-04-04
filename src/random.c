#include "global.h"
#include "random.h"

// The number 1103515245 comes from the example implementation of rand and srand
// in the ISO C standard.

u32 gRngValue;

u16 Random(void)
{
    gRngValue = 1103515245 * gRngValue + Random2();
    return gRngValue >> 16;
}

u16 Random2(void)
{
    gRngValue = 5245231 * gRngValue + 3573241;
    return gRngValue >> 16;
}

void SeedRng(u16 seed)
{
    gRngValue = seed;
}
