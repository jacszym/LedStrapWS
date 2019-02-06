/*
 * ledWS.h
 *
 *  Created on: 21.12.2018
 *      Author: Jacek
 */

#ifndef INC_LEDWS_HPP_
#define INC_LEDWS_HPP_

#include "stdint.h"
#include "cstddef"
#include "transport.hpp"

namespace decorLed
{
static const size_t D_MAX_PIXEL = 32;

#pragma pack(1)
struct singlePixel
{
	uint8_t green;
	uint8_t red;
	uint8_t blue;
};
#pragma pack()

class decorLED
{
public:
	decorLED(uint32_t x, uint32_t y = 1):
		maxPixelX(x),
		maxPixelY(y),
		maxPixelNr(x*y)
	{
		singlePixel zeroPixel = {0};
		for(uint32_t index = 0; index < D_MAX_PIXEL; index++)
			pixels[index] = zeroPixel;
	};

	void setPixel(uint32_t x, uint32_t y, singlePixel pixelValue)
	{
		if(( x < maxPixelX ) && ( y < maxPixelY )) pixels[x+y*maxPixelX] = pixelValue;
	}

	void setPixel(uint32_t x, singlePixel pixelValue)
	{
		if( x < maxPixelNr ) pixels[x] = pixelValue;
	}

	singlePixel getPixel(uint32_t x, uint32_t y = 0)
	{
		if(( x < maxPixelX ) && ( y < maxPixelY ))
			return pixels[x+y*maxPixelX];
		else
		{
			singlePixel zeroPixel = {0};
			return zeroPixel;
		}
	}

	void refresh(void)
	{
		transport.transmitActiveBuffer();
	}

private:
	//
#pragma pack(1)
	singlePixel pixels[D_MAX_PIXEL];
#pragma pack()
	spiTransport::transportLayer transport;
	//
	const uint32_t maxPixelX;
	const uint32_t maxPixelY;
	const uint32_t maxPixelNr;
};
}
#endif /* INC_LEDWS_HPP_ */
