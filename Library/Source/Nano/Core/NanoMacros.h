/*	NAME:
		NanoMacros.h

	DESCRIPTION:
		Nano macros.
	
	COPYRIGHT:
		Copyright (c) 2006-2013, refNum Software
		<http://www.refnum.com/>

		All rights reserved. Released under the terms of licence.html.
	__________________________________________________________________________
*/
#ifndef NANO_MACROS_HDR
#define NANO_MACROS_HDR
//============================================================================
//		Include files
//----------------------------------------------------------------------------





//============================================================================
//		Macros
//----------------------------------------------------------------------------
// Mark unused parameters
#define NN_UNUSED(_x)												(void) _x


// Get the size of an array
#define NN_ARRAY_SIZE(_a)											((NIndex) (sizeof((_a)) / sizeof((_a)[0])))


// Get a clamped value
#define NN_CLAMP_VALUE(_v, _min, _max)								std::min((_max), std::max((_min), (_v)))


// Interpolate between values
#define NN_LERP_VALUE(_start, _end, _progress)						( ((_start) * (1.0f - (_progress))) + ((_end) * (_progress)) )


// Is an address aligned?
#define NN_ALIGNED_TO(_value, _size)								((((uintptr_t) _value) % _size) == 0)


// Is a bit set?
#define NN_TEST_BIT(_value, _mask)									(((_value) & (_mask)) == (_mask))


// Declare a binary constant
//
//		NN_B8( 01010101)									= 85
//		NN_B16(10101010, 01010101)							= 43605
//		NN_B32(10000000, 11111111, 10101010, 01010101)		= 2164238933
//
// Based on a PD implementation by Tom Torfs ("Binary constant macros").
#define _NN_HEX(_n)	0x ## _n ## LU

#define _NN_B8(_x)										(uint8_t)  (((_x & 0x0000000FLU) ?   1 : 0) | \
																	((_x & 0x000000F0LU) ?   2 : 0) | \
																	((_x & 0x00000F00LU) ?   4 : 0) | \
																	((_x & 0x0000F000LU) ?   8 : 0) | \
																	((_x & 0x000F0000LU) ?  16 : 0) | \
																	((_x & 0x00F00000LU) ?  32 : 0) | \
																	((_x & 0x0F000000LU) ?  64 : 0) | \
																	((_x & 0xF0000000LU) ? 128 : 0))

#define NN_B8(_byte1)											(	(uint8_t) _NN_B8(_NN_HEX(_byte1)) )

#define NN_B16(_byte1, _byte2)									(	(((uint16_t) NN_B8(_byte1)) <<  8) | \
																	(((uint16_t) NN_B8(_byte2)) <<  0) )

#define NN_B32(_byte1, _byte2, _byte3, _byte4)					(	(((uint32_t) NN_B8(_byte1)) << 24) | \
																	(((uint32_t) NN_B8(_byte2)) << 16) | \
																	(((uint32_t) NN_B8(_byte3)) <<  8) | \
																	(((uint32_t) NN_B8(_byte4)) <<  0) )

	
	
	
#endif // NANO_MACROS_HDR



