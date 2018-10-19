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


#ifndef TOOLKIT_FILEHASH_DEFINITION_H
#define TOOLKIT_FILEHASH_DEFINITION_H

#include "Platform.h"

//////////////////////////////////////////////////
// Main definitions
// 
//Code definitions
#define BYTES_TO_BITS                                 8U
#define DEFAULT_LARGE_BUFFER_SIZE                     4096U                       //Default size of large buffer(4KB/4096 bytes)
#define FILE_BUFFER_SIZE                              DEFAULT_LARGE_BUFFER_SIZE   //Maximum size of file buffer(4KB/4096 bytes)
#if defined(PLATFORM_WIN)
	#define MBSTOWCS_NULL_TERMINATE                       (-1)                        //MultiByteToWideChar function find null-terminate.
#endif
#define NULL_TERMINATE_LENGTH                         1U                          //Length of C style string null
#define MEMORY_RESERVED_BYTES                         2U                          //Padding reserved bytes(2 bytes)

//Character value definitions
#define ASCII_SPACE                                   32                          //" "
#define ASCII_MINUS                                   45                          //"-"
#define ASCII_PERIOD                                  46                          //"."

//Version definitions
#define FULL_VERSION                                  L"0.3.9.3"
#define COPYRIGHT_MESSAGE                             L"Copyright (C) 2012-2018 Chengr28"

//Command definitions
#define COMMAND_MIN_COUNT                             2
#define COMMAND_MAX_COUNT                             6
#if defined(PLATFORM_WIN)
	#define COMMAND_LONG_PRINT_VERSION                    L"--VERSION"
	#define COMMAND_SHORT_PRINT_VERSION                   L"-V"
	#define COMMAND_LONG_HELP                             L"--HELP"
	#define COMMAND_SHORT_HELP                            L"-H"
	#define COMMAND_SIGN_HELP                             L"-?"
	#define COMMAND_LOWERCASE                             L"--LOWERCASE"
	#define COMMAND_OUTPUT_FILE                           L"--OUTPUT"
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define COMMAND_LONG_PRINT_VERSION                    ("--VERSION")
	#define COMMAND_SHORT_PRINT_VERSION                   ("-V")
	#define COMMAND_LONG_HELP                             ("--HELP")
	#define COMMAND_SHORT_HELP                            ("-H")
	#define COMMAND_SIGN_HELP                             ("-?")
	#define COMMAND_LOWERCASE                             ("--LOWERCASE")
	#define COMMAND_OUTPUT_FILE                           ("--OUTPUT")
#endif

