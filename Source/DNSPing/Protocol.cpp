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


#include "Protocol.h"

//Minimum supported system of Windows Version Helpers is Windows Vista.
#if defined(PLATFORM_WIN_XP)
//Check current operation system which higher than Windows 7.
bool IsLowerThanWin8(
	void)
{
	OSVERSIONINFOEX OSVI;
	memset(&OSVI, 0, sizeof(OSVI));
	OSVI.dwOSVersionInfoSize = sizeof(OSVI);
	BOOL bOsVersionInfoEx = GetVersionExW((OSVERSIONINFO *)&OSVI);

	if (bOsVersionInfoEx && OSVI.dwPlatformId == VER_PLATFORM_WIN32_NT && 
		(OSVI.dwMajorVersion < 6U || (OSVI.dwMajorVersion == 6U && OSVI.dwMinorVersion < 2U)))
			return true;

	return false;
}
#endif

//Check empty buffer
bool CheckEmptyBuffer(
	const void *Buffer, 
	const size_t Length)
{
//Null pointer
	if (Buffer == nullptr)
	{
		return false;
	}
	else {
	//Scan all data.
		for (size_t Index = 0;Index < Length;++Index)
		{
			if (*(((uint8_t *)Buffer) + Index) != 0)
				return false;
		}
	}

	return true;
}

//Convert multiple bytes to wide char string
bool MBSToWCSString(
	const uint8_t *Buffer, 
	const size_t MaxLen, 
	std::wstring &Target)
{
//Check buffer.
	Target.clear();
	if (Buffer == nullptr || MaxLen == 0)
		return false;
	size_t Length = strnlen_s((const char *)Buffer, MaxLen);
	if (Length == 0 || CheckEmptyBuffer(Buffer, Length))
		return false;

//Convert string.
	std::shared_ptr<wchar_t> TargetPTR(new wchar_t[Length + PADDING_RESERVED_BYTES]());
	wmemset(TargetPTR.get(), 0, Length + PADDING_RESERVED_BYTES);
#if defined(PLATFORM_WIN)
	if (MultiByteToWideChar(
			CP_ACP, 
			0, 
			(LPCCH)Buffer, 
			MBSTOWCS_NULLTERMINATE, 
			TargetPTR.get(), 
			(int)(Length + PADDING_RESERVED_BYTES)) == 0)
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	if (mbstowcs(TargetPTR.get(), (const char *)Buffer, Length + PADDING_RESERVED_BYTES) == (size_t)RETURN_ERROR)
#endif
	{
		return false;
	}
	else {
		if (wcsnlen_s(TargetPTR.get(), Length + PADDING_RESERVED_BYTES) == 0)
			return false;
		else 
			Target = TargetPTR.get();
	}

	return true;
}

//Convert wide char string to multiple bytes
bool WCSToMBSString(
	const wchar_t *Buffer, 
	const size_t MaxLen, 
	std::string &Target)
{
//Check buffer.
	Target.clear();
	if (Buffer == nullptr || MaxLen == 0)
		return false;
	size_t Length = wcsnlen_s(Buffer, MaxLen);
	if (Length == 0 || CheckEmptyBuffer(Buffer, sizeof(wchar_t) * Length))
		return false;

//Convert string.
	std::shared_ptr<uint8_t> TargetPTR(new uint8_t[Length + PADDING_RESERVED_BYTES]());
	memset(TargetPTR.get(), 0, Length + PADDING_RESERVED_BYTES);
#if defined(PLATFORM_WIN)
	if (WideCharToMultiByte(
			CP_ACP, 
			0, 
			Buffer, 
			MBSTOWCS_NULLTERMINATE, 
			(LPSTR)TargetPTR.get(), 
			(int)(Length + PADDING_RESERVED_BYTES), 
			nullptr, 
			nullptr) == 0)
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	if (wcstombs((char *)TargetPTR.get(), Buffer, Length + PADDING_RESERVED_BYTES) == (size_t)RETURN_ERROR)
#endif
	{
		return false;
	}
	else {
		if (strnlen_s((const char *)TargetPTR.get(), Length + PADDING_RESERVED_BYTES) == 0)
			return false;
		else 
			Target = (const char *)TargetPTR.get();
	}

	return true;
}

//Convert lowercase/uppercase words to uppercase/lowercase words(C-Style version)
void CaseConvert(
	const bool IsLowerUpper, 
	uint8_t *Buffer, 
	const size_t Length)
{
	for (size_t Index = 0;Index < Length;++Index)
	{
	//Lowercase to uppercase
		if (IsLowerUpper)
			Buffer[Index] = (uint8_t)toupper(Buffer[Index]);
	//Uppercase to lowercase
		else 
			Buffer[Index] = (uint8_t)tolower(Buffer[Index]);
	}

	return;
}

//Convert lowercase/uppercase words to uppercase/lowercase words(C++ wstring version)
void CaseConvert(
	std::wstring &Buffer, 
	const bool IsLowerToUpper)
{
	for (auto &StringIter:Buffer)
	{
	//Lowercase to uppercase
		if (IsLowerToUpper)
			StringIter = (wchar_t)toupper(StringIter);
	//Uppercase to lowercase
		else 
			StringIter = (wchar_t)tolower(StringIter);
	}

	return;
}

