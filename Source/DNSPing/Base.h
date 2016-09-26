// This code is part of Toolkit(DNSPing)
// A useful and powerful toolkit(DNSPing)
// Copyright (C) 2014-2016 Chengr28
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


#include "Structures.h"

//////////////////////////////////////////////////
// Main header
// 
#define ADDRESS_STRING_MAXSIZE       64U         //Maximum size of addresses(IPv4/IPv6) words
#define BYTES_TO_BITS                8U
#if defined(PLATFORM_WIN)
	#define DEFAULT_TIME_OUT             2000U       //Default timeout, 2000 ms(2 seconds)
	#define MBSTOWCS_NULLTERMINATE       (-1)        //MultiByteToWideChar() find null-terminate.
	#define STANDARD_TIME_OUT            1000U       //Standard timeout, 1000 ms(1 second)
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	#define DEFAULT_TIME_OUT             2U          //Default timeout, 2 seconds
	#define SECOND_TO_MILLISECOND        1000U       //1000 milliseconds(1 second)
	#define STANDARD_TIME_OUT            1000000U    //Standard timeout, 1000000 us(1000 ms or 1 second)
#endif
#define DEFAULT_SEND_TIMES           4U          //Default send times
#define DOMAIN_MAXSIZE               256U        //Maximum size of whole level domain is 256 bytes(Section 2.3.1 in RFC 1035).
#define DOMAIN_MINSIZE               2U          //Minimum size of whole level domain is 3 bytes(Section 2.3.1 in RFC 1035).
#define FILE_BUFFER_SIZE             4096U       //Maximum size of file buffer(4KB/4096 bytes)
#define HIGHEST_BIT_U16              0x7FFF      //Get highest bit in uint16_t/16 bits data
#define HIGHEST_MOVE_BIT_U16         15U         //Move 15 bits to get highest bit in uint16_t/16 bits data.
#define LARGE_PACKET_MAXSIZE         4096U       //Maximum size of packets(4KB/4096 bytes) of TCP protocol
#define MICROSECOND_TO_MILLISECOND   1000U       //1000 microseconds(1 millisecond)
#define NUM_HEX                      16
#define PACKET_MAXSIZE               1500U       //Maximum size of packets, Standard MTU of Ethernet II network
#define PACKET_MINSIZE               64U         //Minimum size of packets in Ethernet network.
#define PADDING_RESERVED_BYTES       2U          //Padding reserved bytes(2 bytes)
#define SECONDS_IN_DAY               86400U      //86400 seconds in a day
#define SECONDS_IN_HOUR              3600U       //3600 seconds in an hour
#define SECONDS_IN_MINUTE            60U         //60 seconds in a minute
#define SECONDS_IN_MONTH             2592000U    //2592000 seconds in a month(30 days in a month)
#define SECONDS_IN_YEAR              31536000U   //31536000 seconds in a year(30 days in a month and 12 months in a year)
#define TIME_OUT_MIN                 500U        //Minimum timeout, 500 ms
#define U16_NUM_ONE                  0x0001
#define UINT4_MAX                    0x000F
#define DNS_PACKET_MINSIZE           (sizeof(dns_hdr) + 4U + sizeof(dns_qry))   //Minimum DNS packet size(DNS Header + Minimum Domain + DNS Query)

//Version definitions
#define FULL_VERSION                                  L"0.3.3.0"
#define COPYRIGHT_MESSAGE                             L"Copyright (C) 2014-2016 Chengr28"


//////////////////////////////////////////////////
// Linux and Mac OS X compatible
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	#define SD_BOTH                                                          SHUT_RDWR
	#define SD_RECV                                                          SHUT_RD
	#define SD_SEND                                                          SHUT_WR
	#define closesocket                                                      close
	#define fwprintf_s                                                       fwprintf
	#define GetLastError()                                                   errno
	#define _set_errno(Value)                                                errno = Value
	#define strnlen_s                                                        strnlen
	#define wcsnlen_s                                                        wcsnlen
	#define WSAGetLastError()                                                errno
	#define localtime_s(TimeStructure, TimeValue)                            localtime_r(TimeValue, TimeStructure)
	#define memcpy_s(Dst, DstSize, Src, Size)                                memcpy(Dst, Src, Size)
	#define memmove_s(Dst, DstSize, Src, Size)                               memmove(Dst, Src, Size)
	#if defined(PLATFORM_LINUX)
		#define send(Socket, Buffer, Length, Signal)                         send(Socket, Buffer, Length, Signal|MSG_NOSIGNAL)
		#define sendto(Socket, Buffer, Length, Signal, SockAddr, AddrLen)    sendto(Socket, Buffer, Length, Signal|MSG_NOSIGNAL, SockAddr, AddrLen)
	#endif
