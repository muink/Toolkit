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


#ifndef TOOLKIT_FILEHASH_INCLUDE_H
#define TOOLKIT_FILEHASH_INCLUDE_H

#include "Definition.h"

//////////////////////////////////////////////////
// Main functions
// 
//Base.h
bool CheckEmptyBuffer(
	const void * const Buffer, 
	const size_t Length);
bool MBS_To_WCS_String(
	const uint8_t * const Buffer, 
	const size_t MaxLen, 
	std::wstring &Target);
void CaseConvert(
	std::string &Buffer, 
	const bool IsLowerToUpper);
void CaseConvert(
	std::wstring &Buffer, 
	const bool IsLowerToUpper);
uint8_t *sodium_bin2hex(
	uint8_t * const hex, 
	const size_t hex_maxlen, 
	const uint8_t * const bin, 
	const size_t bin_len);
void ErrorCodeToMessage(
	const ssize_t ErrorCode, 
	std::wstring &Message);
void WriteMessage_ScreenFile(
	const FILE * const FileHandle, 
	const uint8_t * const Message);
void PrintDescription(
	void);

//BLAKE.h
bool ReadCommand_BLAKE(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool BLAKE_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//BLAKE2.h
bool ReadCommand_BLAKE2(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool BLAKE2_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//Checksum.h
bool Checksum_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//CRC.h
bool ReadCommand_CRC(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool CRC_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//MD2.h
bool MD2_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//MD4.h
bool MD4_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//MD5.h
bool MD5_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//RIPEMD.h
bool ReadCommand_RIPEMD(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool RIPEMD_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//SHA-1.h
bool SHA1_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//SHA-2.h
bool SHA2_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);
bool ReadCommand_SHA2(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif

//SHA-3.h
bool SHA3_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);
bool ReadCommand_SHA3(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
#endif
