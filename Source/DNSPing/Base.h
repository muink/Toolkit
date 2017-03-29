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


#ifndef TOOLKIT_DNSPING_BASE_H
#define TOOLKIT_DNSPING_BASE_H

#include "Structure.h"

//////////////////////////////////////////////////
// Main header
// 
//Code definitions
#define COMMAND_MIN_COUNT            2
#if defined(PLATFORM_WIN)
	#define MBSTOWCS_NULL_TERMINATE      (-1)                        //MultiByteToWideChar function null-terminate
	#define WCSTOMBS_NULL_TERMINATE      MBSTOWCS_NULL_TERMINATE     //WideCharToMultiByte function null-terminate
#endif
#define DEFAULT_SEND_TIMES           4U                          //Default send times
#define HIGHEST_BIT_U16              0x7FFF                      //Get highest bit in uint16_t/16 bits structure.
#define HIGHEST_MOVE_BIT_U16         15U                         //Move 15 bits to get highest bit in uint16_t/16 bits structure.
#define LOWEST_MOVE_BIT_U16          11U                         //Move 11 bits to make highest bit to lowest bits in uint16_t/16 bits structure.
#define NUM_HEX                      16
#define U16_NUM_ONE                  0x0001
#define UINT4_MAX                    0x000F

//Time definitions
#if defined(PLATFORM_WIN)
	#define DEFAULT_TIMEOUT              2000U                       //Default timeout, 2000 ms(2 seconds)
	#define STANDARD_TIMEOUT             1000U                       //Standard timeout(1000 ms/1 second)
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define DEFAULT_TIMEOUT              2U                          //Default timeout, 2 seconds
	#define STANDARD_TIMEOUT             1000000U                    //Standard timeout, 1000000 us(1000 ms or 1 second)
#endif
#define MICROSECOND_TO_MILLISECOND   1000U                       //1000 microseconds(1 millisecond)
#define NANOSECOND_TO_MICROSECOND    1000U                       //1000 nanoseconds(1 microsecond)
#define SECONDS_IN_DAY               86400U                      //86400 seconds in a day
#define SECONDS_IN_HOUR              3600U                       //3600 seconds in an hour
#define SECONDS_IN_MINUTE            60U                         //60 seconds in a minute
#define SECONDS_IN_MONTH             2592000U                    //2592000 seconds in a month(30 days in a month)
#define SECONDS_IN_YEAR              31536000U                   //31536000 seconds in a year(30 days in a month and 12 months in a year)
#define SECOND_TO_MILLISECOND        1000U                       //1000 milliseconds(1 second)
#if defined(PLATFORM_WIN)
	#define SOCKET_MIN_TIME_INTERVAL     1
#endif
#define SOCKET_MIN_TIMEOUT           500U                        //The shortest socket timeout(500 ms)

//Size and length definitions
#define ADDRESS_STRING_MAXSIZE       64U                         //Maximum size of addresses(IPv4/IPv6) words(64 bytes)
#define BYTES_TO_BITS                8U
#define DEFAULT_LARGE_BUFFER_SIZE    4096U                       //Default size of large buffer(4KB/4096 bytes)
#define DOMAIN_MAXSIZE               256U                        //Maximum size of whole level domain is 256 bytes(Section 2.3.1 in RFC 1035).
#define DOMAIN_MINSIZE               2U                          //Minimum size of whole level domain is 3 bytes(Section 2.3.1 in RFC 1035).
#define FILE_BUFFER_SIZE             DEFAULT_LARGE_BUFFER_SIZE   //Maximum size of file buffer(4KB/4096 bytes)
#define IPV4_SHORTEST_ADDR_STRING    6U                          //The shortest IPv4 address strings(*.*.*.*)
#define IPV4_LONGEST_ADDR_STRING     15U                         //The longest IPv6 address strings(***.***.***.***)
#define IPV6_SHORTEST_ADDR_STRING    2U                          //The shortest IPv6 address strings(::)
#define IPV6_LONGEST_ADDR_STRING     46U                         //The longest IPv6 address strings(xxxx::xxxx::xxxx::xxxx::xxxx::xxxx::xxxx::xxxx)
#define NULL_TERMINATE_LENGTH        1U                          //Length of C style string null
#define PACKET_MAXSIZE               1500U                       //Maximum size of packets, Standard MTU of Ethernet II network
#define PACKET_MINSIZE               64U                         //Minimum size of packets in Ethernet network.
#define PADDING_RESERVED_BYTES       2U                          //Padding reserved bytes(2 bytes)
#define DNS_PACKET_MINSIZE           (sizeof(dns_hdr) + NULL_TERMINATE_LENGTH + sizeof(dns_qry))   //Minimum DNS packet size(DNS Header + Minimum Domain<ROOT> + DNS Query)

