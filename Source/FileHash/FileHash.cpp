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


#include "FileHash.h"

//Main function of program
#if defined(PLATFORM_WIN)
int wmain(
	int argc, 
	wchar_t *argv[])
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
int main(
	int argc, 
	char *argv[])
#endif
{
#if defined(PLATFORM_WIN)
//Environment initialization
	if (!EnvironmentInitialization())
		return EXIT_FAILURE;
#endif

//Read commands.
#if defined(PLATFORM_WIN)
	std::wstring FileName, OutputFile;
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string FileName, OutputFile;
#endif
	if (argc < COMMAND_MIN_COUNT)
	{
		PrintDescription();
		return EXIT_SUCCESS;
	}
	else if (argc > COMMAND_MAX_COUNT)
	{
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return EXIT_FAILURE;
	}

//List all commands.
#if defined(PLATFORM_WIN)
	std::vector<std::wstring> CommandList;
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::vector<std::string> CommandList;
#endif
	for (size_t Index = 1U;static_cast<const int>(Index) < argc;++Index)
	{
		CommandList.push_back(argv[Index]);
		if (CommandList.at(Index - 1U).empty())
		{
			fwprintf_s(stderr, L"[Error] Commands error.\n");
			return EXIT_FAILURE;
		}
	}

//Read command process
	if (!ReadCommand(CommandList, FileName, OutputFile))
	{
		return EXIT_FAILURE;
	}
	else if (FileName.empty())
	{
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return EXIT_FAILURE;
	}
	else if (HashFamilyID == 0)
	{
		HashFamilyID = DEFAULT_HASH_ID;
	}

//Open file.
	FILE *FileHandle = nullptr;
	ssize_t ResultValue = 0;
#if defined(PLATFORM_WIN)
	ResultValue = _wfopen_s(&FileHandle, FileName.c_str(), L"rb");
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	errno = 0;
	FileHandle = fopen(FileName.c_str(), "rb");
#endif
	if (FileHandle == nullptr)
	{
	#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		ResultValue = errno;
	#endif
		std::wstring Message(L"[Error] Read file error");
		ErrorCodeToMessage(ResultValue, Message);
		if (ResultValue == 0)
			fwprintf_s(stderr, Message.c_str());
		else 
			fwprintf_s(stderr, Message.c_str(), ResultValue);

		return EXIT_FAILURE;
	}

//Open output file.
	FILE *OutputFileHandle = nullptr;
	if (!OutputFile.empty())
	{
	#if defined(PLATFORM_WIN)
		ResultValue = _wfopen_s(&OutputFileHandle, OutputFile.c_str(), L"a,ccs=UTF-8");
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		errno = 0;
		OutputFileHandle = fopen(OutputFile.c_str(), "a");
	#endif
		if (OutputFileHandle == nullptr)
		{
		#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			ResultValue = errno;
		#endif
			std::wstring Message(L"[Error] Read file error");
			ErrorCodeToMessage(ResultValue, Message);
			if (ResultValue == 0)
				fwprintf_s(stderr, Message.c_str());
			else 
				fwprintf_s(stderr, Message.c_str(), ResultValue);

		//Close all file and network handles.
			fclose(FileHandle);
		#if defined(PLATFORM_WIN)
			_fcloseall();
		#elif (defined(PLATFORM_FREEBSD) || (defined(PLATFORM_LINUX) && !defined(PLATFORM_OPENWRT)))
			fcloseall();
		#endif

			return EXIT_FAILURE;
		}
	}

//Main hash process
	if (!MainHashProcess(FileHandle, OutputFileHandle))
		return EXIT_FAILURE;

	return EXIT_SUCCESS;
}

#if defined(PLATFORM_WIN)
//Environment initialization process
bool EnvironmentInitialization(
	void)
{
//Screen output buffer settings
	_set_errno(0);
	if (setvbuf(stderr, nullptr, _IONBF, 0) != 0)
	{
		fwprintf_s(stderr, L"[Error] Screen output buffer setting error.\n");
		return false;
	}

//Set console codepage.
	if (SetConsoleCP(CP_UTF8) == 0 || SetConsoleOutputCP(CP_UTF8) == 0)
	{
		fwprintf_s(stderr, L"[Error] Set console codepage error.\n");
		return false;
	}

	return true;
}
#endif

