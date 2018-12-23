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


#include "Protocol.h"

//Check empty buffer
bool CheckEmptyBuffer(
	const void * const Buffer, 
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
			if (*(static_cast<const uint8_t *>(Buffer) + Index) != 0)
				return false;
		}
	}

	return true;
}

//Convert multiple bytes to wide char string
bool MBS_To_WCS_String(
	const uint8_t * const Buffer, 
	const size_t MaxLen, 
	std::wstring &Target)
{
//Check buffer.
	Target.clear();
	if (Buffer == nullptr || MaxLen == 0)
		return false;
	const auto Length = strnlen_s(reinterpret_cast<const char *>(Buffer), MaxLen);
	if (Length == 0 || CheckEmptyBuffer(Buffer, Length))
		return false;

//Convert string.
	auto TargetBuffer = std::make_unique<wchar_t[]>(Length + MEMORY_RESERVED_BYTES);
	wmemset(TargetBuffer.get(), 0, Length + MEMORY_RESERVED_BYTES);
#if defined(PLATFORM_WIN)
	if (MultiByteToWideChar(
			CP_ACP, 
			0, 
			reinterpret_cast<LPCCH>(Buffer), 
			MBSTOWCS_NULL_TERMINATE, 
			TargetBuffer.get(), 
			static_cast<const int>(Length + NULL_TERMINATE_LENGTH)) == 0)
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	if (mbstowcs(TargetBuffer.get(), reinterpret_cast<const char *>(Buffer), Length + NULL_TERMINATE_LENGTH) == static_cast<const size_t>(RETURN_ERROR))
#endif
	{
		return false;
	}
	else {
		if (wcsnlen_s(TargetBuffer.get(), Length + NULL_TERMINATE_LENGTH) == 0)
			return false;
		else 
			Target = TargetBuffer.get();
	}

	return true;
}

//Convert wide char string to multiple bytes
bool WCS_To_MBS_String(
	const wchar_t * const Buffer, 
	const size_t MaxLen, 
	std::string &Target)
{
//Check buffer.
	Target.clear();
	if (Buffer == nullptr || MaxLen == 0)
		return false;
	const size_t Length = wcsnlen_s(Buffer, MaxLen);
	if (Length == 0 || CheckEmptyBuffer(Buffer, sizeof(wchar_t) * Length))
		return false;

//Convert string.
	auto TargetBuffer = std::make_unique<uint8_t[]>(Length + MEMORY_RESERVED_BYTES);
	memset(TargetBuffer.get(), 0, Length + MEMORY_RESERVED_BYTES);
#if defined(PLATFORM_WIN)
	if (WideCharToMultiByte(
			CP_ACP, 
			0, 
			Buffer, 
			WCSTOMBS_NULL_TERMINATE, 
			reinterpret_cast<LPSTR>(TargetBuffer.get()), 
			static_cast<const int>(Length + NULL_TERMINATE_LENGTH), 
			nullptr, 
			nullptr) == 0)
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	if (wcstombs(reinterpret_cast<char *>(TargetBuffer.get()), Buffer, Length + NULL_TERMINATE_LENGTH) == static_cast<const size_t>(RETURN_ERROR))
#endif
	{
		return false;
	}
	else {
		if (strnlen_s(reinterpret_cast<const char *>(TargetBuffer.get()), Length + NULL_TERMINATE_LENGTH) == 0)
			return false;
		else 
			Target = reinterpret_cast<const char *>(TargetBuffer.get());
	}

	return true;
}

//Convert lowercase/uppercase words to uppercase/lowercase words(C-Style version)
void CaseConvert(
	const bool IsLowerUpper, 
	uint8_t * const Buffer, 
	const size_t Length)
{
	for (size_t Index = 0;Index < Length;++Index)
	{
	//Lowercase to uppercase
		if (IsLowerUpper)
			Buffer[Index] = static_cast<const uint8_t>(toupper(Buffer[Index]));
	//Uppercase to lowercase
		else 
			Buffer[Index] = static_cast<const uint8_t>(tolower(Buffer[Index]));
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
			StringIter = static_cast<const wchar_t>(toupper(StringIter));
	//Uppercase to lowercase
		else 
			StringIter = static_cast<const wchar_t>(tolower(StringIter));
	}

	return;
}

