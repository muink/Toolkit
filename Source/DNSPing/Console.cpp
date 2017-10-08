// This code is part of Toolkit(DNSPing)
// DNSPing, a useful and powerful toolkit
// Copyright (C) 2014-2017 Chengr28
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

#if defined(PLATFORM_WIN)
//Global variables
extern ConfigurationTable ConfigurationParameter;

//Catch Control-C exception from keyboard
BOOL WINAPI CtrlHandler(
	const DWORD ControlType)
{
//Print to screen.
	switch (ControlType)
	{
	//Handle the CTRL-C signal.
		case CTRL_C_EVENT:
		{
			fwprintf_s(stderr, L"[Notice] Get Control-C.\n");
			PrintProcess(true, true);
		}break;
	//Handle the CTRL-Break signal.
		case CTRL_BREAK_EVENT:
		{
			fwprintf_s(stderr, L"[Notice] Get Control-Break.\n");
			PrintProcess(true, true);

		//Exit process.
			return TRUE;
		}break;
	//Handle other signals.
		default:
		{
			fwprintf_s(stderr, L"[Notice] Get closing signal.\n");
			PrintProcess(true, true);
		}
	}

//Close file handle.
	if (ConfigurationParameter.OutputFile != nullptr)
	{
		fclose(ConfigurationParameter.OutputFile);
		ConfigurationParameter.OutputFile = nullptr;
	}

//Close all file handles and WinSock cleanup.
#if defined(PLATFORM_WIN)
	if (ConfigurationParameter.IsInitialized_WinSock)
	{
		WSACleanup();
		ConfigurationParameter.IsInitialized_WinSock = false;
	}

	_fcloseall();
#elif (defined(PLATFORM_LINUX) && !defined(PLATFORM_OPENWRT))
	fcloseall();
#endif

//Exit process.
//	exit(EXIT_SUCCESS);
	return FALSE;
}
#endif