//Hash definitions
#define HASH_ID_BLAKE                                 1U
#define HASH_ID_BLAKE2                                2U
#define HASH_ID_CRC                                   3U
#define HASH_ID_CHECKSUM                              4U
#define HASH_ID_MD2                                   5U
#define HASH_ID_MD4                                   6U
#define HASH_ID_ED2K                                  7U
#define HASH_ID_MD5                                   8U
#define HASH_ID_MD                                    HASH_ID_MD5
#define HASH_ID_RIPEMD                                9U
#define HASH_ID_SHA1                                  10U
#define HASH_ID_SHA2                                  11U
#define HASH_ID_SHA3                                  12U
#define HASH_ID_SHA                                   HASH_ID_SHA3
#define DEFAULT_HASH_ID                               HASH_ID_SHA3
#if defined(PLATFORM_WIN)
	#define HASH_COMMAND_BLAKE                            L"-BLAKE"
	#define HASH_COMMAND_BLAKE2                           L"-BLAKE2"
	#define HASH_COMMAND_CRC                              L"-CRC"
	#define HASH_COMMAND_CHECKSUM                         L"-CHECKSUM"
	#define HASH_COMMAND_MD                               L"-MD"
	#define HASH_COMMAND_MD2                              L"-MD2"
	#define HASH_COMMAND_MD4                              L"-MD4"
	#define HASH_COMMAND_ED2K                             L"-ED2K"
	#define HASH_COMMAND_MD5                              L"-MD5"
	#define HASH_COMMAND_RIPEMD                           L"-RIPEMD"
	#define HASH_COMMAND_SHA                              L"-SHA"
	#define HASH_COMMAND_SHA1                             L"-SHA1"
	#define HASH_COMMAND_SHA1_UNDERLINE                   L"-SHA_1"
	#define HASH_COMMAND_SHA2                             L"-SHA2"
	#define HASH_COMMAND_SHA2_UNDERLINE                   L"-SHA_2"
	#define HASH_COMMAND_SHA2_256                         L"-SHA256"
	#define HASH_COMMAND_SHA2_384                         L"-SHA384"
	#define HASH_COMMAND_SHA2_512                         L"-SHA512"
	#define HASH_COMMAND_SHA3                             L"-SHA3"
	#define HASH_COMMAND_SHA3_UNDERLINE                   L"-SHA_3"
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define HASH_COMMAND_BLAKE                            ("-BLAKE")
	#define HASH_COMMAND_BLAKE2                           ("-BLAKE2")
	#define HASH_COMMAND_CRC                              ("-CRC")
	#define HASH_COMMAND_CHECKSUM                         ("-CHECKSUM")
	#define HASH_COMMAND_MD                               ("-MD")
	#define HASH_COMMAND_MD2                              ("-MD2")
	#define HASH_COMMAND_MD4                              ("-MD4")
	#define HASH_COMMAND_ED2K                             ("-ED2K")
	#define HASH_COMMAND_MD5                              ("-MD5")
	#define HASH_COMMAND_RIPEMD                           ("-RIPEMD")
	#define HASH_COMMAND_SHA                              ("-SHA")
	#define HASH_COMMAND_SHA1                             ("-SHA1")
	#define HASH_COMMAND_SHA1_UNDERLINE                   ("-SHA_1")
	#define HASH_COMMAND_SHA2                             ("-SHA2")
	#define HASH_COMMAND_SHA2_UNDERLINE                   ("-SHA_2")
	#define HASH_COMMAND_SHA2_256                         ("-SHA256")
	#define HASH_COMMAND_SHA2_384                         ("-SHA384")
	#define HASH_COMMAND_SHA2_512                         ("-SHA512")
	#define HASH_COMMAND_SHA3                             ("-SHA3")
	#define HASH_COMMAND_SHA3_UNDERLINE                   ("-SHA_3")
#endif

//Function definitions
#define hton16_Force(Value)                           (static_cast<const uint16_t>((reinterpret_cast<const uint8_t *>(&(Value)))[0] <<                                      \
														(sizeof(uint8_t) * BYTES_TO_BITS) | (reinterpret_cast<const uint8_t *>(&(Value)))[1U]))
#define ntoh16_Force                                  hton16_Force
#define hton16                                        htons
#define ntoh16                                        ntohs
#define hton32_Force(Value)                           (static_cast<const uint32_t>((reinterpret_cast<const uint8_t *>(&(Value)))[0] <<                                      \
														((sizeof(uint16_t) + sizeof(uint8_t)) * BYTES_TO_BITS) |                                                            \
														(reinterpret_cast<const uint8_t *>(&(Value)))[1U] << (sizeof(uint16_t) * BYTES_TO_BITS) |                           \
														(reinterpret_cast<const uint8_t *>(&(Value)))[2U] << (sizeof(uint8_t) * BYTES_TO_BITS) |                            \
														(reinterpret_cast<const uint8_t *>(&(Value)))[3U]))
#define ntoh32_Force                                  hton32_Force
#define hton32                                        htonl
#define ntoh32                                        ntohl
#define hton64_Force(Value)                           (static_cast<const uint64_t>(((static_cast<const uint64_t>(hton32(static_cast<const uint32_t>                         \
														(((Value) << (sizeof(uint32_t) * BYTES_TO_BITS)) >>                                                                 \
														(sizeof(uint32_t) * BYTES_TO_BITS))))) <<                                                                           \
														(sizeof(uint32_t) * BYTES_TO_BITS)) | static_cast<const uint32_t>(hton32(static_cast<const uint32_t>((Value) >>     \
														(sizeof(uint32_t) * BYTES_TO_BITS))))))
#define ntoh64_Force                                  hton64_Force
#if BYTE_ORDER == LITTLE_ENDIAN
	#define hton64(Value)                                 hton64_Force(Value)
#else //BIG_ENDIAN
	#define hton64(Value)                                 (Value)
#endif
#define ntoh64                                        hton64
#endif