//Convert address strings to binary.
bool AddressStringToBinary(
	const uint16_t Protocol, 
	const uint8_t *AddrString, 
	void *OriginalAddr, 
	ssize_t &ErrorCode)
{
	size_t UnsignedResult = 0;

//Minimum supported system of inet_ntop and inet_pton functions is Windows Vista. [Roy Tam]
#if defined(PLATFORM_WIN_XP)
	sockaddr_storage SockAddr;
	memset(&SockAddr, 0, sizeof(SockAddr));
	socklen_t SockLength = 0;
#endif

//IPv6
	if (Protocol == AF_INET6)
	{
	//Check IPv6 addresses
		for (UnsignedResult = 0;UnsignedResult < strnlen_s((const char *)AddrString, ADDRESS_STRING_MAXSIZE);++UnsignedResult)
		{
			if (AddrString[UnsignedResult] < ASCII_ZERO || (AddrString[UnsignedResult] > ASCII_COLON && 
				AddrString[UnsignedResult] < ASCII_UPPERCASE_A) || (AddrString[UnsignedResult] > ASCII_UPPERCASE_F && 
				AddrString[UnsignedResult] < ASCII_LOWERCASE_A) || AddrString[UnsignedResult] > ASCII_LOWERCASE_F)
					break;
		}

		std::string sAddrString((const char *)AddrString);
	//Check abbreviation format.
		if (sAddrString.find(ASCII_COLON) == std::string::npos)
		{
			sAddrString.clear();
			sAddrString.append("::");
			sAddrString.append((const char *)AddrString);
		}
		else if (sAddrString.find(ASCII_COLON) == sAddrString.rfind(ASCII_COLON))
		{
			sAddrString.replace(sAddrString.find(ASCII_COLON), 1U, ("::"));
		}

	//Convert to binary.
	#if defined(PLATFORM_WIN_XP)
		SockLength = sizeof(sockaddr_in6);
		if (WSAStringToAddressA((char *)sAddrString.c_str(), AF_INET6, nullptr, (PSOCKADDR)&SockAddr, &SockLength) == SOCKET_ERROR)
	#else
		ssize_t SignedResult = inet_pton(AF_INET6, sAddrString.c_str(), OriginalAddr);
		if (SignedResult == SOCKET_ERROR || SignedResult == FALSE)
	#endif
		{
			ErrorCode = WSAGetLastError();
			return false;
		}
	#if defined(PLATFORM_WIN_XP)
		memcpy_s(OriginalAddr, sizeof(in6_addr), &((PSOCKADDR_IN6)&SockAddr)->sin6_addr, sizeof(((PSOCKADDR_IN6)&SockAddr)->sin6_addr));
	#endif
	}
//IPv4
	else {
		size_t CommaNum = 0;
		for (UnsignedResult = 0;UnsignedResult < strnlen_s((const char *)AddrString, ADDRESS_STRING_MAXSIZE);++UnsignedResult)
		{
			if ((AddrString[UnsignedResult] != ASCII_PERIOD && AddrString[UnsignedResult] < ASCII_ZERO) || AddrString[UnsignedResult] > ASCII_NINE)
				return false;
			else if (AddrString[UnsignedResult] == ASCII_PERIOD)
				++CommaNum;
		}

		std::string sAddrString((const char *)AddrString);
	//Delete zero(s) before whole data.
		while (sAddrString.length() > 1U && sAddrString[0] == ASCII_ZERO && sAddrString[1U] != ASCII_PERIOD)
			sAddrString.erase(0, 1U);

	//Check abbreviation format.
		if (CommaNum == 0)
		{
			sAddrString.clear();
			sAddrString.append("0.0.0.");
			sAddrString.append((const char *)AddrString);
		}
		else if (CommaNum == 1U)
		{
			sAddrString.replace(sAddrString.find(ASCII_PERIOD), 1U, (".0.0."));
		}
		else if (CommaNum == 2U)
		{
			sAddrString.replace(sAddrString.find(ASCII_PERIOD), 1U, (".0."));
		}

	//Delete zero(s) before data.
		while (sAddrString.find(".00") != std::string::npos)
			sAddrString.replace(sAddrString.find(".00"), 3U, ("."));
		while (sAddrString.find(".0") != std::string::npos)
			sAddrString.replace(sAddrString.find(".0"), 2U, ("."));
		while (sAddrString.find("..") != std::string::npos)
			sAddrString.replace(sAddrString.find(".."), 2U, (".0."));
		if (sAddrString[sAddrString.length() - 1U] == ASCII_PERIOD)
			sAddrString.append("0");

	//Convert to binary.
	#if defined(PLATFORM_WIN_XP)
		SockLength = sizeof(sockaddr_in);
		if (WSAStringToAddressA((char *)sAddrString.c_str(), AF_INET, nullptr, (PSOCKADDR)&SockAddr, &SockLength) == SOCKET_ERROR)
	#else
		ssize_t SignedResult = inet_pton(AF_INET, sAddrString.c_str(), OriginalAddr);
		if (SignedResult == SOCKET_ERROR || SignedResult == FALSE)
	#endif
		{
			ErrorCode = WSAGetLastError();
			return false;
		}
	#if defined(PLATFORM_WIN_XP)
		memcpy_s(OriginalAddr, sizeof(in_addr), &((PSOCKADDR_IN)&SockAddr)->sin_addr, sizeof(((PSOCKADDR_IN)&SockAddr)->sin_addr));
	#endif
	}

	return true;
}

//Convert binary address strings
bool BinaryToAddressString(
	const uint16_t Protocol, 
	const void *OriginalAddr, 
	void *AddressString, 
	const size_t StringSize, 
	ssize_t *ErrorCode)
{
//Initialization
	if (ErrorCode != nullptr)
		*ErrorCode = 0;

//Convert address.
//Minimum supported system of inet_ntop function and inet_pton function is Windows Vista. [Roy Tam]
#if defined(PLATFORM_WIN_XP)
	sockaddr_storage SockAddr;
	memset(&SockAddr, 0, sizeof(SockAddr));
	if (Protocol == AF_INET6)
	{
		SockAddr.ss_family = AF_INET6;
		((PSOCKADDR_IN6)&SockAddr)->sin6_addr = *(in6_addr *)OriginalAddr;
	}
	else if (Protocol == AF_INET)
	{
		SockAddr.ss_family = AF_INET;
		((PSOCKADDR_IN)&SockAddr)->sin_addr = *(in_addr *)OriginalAddr;
	}
	else {
		return false;
	}

	DWORD BufferLength = StringSize;
	if (WSAAddressToStringA(
		(PSOCKADDR)&SockAddr, 
		sizeof(sockaddr_in6), 
		nullptr, 
		(LPSTR)AddressString, 
		&BufferLength) == SOCKET_ERROR)
#else
	if (inet_ntop(Protocol, (void *)OriginalAddr, (char *)AddressString, (socklen_t)StringSize) == nullptr)
#endif
	{
		if (ErrorCode != nullptr)
			*ErrorCode = WSAGetLastError();

		return false;
	}

	return true;
}