//Convert address strings to binary
bool AddressStringToBinary(
	const uint16_t Protocol, 
	const uint8_t * const AddrBuffer, 
	void * const OriginalAddr, 
	ssize_t * const ErrorCode)
{
//Protocol check
	if (Protocol == AF_INET6)
		memset(OriginalAddr, 0, sizeof(in6_addr));
	else if (Protocol == AF_INET)
		memset(OriginalAddr, 0, sizeof(in_addr));
	else 
		return false;

//Initialization
	std::string AddrString(reinterpret_cast<const char *>(AddrBuffer));
	if (ErrorCode != nullptr)
		*ErrorCode = 0;

//Convert address.
	ssize_t ResultValue = 0;
	if (Protocol == AF_INET6)
	{
	//Check address.
		if (AddrString.find(ASCII_COLON) == std::string::npos || AddrString.find(ASCII_PERIOD) != std::string::npos || 
			AddrString.find("::") != AddrString.rfind("::"))
				return false;
		for (const auto &StringIter:AddrString)
		{
			if (StringIter < ASCII_ZERO || 
				(StringIter > ASCII_COLON && StringIter < ASCII_UPPERCASE_A) || 
				(StringIter > ASCII_UPPERCASE_F && StringIter < ASCII_LOWERCASE_A) || 
				StringIter > ASCII_LOWERCASE_F)
					return false;
		}

	//Check abbreviation format.
		if (AddrString.find(ASCII_COLON) == std::string::npos)
		{
			AddrString = ("::");
			AddrString.append(reinterpret_cast<const char *>(AddrBuffer));
		}
		else if (AddrString.find(ASCII_COLON) == AddrString.rfind(ASCII_COLON))
		{
			AddrString.replace(AddrString.find(ASCII_COLON), 1U, "::");
		}

	//Convert to binary.
		ResultValue = inet_pton(AF_INET6, AddrString.c_str(), OriginalAddr);
		if (ResultValue == SOCKET_ERROR || ResultValue == 0)
		{
			if (ResultValue != 0 && ErrorCode != nullptr)
				*ErrorCode = WSAGetLastError();

			return false;
		}
	}
	else if (Protocol == AF_INET)
	{
	//Check address.
		if (AddrString.find(ASCII_PERIOD) == std::string::npos || AddrString.find(ASCII_COLON) != std::string::npos)
			return false;
		size_t CommaNum = 0;
		for (const auto &StringIter:AddrString)
		{
			if ((StringIter != ASCII_PERIOD && StringIter < ASCII_ZERO) || StringIter > ASCII_NINE)
				return false;
			else if (StringIter == ASCII_PERIOD)
				++CommaNum;
		}

	//Remove zeros before whole data.
		while (AddrString.length() > 1U && AddrString.front() == ASCII_ZERO && AddrString.at(1U) != ASCII_PERIOD)
			AddrString.erase(0, 1U);

	//Check abbreviation format.
		switch (CommaNum)
		{
			case 0:
			{
				AddrString = ("0.0.0.");
				AddrString.append(reinterpret_cast<const char *>(AddrBuffer));
			}break;
			case 1U:
			{
				AddrString.replace(AddrString.find(ASCII_PERIOD), 1U, ".0.0.");
			}break;
			case 2U:
			{
				AddrString.replace(AddrString.find(ASCII_PERIOD), 1U, ".0.");
			}break;
		}

	//Remove zeros before data.
		while (AddrString.find(".00") != std::string::npos)
			AddrString.replace(AddrString.find(".00"), 3U, ".");
		while (AddrString.find(".0") != std::string::npos)
			AddrString.replace(AddrString.find(".0"), 2U, ".");
		while (AddrString.find("..") != std::string::npos)
			AddrString.replace(AddrString.find(".."), 2U, ".0.");
		if (AddrString.at(AddrString.length() - 1U) == ASCII_PERIOD)
			AddrString.append("0");

	//Convert to binary.
		ResultValue = inet_pton(AF_INET, AddrString.c_str(), OriginalAddr);
		if (ResultValue == SOCKET_ERROR || ResultValue == 0)
		{
			if (ResultValue != 0 && ErrorCode != nullptr)
				*ErrorCode = WSAGetLastError();

			return false;
		}
	}
	else {
		return false;
	}

	return true;
}

