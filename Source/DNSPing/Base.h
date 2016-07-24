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
#define ADDR_STRING_MAXSIZE          64U         //Maximum size of addresses(IPv4/IPv6) words
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
#define HIGHEST_BIT_U16              0x7FFF      //Get highest bit in uint16_t/16 bits data
#define HIGHEST_MOVE_BIT_U16         15U         //Move 15 bits to get highest bit in uint16_t/16 bits data.
#define LARGE_PACKET_MAXSIZE         4096U       //Maximum size of packets(4KB/4096 bytes) of TCP protocol
#define MICROSECOND_TO_MILLISECOND   1000U       //1000 microseconds(1 millisecond)
#define NUM_HEX                      16
#define PACKET_MAXSIZE               1500U       //Maximum size of packets, Standard MTU of Ethernet II network
#define PACKET_MINSIZE               64U         //Minimum size of packets in Ethernet network.
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
#define FULL_VERSION                                  L"0.3.1.0"
#define COPYRIGHT_MESSAGE                             L"Copyright (C) 2014-2016 Chengr28"


//////////////////////////////////////////////////
// Linux and Mac OS X compatible
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	#define SD_BOTH                                                          SHUT_RDWR
	#define SD_RECV                                                          SHUT_RD
	#define SD_SEND                                                          SHUT_WR
	#define closesocket                                                      close
	#if defined(PLATFORM_LINUX)
		#define _fcloseall                                                       fcloseall
	#endif
	#define fwprintf_s                                                       fwprintf
	#define GetCurrentProcessId                                              pthread_self
	#define GetLastError()                                                   errno
	#define _set_errno(Value)                                                errno = Value
	#define strnlen_s                                                        strnlen
	#define wcsnlen_s                                                        wcsnlen
	#define WSAGetLastError                                                  GetLastError
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
	bool Initialization_WinSock;
#endif

//C-Syle type parameter block
	size_t TransmissionInterval;
	size_t BufferSize;
	size_t RawDataLen;
	size_t EDNSPayloadSize;
	uint16_t Protocol;
	uint16_t ServiceType;
	bool ReverseLookup;
	bool RawSocket;
	bool EDNS;
	bool DNSSEC;
	bool Validate;
	bool ShowResponse;
	bool ShowResponseHex;
	int IP_HopLimits;
#if defined(PLATFORM_WIN)
	int SocketTimeout;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	timeval SocketTimeout;
#endif
#if (defined(PLATFORM_WIN) || defined(PLATFORM_LINUX))
	bool IPv4_DF;
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
#if (defined(PLATFORM_WIN) || (defined(PLATFORM_LINUX) && !defined(PLATFORM_OPENWRT)))
	~ConfigurationTable(
		void);
#endif
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
size_t ConfigurationInitialization(
	void);
#if defined(PLATFORM_WIN)
size_t ReadCommands(
	int argc, 
	wchar_t* argv[]);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
size_t ReadCommands(
	int argc, 
	char *argv[]);
#endif
size_t OutputResultToFile(
	void);
void PrintHeaderToScreen(
	const std::wstring wTargetAddressString, 
	const std::wstring wTestDomain);

//Process.h
size_t SendProcess(
	const sockaddr_storage &Target, 
	const bool LastSend);
size_t PrintProcess(
	const bool IsPacketStatistics, 
	const bool IsTimeStatistics);
void PrintDescription(
	void);

//Protocol.h
bool CheckEmptyBuffer(
	const void *Buffer, 
	const size_t Length);
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
void MBSToWCSString(
	std::wstring &Target, 
	const uint8_t *Buffer);
#endif
size_t CaseConvert(
	const bool IsLowerUpper, 
	uint8_t *Buffer, 
	const size_t Length);
size_t AddressStringToBinary(
	const uint8_t *AddrString, 
	void *pAddr, 
	const uint16_t Protocol, 
	ssize_t &ErrCode);
uint16_t ProtocolNameToPort(
	const std::wstring &Buffer);
uint16_t ServiceNameToPort(
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
	const time_t Seconds, 
	FILE *FileHandle);
void PrintDateTime(
	const time_t Time, 
	FILE *FileHandle);

//Resolver.h
void PrintResponseHex(
	const uint8_t *Buffer, 
	const size_t Length, 
	FILE *FileHandle);
void PrintResponse(
	const uint8_t *Buffer, 
	const size_t Length, 
	FILE *FileHandle);