//Version definitions
#define FULL_VERSION                 L"0.3.7.0"
#define COPYRIGHT_MESSAGE            L"Copyright (C) 2014-2017 Chengr28"


//////////////////////////////////////////////////
// Linux and macOS compatible
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define SD_BOTH                                                          SHUT_RDWR
	#define SD_RECV                                                          SHUT_RD
	#define SD_SEND                                                          SHUT_WR
	#define closesocket                                                      close
	#define fwprintf_s                                                       fwprintf
	#define GetLastError()                                                   errno
	#define _set_errno(Value)                                                errno = (Value)
	#define strnlen_s                                                        strnlen
	#define wcsnlen_s                                                        wcsnlen
	#define WSAGetLastError()                                                errno
	#define localtime_s(TimeStructure, TimeValue)                            localtime_r(TimeValue, TimeStructure)
	#define memcpy_s(Dst, DstSize, Src, Size)                                memcpy(Dst, Src, Size)
	#define memmove_s(Dst, DstSize, Src, Size)                               memmove(Dst, Src, Size)
#endif


//////////////////////////////////////////////////
// Class and structure definitions
class ConfigurationTable
{
public:
//Global status
	FILE *OutputFile;
#if defined(PLATFORM_WIN)
	bool IsInitialized_WinSock;
#endif

//C-Syle type parameter block
	size_t TransmissionInterval;
	size_t BufferSize;
	size_t RawDataLen;
	size_t EDNSPayloadSize;
	uint16_t Protocol;
	uint16_t ServiceType;
	bool IsReverseLookup;
	bool IsRawSocket;
	bool IsEDNS;
	bool IsDNSSEC;
	bool IsValidated;
	bool IsShowResponse;
	bool IsShowHexResponse;
#if (defined(PLATFORM_WIN) || defined(PLATFORM_LINUX))
	bool IsDoNotFragment;
#endif
#if defined(PLATFORM_WIN)
	DWORD PacketHopLimits;
	DWORD SocketTimeout;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	int PacketHopLimits;
	timeval SocketTimeout;
#endif
	sockaddr_storage SockAddr_Normal;
	dns_hdr Parameter_Header;
	dns_qry Parameter_Query;
	dns_record_opt Parameter_EDNS;

//C-Syle type result block
	size_t Statistics_PlanSend;
	size_t Statistics_RealSend;
	size_t Statistics_RecvNum;
	long double Statistics_TotalTime;
	long double Statistics_MaxTime;
	long double Statistics_MinTime;

//C++ STL type block
	std::string TestDomainString;
	std::string TargetAddressString;
	std::string TargetString_Normal;
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string OutputFileName;
#endif
	std::wstring WideTargetString;
	std::wstring WideOutputFileName;
	std::unique_ptr<uint8_t[]> RawDataBuffer;

//Member functions
	ConfigurationTable(
		void);
	~ConfigurationTable(
		void);
};

//Console.cpp
#if defined(PLATFORM_WIN)
BOOL WINAPI CtrlHandler(
	const DWORD ControlType);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
//Terminal.cpp
void SIG_Handler(
	const int Signal);
#endif

//DNSPing.cpp
bool ConfigurationInitialization(
	void);
bool ParameterCheckAndSetting(
	void);
bool MainProcess(
	void);

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

//ReadCommand.cpp
#if defined(PLATFORM_WIN)
bool ReadCommand(
	int argc, 
	wchar_t *argv[]);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
bool ReadCommand(
	int argc, 
	char *argv[]);
#endif

//Resolver.h
void PrintHexResponse(
	FILE * const FileHandle, 
	const uint8_t * const Buffer, 
	const size_t Length);
void PrintResponse(
	FILE * const FileHandle, 
	const uint8_t * const Buffer, 
	const size_t Length);
#endif