//Convert binary address strings
bool BinaryToAddressString(
	const uint16_t Protocol, 
	const void * const OriginalAddr, 
	void * const AddressString, 
	const size_t StringSize, 
	ssize_t * const ErrorCode)
{
//Initialization
	if (ErrorCode != nullptr)
		*ErrorCode = 0;

//Convert address.
	if (inet_ntop(Protocol, OriginalAddr, static_cast<char *>(AddressString), static_cast<const socklen_t>(StringSize)) == nullptr)
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
		return hton16(IPPORT_TCPMUX);
	else if (InnerBuffer == (L"ECHO"))
		return hton16(IPPORT_ECHO);
	else if (InnerBuffer == (L"DISCARD"))
		return hton16(IPPORT_DISCARD);
	else if (InnerBuffer == (L"SYSTAT"))
		return hton16(IPPORT_SYSTAT);
	else if (InnerBuffer == (L"DAYTIME"))
		return hton16(IPPORT_DAYTIME);
	else if (InnerBuffer == (L"NETSTAT"))
		return hton16(IPPORT_NETSTAT);
	else if (InnerBuffer == (L"QOTD"))
		return hton16(IPPORT_QOTD);
	else if (InnerBuffer == (L"MSP"))
		return hton16(IPPORT_MSP);
	else if (InnerBuffer == (L"CHARGEN"))
		return hton16(IPPORT_CHARGEN);
	else if (InnerBuffer == (L"FTPDATA"))
		return hton16(IPPORT_FTP_DATA);
	else if (InnerBuffer == (L"FTP"))
		return hton16(IPPORT_FTP);
	else if (InnerBuffer == (L"SSH"))
		return hton16(IPPORT_SSH);
	else if (InnerBuffer == (L"TELNET"))
		return hton16(IPPORT_TELNET);
	else if (InnerBuffer == (L"SMTP"))
		return hton16(IPPORT_SMTP);
	else if (InnerBuffer == (L"TIMESERVER"))
		return hton16(IPPORT_TIMESERVER);
	else if (InnerBuffer == (L"RAP"))
		return hton16(IPPORT_RAP);
	else if (InnerBuffer == (L"RLP"))
		return hton16(IPPORT_RLP);
	else if (InnerBuffer == (L"NAMESERVER"))
		return hton16(IPPORT_NAMESERVER);
	else if (InnerBuffer == (L"WHOIS"))
		return hton16(IPPORT_WHOIS);
	else if (InnerBuffer == (L"TACACS"))
		return hton16(IPPORT_TACACS);
	else if (InnerBuffer == (L"DNS"))
		return hton16(IPPORT_DNS);
	else if (InnerBuffer == (L"XNSAUTH"))
		return hton16(IPPORT_XNSAUTH);
	else if (InnerBuffer == (L"MTP"))
		return hton16(IPPORT_MTP);
	else if (InnerBuffer == (L"BOOTPS"))
		return hton16(IPPORT_BOOTPS);
	else if (InnerBuffer == (L"BOOTPC"))
		return hton16(IPPORT_BOOTPC);
	else if (InnerBuffer == (L"TFTP"))
		return hton16(IPPORT_TFTP);
	else if (InnerBuffer == (L"RJE"))
		return hton16(IPPORT_RJE);
	else if (InnerBuffer == (L"FINGER"))
		return hton16(IPPORT_FINGER);
	else if (InnerBuffer == (L"HTTP"))
		return hton16(IPPORT_HTTP);
	else if (InnerBuffer == (L"HTTPBACKUP"))
		return hton16(IPPORT_HTTPBACKUP);
	else if (InnerBuffer == (L"TTYLINK"))
		return hton16(IPPORT_TTYLINK);
	else if (InnerBuffer == (L"SUPDUP"))
		return hton16(IPPORT_SUPDUP);
	else if (InnerBuffer == (L"POP3"))
		return hton16(IPPORT_POP3);
	else if (InnerBuffer == (L"SUNRPC"))
		return hton16(IPPORT_SUNRPC);
	else if (InnerBuffer == (L"SQL"))
		return hton16(IPPORT_SQL);
	else if (InnerBuffer == (L"NTP"))
		return hton16(IPPORT_NTP);
	else if (InnerBuffer == (L"EPMAP"))
		return hton16(IPPORT_EPMAP);
	else if (InnerBuffer == (L"NETBIOS_NS"))
		return hton16(IPPORT_NETBIOS_NS);
	else if (InnerBuffer == (L"NETBIOS_DGM"))
		return hton16(IPPORT_NETBIOS_DGM);
	else if (InnerBuffer == (L"NETBIOS_SSN"))
		return hton16(IPPORT_NETBIOS_SSN);
	else if (InnerBuffer == (L"IMAP"))
		return hton16(IPPORT_IMAP);
	else if (InnerBuffer == (L"BFTP"))
		return hton16(IPPORT_BFTP);
	else if (InnerBuffer == (L"SGMP"))
		return hton16(IPPORT_SGMP);
	else if (InnerBuffer == (L"SQLSRV"))
		return hton16(IPPORT_SQLSRV);
	else if (InnerBuffer == (L"DMSP"))
		return hton16(IPPORT_DMSP);
	else if (InnerBuffer == (L"SNMP"))
		return hton16(IPPORT_SNMP);
	else if (InnerBuffer == (L"SNMP_TRAP"))
		return hton16(IPPORT_SNMP_TRAP);
	else if (InnerBuffer == (L"ATRTMP"))
		return hton16(IPPORT_ATRTMP);
	else if (InnerBuffer == (L"ATHBP"))
		return hton16(IPPORT_ATHBP);
	else if (InnerBuffer == (L"QMTP"))
		return hton16(IPPORT_QMTP);
	else if (InnerBuffer == (L"IPX"))
		return hton16(IPPORT_IPX);
	else if (InnerBuffer == (L"IMAP3"))
		return hton16(IPPORT_IMAP3);
	else if (InnerBuffer == (L"BGMP"))
		return hton16(IPPORT_BGMP);
	else if (InnerBuffer == (L"TSP"))
		return hton16(IPPORT_TSP);
	else if (InnerBuffer == (L"IMMP"))
		return hton16(IPPORT_IMMP);
	else if (InnerBuffer == (L"ODMR"))
		return hton16(IPPORT_ODMR);
	else if (InnerBuffer == (L"RPC2PORTMAP"))
		return hton16(IPPORT_RPC2PORTMAP);
	else if (InnerBuffer == (L"CLEARCASE"))
		return hton16(IPPORT_CLEARCASE);
	else if (InnerBuffer == (L"HPALARMMGR"))
		return hton16(IPPORT_HPALARMMGR);
	else if (InnerBuffer == (L"ARNS"))
		return hton16(IPPORT_ARNS);
	else if (InnerBuffer == (L"AURP"))
		return hton16(IPPORT_AURP);
	else if (InnerBuffer == (L"LDAP"))
		return hton16(IPPORT_LDAP);
	else if (InnerBuffer == (L"UPS"))
		return hton16(IPPORT_UPS);
	else if (InnerBuffer == (L"SLP"))
		return hton16(IPPORT_SLP);
	else if (InnerBuffer == (L"HTTPS"))
		return hton16(IPPORT_HTTPS);
	else if (InnerBuffer == (L"SNPP"))
		return hton16(IPPORT_SNPP);
	else if (InnerBuffer == (L"MICROSOFTDS"))
		return hton16(IPPORT_MICROSOFT_DS);
	else if (InnerBuffer == (L"KPASSWD"))
		return hton16(IPPORT_KPASSWD);
	else if (InnerBuffer == (L"TCPNETHASPSRV"))
		return hton16(IPPORT_TCPNETHASPSRV);
	else if (InnerBuffer == (L"RETROSPECT"))
		return hton16(IPPORT_RETROSPECT);
	else if (InnerBuffer == (L"ISAKMP"))
		return hton16(IPPORT_ISAKMP);
	else if (InnerBuffer == (L"BIFFUDP"))
		return hton16(IPPORT_BIFFUDP);
	else if (InnerBuffer == (L"WHOSERVER"))
		return hton16(IPPORT_WHOSERVER);
	else if (InnerBuffer == (L"SYSLOG"))
		return hton16(IPPORT_SYSLOG);
	else if (InnerBuffer == (L"ROUTERSERVER"))
		return hton16(IPPORT_ROUTESERVER);
	else if (InnerBuffer == (L"NCP"))
		return hton16(IPPORT_NCP);
	else if (InnerBuffer == (L"COURIER"))
		return hton16(IPPORT_COURIER);
	else if (InnerBuffer == (L"COMMERCE"))
		return hton16(IPPORT_COMMERCE);
	else if (InnerBuffer == (L"RTSP"))
		return hton16(IPPORT_RTSP);
	else if (InnerBuffer == (L"NNTP"))
		return hton16(IPPORT_NNTP);
	else if (InnerBuffer == (L"HTTPRPCEPMAP"))
		return hton16(IPPORT_HTTPRPCEPMAP);
	else if (InnerBuffer == (L"IPP"))
		return hton16(IPPORT_IPP);
	else if (InnerBuffer == (L"LDAPS"))
		return hton16(IPPORT_LDAPS);
	else if (InnerBuffer == (L"MSDP"))
		return hton16(IPPORT_MSDP);
	else if (InnerBuffer == (L"AODV"))
		return hton16(IPPORT_AODV);
	else if (InnerBuffer == (L"FTPSDATA"))
		return hton16(IPPORT_FTPSDATA);
	else if (InnerBuffer == (L"FTPS"))
		return hton16(IPPORT_FTPS);
	else if (InnerBuffer == (L"NAS"))
		return hton16(IPPORT_NAS);
	else if (InnerBuffer == (L"TELNETS"))
		return hton16(IPPORT_TELNETS);

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
		return hton16(DNS_CLASS_INTERNET);
	else if (InnerBuffer == L"CSNET")
		return hton16(DNS_CLASS_CSNET);
	else if (InnerBuffer == L"CHAOS")
		return hton16(DNS_CLASS_CHAOS);
	else if (InnerBuffer == L"HESIOD")
		return hton16(DNS_CLASS_HESIOD);
	else if (InnerBuffer == L"NONE")
		return hton16(DNS_CLASS_NONE);
	else if (InnerBuffer == L"ALL")
		return hton16(DNS_CLASS_ALL);
	else if (InnerBuffer == L"ANY")
		return hton16(DNS_CLASS_ANY);

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
		return hton16(DNS_TYPE_A);
	else if (InnerBuffer == (L"NS"))
		return hton16(DNS_TYPE_NS);
	else if (InnerBuffer == (L"MD"))
		return hton16(DNS_TYPE_MD);
	else if (InnerBuffer == (L"MF"))
		return hton16(DNS_TYPE_MF);
	else if (InnerBuffer == (L"CNAME"))
		return hton16(DNS_TYPE_CNAME);
	else if (InnerBuffer == (L"SOA"))
		return hton16(DNS_TYPE_SOA);
	else if (InnerBuffer == (L"MB"))
		return hton16(DNS_TYPE_MB);
	else if (InnerBuffer == (L"MG"))
		return hton16(DNS_TYPE_MG);
	else if (InnerBuffer == (L"MR"))
		return hton16(DNS_TYPE_MR);
	else if (InnerBuffer == (L"PTR"))
		return hton16(DNS_TYPE_PTR);
	else if (InnerBuffer == (L"NULL"))
		return hton16(DNS_TYPE_NULL);
	else if (InnerBuffer == (L"WKS"))
		return hton16(DNS_TYPE_WKS);
	else if (InnerBuffer == (L"HINFO"))
		return hton16(DNS_TYPE_HINFO);
	else if (InnerBuffer == (L"MINFO"))
		return hton16(DNS_TYPE_MINFO);
	else if (InnerBuffer == (L"MX"))
		return hton16(DNS_TYPE_MX);
	else if (InnerBuffer == (L"TXT"))
		return hton16(DNS_TYPE_TEXT);
	else if (InnerBuffer == (L"RP"))
		return hton16(DNS_TYPE_RP);
	else if (InnerBuffer == (L"SIG"))
		return hton16(DNS_TYPE_SIG);
	else if (InnerBuffer == (L"AFSDB"))
		return hton16(DNS_TYPE_AFSDB);
	else if (InnerBuffer == (L"X25"))
		return hton16(DNS_TYPE_X25);
	else if (InnerBuffer == (L"ISDN"))
		return hton16(DNS_TYPE_ISDN);
	else if (InnerBuffer == (L"RT"))
		return hton16(DNS_TYPE_RT);
	else if (InnerBuffer == (L"NSAP"))
		return hton16(DNS_TYPE_NSAP);
	else if (InnerBuffer == (L"NSAPPTR"))
		return hton16(DNS_TYPE_NSAPPTR);
	else if (InnerBuffer == (L"SIG"))
		return hton16(DNS_TYPE_SIG);
	else if (InnerBuffer == (L"KEY"))
		return hton16(DNS_TYPE_KEY);
	else if (InnerBuffer == (L"AAAA"))
		return hton16(DNS_TYPE_AAAA);
	else if (InnerBuffer == (L"PX"))
		return hton16(DNS_TYPE_PX);
	else if (InnerBuffer == (L"GPOS"))
		return hton16(DNS_TYPE_GPOS);
	else if (InnerBuffer == (L"LOC"))
		return hton16(DNS_TYPE_LOC);
	else if (InnerBuffer == (L"NXT"))
		return hton16(DNS_TYPE_NXT);
	else if (InnerBuffer == (L"EID"))
		return hton16(DNS_TYPE_EID);
	else if (InnerBuffer == (L"NIMLOC"))
		return hton16(DNS_TYPE_NIMLOC);
	else if (InnerBuffer == (L"SRV"))
		return hton16(DNS_TYPE_SRV);
	else if (InnerBuffer == (L"ATMA"))
		return hton16(DNS_TYPE_ATMA);
	else if (InnerBuffer == (L"NAPTR"))
		return hton16(DNS_TYPE_NAPTR);
	else if (InnerBuffer == (L"KX"))
		return hton16(DNS_TYPE_KX);
	else if (InnerBuffer == (L"CERT"))
		return hton16(DNS_TYPE_CERT);
	else if (InnerBuffer == (L"A6"))
		return hton16(DNS_TYPE_A6);
	else if (InnerBuffer == (L"DNAME"))
		return hton16(DNS_TYPE_DNAME);
	else if (InnerBuffer == (L"SINK"))
		return hton16(DNS_TYPE_SINK);
	else if (InnerBuffer == (L"OPT"))
		return hton16(DNS_TYPE_OPT);
	else if (InnerBuffer == (L"APL"))
		return hton16(DNS_TYPE_APL);
	else if (InnerBuffer == (L"DS"))
		return hton16(DNS_TYPE_DS);
	else if (InnerBuffer == (L"SSHFP"))
		return hton16(DNS_TYPE_SSHFP);
	else if (InnerBuffer == (L"IPSECKEY"))
		return hton16(DNS_TYPE_IPSECKEY);
	else if (InnerBuffer == (L"RRSIG"))
		return hton16(DNS_TYPE_RRSIG);
	else if (InnerBuffer == (L"NSEC"))
		return hton16(DNS_TYPE_NSEC);
	else if (InnerBuffer == (L"DNSKEY"))
		return hton16(DNS_TYPE_DNSKEY);
	else if (InnerBuffer == (L"DHCID"))
		return hton16(DNS_TYPE_DHCID);
	else if (InnerBuffer == (L"NSEC3"))
		return hton16(DNS_TYPE_NSEC3);
	else if (InnerBuffer == (L"NSEC3PARAM"))
		return hton16(DNS_TYPE_NSEC3PARAM);
	else if (InnerBuffer == (L"TLSA"))
		return hton16(DNS_TYPE_TLSA);
	else if (InnerBuffer == (L"HIP"))
		return hton16(DNS_TYPE_HIP);
	else if (InnerBuffer == (L"HINFO"))
		return hton16(DNS_TYPE_HINFO);
	else if (InnerBuffer == (L"RKEY"))
		return hton16(DNS_TYPE_RKEY);
	else if (InnerBuffer == (L"TALINK"))
		return hton16(DNS_TYPE_TALINK);
	else if (InnerBuffer == (L"CDS"))
		return hton16(DNS_TYPE_CDS);
	else if (InnerBuffer == (L"CDNSKEY"))
		return hton16(DNS_TYPE_CDNSKEY);
	else if (InnerBuffer == (L"OPENPGPKEY"))
		return hton16(DNS_TYPE_OPENPGPKEY);
	else if (InnerBuffer == (L"SPF"))
		return hton16(DNS_TYPE_SPF);
	else if (InnerBuffer == (L"UINFO"))
		return hton16(DNS_TYPE_UINFO);
	else if (InnerBuffer == (L"UID"))
		return hton16(DNS_TYPE_UID);
	else if (InnerBuffer == (L"GID"))
		return hton16(DNS_TYPE_GID);
	else if (InnerBuffer == (L"UNSPEC"))
		return hton16(DNS_TYPE_UNSPEC);
	else if (InnerBuffer == (L"NID"))
		return hton16(DNS_TYPE_NID);
	else if (InnerBuffer == (L"L32"))
		return hton16(DNS_TYPE_L32);
	else if (InnerBuffer == (L"L64"))
		return hton16(DNS_TYPE_L64);
	else if (InnerBuffer == (L"LP"))
		return hton16(DNS_TYPE_LP);
	else if (InnerBuffer == (L"EUI48"))
		return hton16(DNS_TYPE_EUI48);
	else if (InnerBuffer == (L"EUI64"))
		return hton16(DNS_TYPE_EUI64);
	else if (InnerBuffer == (L"ADDRS"))
		return hton16(DNS_TYPE_ADDRS);
	else if (InnerBuffer == (L"TKEY"))
		return hton16(DNS_TYPE_TKEY);
	else if (InnerBuffer == (L"TSIG"))
		return hton16(DNS_TYPE_TSIG);
	else if (InnerBuffer == (L"IXFR"))
		return hton16(DNS_TYPE_IXFR);
	else if (InnerBuffer == (L"AXFR"))
		return hton16(DNS_TYPE_AXFR);
	else if (InnerBuffer == (L"MAILB"))
		return hton16(DNS_TYPE_MAILB);
	else if (InnerBuffer == (L"MAILA"))
		return hton16(DNS_TYPE_MAILA);
	else if (InnerBuffer == (L"ANY"))
		return hton16(DNS_TYPE_ANY);
	else if (InnerBuffer == (L"URI"))
		return hton16(DNS_TYPE_URI);
	else if (InnerBuffer == (L"CAA"))
		return hton16(DNS_TYPE_CAA);
	else if (InnerBuffer == (L"TA"))
		return hton16(DNS_TYPE_TA);
	else if (InnerBuffer == (L"DLV"))
		return hton16(DNS_TYPE_DLV);
	else if (InnerBuffer == (L"RESERVED"))
		return hton16(DNS_TYPE_RESERVED);

//No match.
	return 0;
}