//Convert protocol name to hex
uint16_t ProtocolNameToBinary(
	const std::wstring &Buffer)
{
	std::wstring InnerBuffer(Buffer);
	CaseConvert(InnerBuffer, true);

//Internet Protocol Number(Part 1)
	if (InnerBuffer == (L"HOPOPTS"))
		return IPPROTO_HOPOPTS;
	else if (InnerBuffer == (L"ICMP"))
		return IPPROTO_ICMP;
	else if (InnerBuffer == (L"IGMP"))
		return IPPROTO_IGMP;
	else if (InnerBuffer == (L"GGP"))
		return IPPROTO_GGP;
	else if (InnerBuffer == (L"IPV4"))
		return IPPROTO_IPV4;
	else if (InnerBuffer == (L"ST"))
		return IPPROTO_ST;
	else if (InnerBuffer == (L"TCP"))
		return IPPROTO_TCP;
	else if (InnerBuffer == (L"CBT"))
		return IPPROTO_CBT;
	else if (InnerBuffer == (L"EGP"))
		return IPPROTO_EGP;
	else if (InnerBuffer == (L"IGP"))
		return IPPROTO_IGP;
	else if (InnerBuffer == (L"BBNRCCMON"))
		return IPPROTO_BBN_RCC_MON;
	else if (InnerBuffer == (L"NVPII"))
		return IPPROTO_NVP_II;
	else if (InnerBuffer == (L"PUP"))
		return IPPROTO_PUP;
	else if (InnerBuffer == (L"ARGUS"))
		return IPPROTO_ARGUS;
	else if (InnerBuffer == (L"EMCON"))
		return IPPROTO_EMCON;
	else if (InnerBuffer == (L"XNET"))
		return IPPROTO_XNET;
	else if (InnerBuffer == (L"CHAOS"))
		return IPPROTO_CHAOS;
	else if (InnerBuffer == (L"UDP"))
		return IPPROTO_UDP;
	else if (InnerBuffer == (L"MUX"))
		return IPPROTO_MUX;
	else if (InnerBuffer == (L"DCN"))
		return IPPROTO_DCN;
	else if (InnerBuffer == (L"HMP"))
		return IPPROTO_HMP;
	else if (InnerBuffer == (L"PRM"))
		return IPPROTO_PRM;
	else if (InnerBuffer == (L"IDP"))
		return IPPROTO_IDP;
	else if (InnerBuffer == (L"TRUNK-1"))
		return IPPROTO_TRUNK_1;
	else if (InnerBuffer == (L"TRUNK-2"))
		return IPPROTO_TRUNK_2;
	else if (InnerBuffer == (L"LEAF-1"))
		return IPPROTO_LEAF_1;
	else if (InnerBuffer == (L"LEAF"))
		return IPPROTO_LEAF_2;
	else if (InnerBuffer == (L"RDP"))
		return IPPROTO_RDP;
	else if (InnerBuffer == (L"IRTP"))
		return IPPROTO_IRTP;
	else if (InnerBuffer == (L"ISOTP4"))
		return IPPROTO_ISO_TP4;
	else if (InnerBuffer == (L"NETBLT"))
		return IPPROTO_NETBLT;
	else if (InnerBuffer == (L"MFE"))
		return IPPROTO_MFE;
	else if (InnerBuffer == (L"MERIT"))
		return IPPROTO_MERIT;
	else if (InnerBuffer == (L"DCCP"))
		return IPPROTO_DCCP;
	else if (InnerBuffer == (L"3PC"))
		return IPPROTO_3PC;
	else if (InnerBuffer == (L"IDPR"))
		return IPPROTO_IDPR;
	else if (InnerBuffer == (L"XTP"))
		return IPPROTO_XTP;
	else if (InnerBuffer == (L"DDP"))
		return IPPROTO_DDP;
	else if (InnerBuffer == (L"IDPRCMTP"))
		return IPPROTO_IDPR_CMTP;
	else if (InnerBuffer == (L"TP++"))
		return IPPROTO_TPPLUS;
	else if (InnerBuffer == (L"IL"))
		return IPPROTO_IL;
	else if (InnerBuffer == (L"IPV6"))
		return IPPROTO_IPV6;
	else if (InnerBuffer == (L"SDRP"))
		return IPPROTO_SDRP;
	else if (InnerBuffer == (L"ROUTING"))
		return IPPROTO_ROUTING;
	else if (InnerBuffer == (L"FRAGMENT"))
		return IPPROTO_FRAGMENT;
	else if (InnerBuffer == (L"IDRP"))
		return IPPROTO_IDRP;
	else if (InnerBuffer == (L"RSVP"))
		return IPPROTO_RSVP;
	else if (InnerBuffer == (L"GRE"))
		return IPPROTO_GRE;
	else if (InnerBuffer == (L"DSR"))
		return IPPROTO_DSR;
	else if (InnerBuffer == (L"BNA"))
		return IPPROTO_BNA;
	else if (InnerBuffer == (L"ESP"))
		return IPPROTO_ESP;
	else if (InnerBuffer == (L"AH"))
		return IPPROTO_AH;
	else if (InnerBuffer == (L"NLSP"))
		return IPPROTO_NLSP;
	else if (InnerBuffer == (L"SWIPE"))
		return IPPROTO_SWIPE;
	else if (InnerBuffer == (L"NARP"))
		return IPPROTO_NARP;
	else if (InnerBuffer == (L"MOBILE"))
		return IPPROTO_MOBILE;
	else if (InnerBuffer == (L"TLSP"))
		return IPPROTO_TLSP;
	else if (InnerBuffer == (L"SKIP"))
		return IPPROTO_SKIP;
	else if (InnerBuffer == (L"ICMPV6"))
		return IPPROTO_ICMPV6;
	else if (InnerBuffer == (L"NONE"))
		return IPPROTO_NONE;
	else if (InnerBuffer == (L"DSTOPTS"))
		return IPPROTO_DSTOPTS;
	else if (InnerBuffer == (L"AHI"))
		return IPPROTO_AHI;
	else if (InnerBuffer == (L"CFTP"))
		return IPPROTO_CFTP;
	else if (InnerBuffer == (L"ALN"))
		return IPPROTO_ALN;
	else if (InnerBuffer == (L"SAT"))
		return IPPROTO_SAT;
	else if (InnerBuffer == (L"KRYPTOLAN"))
		return IPPROTO_KRYPTOLAN;
	else if (InnerBuffer == (L"RVD"))
		return IPPROTO_RVD;
	else if (InnerBuffer == (L"IPPC"))
		return IPPROTO_IPPC;
	else if (InnerBuffer == (L"ADF"))
		return IPPROTO_ADF;
	else if (InnerBuffer == (L"SATMON"))
		return IPPROTO_SAT_MON;
	else if (InnerBuffer == (L"VISA"))
		return IPPROTO_VISA;
	else if (InnerBuffer == (L"IPCV"))
		return IPPROTO_IPCV;
	else if (InnerBuffer == (L"CPNX"))
		return IPPROTO_CPNX;
	else if (InnerBuffer == (L"CPHB"))
		return IPPROTO_CPHB;
	else if (InnerBuffer == (L"WSN"))
		return IPPROTO_WSN;
	else if (InnerBuffer == (L"PVP"))
		return IPPROTO_PVP;
	else if (InnerBuffer == (L"BR"))
		return IPPROTO_BR;
	else if (InnerBuffer == (L"ND"))
		return IPPROTO_ND;
	else if (InnerBuffer == (L"ICLFXBM"))
		return IPPROTO_ICLFXBM;
	else if (InnerBuffer == (L"WBEXPAK"))
		return IPPROTO_WBEXPAK;
	else if (InnerBuffer == (L"ISO"))
		return IPPROTO_ISO;
	else if (InnerBuffer == (L"VMTP"))
		return IPPROTO_VMTP;
	else if (InnerBuffer == (L"SVMTP"))
		return IPPROTO_SVMTP;
	else if (InnerBuffer == (L"VINES"))
		return IPPROTO_VINES;
	else if (InnerBuffer == (L"TTP"))
		return IPPROTO_TTP;
	else if (InnerBuffer == (L"IPTM"))
		return IPPROTO_IPTM;
	else if (InnerBuffer == (L"NSFNET"))
		return IPPROTO_NSFNET;
	else if (InnerBuffer == (L"DGP"))
		return IPPROTO_DGP;
	else if (InnerBuffer == (L"TCF"))
		return IPPROTO_TCF;
	else if (InnerBuffer == (L"EIGRP"))
		return IPPROTO_EIGRP;
	else if (InnerBuffer == (L"SPRITE"))
		return IPPROTO_SPRITE;
	else if (InnerBuffer == (L"LARP"))
		return IPPROTO_LARP;
	else if (InnerBuffer == (L"MTP"))
		return IPPROTO_MTP;
	else if (InnerBuffer == (L"AX25"))
		return IPPROTO_AX25;
	else if (InnerBuffer == (L"IPIP"))
		return IPPROTO_IPIP;
	else if (InnerBuffer == (L"MICP"))
		return IPPROTO_MICP;
	else if (InnerBuffer == (L"SCC"))
		return IPPROTO_SCC;
	else if (InnerBuffer == (L"ETHERIP"))
		return IPPROTO_ETHERIP;
	else if (InnerBuffer == (L"ENCAP"))
		return IPPROTO_ENCAP;
	else if (InnerBuffer == (L"APES"))
		return IPPROTO_APES;
	else if (InnerBuffer == (L"GMTP"))
		return IPPROTO_GMTP;
	else if (InnerBuffer == (L"IFMP"))
		return IPPROTO_IFMP;
	else if (InnerBuffer == (L"PIM"))
		return IPPROTO_PIM;
	else if (InnerBuffer == (L"PNNI"))
		return IPPROTO_PNNI;
	else if (InnerBuffer == (L"ARIS"))
		return IPPROTO_ARIS;
	else if (InnerBuffer == (L"SCPS"))
		return IPPROTO_SCPS;
	else if (InnerBuffer == (L"QNX"))
		return IPPROTO_QNX;
	else if (InnerBuffer == (L"AN"))
		return IPPROTO_AN;
	else if (InnerBuffer == (L"IPCOMP"))
		return IPPROTO_IPCOMP;
	else if (InnerBuffer == (L"SNP"))
		return IPPROTO_SNP;
	else if (InnerBuffer == (L"COMPAQ"))
		return IPPROTO_COMPAQ;
	else if (InnerBuffer == (L"IPX"))
		return IPPROTO_IPX;
	else if (InnerBuffer == (L"PGM"))
		return IPPROTO_PGM;
	else if (InnerBuffer == (L"0HOP"))
		return IPPROTO_0HOP;
	else if (InnerBuffer == (L"L2TP"))
		return IPPROTO_L2TP;
	else if (InnerBuffer == (L"DDX"))
		return IPPROTO_DDX;
	else if (InnerBuffer == (L"IATP"))
		return IPPROTO_IATP;
	else if (InnerBuffer == (L"STP"))
		return IPPROTO_STP;
	else if (InnerBuffer == (L"SRP"))
		return IPPROTO_SRP;
	else if (InnerBuffer == (L"UTI"))
		return IPPROTO_UTI;
	else if (InnerBuffer == (L"SMP"))
		return IPPROTO_SMP;
	else if (InnerBuffer == (L"SM"))
		return IPPROTO_SM;
	else if (InnerBuffer == (L"PTP"))
		return IPPROTO_PTP;

//Internet Protocol Number(Part 2)
	if (InnerBuffer == (L"ISIS"))
		return IPPROTO_ISIS;
	else if (InnerBuffer == (L"FIRE"))
		return IPPROTO_FIRE;
	else if (InnerBuffer == (L"CRTP"))
		return IPPROTO_CRTP;
	else if (InnerBuffer == (L"CRUDP"))
		return IPPROTO_CRUDP;
	else if (InnerBuffer == (L"SSCOPMCE"))
		return IPPROTO_SSCOPMCE;
	else if (InnerBuffer == (L"IPLT"))
		return IPPROTO_IPLT;
	else if (InnerBuffer == (L"SPS"))
		return IPPROTO_SPS;
	else if (InnerBuffer == (L"PIPE"))
		return IPPROTO_PIPE;
	else if (InnerBuffer == (L"SCTP"))
		return IPPROTO_SCTP;
	else if (InnerBuffer == (L"FC"))
		return IPPROTO_FC;
	else if (InnerBuffer == (L"RSVPE2E"))
		return IPPROTO_RSVP_E2E;
	else if (InnerBuffer == (L"MOBILITY"))
		return IPPROTO_MOBILITY;
	else if (InnerBuffer == (L"UDPLITE"))
		return IPPROTO_UDPLITE;
	else if (InnerBuffer == (L"MPLS"))
		return IPPROTO_MPLS;
	else if (InnerBuffer == (L"MANET"))
		return IPPROTO_MANET;
	else if (InnerBuffer == (L"HIP"))
		return IPPROTO_HIP;
	else if (InnerBuffer == (L"SHIM6"))
		return IPPROTO_SHIM6;
	else if (InnerBuffer == (L"WESP"))
		return IPPROTO_WESP;
	else if (InnerBuffer == (L"ROHC"))
		return IPPROTO_ROHC;
	else if (InnerBuffer == (L"TEST-1"))
		return IPPROTO_TEST_1;
	else if (InnerBuffer == (L"TEST-2"))
		return IPPROTO_TEST_2;
	else if (InnerBuffer == (L"RAW"))
		return IPPROTO_RAW;

//No match.
	return 0;
}

