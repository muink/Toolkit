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


#ifndef TOOLKIT_FILEHASH_FILEHASH_H
#define TOOLKIT_FILEHASH_FILEHASH_H

#include "Include.h"

//Global variables
size_t HashFamilyID = 0;
auto IsLowerCase = false;

//Functions
bool ReadCommand(
#if defined(PLATFORM_WIN)
	std::vector<std::wstring> CommandList, 
	std::wstring &FileName, 
	std::wstring &OutputFile);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::vector<std::string> CommandList, 
	std::string &FileName, 
	std::string &OutputFile);
#endif
bool MainHashProcess(
	FILE * const FileHandle, 
	FILE * const OutputFile);
#endif
