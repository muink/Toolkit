// This code is part of Toolkit(DNSPing)
// DNSPing, a useful and powerful toolkit
// Copyright (C) 2014-2019 Chengr28
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


#ifndef TOOLKIT_DNSPING_INCLUDE_H
#define TOOLKIT_DNSPING_INCLUDE_H

#include "Type.h"

//////////////////////////////////////////////////
// Main functions
// 
//Console.h
#if defined(PLATFORM_WIN)
BOOL WINAPI SignalHandler(
	const DWORD SignalType);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
//Terminal.h
void SignalHandler(
	const int SignalType);
#endif

//Process.h
bool SendRequestProcess(
	const sockaddr_storage &Target, 
	const bool IsLastSend);
void PrintProcess(
	const bool IsPacketStatistics, 
	const bool IsTimeStatistics);
bool OutputResultToFile(
	void);
void PrintHeaderToScreen(
	const std::wstring WideTargetAddressString, 
	const std::wstring WideTestDomainString);
void PrintErrorToScreen(
	const wchar_t * const Message, 
	const ssize_t ErrorCode);
void PrintDescription(
	void);

//Protocol.h
bool CheckEmptyBuffer(
	const void * const Buffer, 
	const size_t Length);
bool MBS_To_WCS_String(
	const uint8_t * const Buffer, 
	const size_t MaxLen, 
	std::wstring &Target);
bool WCS_To_MBS_String(
	const wchar_t * const Buffer, 
	const size_t MaxLen, 
	std::string &Target);
void CaseConvert(
	const bool IsLowerUpper, 
	uint8_t * const Buffer, 
	const size_t Length);
void CaseConvert(
	std::wstring &Buffer, 
	const bool IsLowerToUpper);
bool AddressStringToBinary(
	const uint16_t Protocol, 
	const uint8_t * const AddrBuffer, 
	void * const OriginalAddr, 
	ssize_t * const ErrorCode);
bool BinaryToAddressString(
	const uint16_t Protocol, 
	const void * const OriginalAddr, 
	void * const AddressString, 
	const size_t StringSize, 
	ssize_t * const ErrorCode);
uint16_t ProtocolNameToBinary(
	const std::wstring &Buffer);
uint16_t ServiceNameToBinary(
	const std::wstring &Buffer);
uint16_t DNSClassesNameToBinary(
	const std::wstring &Buffer);
uint16_t DNSTypeNameToBinary(
	const std::wstring &Buffer);
size_t StringToPacketQuery(
	const uint8_t * const FName, 
	uint8_t * const TName);
size_t PacketQueryToString(
	const uint8_t * const TName, 
	uint8_t * const FName, 
	uint16_t &TruncateLocation);
bool ValidatePacket(
	const uint8_t * const Buffer, 
	const size_t Length, 
	const uint16_t DNS_ID);
void PrintSecondsInDateTime(
	FILE * const FileHandle, 
	const time_t Seconds);
void PrintDateTime(
	FILE * const FileHandle, 
	const time_t Time);

//ReadCommand.h
#if defined(PLATFORM_WIN)
bool ReadCommand(
	int argc, 
	wchar_t *argv[]);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
bool ReadCommand(
	int argc, 
	char *argv[]);
#endif

//Resolver.h
void PrintHexResponse(
	FILE * const FileHandle, 
	const uint8_t * const Buffer, 
	const size_t Length);
void PrintFormattedResponse(
	FILE * const FileHandle, 
	const uint8_t * const Buffer, 
	const size_t Length);
#endif