//Convert service name to port
uint16_t ServiceNameToBinary(
	const std::wstring &Buffer)
{
	std::wstring InnerBuffer(Buffer);
	CaseConvert(InnerBuffer, true);

//Server name
	if (InnerBuffer == (L"TCPMUX"))
		return htons(IPPORT_TCPMUX);
	else if (InnerBuffer == (L"ECHO"))
		return htons(IPPORT_ECHO);
	else if (InnerBuffer == (L"DISCARD"))
		return htons(IPPORT_DISCARD);
	else if (InnerBuffer == (L"SYSTAT"))
		return htons(IPPORT_SYSTAT);
	else if (InnerBuffer == (L"DAYTIME"))
		return htons(IPPORT_DAYTIME);
	else if (InnerBuffer == (L"NETSTAT"))
		return htons(IPPORT_NETSTAT);
	else if (InnerBuffer == (L"QOTD"))
		return htons(IPPORT_QOTD);
	else if (InnerBuffer == (L"MSP"))
		return htons(IPPORT_MSP);
	else if (InnerBuffer == (L"CHARGEN"))
		return htons(IPPORT_CHARGEN);
	else if (InnerBuffer == (L"FTPDATA"))
		return htons(IPPORT_FTP_DATA);
	else if (InnerBuffer == (L"FTP"))
		return htons(IPPORT_FTP);
	else if (InnerBuffer == (L"SSH"))
		return htons(IPPORT_SSH);
	else if (InnerBuffer == (L"TELNET"))
		return htons(IPPORT_TELNET);
	else if (InnerBuffer == (L"SMTP"))
		return htons(IPPORT_SMTP);
	else if (InnerBuffer == (L"TIMESERVER"))
		return htons(IPPORT_TIMESERVER);
	else if (InnerBuffer == (L"RAP"))
		return htons(IPPORT_RAP);
	else if (InnerBuffer == (L"RLP"))
		return htons(IPPORT_RLP);
	else if (InnerBuffer == (L"NAMESERVER"))
		return htons(IPPORT_NAMESERVER);
	else if (InnerBuffer == (L"WHOIS"))
		return htons(IPPORT_WHOIS);
	else if (InnerBuffer == (L"TACACS"))
		return htons(IPPORT_TACACS);
	else if (InnerBuffer == (L"DNS"))
		return htons(IPPORT_DNS);
	else if (InnerBuffer == (L"XNSAUTH"))
		return htons(IPPORT_XNSAUTH);
	else if (InnerBuffer == (L"MTP"))
		return htons(IPPORT_MTP);
	else if (InnerBuffer == (L"BOOTPS"))
		return htons(IPPORT_BOOTPS);
	else if (InnerBuffer == (L"BOOTPC"))
		return htons(IPPORT_BOOTPC);
	else if (InnerBuffer == (L"TFTP"))
		return htons(IPPORT_TFTP);
	else if (InnerBuffer == (L"RJE"))
		return htons(IPPORT_RJE);
	else if (InnerBuffer == (L"FINGER"))
		return htons(IPPORT_FINGER);
	else if (InnerBuffer == (L"HTTP"))
		return htons(IPPORT_HTTP);
	else if (InnerBuffer == (L"HTTPBACKUP"))
		return htons(IPPORT_HTTPBACKUP);
	else if (InnerBuffer == (L"TTYLINK"))
		return htons(IPPORT_TTYLINK);
	else if (InnerBuffer == (L"SUPDUP"))
		return htons(IPPORT_SUPDUP);
	else if (InnerBuffer == (L"POP3"))
		return htons(IPPORT_POP3);
	else if (InnerBuffer == (L"SUNRPC"))
		return htons(IPPORT_SUNRPC);
	else if (InnerBuffer == (L"SQL"))
		return htons(IPPORT_SQL);
	else if (InnerBuffer == (L"NTP"))
		return htons(IPPORT_NTP);
	else if (InnerBuffer == (L"EPMAP"))
		return htons(IPPORT_EPMAP);
	else if (InnerBuffer == (L"NETBIOS_NS"))
		return htons(IPPORT_NETBIOS_NS);
	else if (InnerBuffer == (L"NETBIOS_DGM"))
		return htons(IPPORT_NETBIOS_DGM);
	else if (InnerBuffer == (L"NETBIOS_SSN"))
		return htons(IPPORT_NETBIOS_SSN);
	else if (InnerBuffer == (L"IMAP"))
		return htons(IPPORT_IMAP);
	else if (InnerBuffer == (L"BFTP"))
		return htons(IPPORT_BFTP);
	else if (InnerBuffer == (L"SGMP"))
		return htons(IPPORT_SGMP);
	else if (InnerBuffer == (L"SQLSRV"))
		return htons(IPPORT_SQLSRV);
	else if (InnerBuffer == (L"DMSP"))
		return htons(IPPORT_DMSP);
	else if (InnerBuffer == (L"SNMP"))
		return htons(IPPORT_SNMP);
	else if (InnerBuffer == (L"SNMP_TRAP"))
		return htons(IPPORT_SNMP_TRAP);
	else if (InnerBuffer == (L"ATRTMP"))
		return htons(IPPORT_ATRTMP);
	else if (InnerBuffer == (L"ATHBP"))
		return htons(IPPORT_ATHBP);
	else if (InnerBuffer == (L"QMTP"))
		return htons(IPPORT_QMTP);
	else if (InnerBuffer == (L"IPX"))
		return htons(IPPORT_IPX);
	else if (InnerBuffer == (L"IMAP3"))
		return htons(IPPORT_IMAP3);
	else if (InnerBuffer == (L"BGMP"))
		return htons(IPPORT_BGMP);
	else if (InnerBuffer == (L"TSP"))
		return htons(IPPORT_TSP);
	else if (InnerBuffer == (L"IMMP"))
		return htons(IPPORT_IMMP);
	else if (InnerBuffer == (L"ODMR"))
		return htons(IPPORT_ODMR);
	else if (InnerBuffer == (L"RPC2PORTMAP"))
		return htons(IPPORT_RPC2PORTMAP);
	else if (InnerBuffer == (L"CLEARCASE"))
		return htons(IPPORT_CLEARCASE);
	else if (InnerBuffer == (L"HPALARMMGR"))
		return htons(IPPORT_HPALARMMGR);
	else if (InnerBuffer == (L"ARNS"))
		return htons(IPPORT_ARNS);
	else if (InnerBuffer == (L"AURP"))
		return htons(IPPORT_AURP);
	else if (InnerBuffer == (L"LDAP"))
		return htons(IPPORT_LDAP);
	else if (InnerBuffer == (L"UPS"))
		return htons(IPPORT_UPS);
	else if (InnerBuffer == (L"SLP"))
		return htons(IPPORT_SLP);
	else if (InnerBuffer == (L"HTTPS"))
		return htons(IPPORT_HTTPS);
	else if (InnerBuffer == (L"SNPP"))
		return htons(IPPORT_SNPP);
	else if (InnerBuffer == (L"MICROSOFTDS"))
		return htons(IPPORT_MICROSOFT_DS);
	else if (InnerBuffer == (L"KPASSWD"))
		return htons(IPPORT_KPASSWD);
	else if (InnerBuffer == (L"TCPNETHASPSRV"))
		return htons(IPPORT_TCPNETHASPSRV);
	else if (InnerBuffer == (L"RETROSPECT"))
		return htons(IPPORT_RETROSPECT);
	else if (InnerBuffer == (L"ISAKMP"))
		return htons(IPPORT_ISAKMP);
	else if (InnerBuffer == (L"BIFFUDP"))
		return htons(IPPORT_BIFFUDP);
	else if (InnerBuffer == (L"WHOSERVER"))
		return htons(IPPORT_WHOSERVER);
	else if (InnerBuffer == (L"SYSLOG"))
		return htons(IPPORT_SYSLOG);
	else if (InnerBuffer == (L"ROUTERSERVER"))
		return htons(IPPORT_ROUTESERVER);
	else if (InnerBuffer == (L"NCP"))
		return htons(IPPORT_NCP);
	else if (InnerBuffer == (L"COURIER"))
		return htons(IPPORT_COURIER);
	else if (InnerBuffer == (L"COMMERCE"))
		return htons(IPPORT_COMMERCE);
	else if (InnerBuffer == (L"RTSP"))
		return htons(IPPORT_RTSP);
	else if (InnerBuffer == (L"NNTP"))
		return htons(IPPORT_NNTP);
	else if (InnerBuffer == (L"HTTPRPCEPMAP"))
		return htons(IPPORT_HTTPRPCEPMAP);
	else if (InnerBuffer == (L"IPP"))
		return htons(IPPORT_IPP);
	else if (InnerBuffer == (L"LDAPS"))
		return htons(IPPORT_LDAPS);
	else if (InnerBuffer == (L"MSDP"))
		return htons(IPPORT_MSDP);
	else if (InnerBuffer == (L"AODV"))
		return htons(IPPORT_AODV);
	else if (InnerBuffer == (L"FTPSDATA"))
		return htons(IPPORT_FTPSDATA);
	else if (InnerBuffer == (L"FTPS"))
		return htons(IPPORT_FTPS);
	else if (InnerBuffer == (L"NAS"))
		return htons(IPPORT_NAS);
	else if (InnerBuffer == (L"TELNETS"))
		return htons(IPPORT_TELNETS);

//No match.
	return 0;
}

