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


#include "SHA-3.h"

//////////////////////////////////////////////////
// Hash function
// 
//Read commands(SHA-3)
bool ReadCommand_SHA3(
#if defined(PLATFORM_WIN)
	std::wstring &Command)
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command)
#endif
{
//Hash function check
	if (Command == HASH_COMMAND_SHA) //Default SHA command is SHA-3 256 bits
	{
		return true;
	}
	else if (Command == COMMAND_SHA3_224) //SHA-3 224 bits
	{
		SHA3_HashFunctionID = HASH_ID_SHA3_224;
	}
	else if (Command == HASH_COMMAND_SHA3 || Command == HASH_COMMAND_SHA3_UNDERLINE || 
		Command == COMMAND_SHA3_256) //SHA-3 256 bits
	{
		SHA3_HashFunctionID = HASH_ID_SHA3_256;
	}
	else if (Command == COMMAND_SHA3_384) //SHA-3 384 bits
	{
		SHA3_HashFunctionID = HASH_ID_SHA3_384;
	}
	else if (Command == COMMAND_SHA3_512) //SHA-3 512 bits
	{
		SHA3_HashFunctionID = HASH_ID_SHA3_512;
	}
	else if (Command.find(COMMAND_SHA3_SHAKE) == 0) //SHA-3 SHAKE family
	{
	//Default command is SHA-3 SHAKE 128 bits and output 256 bits.
		if (Command == COMMAND_SHA3_SHAKE)
		{
			SHA3_HashFunctionID = HASH_ID_SHA3_SHAKE_128;
			SHA3_SHAKE_Length = SHA3_DIGEST_SIZE_256;
		}
	//SHA-3 SHAKE 128 bits and default command is SHA-3 SHAKE 128 bits and output 256 bits..
		else if (Command.find(COMMAND_SHA3_SHAKE_SIZE) == 0 || Command.find(COMMAND_SHA3_SHAKE_128) == 0)
		{
		//Initialization
			size_t Offset = 0;
			if (Command.find(COMMAND_SHA3_SHAKE_SIZE) == 0)
			#if defined(PLATFORM_WIN)
				Offset = wcslen(COMMAND_SHA3_SHAKE_SIZE);
			#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				Offset = strlen(COMMAND_SHA3_SHAKE_SIZE);
			#endif
			else 
			#if defined(PLATFORM_WIN)
				Offset = wcslen(COMMAND_SHA3_SHAKE_128);
			#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				Offset = strlen(COMMAND_SHA3_SHAKE_128);
			#endif
			SHA3_HashFunctionID = HASH_ID_SHA3_SHAKE_128;

		//Format check
			if (Command.find(ASCII_MINUS) != std::string::npos)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//SHA-3 SHAKE output length.
			_set_errno(0);
		#if defined(PLATFORM_WIN)
			auto ResultValue = wcstoul(Command.c_str() + Offset, nullptr, 0);
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			auto ResultValue = strtoul(Command.c_str() + Offset, nullptr, 0);
		#endif
			if (ResultValue >= FILE_BUFFER_SIZE)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}
			else {
				SHA3_SHAKE_Length = ResultValue;
			}
		}
	//SHA-3 SHAKE 256 bits
		else if (Command.find(COMMAND_SHA3_SHAKE_256) == 0)
		{
			SHA3_HashFunctionID = HASH_ID_SHA3_SHAKE_256;

		//Format check
			if (Command.find(ASCII_MINUS) != std::string::npos)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//SHA-3 SHAKE output length.
			_set_errno(0);
		#if defined(PLATFORM_WIN)
			auto ResultValue = wcstoul(Command.c_str() + wcslen(COMMAND_SHA3_SHAKE_256), nullptr, 0);
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			auto ResultValue = strtoul(Command.c_str() + strlen(COMMAND_SHA3_SHAKE_256), nullptr, 0);
		#endif
			if (ResultValue >= FILE_BUFFER_SIZE)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}
			else {
				SHA3_SHAKE_Length = ResultValue;
			}
		}
	//Commands error
		else {
			fwprintf_s(stderr, L"[Error] Commands error.\n");
			return false;
		}
	}
	else { //Commands error
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return false;
	}

	return true;
}