#endif


//////////////////////////////////////////////////
// Class and structure definitions
class ConfigurationTable
{
public:
//Global status
	FILE *OutputFile;
#if defined(PLATFORM_WIN)
	bool IsWinSockInitialized;
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
	bool IsValidate;
	bool IsShowResponse;
	bool IsShowHexResponse;
#if (defined(PLATFORM_WIN) || defined(PLATFORM_LINUX))
	bool IsDoNotFragment;
#endif
#if defined(PLATFORM_WIN)
	DWORD PacketHopLimits;
	DWORD SocketTimeout;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	int PacketHopLimits;
	timeval SocketTimeout;
#endif
	sockaddr_storage SockAddr_Normal;
	sockaddr_storage SockAddr_SOCKS;
	dns_hdr Parameter_Header;
	dns_qry Parameter_Query;
	dns_opt_record Parameter_EDNS;

//C-Syle type result block
	size_t Statistics_Send;
	size_t Statistics_RealSend;
	size_t Statistics_RecvNum;
	long double Statistics_TotalTime;
	long double Statistics_MaxTime;
	long double Statistics_MinTime;

//C++ STL type block
	std::string TestDomain;
	std::string TargetString_Normal;
	std::string TargetAddressString;
	std::string SOCKS_Username;
	std::string SOCKS_Password;
	std::wstring wTargetString;
	std::wstring TargetString_SOCKS;
	std::wstring wOutputFileName;
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	std::string OutputFileName;
#endif
	std::shared_ptr<uint8_t> RawData;

//Member functions
	ConfigurationTable(
		void);
	~ConfigurationTable(
		void);
};

//Console.h
#if defined(PLATFORM_WIN)
BOOL CtrlHandler(
	const DWORD fdwCtrlType);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
void SIG_Handler(
	const int Signal);
#endif

//DNSPing.cpp
bool ConfigurationInitialization(
	void);

//Process.h
bool SendProcess(
	const sockaddr_storage &Target, 
	const bool LastSend);
void PrintProcess(
	const bool IsPacketStatistics, 
	const bool IsTimeStatistics);
bool OutputResultToFile(
	void);
void PrintHeaderToScreen(
	const std::wstring wTargetAddressString, 
	const std::wstring wTestDomain);
void PrintErrorToScreen(
	const wchar_t *Message, 
	const ssize_t ErrorCode);
void PrintDescription(
	void);

//Protocol.h
bool CheckEmptyBuffer(
	const void *Buffer, 
	const size_t Length);
bool MBSToWCSString(
	const uint8_t *Buffer, 
	const size_t MaxLen, 
	std::wstring &Target);
bool WCSToMBSString(
	const wchar_t *Buffer, 
	const size_t MaxLen, 
	std::string &Target);
void CaseConvert(
	const bool IsLowerUpper, 
	uint8_t *Buffer, 
	const size_t Length);
void CaseConvert(
	std::wstring &Buffer, 
	const bool IsLowerToUpper);
bool AddressStringToBinary(
	const uint16_t Protocol, 
	const uint8_t *AddrString, 
	void *OriginalAddr, 
	ssize_t &ErrorCode);
bool BinaryToAddressString(
	const uint16_t Protocol, 
	const void *OriginalAddr, 
	void *AddressString, 
	const size_t StringSize, 
	ssize_t *ErrorCode);
uint16_t ProtocolNameToBinary(
	const std::wstring &Buffer);
uint16_t ServiceNameToBinary(
	const std::wstring &Buffer);
uint16_t DNSClassesNameToBinary(
	const std::wstring &Buffer);
uint16_t DNSTypeNameToBinary(
	const std::wstring &Buffer);
size_t CharToDNSQuery(
	const uint8_t *FName, 
	uint8_t *TName);
size_t DNSQueryToChar(
	const uint8_t *TName, 
	uint8_t *FName, 
	uint16_t &Truncated);
bool ValidatePacket(
	const uint8_t *Buffer, 
	const size_t Length, 
	const uint16_t DNS_ID);
void PrintSecondsInDateTime(
	FILE *FileHandle, 
	const time_t Seconds);
void PrintDateTime(
	FILE *FileHandle, 
	const time_t Time);

//ReadCommands.cpp
#if defined(PLATFORM_WIN)
bool ReadCommands(
	int argc, 
	wchar_t *argv[]);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
bool ReadCommands(
	int argc, 
	char *argv[]);
#endif

//Resolver.h
void PrintHexResponse(
	FILE *FileHandle, 
	const uint8_t *Buffer, 
	const size_t Length);
void PrintResponse(
	FILE *FileHandle, 
	const uint8_t *Buffer, 
	const size_t Length);