//Convert DNS classes name to hex
uint16_t DNSClassesNameToBinary(
	const std::wstring &Buffer)
{
	std::wstring InnerBuffer(Buffer);
	CaseConvert(InnerBuffer, true);

//DNS classes name
	if (InnerBuffer == L"INTERNET")
		return htons(DNS_CLASS_IN);
	else if (InnerBuffer == L"CSNET")
		return htons(DNS_CLASS_CSNET);
	else if (InnerBuffer == L"CHAOS")
		return htons(DNS_CLASS_CHAOS);
	else if (InnerBuffer == L"HESIOD")
		return htons(DNS_CLASS_HESIOD);
	else if (InnerBuffer == L"NONE")
		return htons(DNS_CLASS_NONE);
	else if (InnerBuffer == L"ALL")
		return htons(DNS_CLASS_ALL);
	else if (InnerBuffer == L"ANY")
		return htons(DNS_CLASS_ANY);

//No match.
	return 0;
}

//Convert DNS type name to hex
uint16_t DNSTypeNameToBinary(
	const std::wstring &Buffer)
{
	std::wstring InnerBuffer(Buffer);
	CaseConvert(InnerBuffer, true);

//DNS type name
	if (InnerBuffer == (L"A"))
		return htons(DNS_RECORD_A);
	else if (InnerBuffer == (L"NS"))
		return htons(DNS_RECORD_NS);
	else if (InnerBuffer == (L"MD"))
		return htons(DNS_RECORD_MD);
	else if (InnerBuffer == (L"MF"))
		return htons(DNS_RECORD_MF);
	else if (InnerBuffer == (L"CNAME"))
		return htons(DNS_RECORD_CNAME);
	else if (InnerBuffer == (L"SOA"))
		return htons(DNS_RECORD_SOA);
	else if (InnerBuffer == (L"MB"))
		return htons(DNS_RECORD_MB);
	else if (InnerBuffer == (L"MG"))
		return htons(DNS_RECORD_MG);
	else if (InnerBuffer == (L"MR"))
		return htons(DNS_RECORD_MR);
	else if (InnerBuffer == (L"PTR"))
		return htons(DNS_RECORD_PTR);
	else if (InnerBuffer == (L"NULL"))
		return htons(DNS_RECORD_NULL);
	else if (InnerBuffer == (L"WKS"))
		return htons(DNS_RECORD_WKS);
	else if (InnerBuffer == (L"HINFO"))
		return htons(DNS_RECORD_HINFO);
	else if (InnerBuffer == (L"MINFO"))
		return htons(DNS_RECORD_MINFO);
	else if (InnerBuffer == (L"MX"))
		return htons(DNS_RECORD_MX);
	else if (InnerBuffer == (L"TXT"))
		return htons(DNS_RECORD_TXT);
	else if (InnerBuffer == (L"RP"))
		return htons(DNS_RECORD_RP);
	else if (InnerBuffer == (L"SIG"))
		return htons(DNS_RECORD_SIG);
	else if (InnerBuffer == (L"AFSDB"))
		return htons(DNS_RECORD_AFSDB);
	else if (InnerBuffer == (L"X25"))
		return htons(DNS_RECORD_X25);
	else if (InnerBuffer == (L"ISDN"))
		return htons(DNS_RECORD_ISDN);
	else if (InnerBuffer == (L"RT"))
		return htons(DNS_RECORD_RT);
	else if (InnerBuffer == (L"NSAP"))
		return htons(DNS_RECORD_NSAP);
	else if (InnerBuffer == (L"NSAPPTR"))
		return htons(DNS_RECORD_NSAP_PTR);
	else if (InnerBuffer == (L"SIG"))
		return htons(DNS_RECORD_SIG);
	else if (InnerBuffer == (L"KEY"))
		return htons(DNS_RECORD_KEY);
	else if (InnerBuffer == (L"AAAA"))
		return htons(DNS_RECORD_AAAA);
	else if (InnerBuffer == (L"PX"))
		return htons(DNS_RECORD_PX);
	else if (InnerBuffer == (L"GPOS"))
		return htons(DNS_RECORD_GPOS);
	else if (InnerBuffer == (L"LOC"))
		return htons(DNS_RECORD_LOC);
	else if (InnerBuffer == (L"NXT"))
		return htons(DNS_RECORD_NXT);
	else if (InnerBuffer == (L"EID"))
		return htons(DNS_RECORD_EID);
	else if (InnerBuffer == (L"NIMLOC"))
		return htons(DNS_RECORD_NIMLOC);
	else if (InnerBuffer == (L"SRV"))
		return htons(DNS_RECORD_SRV);
	else if (InnerBuffer == (L"ATMA"))
		return htons(DNS_RECORD_ATMA);
	else if (InnerBuffer == (L"NAPTR"))
		return htons(DNS_RECORD_NAPTR);
	else if (InnerBuffer == (L"KX"))
		return htons(DNS_RECORD_KX);
	else if (InnerBuffer == (L"CERT"))
		return htons(DNS_RECORD_CERT);
	else if (InnerBuffer == (L"A6"))
		return htons(DNS_RECORD_A6);
	else if (InnerBuffer == (L"DNAME"))
		return htons(DNS_RECORD_DNAME);
	else if (InnerBuffer == (L"SINK"))
		return htons(DNS_RECORD_SINK);
	else if (InnerBuffer == (L"OPT"))
		return htons(DNS_RECORD_OPT);
	else if (InnerBuffer == (L"APL"))
		return htons(DNS_RECORD_APL);
	else if (InnerBuffer == (L"DS"))
		return htons(DNS_RECORD_DS);
	else if (InnerBuffer == (L"SSHFP"))
		return htons(DNS_RECORD_SSHFP);
	else if (InnerBuffer == (L"IPSECKEY"))
		return htons(DNS_RECORD_IPSECKEY);
	else if (InnerBuffer == (L"RRSIG"))
		return htons(DNS_RECORD_RRSIG);
	else if (InnerBuffer == (L"NSEC"))
		return htons(DNS_RECORD_NSEC);
	else if (InnerBuffer == (L"DNSKEY"))
		return htons(DNS_RECORD_DNSKEY);
	else if (InnerBuffer == (L"DHCID"))
		return htons(DNS_RECORD_DHCID);
	else if (InnerBuffer == (L"NSEC3"))
		return htons(DNS_RECORD_NSEC3);
	else if (InnerBuffer == (L"NSEC3PARAM"))
		return htons(DNS_RECORD_NSEC3PARAM);
	else if (InnerBuffer == (L"TLSA"))
		return htons(DNS_RECORD_TLSA);
	else if (InnerBuffer == (L"HIP"))
		return htons(DNS_RECORD_HIP);
	else if (InnerBuffer == (L"HINFO"))
		return htons(DNS_RECORD_HINFO);
	else if (InnerBuffer == (L"RKEY"))
		return htons(DNS_RECORD_RKEY);
	else if (InnerBuffer == (L"TALINK"))
		return htons(DNS_RECORD_TALINK);
	else if (InnerBuffer == (L"CDS"))
		return htons(DNS_RECORD_CDS);
	else if (InnerBuffer == (L"CDNSKEY"))
		return htons(DNS_RECORD_CDNSKEY);
	else if (InnerBuffer == (L"OPENPGPKEY"))
		return htons(DNS_RECORD_OPENPGPKEY);
	else if (InnerBuffer == (L"SPF"))
		return htons(DNS_RECORD_SPF);
	else if (InnerBuffer == (L"UINFO"))
		return htons(DNS_RECORD_UINFO);
	else if (InnerBuffer == (L"UID"))
		return htons(DNS_RECORD_UID);
	else if (InnerBuffer == (L"GID"))
		return htons(DNS_RECORD_GID);
	else if (InnerBuffer == (L"UNSPEC"))
		return htons(DNS_RECORD_UNSPEC);
	else if (InnerBuffer == (L"NID"))
		return htons(DNS_RECORD_NID);
	else if (InnerBuffer == (L"L32"))
		return htons(DNS_RECORD_L32);
	else if (InnerBuffer == (L"L64"))
		return htons(DNS_RECORD_L64);
	else if (InnerBuffer == (L"LP"))
		return htons(DNS_RECORD_LP);
	else if (InnerBuffer == (L"EUI48"))
		return htons(DNS_RECORD_EUI48);
	else if (InnerBuffer == (L"EUI64"))
		return htons(DNS_RECORD_EUI64);
	else if (InnerBuffer == (L"TKEY"))
		return htons(DNS_RECORD_TKEY);
	else if (InnerBuffer == (L"TSIG"))
		return htons(DNS_RECORD_TSIG);
	else if (InnerBuffer == (L"IXFR"))
		return htons(DNS_RECORD_IXFR);
	else if (InnerBuffer == (L"AXFR"))
		return htons(DNS_RECORD_AXFR);
	else if (InnerBuffer == (L"MAILB"))
		return htons(DNS_RECORD_MAILB);
	else if (InnerBuffer == (L"MAILA"))
		return htons(DNS_RECORD_MAILA);
	else if (InnerBuffer == (L"ANY"))
		return htons(DNS_RECORD_ANY);
	else if (InnerBuffer == (L"URI"))
		return htons(DNS_RECORD_URI);
	else if (InnerBuffer == (L"CAA"))
		return htons(DNS_RECORD_CAA);
	else if (InnerBuffer == (L"TA"))
		return htons(DNS_RECORD_TA);
	else if (InnerBuffer == (L"DLV"))
		return htons(DNS_RECORD_DLV);
	else if (InnerBuffer == (L"RESERVED"))
		return htons(DNS_RECORD_RESERVED);

//No match.
	return 0;
}

