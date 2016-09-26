// This code is part of Toolkit(FileHash)
// A useful and powerful toolkit(FileHash)
// Copyright (C) 2012-2016 Chengr28
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


#include "Base.h"

//Hash function ID
#define HASH_ID_BLAKE2B_160         1U
#define HASH_ID_BLAKE2B_256         2U
#define HASH_ID_BLAKE2B_384         3U
#define HASH_ID_BLAKE2B_512         4U
#define HASH_ID_BLAKE2B             HASH_ID_BLAKE2B_256
#define HASH_ID_BLAKE2S_128         5U
#define HASH_ID_BLAKE2S_160         6U
#define HASH_ID_BLAKE2S_224         7U
#define HASH_ID_BLAKE2S_256         8U
#define HASH_ID_BLAKE2S             HASH_ID_BLAKE2S_256
#define DEFAULT_HASH_FUNCTION_ID    HASH_ID_BLAKE2B_256

//Size definitions
#define BLAKE2_DIGEST_SIZE_128      128U
#define BLAKE2_DIGEST_SIZE_160      160U
#define BLAKE2_DIGEST_SIZE_224      224U
#define BLAKE2_DIGEST_SIZE_256      256U
#define BLAKE2_DIGEST_SIZE_384      384U
#define BLAKE2_DIGEST_SIZE_512      512U

//Commands definitions
#if defined(PLATFORM_WIN)
	#define COMMAND_BLAKE2B              L"-BLAKE2B"
	#define COMMAND_BLAKE2_128           L"-BLAKE2_128"
	#define COMMAND_BLAKE2_224           L"-BLAKE2_224"
	#define COMMAND_BLAKE2_384           L"-BLAKE2_384"
	#define COMMAND_BLAKE2_512           L"-BLAKE2_512"
	#define COMMAND_BLAKE2B_160          L"-BLAKE2B_160"
	#define COMMAND_BLAKE2B_256          L"-BLAKE2B_256"
	#define COMMAND_BLAKE2B_384          L"-BLAKE2B_384"
	#define COMMAND_BLAKE2B_512          L"-BLAKE2B_512"
	#define COMMAND_BLAKE2S              L"-BLAKE2S"
	#define COMMAND_BLAKE2S_128          L"-BLAKE2S_128"
	#define COMMAND_BLAKE2S_160          L"-BLAKE2S_160"
	#define COMMAND_BLAKE2S_224          L"-BLAKE2S_224"
	#define COMMAND_BLAKE2S_256          L"-BLAKE2S_256"
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	#define COMMAND_BLAKE2B              ("-BLAKE2B")
	#define COMMAND_BLAKE2_128           ("-BLAKE2_128")
	#define COMMAND_BLAKE2_224           ("-BLAKE2_224")
	#define COMMAND_BLAKE2_384           ("-BLAKE2_384")
	#define COMMAND_BLAKE2_512           ("-BLAKE2_512")
	#define COMMAND_BLAKE2B_160          ("-BLAKE2B_160")
	#define COMMAND_BLAKE2B_256          ("-BLAKE2B_256")
	#define COMMAND_BLAKE2B_384          ("-BLAKE2B_384")
	#define COMMAND_BLAKE2B_512          ("-BLAKE2B_512")
	#define COMMAND_BLAKE2S              ("-BLAKE2S")
	#define COMMAND_BLAKE2S_128          ("-BLAKE2S_128")
	#define COMMAND_BLAKE2S_160          ("-BLAKE2S_160")
	#define COMMAND_BLAKE2S_224          ("-BLAKE2S_224")
	#define COMMAND_BLAKE2S_256          ("-BLAKE2S_256")
#endif

//Global variables
extern size_t HashFamilyID;
size_t BLAKE2_HashFunctionID = DEFAULT_HASH_FUNCTION_ID;

//BLAKE2b Hashing Context and API Prototypes
// State context
typedef struct
{
	uint8_t b[128U];                    // Input buffer
	uint64_t h[8U];                     // Chained state
	uint64_t t[2U];                     // Total number of bytes
	size_t c;                           // Pointer for b[]
	size_t outlen;                      // Digest size
}blake2b_ctx;

// Initialize the hashing context "ctx" with optional key "key".
// 1 <= outlen <= 64 gives the digest size in bytes.
// Secret key (also <= 64 bytes) is optional (keylen = 0).
int blake2b_init(
	blake2b_ctx *ctx, 
	size_t outlen, 
	const void *key, 
	size_t keylen);                     // Secret key

// Add "inlen" bytes from "in" into the hash.
void blake2b_update(
	blake2b_ctx *ctx,                   // Context
	const void *in, 
	size_t inlen);                      // Data to be hashed

// Generate the message digest(size given in init).
// Result placed in "out".
void blake2b_final(
	blake2b_ctx *ctx, 
	void *out);

//BLAKE2s Hashing Context and API Prototypes
// State context
typedef struct
{
	uint8_t b[64U];                     // Input buffer
	uint32_t h[8U];                     // Chained state
	uint32_t t[2U];                     // Total number of bytes
	size_t c;                           // Pointer for b[]
	size_t outlen;                      // Digest size
} blake2s_ctx;

// Initialize the hashing context "ctx" with optional key "key".
// 1 <= outlen <= 32 gives the digest size in bytes.
// Secret key (also <= 32 bytes) is optional (keylen = 0).
int blake2s_init(
	blake2s_ctx *ctx, 
	size_t outlen, 
	const void *key, 
	size_t keylen);                     // Secret key

// Add "inlen" bytes from "in" into the hash.
void blake2s_update(
	blake2s_ctx *ctx,                   // Context
	const void *in, 
	size_t inlen);                      // Data to be hashed

// Generate the message digest (size given in init).
// Result placed in "out".
void blake2s_final(
	blake2s_ctx *ctx, 
	void *out);
