/*
 * transport.hpp
 *
 *  Created on: 28.12.2018
 *      Author: Jacek
 */

#ifndef INC_TRANSPORT_HPP_
#define INC_TRANSPORT_HPP_

#include "stm32f3xx_hal.h"
#include "string.h"
#include "spi.h"

namespace spiTransport
{

static const size_t T_MAX_LED       = 32;
static const size_t T_MAX_LED_BYTES = (T_MAX_LED * 3);		// 3 color bytes per pixe;
static const size_t T_MAX_LED_BITS  = (T_MAX_LED_BYTES * 8);	// 8 bits per byte

class transportLayer
{
public:
	transportLayer():
	doubleBufferingUsed(false),
	activeBuffer(0),
	activeBufferDataSize(0)
	{
		// "clear" bits buffer
		for(int32_t buffNr = 0; buffNr < 2; buffNr++)
			for(size_t index = 0; index < (sizeof(encodedBitsBuffer[0])/sizeof(uint16_t)); index++)
				encodedBitsBuffer[buffNr][index] = WS_ZERO;
	}

	~transportLayer()
	{

	}

	void transmitActiveBuffer(void)
	{
		uint8_t *pBuffer = nullptr;

		if( doubleBufferingUsed )
		{
			pBuffer = activeBuffer == 0 ? (uint8_t*)(&encodedBitsBuffer[0]) : (uint8_t*)(&encodedBitsBuffer[1]);
		}
		else
		{
			pBuffer = (uint8_t*)(&encodedBitsBuffer[0]);
		}
		HAL_SPI_Transmit_DMA(&hspi1, pBuffer, activeBufferDataSize);
	}

	void encodeData(uint8_t *data, size_t size)
	{
		if( size > T_MAX_LED_BYTES )
			return;

		uint16_t *pBuffer = nullptr;

		if( doubleBufferingUsed )
		{
			if( activeBuffer == 0 )
			{
				pBuffer = encodedBitsBuffer[1];
				activeBuffer = 1;
			}
			else
			{
				pBuffer = encodedBitsBuffer[0];
				activeBuffer = 0;
			}
		}
		else
		{
			pBuffer = encodedBitsBuffer[0];
		}

		uint16_t *pStart = pBuffer;

		for(uint32_t index = 0; index < size; index++)
		{
			uint8_t value = *data++;
			for(int32_t bit_nr = 0; bit_nr < 8; bit_nr++)
			{
				*pBuffer++ = ( value & 0x80 ) ? WS_ONE : WS_ZERO;
				value <<= 1;
			}
		}
		activeBufferDataSize =  (uint16_t)(pBuffer - pStart);
	}



private:
	static const uint16_t WS_ONE = 0x03f0;	// b: 0000001110000000
	static const uint16_t WS_ZERO = 0x0380;	// b: 0000001111110000

	bool doubleBufferingUsed;
	uint8_t activeBuffer;
	uint16_t activeBufferDataSize;
	uint16_t encodedBitsBuffer[2][T_MAX_LED_BITS];
};

} /* spiTransport */

#endif /* INC_TRANSPORT_HPP_ */