//Convert data from string to DNS query
size_t CharToDNSQuery(
	const uint8_t *FName, 
	uint8_t *TName)
{
//Initialization
	int Index[]{(int)strnlen_s((const char *)FName, DOMAIN_MAXSIZE) - 1, 0, 0};
	Index[2U] = Index[0] + 1;
	*(TName + Index[0] + 2) = 0;

//Convert domain.
	for (;Index[0] >= 0;--Index[0], --Index[2U])
	{
		if (FName[Index[0]] == ASCII_PERIOD)
		{
			*(TName + Index[2U]) = (uint8_t)Index[1U];
			Index[1U] = 0;
		}
		else {
			*(TName + Index[2U]) = FName[Index[0]];
			++Index[1U];
		}
	}

	*(TName + Index[2U]) = (uint8_t)Index[1U];
	return strnlen_s((const char *)TName, DOMAIN_MAXSIZE - 1U) + 1U;
}

//Convert data from DNS query to char(s)
size_t DNSQueryToChar(
	const uint8_t *TName, 
	uint8_t *FName, 
	uint16_t &Truncated)
{
//Initialization
	size_t uIndex = 0;
	int Index[] = {0, 0};

//Convert domain.
	for (uIndex = 0;uIndex < DOMAIN_MAXSIZE;++uIndex)
	{
	//Pointer
		if (TName[uIndex] >= 0xC0)
		{
			Truncated = (uint8_t)(TName[uIndex] & 0x3F);
			Truncated = Truncated << sizeof(uint8_t) * BYTES_TO_BITS;
			Truncated += (uint8_t)TName[uIndex + 1U];
			return uIndex + sizeof(uint16_t);
		}
		else if (uIndex == 0)
		{
			Index[0] = TName[uIndex];
		}
		else if (uIndex == Index[0] + Index[1U] + 1U)
		{
			Index[0] = TName[uIndex];
			if (Index[0] == 0)
				break;
			Index[1U] = (int)uIndex;

			FName[uIndex - 1U] = ASCII_PERIOD;
		}
		else {
			FName[uIndex - 1U] = TName[uIndex];
		}
	}

	Truncated = 0;
	return uIndex;
}