//Read commands
bool ReadCommand(
#if defined(PLATFORM_WIN)
	std::vector<std::wstring> CommandList, 
	std::wstring &FileName, 
	std::wstring &OutputFile
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::vector<std::string> CommandList, 
	std::string &FileName, 
	std::string &OutputFile
#endif
)
{
//Command list check
	for (size_t Index = 0;Index < CommandList.size();++Index)
	{
	#if defined(PLATFORM_WIN)
		std::wstring InnerCommand(CommandList.at(Index));
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		std::string InnerCommand(CommandList.at(Index));
	#endif
		CaseConvert(InnerCommand, true);

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
	//Options check
		else if (InnerCommand == COMMAND_LOWERCASE) //Lowercase command
		{
			IsLowerCase = true;
		}
		else if (InnerCommand == COMMAND_OUTPUT_FILE) //Output hash result to file command
		{
		//Output command check
			if (Index + 1U >= CommandList.size())
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}
		//Mark output file name.
			else {
				++Index;

			//Path and file name check.
			#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (CommandList.at(Index).length() >= PATH_MAX)
				{
					fwprintf_s(stderr, L"[Error] Commands error.\n");
					return false;
				}
			#endif

				OutputFile = CommandList.at(Index);
			}
		}
	//Algorithms check
		else if (InnerCommand.find(HASH_COMMAND_BLAKE2) == 0) //BLAKE2 family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_BLAKE2;
			if (!ReadCommand_BLAKE2(InnerCommand))
				return false;
		}
		else if (InnerCommand.find(HASH_COMMAND_BLAKE) == 0) //BLAKE family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_BLAKE;
			if (!ReadCommand_BLAKE(InnerCommand))
				return false;
		}
		else if (InnerCommand.find(HASH_COMMAND_CRC) == 0) //CRC family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_CRC;
			if (!ReadCommand_CRC(InnerCommand))
				return false;
		}
		else if (InnerCommand == HASH_COMMAND_CHECKSUM) //Internet Protocol Checksum
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_CHECKSUM;
		}
		else if (InnerCommand == HASH_COMMAND_MD2) //MD2
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_MD2;
		}
		else if (InnerCommand == HASH_COMMAND_MD4) //MD4 family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_MD4;
		}
		else if (InnerCommand == HASH_COMMAND_ED2K) //MD4 family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_ED2K;
		}
		else if (InnerCommand == HASH_COMMAND_MD || InnerCommand == HASH_COMMAND_MD5) //MD5
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_MD5;
		}
		else if (InnerCommand.find(HASH_COMMAND_RIPEMD) == 0) //RIPEMD family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_RIPEMD;
			if (!ReadCommand_RIPEMD(InnerCommand))
				return false;
		}
		else if (InnerCommand == HASH_COMMAND_SHA1_UNDERLINE || InnerCommand.find(HASH_COMMAND_SHA1) == 0) //SHA-1
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_SHA1;
		}
		else if (InnerCommand == HASH_COMMAND_SHA2_384 || InnerCommand.find(HASH_COMMAND_SHA2_512) == 0 || 
			InnerCommand == HASH_COMMAND_SHA2_UNDERLINE || InnerCommand.find(HASH_COMMAND_SHA2) == 0) //SHA-2 family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_SHA2;
			if (!ReadCommand_SHA2(InnerCommand))
				return false;
		}
		else if (InnerCommand == HASH_COMMAND_SHA || InnerCommand == HASH_COMMAND_SHA3_UNDERLINE || 
			InnerCommand.find(HASH_COMMAND_SHA3) == 0) //SHA-3 family
		{
		//Hash family check
			if (HashFamilyID != 0)
			{
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}

		//Main process
			HashFamilyID = HASH_ID_SHA3;
			if (!ReadCommand_SHA3(InnerCommand))
				return false;
		}
	//Mark file name.
		else if (Index + 1U == CommandList.size())
		{
			FileName = CommandList.at(Index);
		}
	}

	return true;
}

//Main hash process
bool MainHashProcess(
	FILE * const FileHandle, 
	FILE * const OutputFile)
{
//Hash process
	auto IsHashSuccess = true;
	if ((HashFamilyID == HASH_ID_BLAKE && !BLAKE_Hash(FileHandle, OutputFile)) ||                                 //BLAKE family
		(HashFamilyID == HASH_ID_BLAKE2 && !BLAKE2_Hash(FileHandle, OutputFile)) ||                               //BLAKE2 family
		(HashFamilyID == HASH_ID_CRC && !CRC_Hash(FileHandle, OutputFile)) ||                                     //CRC family
		(HashFamilyID == HASH_ID_CHECKSUM && !Checksum_Hash(FileHandle, OutputFile)) ||                           //Internet Protocol Checksum
		(HashFamilyID == HASH_ID_MD2 && !MD2_Hash(FileHandle, OutputFile)) ||                                     //MD2
		((HashFamilyID == HASH_ID_MD4 || HashFamilyID == HASH_ID_ED2K) && !MD4_Hash(FileHandle, OutputFile)) ||   //MD4 family
		(HashFamilyID == HASH_ID_MD5 && !MD5_Hash(FileHandle, OutputFile)) ||                                     //MD5
		(HashFamilyID == HASH_ID_RIPEMD && !RIPEMD_Hash(FileHandle, OutputFile)) ||                               //RIPEMD family
		(HashFamilyID == HASH_ID_SHA1 && !SHA1_Hash(FileHandle, OutputFile)) ||                                   //SHA-1
		(HashFamilyID == HASH_ID_SHA2 && !SHA2_Hash(FileHandle, OutputFile)) ||                                   //SHA-2 family
		(HashFamilyID == HASH_ID_SHA3 && !SHA3_Hash(FileHandle, OutputFile)))                                     //SHA-3 family
			IsHashSuccess = false;

//Close all file and network handles.
	fclose(FileHandle);
	if (OutputFile != nullptr)
		fclose(OutputFile);
#if defined(PLATFORM_WIN)
	_fcloseall();
#elif (defined(PLATFORM_FREEBSD) || (defined(PLATFORM_LINUX) && !defined(PLATFORM_OPENWRT)))
	fcloseall();
#endif

	return IsHashSuccess;
}
