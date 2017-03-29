// This code is part of Toolkit(FileHash)
// FileHash, a useful and powerful toolkit
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


#ifndef TOOLKIT_FILEHASH_SHA_2_H
#define TOOLKIT_FILEHASH_SHA_2_H

#include "Base.h"

//Hash function ID
#define HASH_ID_SHA2_224           1U
#define HASH_ID_SHA2_256           2U
#define HASH_ID_SHA2_384           3U
#define HASH_ID_SHA2_512           4U
#define HASH_ID_SHA2_512_224       5U
#define HASH_ID_SHA2_512_256       6U
#define DEFAULT_HASH_FUNCTION_ID   HASH_ID_SHA2_256

//Size definitions
#define SHA2_DIGEST_SIZE_224       224U
#define SHA2_DIGEST_SIZE_256       256U
#define SHA2_DIGEST_SIZE_384       384U
#define SHA2_DIGEST_SIZE_512       512U
#define SHA2_DIGEST_SIZE_512_224   224U
#define SHA2_DIGEST_SIZE_512_256   256U

//Commands definitions
#if defined(PLATFORM_WIN)
	#define COMMAND_SHA2_224           L"-SHA224"
	#define COMMAND_SHA2_256           L"-SHA256"
	#define COMMAND_SHA2_512_224       L"-SHA512_224"
	#define COMMAND_SHA2_512_256       L"-SHA512_256"
	#define COMMAND_SHA2_224_UL        L"-SHA2_224"
	#define COMMAND_SHA2_256_UL        L"-SHA2_256"
	#define COMMAND_SHA2_384_UL        L"-SHA2_384"
	#define COMMAND_SHA2_512_UL        L"-SHA2_512"
	#define COMMAND_SHA2_512_224_UL    L"-SHA2_512_224"
	#define COMMAND_SHA2_512_256_UL    L"-SHA2_512_256"
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define COMMAND_SHA2_224           ("-SHA224")
	#define COMMAND_SHA2_256           ("-SHA256")
	#define COMMAND_SHA2_512_224       ("-SHA512_224")
	#define COMMAND_SHA2_512_256       ("-SHA512_256")
	#define COMMAND_SHA2_224_UL        ("-SHA2_224")
	#define COMMAND_SHA2_256_UL        ("-SHA2_256")
	#define COMMAND_SHA2_384_UL        ("-SHA2_384")
	#define COMMAND_SHA2_512_UL        ("-SHA2_512")
	#define COMMAND_SHA2_512_224_UL    ("-SHA2_512_224")
	#define COMMAND_SHA2_512_256_UL    ("-SHA2_512_256")
#endif

//Global variables
extern size_t HashFamilyID;
extern bool IsLowerCase;
size_t SHA2_HashFunctionID = DEFAULT_HASH_FUNCTION_ID;

//Endianness testing and definitions
#define TestEndianness(variable) {                   \
				int i = 1;variable = PCT_BIG_ENDIAN; \
				if (*((uint8_t *) & i) == 1)         \
				variable = PCT_LITTLE_ENDIAN;        \
			}
#define PCT_LITTLE_ENDIAN        1
#define PCT_BIG_ENDIAN           0

//Some useful types
#ifndef Py_LL
	#define Py_LL(x) x ## LL
#endif
#ifndef Py_ULL
	#define Py_ULL(x) Py_LL(x ## U)
#endif

//The SHA-2(256) block size, message digest size in bytes and some useful types.
#define SHA2_BLOCK_SIZE_256        64U
#define SHA2_BLOCK_SIZE_512        128U
typedef uint8_t                    SHA2_256_BYTE;
typedef uint8_t                    SHA2_512_BYTE;
typedef uint32_t                   SHA2_INT32;
typedef uint64_t                   SHA2_INT64;

//The structure for storing SHA info
typedef struct _sha2_256_object_
{
	SHA2_INT32      Digest[8U];                  //Message digest
	SHA2_INT32      CountLow, CountHigh;         //64-bit bit count
	SHA2_256_BYTE   Data[SHA2_BLOCK_SIZE_256];   //SHA data buffer
	int             Endianness;
	int             Local;                       //Unprocessed amount in data
	int             DigestSize;
}SHA2_256_Object;

//The structure for storing SHA info
typedef struct _sha2_512_object_
{
	SHA2_INT64      Digest[8U];                  //Message digest
	SHA2_INT32      CountLow, CountHigh;         //64-bit bit count
	SHA2_512_BYTE   Data[SHA2_BLOCK_SIZE_512];   //SHA data buffer
	int             Endianness;
	int             Local;                       //Unprocessed amount in data
	int             DigestSize;
}SHA2_512_Object;
#endif
