// This code is part of Toolkit(FileHash)
// A useful and powerful toolkit(FileHash)
// Copyright (C) 2012-2017 Chengr28
// 
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either
// version 2 of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


#ifndef TOOLKIT_FILEHASH_SHA_1_H
#define TOOLKIT_FILEHASH_SHA_1_H

#include "Base.h"

//The SHA-1 block size, message digest size in bytes and some useful types.
#define SHA1_BLOCK_SIZE         64U
#define SHA1_DIGEST_SIZE        20U
typedef int32_t                 SHA1_INT32;
typedef int64_t                 SHA1_INT64;

//Global variables
extern size_t HashFamilyID;
extern bool IsLowerCase;

//The structure for storing SHA1 info
typedef struct _sha1_state_
{
	SHA1_INT64     Length;
	SHA1_INT32     State[5U], Curlen;
	uint8_t        Buffer[SHA1_BLOCK_SIZE];
}SHA1_State;

//Rotate the hard way(platform optimizations could be done).
#define ROL(x, y) ((((uint32_t)(x) << (uint32_t)((y) & 31)) | (((uint32_t)(x) & 0xFFFFFFFFUL) >> (uint32_t)(32 - ((y) & 31)))) & 0xFFFFFFFFUL)
#define ROLc(x, y) ((((uint32_t)(x) << (uint32_t)((y) & 31)) | (((uint32_t)(x) & 0xFFFFFFFFUL) >> (uint32_t)(32 - ((y) & 31)))) & 0xFFFFFFFFUL)

//Endian Neutral macros that work on all platforms
#define STORE32H(x, y)                                                                      \
	{(y)[0] = (uint8_t)(((x) >> 24U) & 255); (y)[1U] = (uint8_t)(((x) >> 16U) & 255);       \
	(y)[2U] = (uint8_t)(((x) >> 8U) & 255); (y)[3U] = (uint8_t)((x) & 255);}

#define LOAD32H(x, y)                                                                       \
	{x = ((SHA1_INT32)((y)[0] & 255) << 24U) |                                              \
		((SHA1_INT32)((y)[1U] & 255) << 16U) |                                              \
		((SHA1_INT32)((y)[2U] & 255) << 8U)  |                                              \
		((SHA1_INT32)((y)[3U] & 255));}

#define STORE64H(x, y)                                                                      \
	{(y)[0] = (uint8_t)(((x) >> 56U) & 255); (y)[1U] = (uint8_t)(((x) >> 48U) & 255);       \
		(y)[2U] = (uint8_t)(((x) >> 40U) & 255); (y)[3U] = (uint8_t)(((x) >> 32U) & 255);   \
		(y)[4U] = (uint8_t)(((x) >> 24U) & 255); (y)[5U] = (uint8_t)(((x) >> 16U) & 255);   \
		(y)[6U] = (uint8_t)(((x) >> 8U) & 255); (y)[7U] = (uint8_t)((x) & 255);}

#ifndef MIN
	#define MIN(x, y) (((x)<(y)) ? (x):(y))
#endif

//SHA-1 macros
#define SHA1_F0(x, y, z) (z ^ (x & (y ^ z)))
#define SHA1_F1(x, y, z) (x ^ y ^ z)
#define SHA1_F2(x, y, z) ((x & y) | (z & (x | y)))
#define SHA1_F3(x, y, z) (x ^ y ^ z)
#endif