//Convert data from string to DNS query
size_t StringToPacketQuery(
	const uint8_t * const FName, 
	uint8_t * const TName)
{
//Initialization
	std::array<int, 3U> Index{};
	Index.at(0) = static_cast<const int>(strnlen_s(reinterpret_cast<const char *>(FName), DOMAIN_MAXSIZE));
	if (Index.at(0) <= 0)
	{
		return 0;
	}
	else {
		--Index.at(0);
		Index.at(2U) = Index.at(0) + 1;
		*(TName + Index.at(0) + 2) = 0;
	}

//Convert domain.
	for (;Index.at(0) >= 0;--Index.at(0), --Index.at(2U))
	{
		if (FName[Index.at(0)] == ASCII_PERIOD)
		{
			*(TName + Index.at(2U)) = static_cast<const uint8_t>(Index.at(1U));
			Index.at(1U) = 0;
		}
		else {
			*(TName + Index.at(2U)) = FName[Index.at(0)];
			++Index.at(1U);
		}
	}

	*(TName + Index.at(2U)) = static_cast<const uint8_t>(Index.at(1U));
	return strnlen_s(reinterpret_cast<const char *>(TName), DOMAIN_MAXSIZE - 1U) + NULL_TERMINATE_LENGTH;
}

//Convert data from DNS query to string
size_t PacketQueryToString(
	const uint8_t * const TName, 
	uint8_t * const FName, 
	uint16_t &TruncateLocation)
{
//Initialization
	size_t LocateIndex = 0;
	std::array<int, 2U> MarkIndex{};

//Convert domain.
	for (LocateIndex = 0;LocateIndex < DOMAIN_MAXSIZE;++LocateIndex)
	{
	//Pointer
		if (TName[LocateIndex] >= DNS_POINTER_8_BITS)
		{
			TruncateLocation = static_cast<const uint8_t>(TName[LocateIndex] & 0x3F);
			TruncateLocation = TruncateLocation << sizeof(uint8_t) * BYTES_TO_BITS;
			TruncateLocation += static_cast<const uint8_t>(TName[LocateIndex + 1U]);
			return LocateIndex + sizeof(uint16_t);
		}
		else if (LocateIndex == 0)
		{
			MarkIndex.at(0) = TName[LocateIndex];
		}
		else if (LocateIndex == static_cast<const size_t>(MarkIndex.at(0)) + static_cast<const size_t>(MarkIndex.at(1U)) + 1U)
		{
			MarkIndex.at(0) = TName[LocateIndex];
			if (MarkIndex.at(0) == 0)
				break;
			MarkIndex.at(1U) = static_cast<const int>(LocateIndex);

			FName[LocateIndex - 1U] = ASCII_PERIOD;
		}
		else {
			FName[LocateIndex - 1U] = TName[LocateIndex];
		}
	}

	TruncateLocation = 0;
	return LocateIndex;
}