//Validate packets
bool ValidatePacket(
	const uint8_t *Buffer, 
	const size_t Length, 
	const uint16_t DNS_ID)
{
	auto pdns_hdr = (dns_hdr *)Buffer;

//DNS ID and Questions check
	if (pdns_hdr->ID != DNS_ID || pdns_hdr->Questions == 0)
		return false;

//EDNS Label check
	if (ConfigurationParameter.IsEDNS)
	{
		if (pdns_hdr->Additional == 0)
		{
			return false;
		}
		else if (pdns_hdr->Additional == 1U)
		{
			if (Length > sizeof(dns_opt_record))
			{
				auto pdns_opt_record = (dns_opt_record *)(Buffer + Length - sizeof(dns_opt_record));

			//UDP Payload Size and Z Field of DNSSEC check
				if (pdns_opt_record->UDPPayloadSize == 0 || (ConfigurationParameter.IsDNSSEC && pdns_opt_record->Z_Field == 0))
					return false;
			}
			else {
				return false;
			}
		}
	}

	return true;
}

//Print date from seconds to file
void PrintSecondsInDateTime(
	FILE *FileHandle, 
	const time_t Seconds)
{
//Less than 1 minute
	if (Seconds < SECONDS_IN_MINUTE)
		return;

//Initialization
	auto Before = false;
	auto DateTime = Seconds;
	fwprintf_s(FileHandle, L"(");

//Years
	if (DateTime / SECONDS_IN_YEAR > 0)
	{
		fwprintf_s(FileHandle, L"%u year", (unsigned int)(DateTime / SECONDS_IN_YEAR));
		if (DateTime / SECONDS_IN_YEAR > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_YEAR;
		Before = true;
	}
//Months
	if (DateTime / SECONDS_IN_MONTH > 0)
	{
		if (Before)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u month", (unsigned int)(DateTime / SECONDS_IN_MONTH));
		if (DateTime / SECONDS_IN_MONTH > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_MONTH;
		Before = true;
	}
//Days
	if (DateTime / SECONDS_IN_DAY > 0)
	{
		if (Before)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u day", (unsigned int)(DateTime / SECONDS_IN_DAY));
		if (DateTime / SECONDS_IN_DAY > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_DAY;
		Before = true;
	}
//Hours
	if (DateTime / SECONDS_IN_HOUR > 0)
	{
		if (Before)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u hour", (unsigned int)(DateTime / SECONDS_IN_HOUR));
		if (DateTime / SECONDS_IN_HOUR > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_HOUR;
		Before = true;
	}
//Minutes
	if (DateTime / SECONDS_IN_MINUTE > 0)
	{
		if (Before)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u minute", (unsigned int)(DateTime / SECONDS_IN_MINUTE));
		if (DateTime / SECONDS_IN_MINUTE > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_MINUTE;
		Before = true;
	}
//Seconds
	if (DateTime > 0)
	{
		if (Before)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u second", (unsigned int)(DateTime));
		if (DateTime > 1U)
			fwprintf_s(FileHandle, L"s");
	}

	fwprintf_s(FileHandle, L")");
	return;
}

//Print Date and Time with UNIX time to file
void PrintDateTime(
	FILE *FileHandle, 
	const time_t Time)
{
	tm TimeStructure;
	memset(&TimeStructure, 0, sizeof(TimeStructure));
	localtime_s(&TimeStructure, &Time);
	fwprintf_s(
		FileHandle, 
		L"%d-%02d-%02d %02d:%02d:%02d", 
		TimeStructure.tm_year + 1900, 
		TimeStructure.tm_mon + 1, 
		TimeStructure.tm_mday, 
		TimeStructure.tm_hour, 
		TimeStructure.tm_min, 
		TimeStructure.tm_sec);

	return;
}
