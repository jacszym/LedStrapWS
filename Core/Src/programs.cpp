/*
 * programs.cpp
 *
 *  Created on: 06.02.2019
 *      Author: Jacek
 */
#include "stdint.h"

typedef void (*effectFun_t)(void);

typedef struct effectInfo
{
	effectFun_t effectFun;
	uint32_t refreshRate;
} effectInfo_t;

typedef struct staticColorSet
{
	uint8_t mainStep;

} staticColorSet_t;

union stateDataBuffer
{

	uint32_t ui32[8];
	uint8_t  ui8[32];
};

static void staticColor(void)
{

}

static void rainbowColor1(void)
{

}

static const effectInfo_t effectsTabel[1] =
{
	{staticColor, 1000},
	{rainbowColor1, 20}
};