//Validate packets
bool ValidatePacket(
	const uint8_t * const Buffer, 
	const size_t Length, 
	const uint16_t DNS_ID)
{
	const auto DNS_Header = reinterpret_cast<const dns_hdr *>(Buffer);

//DNS ID and Questions check
	if (DNS_Header->ID != DNS_ID || DNS_Header->Question == 0)
		return false;

//EDNS Label check
	if (ConfigurationParameter.IsEDNS)
	{
		if (DNS_Header->Additional == 0)
		{
			return false;
		}
		else if (DNS_Header->Additional == 1U)
		{
			if (Length > sizeof(dns_record_opt))
			{
				const auto DNS_OPT_RECORD = reinterpret_cast<const dns_record_opt *>(Buffer + Length - sizeof(dns_record_opt));

			//UDP Payload Size and Z Field of DNSSEC check
				if (DNS_OPT_RECORD->UDP_PayloadSize == 0 || (ConfigurationParameter.IsDNSSEC && DNS_OPT_RECORD->Z_Field == 0))
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
	FILE * const FileHandle, 
	const time_t Seconds)
{
//Less than 1 minute
	if (Seconds < SECONDS_IN_MINUTE)
		return;

//Initialization
	auto IsBefore = false;
	auto DateTime = Seconds;
	fwprintf_s(FileHandle, L"(");

//Years
	if (DateTime / SECONDS_IN_YEAR > 0)
	{
		fwprintf_s(FileHandle, L"%u year", static_cast<const unsigned int>(DateTime / SECONDS_IN_YEAR));
		if (DateTime / SECONDS_IN_YEAR > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_YEAR;
		IsBefore = true;
	}
//Months
	if (DateTime / SECONDS_IN_MONTH > 0)
	{
		if (IsBefore)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u month", static_cast<const unsigned int>(DateTime / SECONDS_IN_MONTH));
		if (DateTime / SECONDS_IN_MONTH > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_MONTH;
		IsBefore = true;
	}
//Days
	if (DateTime / SECONDS_IN_DAY > 0)
	{
		if (IsBefore)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u day", static_cast<const unsigned int>(DateTime / SECONDS_IN_DAY));
		if (DateTime / SECONDS_IN_DAY > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_DAY;
		IsBefore = true;
	}
//Hours
	if (DateTime / SECONDS_IN_HOUR > 0)
	{
		if (IsBefore)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u hour", static_cast<const unsigned int>(DateTime / SECONDS_IN_HOUR));
		if (DateTime / SECONDS_IN_HOUR > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_HOUR;
		IsBefore = true;
	}
//Minutes
	if (DateTime / SECONDS_IN_MINUTE > 0)
	{
		if (IsBefore)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u minute", static_cast<const unsigned int>(DateTime / SECONDS_IN_MINUTE));
		if (DateTime / SECONDS_IN_MINUTE > 1U)
			fwprintf_s(FileHandle, L"s");
		DateTime %= SECONDS_IN_MINUTE;
		IsBefore = true;
	}
//Seconds
	if (DateTime > 0)
	{
		if (IsBefore)
			fwprintf_s(FileHandle, L" ");
		fwprintf_s(FileHandle, L"%u second", static_cast<const unsigned int>(DateTime));
		if (DateTime > 1U)
			fwprintf_s(FileHandle, L"s");
	}

	fwprintf_s(FileHandle, L")");
	return;
}

//Print Date and Time with UNIX time to file
void PrintDateTime(
	FILE * const FileHandle, 
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
