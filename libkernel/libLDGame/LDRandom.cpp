//
//  LDRandom.cpp
//  libLDGame
//
//  Created by Sidney
//  Copyright (c) 2012 by Sidney
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "LDRandom.h"

#define N 624
#define M 397
#define A 0x9908b0dfUL
#define U 0x80000000UL
#define L 0x7fffffffUL

static uint32_t LDRandomX[N];
static int LDRandomNext = 0;

void LDRandomSeed(uint32_t seed)
{
	printf("Seed: %x\n", seed);
	LDRandomX[0] = seed & 0xffffffffUL;

	for(int i=1; i<N; i++)
	{
		LDRandomX[i] = 1812433253UL * (LDRandomX[i - 1] ^ (LDRandomX[i] >> 30)) + i;
		LDRandomX[i] &= 0xffffffffUL;
	}
}

uint32_t LDRandom()
{
	uint32_t y, a;

	if(LDRandomNext == N)
	{
		LDRandomNext = 0;

		for(int i=0; i<N - 1; i++)
		{
			y = ((LDRandomX[i] & U) | LDRandomX[i + 1]) & L;
			a = (y & 0x1) ? A : 0x0;

			LDRandomX[i] = LDRandomX[(i + M) % N] ^ (y >> 1) ^ a;
		}

		y = ((LDRandomX[N - 1] & U) | LDRandomX[0]) & L;
		a = (y & 0x1) ? A : 0x0;

		LDRandomX[N - 1] = LDRandomX[M - 1] ^ (y >> 1) ^ a;
	}

	y = LDRandomX[LDRandomNext ++];

	y ^= y >> 11;
	y ^= (y << 7) & 0x9d2c5680UL;
	y ^= (y << 15) & 0xefc60000UL;
	y ^= y >> 18;

	return y;
}

uint32_t LDRandomRange(uint32_t min, uint32_t max)
{
	return min + LDRandom() % (max - min);
}
