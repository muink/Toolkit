// This code is part of Toolkit(FileHash)
// FileHash, a useful and powerful toolkit
// Copyright (C) 2012-2018 Chengr28
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


#ifndef TOOLKIT_FILEHASH_BLAKE_H
#define TOOLKIT_FILEHASH_BLAKE_H

#include "Base.h"

//Hash function ID
#define HASH_ID_BLAKE_224           1U
#define HASH_ID_BLAKE_256           2U
#define HASH_ID_BLAKE_384           3U
#define HASH_ID_BLAKE_512           4U
#define DEFAULT_HASH_FUNCTION_ID    HASH_ID_BLAKE_256

//Size definitions
#define BLAKE_DIGEST_SIZE_224       224U
#define BLAKE_DIGEST_SIZE_256       256U
#define BLAKE_DIGEST_SIZE_384       384U
#define BLAKE_DIGEST_SIZE_512       512U

//Commands definitions
#if defined(PLATFORM_WIN)
	#define COMMAND_BLAKE_224           L"-BLAKE_224"
	#define COMMAND_BLAKE_256           L"-BLAKE_256"
	#define COMMAND_BLAKE_384           L"-BLAKE_384"
	#define COMMAND_BLAKE_512           L"-BLAKE_512"
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define COMMAND_BLAKE_224           ("-BLAKE_224")
	#define COMMAND_BLAKE_256           ("-BLAKE_256")
	#define COMMAND_BLAKE_384           ("-BLAKE_384")
	#define COMMAND_BLAKE_512           ("-BLAKE_512")
#endif

//Global variables
extern size_t HashFamilyID;
extern bool IsLowerCase;
size_t BLAKE_HashFunctionID = DEFAULT_HASH_FUNCTION_ID;

//BLAKE definitions
#define U8TO32_BIG(p)                                                    \
	(((uint32_t)((p)[0]) << 24U) | ((uint32_t)((p)[1U]) << 16U) |        \
	((uint32_t)((p)[2U]) << 8U) | ((uint32_t)((p)[3U])))

#define U32TO8_BIG(p, v)                                                 \
	(p)[0] = (uint8_t)((v) >> 24U); (p)[1U] = (uint8_t)((v) >> 16U);     \
	(p)[2U] = (uint8_t)((v) >> 8U); (p)[3U] = (uint8_t)((v));

#define U8TO64_BIG(p)                                                    \
	(((uint64_t)U8TO32_BIG(p) << 32U) | (uint64_t)U8TO32_BIG((p) + 4))

#define U64TO8_BIG(p, v)		                                         \
	U32TO8_BIG((p), (uint32_t)((v) >> 32U));                             \
	U32TO8_BIG((p) + 4, (uint32_t)((v)));

typedef struct state256
{
	uint32_t   h[8U], s[4U], t[2U];
	int        buflen, nullt;
	uint8_t    buf[64U];
}state224;

typedef struct state512
{
	uint64_t   h[8U], s[4U], t[2U];
	int        buflen, nullt;
	uint8_t    buf[128U];
}state384;

const uint8_t sigma[][16U] = 
{
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, 
	{14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }, 
	{11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 }, 
	{ 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 }, 
	{ 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 }, 
	{ 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 }, 
	{12, 5, 1, 15, 14, 13, 4, 10, 0, 7, 6, 3, 9, 2, 8, 11 }, 
	{13, 11, 7, 14, 12, 1, 3, 9, 5, 0, 15, 4, 8, 6, 2, 10 }, 
	{ 6, 15, 14, 9, 11, 3, 0, 8, 12, 2, 13, 7, 1, 4, 10, 5 }, 
	{10, 2, 8, 4, 7, 6, 1, 5, 15, 11, 9, 14, 3, 12, 13 , 0 }, 
	{ 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 }, 
	{14, 10, 4, 8, 9, 15, 13, 6, 1, 12, 0, 2, 11, 7, 5, 3 }, 
	{11, 8, 12, 0, 5, 2, 15, 13, 10, 14, 3, 6, 7, 1, 9, 4 }, 
	{ 7, 9, 3, 1, 13, 12, 11, 14, 2, 6, 5, 10, 4, 0, 15, 8 }, 
	{ 9, 0, 5, 7, 2, 4, 10, 15, 14, 1, 11, 12, 6, 8, 3, 13 }, 
	{ 2, 12, 6, 10, 0, 11, 8, 3, 4, 13, 7, 5, 15, 14, 1, 9 }
};

const uint32_t u256[16U] = 
{
	0x243F6A88, 0x85A308D3, 0x13198A2E, 0x03707344, 
	0xA4093822, 0x299F31D0, 0x082EFA98, 0xEC4E6C89, 
	0x452821E6, 0x38D01377, 0xBE5466CF, 0x34E90C6C, 
	0xC0AC29B7, 0xC97C50DD, 0x3F84D5B5, 0xB5470917
};

const uint64_t u512[16U] = 
{
	0x243F6A8885A308D3ULL, 0x13198A2E03707344ULL, 
	0xA4093822299F31D0ULL, 0x082EFA98EC4E6C89ULL, 
	0x452821E638D01377ULL, 0xBE5466CF34E90C6CULL, 
	0xC0AC29B7C97C50DDULL, 0x3F84D5B5B5470917ULL, 
	0x9216D5D98979FB1BULL, 0xD1310BA698DFB5ACULL, 
	0x2FFD72DBD01ADFB7ULL, 0xB8E1AFED6A267E96ULL, 
	0xBA7C9045F12C7F99ULL, 0x24A19947B3916CF7ULL, 
	0x0801F2E2858EFC16ULL, 0x636920D871574E69ULL
};

static const uint8_t padding[129U] = 
{
	0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
#endif