//SHA-3 hash function
bool SHA3_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile)
{
//Parameters check
	if (HashFamilyID != HASH_ID_SHA3 || FileHandle == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Parameters error.\n");
		return false;
	}

//Initialization
	auto Buffer = std::make_unique<uint8_t[]>(FILE_BUFFER_SIZE + MEMORY_RESERVED_BYTES);
	auto StringBuffer = std::make_unique<uint8_t[]>(FILE_BUFFER_SIZE + MEMORY_RESERVED_BYTES);
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE + MEMORY_RESERVED_BYTES);
	memset(StringBuffer.get(), 0, FILE_BUFFER_SIZE + MEMORY_RESERVED_BYTES);
	size_t ReadLength = 0, DigestSize = 0;

//SHA-3 initialization
	Keccak_HashInstance HashInstance;
	memset(&HashInstance, 0, sizeof(HashInstance));
	if (SHA3_HashFunctionID == HASH_ID_SHA3_224)
	{
		Keccak_HashInitialize_SHA3_224(&HashInstance);
		DigestSize = SHA3_DIGEST_SIZE_224;
	}
	else if (SHA3_HashFunctionID == HASH_ID_SHA3_256)
	{
		Keccak_HashInitialize_SHA3_256(&HashInstance);
		DigestSize = SHA3_DIGEST_SIZE_256;
	}
	else if (SHA3_HashFunctionID == HASH_ID_SHA3_384)
	{
		Keccak_HashInitialize_SHA3_384(&HashInstance);
		DigestSize = SHA3_DIGEST_SIZE_384;
	}
	else if (SHA3_HashFunctionID == HASH_ID_SHA3_512)
	{
		Keccak_HashInitialize_SHA3_512(&HashInstance);
		DigestSize = SHA3_DIGEST_SIZE_512;
	}
	else if (SHA3_HashFunctionID == HASH_ID_SHA3_SHAKE_128)
	{
		Keccak_HashInitialize_SHAKE128(&HashInstance);
		DigestSize = SHA3_SHAKE_Length;
	}
	else if (SHA3_HashFunctionID == HASH_ID_SHA3_SHAKE_256)
	{
		Keccak_HashInitialize_SHAKE256(&HashInstance);
		DigestSize = SHA3_SHAKE_Length;
	}
	else {
		fwprintf_s(stderr, L"[Error] Parameters error.\n");
		return false;
	}

//Hash process
	while (!feof(FileHandle))
	{
		memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
		_set_errno(0);
		ReadLength = fread_s(Buffer.get(), FILE_BUFFER_SIZE, sizeof(uint8_t), FILE_BUFFER_SIZE, FileHandle);
		if (ReadLength == 0 && errno != 0)
		{
			std::wstring Message(L"[Error] Read file error");
			ErrorCodeToMessage(errno, Message);
			if (errno == 0)
				fwprintf_s(stderr, Message.c_str());
			else 
				fwprintf_s(stderr, Message.c_str(), errno);

			return false;
		}
		else if (Keccak_HashUpdate(&HashInstance, reinterpret_cast<const BitSequence *>(Buffer.get()), ReadLength * BYTES_TO_BITS) != SUCCESS)
		{
			fwprintf_s(stderr, L"[Error] Hash process error");
			return false;
		}
	}

//Finish hash process.
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
	if (Keccak_HashFinal(&HashInstance, reinterpret_cast<BitSequence *>(Buffer.get())) != SUCCESS)
	{
		fwprintf_s(stderr, L"[Error] Hash process error");
		return false;
	}
	else if ((SHA3_HashFunctionID == HASH_ID_SHA3_SHAKE_128 || SHA3_HashFunctionID == HASH_ID_SHA3_SHAKE_256) && 
		Keccak_HashSqueeze(&HashInstance, reinterpret_cast<BitSequence *>(Buffer.get()), SHA3_SHAKE_Length) != SUCCESS)
	{
		fwprintf_s(stderr, L"[Error] Hash squeeze error.\n");
		return false;
	}

//Binary to hex
	if (sodium_bin2hex(StringBuffer.get(), FILE_BUFFER_SIZE, Buffer.get(), DigestSize / BYTES_TO_BITS) == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Convert binary to hex error.\n");
		return false;
	}
	else {
	//Lowercase convert.
		std::string Hex(reinterpret_cast<const char *>(StringBuffer.get()));
		if (!IsLowerCase)
			CaseConvert(Hex, true);

	//Print to screen and file.
		WriteMessage_ScreenFile(stderr, reinterpret_cast<const uint8_t *>(Hex.c_str()));
		if (OutputFile != nullptr)
			WriteMessage_ScreenFile(OutputFile, reinterpret_cast<const uint8_t *>(Hex.c_str()));
	}

	return true;
}
