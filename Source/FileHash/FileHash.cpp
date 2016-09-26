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

//Global variables
size_t HashFamilyID = DEFAULT_HASH_ID;

//Main function of program
#if defined(PLATFORM_WIN)
int wmain(
	int argc, 
	wchar_t *argv[])
{
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
int main(
	int argc, 
	char *argv[])
{
#endif

//Initialization(Part 1)
#if defined(PLATFORM_WIN)
	std::wstring FileName;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	std::string FileName;
#endif

//Read commands.
	if (argc < 2)
	{
		PrintDescription();
		return EXIT_SUCCESS;
	}
	else if (argc == 2) //File name only, use default hash function.
	{
		if (!ReadCommands(argv[1U], true))
			return EXIT_FAILURE;
		else //Mark filename.
			FileName = argv[1U];
	}
	else if (argc == 3) //File name with hash function command
	{
		if (!ReadCommands(argv[1U], false))
			return EXIT_FAILURE;
		else //Mark filename.
			FileName = argv[2U];
	}
	else {
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return EXIT_FAILURE;
	}

//Open file.
	FILE *FileHandle = nullptr;
	ssize_t Result = 0;
#if defined(PLATFORM_WIN)
	Result = _wfopen_s(&FileHandle, FileName.c_str(), L"rb");
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	errno = 0;
	FileHandle = fopen(FileName.c_str(), "rb");
#endif
	if (FileHandle == nullptr)
	{
	#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		Result = errno;
	#endif
		std::wstring Message(L"[Error] Read file error");
		ErrorCodeToMessage(Result, Message);
		if (Result == 0)
			fwprintf_s(stderr, Message.c_str());
		else 
			fwprintf_s(stderr, Message.c_str(), Result);

		return EXIT_FAILURE;
	}
	else if (!MainHashProcess(FileHandle))
	{
		return EXIT_FAILURE;
	}
	else {
	//Close all file and network handles.
	#if defined(PLATFORM_WIN)
		_fcloseall();
	#elif (defined(PLATFORM_LINUX) && !defined(PLATFORM_OPENWRT))
		fcloseall();
	#endif
	}

	return EXIT_SUCCESS;
}

//Read commands
bool ReadCommands(
#if defined(PLATFORM_WIN)
	const wchar_t *Command, 
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	const char *Command, 
#endif
	const bool IsFileNameOnly)
{
//Initialization
#if defined(PLATFORM_WIN)
	std::wstring InnerCommand(Command);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	std::string InnerCommand(Command);
#endif
	CaseConvert(InnerCommand, true);

//Commands check
	if (IsFileNameOnly)
	{
	//Print version.
		if (InnerCommand == COMMAND_LONG_PRINT_VERSION || InnerCommand == COMMAND_SHORT_PRINT_VERSION)
		{
			fwprintf_s(stderr, L"FileHash ");
			fwprintf_s(stderr, FULL_VERSION);
			fwprintf_s(stderr, L"\n");

			return false;
		}
	//Print description.
		else if (InnerCommand == COMMAND_LONG_HELP || InnerCommand == COMMAND_SHORT_HELP || InnerCommand == COMMAND_SIGN_HELP)
		{
			PrintDescription();
			return false;
		}
	}
	else {
	//Command length check
		if (InnerCommand.length() < 3U)
		{
			fwprintf_s(stderr, L"[Error] Commands error.\n");
			return false;
		}

	//BLAKE2 family
		if (InnerCommand.find(HASH_COMMAND_BLAKE2) == 0)
		{
			HashFamilyID = HASH_ID_BLAKE2;
			if (!ReadCommands_BLAKE2(InnerCommand))
				return false;
		}
	//BLAKE family
		else if (InnerCommand.find(HASH_COMMAND_BLAKE) == 0)
		{
			HashFamilyID = HASH_ID_BLAKE;
			if (!ReadCommands_BLAKE(InnerCommand))
				return false;
		}
	//CRC family
		else if (InnerCommand.find(HASH_COMMAND_CRC) == 0)
		{
			HashFamilyID = HASH_ID_CRC;
			if (!ReadCommands_CRC(InnerCommand))
				return false;
		}
	//Internet Protocol Checksum
		else if (InnerCommand == HASH_COMMAND_CHECKSUM)
		{
			HashFamilyID = HASH_ID_CHECKSUM;
		}
	//MD2
		else if (InnerCommand == HASH_COMMAND_MD2)
		{
			HashFamilyID = HASH_ID_MD2;
		}
	//MD4 family
		else if (InnerCommand == HASH_COMMAND_MD4)
		{
			HashFamilyID = HASH_ID_MD4;
		}
		else if (InnerCommand == HASH_COMMAND_ED2K)
		{
			HashFamilyID = HASH_ID_ED2K;
		}
	//MD5
		else if (InnerCommand == HASH_COMMAND_MD || InnerCommand == HASH_COMMAND_MD5)
		{
			HashFamilyID = HASH_ID_MD5;
		}
	//RIPEMD family
		else if (InnerCommand.find(HASH_COMMAND_RIPEMD) == 0)
		{
			HashFamilyID = HASH_ID_RIPEMD;
			if (!ReadCommands_RIPEMD(InnerCommand))
				return false;
		}
	//SHA-1
		else if (InnerCommand == HASH_COMMAND_SHA1_UNDERLINE || InnerCommand.find(HASH_COMMAND_SHA1) == 0)
		{
			HashFamilyID = HASH_ID_SHA1;
		}
	//SHA-2 family
		else if (InnerCommand == HASH_COMMAND_SHA2_384 || InnerCommand.find(HASH_COMMAND_SHA2_512) == 0 || 
			InnerCommand == HASH_COMMAND_SHA2_UNDERLINE || InnerCommand.find(HASH_COMMAND_SHA2) == 0)
		{
			HashFamilyID = HASH_ID_SHA2;
			if (!ReadCommands_SHA2(InnerCommand))
				return false;
		}
	//SHA-3 family
		else if (InnerCommand == HASH_COMMAND_SHA || InnerCommand == HASH_COMMAND_SHA3_UNDERLINE || 
			InnerCommand.find(HASH_COMMAND_SHA3) == 0)
		{
			HashFamilyID = HASH_ID_SHA3;
			if (!ReadCommands_SHA3(InnerCommand))
				return false;
		}
	//Commands error
		else {
			fwprintf_s(stderr, L"[Error] Commands error.\n");
			return false;
		}
	}

	return true;
}

//Main hash process
bool MainHashProcess(
	FILE *FileHandle)
{
	if ((HashFamilyID == HASH_ID_BLAKE && !BLAKE_Hash(FileHandle)) ||                                 //BLAKE family
		(HashFamilyID == HASH_ID_BLAKE2 && !BLAKE2_Hash(FileHandle)) ||                               //BLAKE2 family
		(HashFamilyID == HASH_ID_CRC && !CRC_Hash(FileHandle)) ||                                     //CRC family
		(HashFamilyID == HASH_ID_CHECKSUM && !Checksum_Hash(FileHandle)) ||                           //Internet Protocol Checksum
		(HashFamilyID == HASH_ID_MD2 && !MD2_Hash(FileHandle)) ||                                     //MD2
		((HashFamilyID == HASH_ID_MD4 || HashFamilyID == HASH_ID_ED2K) && !MD4_Hash(FileHandle)) ||   //MD4 family
		(HashFamilyID == HASH_ID_MD5 && !MD5_Hash(FileHandle)) ||                                     //MD5
		(HashFamilyID == HASH_ID_RIPEMD && !RIPEMD_Hash(FileHandle)) ||                               //RIPEMD family
		(HashFamilyID == HASH_ID_SHA1 && !SHA1_Hash(FileHandle)) ||                                   //SHA-1
		(HashFamilyID == HASH_ID_SHA2 && !SHA2_Hash(FileHandle)) ||                                   //SHA-2 family
		(HashFamilyID == HASH_ID_SHA3 && !SHA3_Hash(FileHandle)))                                     //SHA-3 family
	{
		fclose(FileHandle);
		return false;
	}
	else {
		fclose(FileHandle);
	}

	return true;
}
