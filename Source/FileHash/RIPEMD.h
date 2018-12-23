// This code is part of Toolkit(FileHash)
// FileHash, a useful and powerful toolkit
// Copyright (C) 2012-2019 Chengr28
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


#ifndef TOOLKIT_FILEHASH_RIPEMD_H
#define TOOLKIT_FILEHASH_RIPEMD_H

#include "Include.h"

//Hash function ID
#define HASH_ID_RIPEMD_128          1U
#define HASH_ID_RIPEMD_160          2U
#define HASH_ID_RIPEMD_256          3U
#define HASH_ID_RIPEMD_320          4U
#define DEFAULT_HASH_FUNCTION_ID    HASH_ID_RIPEMD_160

//Size definitions
#define RIPEMD_DIGEST_SIZE_128      128U
#define RIPEMD_DIGEST_SIZE_160      160U
#define RIPEMD_DIGEST_SIZE_256      256U
#define RIPEMD_DIGEST_SIZE_320      320U
#define RIPEMD_DIGEST_MAXSIZE       RIPEMD_DIGEST_SIZE_320

//Commands definitions
#if defined(PLATFORM_WIN)
	#define COMMAND_RIPEMD_128          L"-RIPEMD_128"
	#define COMMAND_RIPEMD_160          L"-RIPEMD_160"
	#define COMMAND_RIPEMD_256          L"-RIPEMD_256"
	#define COMMAND_RIPEMD_320          L"-RIPEMD_320"
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define COMMAND_RIPEMD_128          ("-RIPEMD_128")
	#define COMMAND_RIPEMD_160          ("-RIPEMD_160")
	#define COMMAND_RIPEMD_256          ("-RIPEMD_256")
	#define COMMAND_RIPEMD_320          ("-RIPEMD_320")
#endif

//Global variables
extern size_t HashFamilyID;
extern bool IsLowerCase;
size_t RIPEMD_HashFunctionID = DEFAULT_HASH_FUNCTION_ID;

//RIPEMD definitions
typedef uint32_t dword;
typedef int the_correct_size_was_chosen[sizeof(dword) == 4 ? 1 : (-1)]; //If this line causes a compiler error, adapt the defintion of dword above

//Macro definitions
#define BYTES_TO_DWORD(strptr)                            \
	(((dword) *((strptr) + 3) << 24U) |                   \
	((dword) *((strptr) + 2) << 16U) |                    \
	((dword) *((strptr) + 1) << 8U) |                     \
	((dword) *(strptr)))

//ROL(x, n) cyclically rotates x over n bits to the left
#define ROL(x, n)             (((x) << (n)) | ((x) >> (32 - (n))))

//The four basic functions F_128(), G_128() and H_128()
#define F_128(x, y, z)        ((x) ^ (y) ^ (z))
#define G_128(x, y, z)        (((x) & (y)) | (~(x) & (z)))
#define H_128(x, y, z)        (((x) | ~(y)) ^ (z))
#define I_128(x, y, z)        (((x) & (z)) | ((y) & ~(z)))

//The eight basic operations FF_128() through III_128()
#define FF_128(a, b, c, d, x, s) {                        \
      (a) += F_128((b), (c), (d)) + (x);                  \
      (a) = ROL((a), (s));                                \
}
#define GG_128(a, b, c, d, x, s) {                        \
      (a) += G_128((b), (c), (d)) + (x) + 0x5A827999UL;   \
      (a) = ROL((a), (s));                                \
}
#define HH_128(a, b, c, d, x, s) {                        \
      (a) += H_128((b), (c), (d)) + (x) + 0x6ED9EBA1UL;   \
      (a) = ROL((a), (s));                                \
}
#define II_128(a, b, c, d, x, s) {                        \
      (a) += I_128((b), (c), (d)) + (x) + 0x8F1BBCDCUL;   \
      (a) = ROL((a), (s));                                \
}
#define FFF_128(a, b, c, d, x, s) {                       \
      (a) += F_128((b), (c), (d)) + (x);                  \
      (a) = ROL((a), (s));                                \
}
#define GGG_128(a, b, c, d, x, s) {                       \
      (a) += G_128((b), (c), (d)) + (x) + 0x6D703EF3UL;   \
      (a) = ROL((a), (s));                                \
}
#define HHH_128(a, b, c, d, x, s) {                       \
      (a) += H_128((b), (c), (d)) + (x) + 0x5C4DD124UL;   \
      (a) = ROL((a), (s));                                \
}
#define III_128(a, b, c, d, x, s) {                       \
      (a) += I_128((b), (c), (d)) + (x) + 0x50A28BE6UL;   \
      (a) = ROL((a), (s));                                \
}

//The five basic functions F_160(), G_160() and H_160()
#define F_160(x, y, z)        ((x) ^ (y) ^ (z))
#define G_160(x, y, z)        (((x) & (y)) | (~(x) & (z)))
#define H_160(x, y, z)        (((x) | ~(y)) ^ (z))
#define I_160(x, y, z)        (((x) & (z)) | ((y) & ~(z)))
#define J_160(x, y, z)        ((x) ^ ((y) | ~(z)))
  
//The ten basic operations FF_160() through III_160()
#define FF_160(a, b, c, d, e, x, s) {                     \
	(a) += F_160((b), (c), (d)) + (x);                    \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define GG_160(a, b, c, d, e, x, s) {                     \
	(a) += G_160((b), (c), (d)) + (x) + 0x5A827999UL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define HH_160(a, b, c, d, e, x, s) {                     \
	(a) += H_160((b), (c), (d)) + (x) + 0x6ED9EBA1UL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define II_160(a, b, c, d, e, x, s) {                     \
	(a) += I_160((b), (c), (d)) + (x) + 0x8F1BBCDCUL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define JJ_160(a, b, c, d, e, x, s) {                     \
	(a) += J_160((b), (c), (d)) + (x) + 0xA953FD4EUL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define FFF_160(a, b, c, d, e, x, s) {                    \
	(a) += F_160((b), (c), (d)) + (x);                    \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define GGG_160(a, b, c, d, e, x, s) {                    \
	(a) += G_160((b), (c), (d)) + (x) + 0x7A6D76E9UL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define HHH_160(a, b, c, d, e, x, s) {                    \
	(a) += H_160((b), (c), (d)) + (x) + 0x6D703EF3UL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define III_160(a, b, c, d, e, x, s) {                    \
	(a) += I_160((b), (c), (d)) + (x) + 0x5C4DD124UL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#define JJJ_160(a, b, c, d, e, x, s) {                    \
	(a) += J_160((b), (c), (d)) + (x) + 0x50A28BE6UL;     \
	(a) = ROL((a), (s)) + (e);                            \
	(c) = ROL((c), 10);                                   \
}
#endif
