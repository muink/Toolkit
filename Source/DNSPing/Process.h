// This code is part of Toolkit(DNSPing)
// A useful and powerful toolkit(DNSPing)
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


#ifndef TOOLKIT_DNSPING_PROCESS_H
#define TOOLKIT_DNSPING_PROCESS_H

#include "Base.h"

//Global variables
extern ConfigurationTable ConfigurationParameter;

//Functions
bool MarkProcessTime(
	const bool IsFinished, 
#if defined(PLATFORM_WIN)
	LARGE_INTEGER &CPU_Frequency, 
	LARGE_INTEGER &BeforeTime, 
	LARGE_INTEGER &AfterTime);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	timeval &BeforeTime, 
	timeval &AfterTime);
#endif
bool PrintSendResult(
	const SOCKET Socket_Normal, 
	const dns_hdr * const DNS_Header, 
	std::shared_ptr<uint8_t> &RecvBuffer, 
	ssize_t &DataLength, 
	long double &Result, 
	bool &IsContinue, 
#if defined(PLATFORM_WIN)
	LARGE_INTEGER &CPU_Frequency, 
	LARGE_INTEGER &BeforeTime, 
	LARGE_INTEGER &AfterTime);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	timeval &BeforeTime, 
	timeval &AfterTime);
#endif
void ErrorCodeToMessage(
	const ssize_t ErrorCode, 
	std::wstring &Message);
#endif
