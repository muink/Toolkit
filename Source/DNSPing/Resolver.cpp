// This code is part of Toolkit(DNSPing)
// DNSPing, a useful and powerful toolkit
// Copyright (C) 2014-2018 Chengr28
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


#include "Resolver.h"

//Print response hex format data
void PrintHexResponse(
	FILE * const FileHandle, 
	const uint8_t * const Buffer, 
	const size_t Length)
{
//Initialization and print header.
	size_t Index = 0;
	fwprintf_s(FileHandle, L"------------------------------ Response Hex ------------------------------\n");

//Print hex format data(Part 1).
	for (Index = 0;Index < Length;++Index)
	{
		if (Index == 0)
		{
			fwprintf_s(FileHandle, L"0000  %02x ", Buffer[Index]);
		}
		else if (Index % NUM_HEX + 1U == NUM_HEX)
		{
			fwprintf_s(FileHandle, L"%02x   ", Buffer[Index]);
			for (size_t InnerIndex = Index - (NUM_HEX - 1U);InnerIndex < Index + 1U;++InnerIndex)
			{
				if (InnerIndex != Index - (NUM_HEX - 1U) && InnerIndex % (NUM_HEX / 2U) == 0)
					fwprintf_s(FileHandle, L" ");
				if (Buffer[InnerIndex] >= ASCII_SPACE && Buffer[InnerIndex] <= ASCII_TILDE)
					fwprintf_s(FileHandle, L"%c", Buffer[InnerIndex]);
				else 
					fwprintf_s(FileHandle, L".");
			}
			if (Index + 1U < Length)
			{
				fwprintf_s(FileHandle, L"\n%04x  ", static_cast<unsigned int>(Index + 1U));
			}
		}
		else {
			if (Index % (NUM_HEX / 2U) == 0 && Index % NUM_HEX != 0)
				fwprintf_s(FileHandle, L" ");
			fwprintf_s(FileHandle, L"%02x ", Buffer[Index]);
		}
	}

//Print spaces.
	if (NUM_HEX - Length % NUM_HEX < NUM_HEX)
	{
		for (Index = 0;Index < NUM_HEX - Length % NUM_HEX;++Index)
			fwprintf_s(FileHandle, L"   ");
	}

//Print hex format data(Part 2).
	if (Length % NUM_HEX != 0)
	{
		fwprintf_s(FileHandle, L"   ");
		for (Index = Length - Length % NUM_HEX;Index < Length;++Index)
		{
			if (Buffer[Index] >= ASCII_SPACE && Buffer[Index] <= ASCII_TILDE)
				fwprintf_s(FileHandle, L"%c", Buffer[Index]);
			else 
				fwprintf_s(FileHandle, L".");
		}
	}
	fwprintf_s(FileHandle, L"\n");

//Finished
	fwprintf_s(FileHandle, L"------------------------------ Response Hex ------------------------------\n");
	return;
}

//Print response result or data to file
void PrintResponse(
	FILE * const FileHandle, 
	const uint8_t * const Buffer, 
	const size_t Length)
{
//Initialization and print header.
	size_t Index = 0, CurrentLength = sizeof(dns_hdr);
	const auto DNS_Header = reinterpret_cast<const dns_hdr *>(Buffer);
	fwprintf_s(FileHandle, L"-------------------------------- Response --------------------------------\n");

//Print DNS header.
	fwprintf_s(FileHandle, L"ID: 0x%04x\n", ntohs(DNS_Header->ID));
	fwprintf_s(FileHandle, L"Flags: 0x%04x", ntohs(DNS_Header->Flags));
	PrintFlags(FileHandle, DNS_Header->Flags);
	fwprintf_s(FileHandle, L"Questions RR Count: %u\n", ntohs(DNS_Header->Question));
	fwprintf_s(FileHandle, L"Answer RR Count: %u\n", ntohs(DNS_Header->Answer));
	fwprintf_s(FileHandle, L"Authority RR Count: %u\n", ntohs(DNS_Header->Authority));
	fwprintf_s(FileHandle, L"Additional RR Count: %u\n", ntohs(DNS_Header->Additional));

//Print Questions RRs.
	if (ntohs(DNS_Header->Question) != 0)
	{
		fwprintf_s(FileHandle, L"Questions RR:\n   Name: ");
		dns_qry *DNS_Query = nullptr;
		for (Index = 0;Index < ntohs(DNS_Header->Question);++Index)
		{
		//Print Name.
			PrintDomainName(FileHandle, Buffer, CurrentLength);
			fwprintf_s(FileHandle, L"\n");
			CurrentLength += strnlen_s(reinterpret_cast<const char *>(Buffer) + CurrentLength, Length - CurrentLength) + NULL_TERMINATE_LENGTH;

		//Print Type and Classes.
			DNS_Query = const_cast<dns_qry *>(reinterpret_cast<const dns_qry *>(Buffer + CurrentLength));
			fwprintf_s(FileHandle, L"   Type: 0x%04x", ntohs(DNS_Query->Type));
			PrintTypeClassesName(FileHandle, DNS_Query->Type, 0);
			fwprintf_s(FileHandle, L"   Classes: 0x%04x", ntohs(DNS_Query->Classes));
			PrintTypeClassesName(FileHandle, 0, DNS_Query->Classes);
			CurrentLength += sizeof(dns_qry);
		}
	}

//Print Answer RRs.
	dns_record_standard *DNS_STANDARD_RECORD = nullptr;
	if (ntohs(DNS_Header->Answer) != 0)
	{
		fwprintf_s(FileHandle, L"Answer RR:\n");
		for (Index = 0;Index < ntohs(DNS_Header->Answer);++Index)
		{
		//Print Name.
			fwprintf_s(FileHandle, L" RR(%u)\n   Name: ", static_cast<unsigned int>(Index + 1U));
			CurrentLength += PrintDomainName(FileHandle, Buffer, CurrentLength);
			fwprintf_s(FileHandle, L"\n");

		//Print Type, Classes, TTL and Length.
			DNS_STANDARD_RECORD = const_cast<dns_record_standard *>(reinterpret_cast<const dns_record_standard *>(Buffer + CurrentLength));
			fwprintf_s(FileHandle, L"   Type: 0x%04x", ntohs(DNS_STANDARD_RECORD->Type));
			PrintTypeClassesName(FileHandle, DNS_STANDARD_RECORD->Type, 0);
			fwprintf_s(FileHandle, L"   Classes: 0x%04x", ntohs(DNS_STANDARD_RECORD->Classes));
			PrintTypeClassesName(FileHandle, 0, DNS_STANDARD_RECORD->Classes);
			fwprintf_s(FileHandle, L"   TTL: %u", ntohl(DNS_STANDARD_RECORD->TTL));
			PrintSecondsInDateTime(FileHandle, ntohl(DNS_STANDARD_RECORD->TTL));
			fwprintf_s(FileHandle, L"\n");
			fwprintf_s(FileHandle, L"   Length: %u", ntohs(DNS_STANDARD_RECORD->Length));
			CurrentLength += sizeof(dns_record_standard);
			PrintResourseData(FileHandle, DNS_STANDARD_RECORD->Type, DNS_STANDARD_RECORD->Classes, Buffer, CurrentLength, ntohs(DNS_STANDARD_RECORD->Length));
			CurrentLength += ntohs(DNS_STANDARD_RECORD->Length);
		}
	}

//Print Authority RR.
	if (ntohs(DNS_Header->Authority) != 0)
	{
		fwprintf_s(FileHandle, L"Authority RR:\n");
		for (Index = 0;Index < ntohs(DNS_Header->Authority);++Index)
		{
		//Print Name.
			fwprintf_s(FileHandle, L" RR(%u)\n   Name: ", static_cast<unsigned int>(Index + 1U));
			CurrentLength += PrintDomainName(FileHandle, Buffer, CurrentLength);
			fwprintf_s(FileHandle, L"\n");

		//Print Type, Classes, TTL and Length.
			DNS_STANDARD_RECORD = const_cast<dns_record_standard *>(reinterpret_cast<const dns_record_standard *>(Buffer + CurrentLength));
			fwprintf_s(FileHandle, L"   Type: 0x%04x", ntohs(DNS_STANDARD_RECORD->Type));
			PrintTypeClassesName(FileHandle, DNS_STANDARD_RECORD->Type, 0);
			fwprintf_s(FileHandle, L"   Classes: 0x%04x", ntohs(DNS_STANDARD_RECORD->Classes));
			PrintTypeClassesName(FileHandle, 0, DNS_STANDARD_RECORD->Classes);
			fwprintf_s(FileHandle, L"   TTL: %u", ntohl(DNS_STANDARD_RECORD->TTL));
			PrintSecondsInDateTime(FileHandle, ntohl(DNS_STANDARD_RECORD->TTL));
			fwprintf_s(FileHandle, L"\n");
			fwprintf_s(FileHandle, L"   Length: %u", ntohs(DNS_STANDARD_RECORD->Length));
			CurrentLength += sizeof(dns_record_standard);
			PrintResourseData(FileHandle, DNS_STANDARD_RECORD->Type, DNS_STANDARD_RECORD->Classes, Buffer, CurrentLength, ntohs(DNS_STANDARD_RECORD->Length));
			CurrentLength += ntohs(DNS_STANDARD_RECORD->Length);
		}
	}

//Print Additional RR.
	if (ntohs(DNS_Header->Additional) != 0)
	{
		fwprintf_s(FileHandle, L"Additional RR:\n");
		for (Index = 0;Index < ntohs(DNS_Header->Additional);++Index)
		{
		//Print Name.
			fwprintf_s(FileHandle, L" RR(%u)\n   Name: ", static_cast<unsigned int>(Index + 1U));
			CurrentLength += PrintDomainName(FileHandle, Buffer, CurrentLength);
			fwprintf_s(FileHandle, L"\n");

		//Print Type, Classes, TTL and Length.
			DNS_STANDARD_RECORD = const_cast<dns_record_standard *>(reinterpret_cast<const dns_record_standard *>(Buffer + CurrentLength));
			fwprintf_s(FileHandle, L"   Type: 0x%04x", ntohs(DNS_STANDARD_RECORD->Type));
			PrintTypeClassesName(FileHandle, DNS_STANDARD_RECORD->Type, 0);
			if (ntohs(DNS_STANDARD_RECORD->Type) == DNS_TYPE_OPT) //EDNS Label
			{
				PrintResourseData(FileHandle, DNS_STANDARD_RECORD->Type, DNS_STANDARD_RECORD->Classes, Buffer, CurrentLength - 1U, ntohs(DNS_STANDARD_RECORD->Length));
				CurrentLength += sizeof(dns_record_standard) + ntohs(DNS_STANDARD_RECORD->Length);
			}
			else {
				fwprintf_s(FileHandle, L"   Classes: 0x%04x", ntohs(DNS_STANDARD_RECORD->Classes));
				PrintTypeClassesName(FileHandle, 0, DNS_STANDARD_RECORD->Classes);
				fwprintf_s(FileHandle, L"   TTL: %u", ntohl(DNS_STANDARD_RECORD->TTL));
				PrintSecondsInDateTime(FileHandle, ntohl(DNS_STANDARD_RECORD->TTL));
				fwprintf_s(FileHandle, L"\n");
				fwprintf_s(FileHandle, L"   Length: %u", ntohs(DNS_STANDARD_RECORD->Length));
				CurrentLength += sizeof(dns_record_standard);
				PrintResourseData(FileHandle, DNS_STANDARD_RECORD->Type, DNS_STANDARD_RECORD->Classes, Buffer, CurrentLength, ntohs(DNS_STANDARD_RECORD->Length));
				CurrentLength += ntohs(DNS_STANDARD_RECORD->Length);
			}
		}
	}

//Finished
	fwprintf_s(FileHandle, L"-------------------------------- Response --------------------------------\n");
	return;
}

//Print Header Flags to file
void PrintFlags(
	FILE * const FileHandle, 
	const uint16_t Flags)
{
//Print Flags
	if (Flags != 0)
	{
		auto FlagsBits = ntohs(Flags);

	//Print OPCode
		fwprintf_s(FileHandle, L"(OPCode: ");
		FlagsBits = FlagsBits & HIGHEST_BIT_U16;
		FlagsBits = FlagsBits >> LOWEST_MOVE_BIT_U16;
		if (FlagsBits == DNS_OPCODE_QUERY)
			fwprintf_s(FileHandle, L"Query");
		else if (FlagsBits == DNS_OPCODE_IQUERY)
			fwprintf_s(FileHandle, L"Inverse Query");
		else if (FlagsBits == DNS_OPCODE_SERVER_STATUS)
			fwprintf_s(FileHandle, L"Server Status");
		else if (FlagsBits == DNS_OPCODE_NOTIFY)
			fwprintf_s(FileHandle, L"Notify");
		else if (FlagsBits == DNS_OPCODE_UPDATE)
			fwprintf_s(FileHandle, L"Update");
		else 
			fwprintf_s(FileHandle, L"%x", FlagsBits);

	//Print RCode.
		fwprintf_s(FileHandle, L"/RCode: ");
		FlagsBits = ntohs(Flags);
		FlagsBits = FlagsBits & UINT4_MAX;
		if (FlagsBits == DNS_RCODE_NOERROR)
			fwprintf_s(FileHandle, L"No Error");
		else if (FlagsBits == DNS_RCODE_FORMERR)
			fwprintf_s(FileHandle, L"Format Error");
		else if (FlagsBits == DNS_RCODE_SERVFAIL)
			fwprintf_s(FileHandle, L"Server Failure");
		else if (FlagsBits == DNS_RCODE_NXDOMAIN)
			fwprintf_s(FileHandle, L"Non-Existent Domain");
		else if (FlagsBits == DNS_RCODE_NOTIMPL)
			fwprintf_s(FileHandle, L"Not Implemented");
		else if (FlagsBits == DNS_RCODE_REFUSED)
			fwprintf_s(FileHandle, L"Query Refused");
		else if (FlagsBits == DNS_RCODE_YXDOMAIN)
			fwprintf_s(FileHandle, L"Name Exists when it should not");
		else if (FlagsBits == DNS_RCODE_YXRRSET)
			fwprintf_s(FileHandle, L"RR Set Exists when it should not");
		else if (FlagsBits == DNS_RCODE_NXRRSET)
			fwprintf_s(FileHandle, L"RR Set that should exist does not");
		else if (FlagsBits == DNS_RCODE_NOTAUTH)
			fwprintf_s(FileHandle, L"Server Not Authoritative for zone/Not Authorized");
		else if (FlagsBits == DNS_RCODE_NOTZONE)
			fwprintf_s(FileHandle, L"Name not contained in zone");
		else if (FlagsBits == DNS_RCODE_BADVERS)
			fwprintf_s(FileHandle, L"Bad OPT Version/TSIG Signature Failure");
		else if (FlagsBits == DNS_RCODE_BADKEY)
			fwprintf_s(FileHandle, L"Key not recognized");
		else if (FlagsBits == DNS_RCODE_BADTIME)
			fwprintf_s(FileHandle, L"Signature out of time window");
		else if (FlagsBits == DNS_RCODE_BADMODE)
			fwprintf_s(FileHandle, L"Bad TKEY Mode");
		else if (FlagsBits == DNS_RCODE_BADNAME)
			fwprintf_s(FileHandle, L"Duplicate key name");
		else if (FlagsBits == DNS_RCODE_BADALG)
			fwprintf_s(FileHandle, L"Algorithm not supported");
		else if (FlagsBits == DNS_RCODE_BADTRUNC)
			fwprintf_s(FileHandle, L"Bad Truncation");
		else if (FlagsBits >= DNS_RCODE_PRIVATE_A && FlagsBits <= DNS_RCODE_PRIVATE_B)
			fwprintf_s(FileHandle, L"Reserved Private use");
		else if (FlagsBits == DNS_OPCODE_RESERVED)
			fwprintf_s(FileHandle, L"Reserved");
		else 
			fwprintf_s(FileHandle, L"%x", FlagsBits);
		fwprintf_s(FileHandle, L")");
	}

	fwprintf_s(FileHandle, L"\n");
	return;
}

//Print Type and Classes name to file
void PrintTypeClassesName(
	FILE * const FileHandle, 
	const uint16_t Type, 
	const uint16_t Classes)
{
//Print Classes.
	if (Classes != 0)
	{
	//Cache flush check
		auto HighBitSet = false;
		auto ClassesTemp = ntohs(Classes);
		if ((ClassesTemp >> HIGHEST_MOVE_BIT_U16) != 0)
		{
			HighBitSet = true;
			ClassesTemp = ntohs(Classes);
			ClassesTemp = ClassesTemp & HIGHEST_BIT_U16;
		}

	//Classes check
		if (ClassesTemp == DNS_CLASS_INTERNET)
		{
			fwprintf_s(FileHandle, L"(Class IN");
			if (HighBitSet)
				fwprintf_s(FileHandle, L"/Unicast Queries or Cache Flush");
			fwprintf_s(FileHandle, L")");
		}
		else if (ClassesTemp == DNS_CLASS_CSNET)
		{
			fwprintf_s(FileHandle, L"(Class CSNET");
			if (HighBitSet)
				fwprintf_s(FileHandle, L"/Unicast Queries or Cache Flush");
			fwprintf_s(FileHandle, L")");
		}
		else if (ClassesTemp == DNS_CLASS_CHAOS)
		{
			fwprintf_s(FileHandle, L"(Class CHAOS");
			if (HighBitSet)
				fwprintf_s(FileHandle, L"/Unicast Queries or Cache Flush");
			fwprintf_s(FileHandle, L")");
		}
		else if (ClassesTemp == DNS_CLASS_HESIOD)
		{
			fwprintf_s(FileHandle, L"(Class HESIOD");
			if (HighBitSet)
				fwprintf_s(FileHandle, L"/Unicast Queries or Cache Flush");
			fwprintf_s(FileHandle, L")");
		}
		else if (ClassesTemp == DNS_CLASS_NONE)
		{
			fwprintf_s(FileHandle, L"(Class NONE");
			if (HighBitSet)
				fwprintf_s(FileHandle, L"/Unicast Queries or Cache Flush");
			fwprintf_s(FileHandle, L")");
		}
		else if (ClassesTemp == DNS_CLASS_ALL)
		{
			fwprintf_s(FileHandle, L"(Class ALL");
			if (HighBitSet)
				fwprintf_s(FileHandle, L"/Unicast Queries or Cache Flush");
			fwprintf_s(FileHandle, L")");
		}
		else if (ClassesTemp == DNS_CLASS_ANY)
		{
			fwprintf_s(FileHandle, L"(Class ANY");
			if (HighBitSet)
				fwprintf_s(FileHandle, L"/Unicast Queries or Cache Flush");
			fwprintf_s(FileHandle, L")");
		}
	}
//Print Type.
	else {
		if (ntohs(Type) == DNS_TYPE_A)
			fwprintf_s(FileHandle, L"(A Record)");
		else if (ntohs(Type) == DNS_TYPE_NS)
			fwprintf_s(FileHandle, L"(NS Record)");
		else if (ntohs(Type) == DNS_TYPE_MD)
			fwprintf_s(FileHandle, L"(MD Record)");
		else if (ntohs(Type) == DNS_TYPE_MF)
			fwprintf_s(FileHandle, L"(MF Record)");
		else if (ntohs(Type) == DNS_TYPE_CNAME)
			fwprintf_s(FileHandle, L"(CNAME Record)");
		else if (ntohs(Type) == DNS_TYPE_SOA)
			fwprintf_s(FileHandle, L"(SOA Record)");
		else if (ntohs(Type) == DNS_TYPE_MB)
			fwprintf_s(FileHandle, L"(MB Record)");
		else if (ntohs(Type) == DNS_TYPE_MG)
			fwprintf_s(FileHandle, L"(MG Record)");
		else if (ntohs(Type) == DNS_TYPE_MR)
			fwprintf_s(FileHandle, L"(MR Record)");
		else if (ntohs(Type) == DNS_TYPE_NULL)
			fwprintf_s(FileHandle, L"(NULL Record)");
		else if (ntohs(Type) == DNS_TYPE_WKS)
			fwprintf_s(FileHandle, L"(WKS Record)");
		else if (ntohs(Type) == DNS_TYPE_PTR)
			fwprintf_s(FileHandle, L"(PTR Record)");
		else if (ntohs(Type) == DNS_TYPE_HINFO)
			fwprintf_s(FileHandle, L"(HINFO Record)");
		else if (ntohs(Type) == DNS_TYPE_MINFO)
			fwprintf_s(FileHandle, L"(MINFO Record)");
		else if (ntohs(Type) == DNS_TYPE_MX)
			fwprintf_s(FileHandle, L"(MX Record)");
		else if (ntohs(Type) == DNS_TYPE_TEXT)
			fwprintf_s(FileHandle, L"(TXT Record)");
		else if (ntohs(Type) == DNS_TYPE_RP)
			fwprintf_s(FileHandle, L"(RP Record)");
		else if (ntohs(Type) == DNS_TYPE_RP)
			fwprintf_s(FileHandle, L"(RP Record)");
		else if (ntohs(Type) == DNS_TYPE_AFSDB)
			fwprintf_s(FileHandle, L"(AFSDB Record)");
		else if (ntohs(Type) == DNS_TYPE_X25)
			fwprintf_s(FileHandle, L"(X25 Record)");
		else if (ntohs(Type) == DNS_TYPE_ISDN)
			fwprintf_s(FileHandle, L"(ISDN Record)");
		else if (ntohs(Type) == DNS_TYPE_RT)
			fwprintf_s(FileHandle, L"(RT Record)");
		else if (ntohs(Type) == DNS_TYPE_NSAP)
			fwprintf_s(FileHandle, L"(NSAP Record)");
		else if (ntohs(Type) == DNS_TYPE_NSAPPTR)
			fwprintf_s(FileHandle, L"(NSAP PTR Record)");
		else if (ntohs(Type) == DNS_TYPE_SIG)
			fwprintf_s(FileHandle, L"(SIG Record)");
		else if (ntohs(Type) == DNS_TYPE_KEY)
			fwprintf_s(FileHandle, L"(KEY Record)");
		else if (ntohs(Type) == DNS_TYPE_PX)
			fwprintf_s(FileHandle, L"(PX Record)");
		else if (ntohs(Type) == DNS_TYPE_GPOS)
			fwprintf_s(FileHandle, L"(GPOS Record)");
		else if (ntohs(Type) == DNS_TYPE_AAAA)
			fwprintf_s(FileHandle, L"(AAAA Record)");
		else if (ntohs(Type) == DNS_TYPE_LOC)
			fwprintf_s(FileHandle, L"(LOC Record)");
		else if (ntohs(Type) == DNS_TYPE_NXT)
			fwprintf_s(FileHandle, L"(NXT Record)");
		else if (ntohs(Type) == DNS_TYPE_EID)
			fwprintf_s(FileHandle, L"(EID Record)");
		else if (ntohs(Type) == DNS_TYPE_NIMLOC)
			fwprintf_s(FileHandle, L"(NIMLOC Record)");
		else if (ntohs(Type) == DNS_TYPE_SRV)
			fwprintf_s(FileHandle, L"(SRV Record)");
		else if (ntohs(Type) == DNS_TYPE_ATMA)
			fwprintf_s(FileHandle, L"(ATMA Record)");
		else if (ntohs(Type) == DNS_TYPE_NAPTR)
			fwprintf_s(FileHandle, L"(NAPTR Record)");
		else if (ntohs(Type) == DNS_TYPE_KX)
			fwprintf_s(FileHandle, L"(KX Record)");
		else if (ntohs(Type) == DNS_TYPE_CERT)
			fwprintf_s(FileHandle, L"(CERT Record)");
		else if (ntohs(Type) == DNS_TYPE_DNAME)
			fwprintf_s(FileHandle, L"(DNAME Record)");
		else if (ntohs(Type) == DNS_TYPE_SINK)
			fwprintf_s(FileHandle, L"(SINK Record)");
		else if (ntohs(Type) == DNS_TYPE_OPT)
			fwprintf_s(FileHandle, L"(OPT/EDNS Record)");
		else if (ntohs(Type) == DNS_TYPE_APL)
			fwprintf_s(FileHandle, L"(APL Record)");
		else if (ntohs(Type) == DNS_TYPE_DS)
			fwprintf_s(FileHandle, L"(DS Record)");
		else if (ntohs(Type) == DNS_TYPE_SSHFP)
			fwprintf_s(FileHandle, L"(SSHFP Record)");
		else if (ntohs(Type) == DNS_TYPE_IPSECKEY)
			fwprintf_s(FileHandle, L"(IPSECKEY Record)");
		else if (ntohs(Type) == DNS_TYPE_RRSIG)
			fwprintf_s(FileHandle, L"(RRSIG Record)");
		else if (ntohs(Type) == DNS_TYPE_NSEC)
			fwprintf_s(FileHandle, L"(NSEC Record)");
		else if (ntohs(Type) == DNS_TYPE_DNSKEY)
			fwprintf_s(FileHandle, L"(DNSKEY Record)");
		else if (ntohs(Type) == DNS_TYPE_DHCID)
			fwprintf_s(FileHandle, L"(DHCID Record)");
		else if (ntohs(Type) == DNS_TYPE_NSEC3)
			fwprintf_s(FileHandle, L"(NSEC3 Record)");
		else if (ntohs(Type) == DNS_TYPE_NSEC3PARAM)
			fwprintf_s(FileHandle, L"(NSEC3PARAM Record)");
		else if (ntohs(Type) == DNS_TYPE_TLSA)
			fwprintf_s(FileHandle, L"(TLSA Record)");
		else if (ntohs(Type) == DNS_TYPE_HIP)
			fwprintf_s(FileHandle, L"(HIP Record)");
		else if (ntohs(Type) == DNS_TYPE_NINFO)
			fwprintf_s(FileHandle, L"(NINFO Record)");
		else if (ntohs(Type) == DNS_TYPE_RKEY)
			fwprintf_s(FileHandle, L"(RKEY Record)");
		else if (ntohs(Type) == DNS_TYPE_TALINK)
			fwprintf_s(FileHandle, L"(TALINK Record)");
		else if (ntohs(Type) == DNS_TYPE_CDS)
			fwprintf_s(FileHandle, L"(CDS Record)");
		else if (ntohs(Type) == DNS_TYPE_CDNSKEY)
			fwprintf_s(FileHandle, L"(CDNSKEY Record)");
		else if (ntohs(Type) == DNS_TYPE_OPENPGPKEY)
			fwprintf_s(FileHandle, L"(OPENPGPKEY Record)");
		else if (ntohs(Type) == DNS_TYPE_SPF)
			fwprintf_s(FileHandle, L"(SPF Record)");
		else if (ntohs(Type) == DNS_TYPE_UID)
			fwprintf_s(FileHandle, L"(UID Record)");
		else if (ntohs(Type) == DNS_TYPE_GID)
			fwprintf_s(FileHandle, L"(GID Record)");
		else if (ntohs(Type) == DNS_TYPE_UNSPEC)
			fwprintf_s(FileHandle, L"(UNSPEC Record)");
		else if (ntohs(Type) == DNS_TYPE_NID)
			fwprintf_s(FileHandle, L"(NID Record)");
		else if (ntohs(Type) == DNS_TYPE_L32)
			fwprintf_s(FileHandle, L"(L32 Record)");
		else if (ntohs(Type) == DNS_TYPE_L64)
			fwprintf_s(FileHandle, L"(L64 Record)");
		else if (ntohs(Type) == DNS_TYPE_LP)
			fwprintf_s(FileHandle, L"(LP Record)");
		else if (ntohs(Type) == DNS_TYPE_EUI48)
			fwprintf_s(FileHandle, L"(EUI48 Record)");
		else if (ntohs(Type) == DNS_TYPE_EUI64)
			fwprintf_s(FileHandle, L"(EUI64 Record)");
		else if (ntohs(Type) == DNS_TYPE_ADDRS)
			fwprintf_s(FileHandle, L"(ADDRS Record)");
		else if (ntohs(Type) == DNS_TYPE_TKEY)
			fwprintf_s(FileHandle, L"(TKEY Record)");
		else if (ntohs(Type) == DNS_TYPE_TSIG)
			fwprintf_s(FileHandle, L"(TSIG Record)");
		else if (ntohs(Type) == DNS_TYPE_IXFR)
			fwprintf_s(FileHandle, L"(IXFR Record)");
		else if (ntohs(Type) == DNS_TYPE_AXFR)
			fwprintf_s(FileHandle, L"(AXFR Record)");
		else if (ntohs(Type) == DNS_TYPE_MAILB)
			fwprintf_s(FileHandle, L"(MAILB Record)");
		else if (ntohs(Type) == DNS_TYPE_MAILA)
			fwprintf_s(FileHandle, L"(MAILA Record)");
		else if (ntohs(Type) == DNS_TYPE_ANY)
			fwprintf_s(FileHandle, L"(ANY Record)");
		else if (ntohs(Type) == DNS_TYPE_URI)
			fwprintf_s(FileHandle, L"(URI Record)");
		else if (ntohs(Type) == DNS_TYPE_CAA)
			fwprintf_s(FileHandle, L"(CAA Record)");
		else if (ntohs(Type) == DNS_TYPE_TA)
			fwprintf_s(FileHandle, L"(TA Record)");
		else if (ntohs(Type) == DNS_TYPE_DLV)
			fwprintf_s(FileHandle, L"(DLV Record)");
		else if (ntohs(Type) >= DNS_TYPE_PRIVATE_A && ntohs(Type) <= DNS_TYPE_PRIVATE_B)
			fwprintf_s(FileHandle, L"(Reserved Private use Record)");
		else if (ntohs(Type) == DNS_TYPE_RESERVED)
			fwprintf_s(FileHandle, L"(Reserved Record)");
	}

	fwprintf_s(FileHandle, L"\n");
	return;
}

//Print Domain Name in response to file
size_t PrintDomainName(
	FILE * const FileHandle, 
	const uint8_t * const Buffer, 
	const size_t Location)
{
//Root check
	if (Buffer[Location] == 0)
	{
		fwprintf_s(FileHandle, L"<Root>");
		return sizeof(uint8_t);
	}

//Initialization
	auto BufferTemp = std::make_unique<uint8_t[]>(PACKET_MAXSIZE + MEMORY_RESERVED_BYTES);
	memset(BufferTemp.get(), 0, PACKET_MAXSIZE + MEMORY_RESERVED_BYTES);
	size_t Index = 0, Result = 0;
	uint16_t TruncateLocation = 0;
	auto IsMultiplePointer = false;

//Convert.
	Result = PacketQueryToString(Buffer + Location, BufferTemp.get(), TruncateLocation);
	if (TruncateLocation != 0)
	{
	//Print once when pointer is not at first.
		if (Result > sizeof(uint16_t))
		{
			for (Index = 0;Index < strnlen_s(reinterpret_cast<const char *>(BufferTemp.get()), PACKET_MAXSIZE);++Index)
				fwprintf_s(FileHandle, L"%c", BufferTemp.get()[Index]);
			memset(BufferTemp.get(), 0, PACKET_MAXSIZE);
			fwprintf_s(FileHandle, L".");
		}

	//Get pointer.
		while (TruncateLocation != 0)
		{
			if (IsMultiplePointer)
				fwprintf_s(FileHandle, L".");
			PacketQueryToString(Buffer + TruncateLocation, BufferTemp.get(), TruncateLocation);
			for (Index = 0;Index < strnlen_s(reinterpret_cast<const char *>(BufferTemp.get()), PACKET_MAXSIZE);++Index)
				fwprintf_s(FileHandle, L"%c", BufferTemp.get()[Index]);
			memset(BufferTemp.get(), 0, PACKET_MAXSIZE);
			IsMultiplePointer = true;
		}
	}
	else {
		++Result;
	}

//Print last.
	for (Index = 0;Index < strnlen_s(reinterpret_cast<const char *>(BufferTemp.get()), PACKET_MAXSIZE);++Index)
		fwprintf_s(FileHandle, L"%c", BufferTemp.get()[Index]);
	return Result;
}

//Print Resourse data to file
void PrintResourseData(
	FILE * const FileHandle, 
	const uint16_t Type, 
	const uint16_t Classes, 
	const uint8_t * const Buffer, 
	const size_t Location, 
	const uint16_t Length)
{
//Length and Type check
	if ((Length == 0 && ntohs(Type) != DNS_TYPE_OPT) || Classes == 0)
		return;
	size_t Index = 0, CurrentLength = 0;

//A Record(IPv4 address)
	if (ntohs(Type) == DNS_TYPE_A && Length == sizeof(in_addr))
	{
		fwprintf_s(FileHandle, L"\n   Data: ");
		const auto Addr = reinterpret_cast<const in_addr *>(Buffer + Location);
		fwprintf_s(FileHandle, L"%u.", *reinterpret_cast<const uint8_t *>(&Addr->s_addr));
		fwprintf_s(FileHandle, L"%u.", *(reinterpret_cast<const uint8_t *>(&Addr->s_addr) + sizeof(uint8_t)));
		fwprintf_s(FileHandle, L"%u.", *(reinterpret_cast<const uint8_t *>(&Addr->s_addr) + sizeof(uint8_t) * 2U));
		fwprintf_s(FileHandle, L"%u", *(reinterpret_cast<const uint8_t *>(&Addr->s_addr) + sizeof(uint8_t) * 3U));
	}
//NS Record(Authoritative Name Server) and CNAME Record(Canonical Name)
	else if (ntohs(Type) == DNS_TYPE_NS || ntohs(Type) == DNS_TYPE_CNAME)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");
		PrintDomainName(FileHandle, Buffer, Location);
	}
//SOA Record(Start Of a zone of Authority)
	else if (ntohs(Type) == DNS_TYPE_SOA)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");

		fwprintf_s(FileHandle, L"Primary Name Server: ");
		CurrentLength = PrintDomainName(FileHandle, Buffer, Location);
		fwprintf_s(FileHandle, L"\n         Responsible authority's mailbox: ");
		CurrentLength += PrintDomainName(FileHandle, Buffer, Location + CurrentLength);
		const auto DNS_SOA_RECORD = reinterpret_cast<const dns_record_soa *>(Buffer + Location + CurrentLength);
		fwprintf_s(FileHandle, L"\n         Serial Number: %u", ntohl(DNS_SOA_RECORD->Serial));
		fwprintf_s(FileHandle, L"\n         Refresh Interval: %u", ntohl(DNS_SOA_RECORD->RefreshInterval));
		PrintSecondsInDateTime(FileHandle, ntohl(DNS_SOA_RECORD->RefreshInterval));
		fwprintf_s(FileHandle, L"\n         Retry Interval: %u", ntohl(DNS_SOA_RECORD->RetryInterval));
		PrintSecondsInDateTime(FileHandle, ntohl(DNS_SOA_RECORD->RetryInterval));
		fwprintf_s(FileHandle, L"\n         Expire Limit: %u", ntohl(DNS_SOA_RECORD->ExpireLimit));
		PrintSecondsInDateTime(FileHandle, ntohl(DNS_SOA_RECORD->ExpireLimit));
		fwprintf_s(FileHandle, L"\n         Minimum TTL: %u", ntohl(DNS_SOA_RECORD->MinimumTTL));
		PrintSecondsInDateTime(FileHandle, ntohl(DNS_SOA_RECORD->MinimumTTL));
	}
//PTR Record(domain name PoinTeR)
	else if (ntohs(Type) == DNS_TYPE_PTR)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");
		PrintDomainName(FileHandle, Buffer, Location);
	}
//MX Record(Mail eXchange)
	else if (ntohs(Type) == DNS_TYPE_MX)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");

		const auto DNS_MX_RECORD = reinterpret_cast<const dns_record_mx *>(Buffer + Location);
		fwprintf_s(FileHandle, L"Preference: %u", ntohs(DNS_MX_RECORD->Preference));
		fwprintf_s(FileHandle, L"\n         Mail Exchange: ");
		PrintDomainName(FileHandle, Buffer, Location + sizeof(dns_record_mx));
	}
//TXT Record(Text strings)
	else if (ntohs(Type) == DNS_TYPE_TEXT)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");

		const auto DNS_MX_RECORD = reinterpret_cast<const dns_record_txt *>(Buffer + Location);
		fwprintf_s(FileHandle, L"Length: %u", DNS_MX_RECORD->Length);
		fwprintf_s(FileHandle, L"\n         TXT: \"");
		for (Index = Location + sizeof(dns_record_txt);Index < Location + Length;++Index)
			fwprintf_s(FileHandle, L"%c", Buffer[Index]);
		fwprintf_s(FileHandle, L"\"");
	}
//AAAA Record(IPv6 address)
	else if (ntohs(Type) == DNS_TYPE_AAAA && Length == sizeof(in6_addr))
	{
		fwprintf_s(FileHandle, L"\n   Data: ");

		uint8_t BufferTemp[ADDRESS_STRING_MAXSIZE]{0};
		ssize_t ErrorCode = 0;
		if (!BinaryToAddressString(AF_INET6, Buffer + Location, BufferTemp, ADDRESS_STRING_MAXSIZE, &ErrorCode))
		{
			PrintErrorToScreen(L"\n[Error] IPv4 address format error error", ErrorCode);
			return;
		}

		CaseConvert(true, BufferTemp, strnlen_s(reinterpret_cast<const char *>(BufferTemp), ADDRESS_STRING_MAXSIZE));
		for (Index = 0;Index < strnlen_s(reinterpret_cast<const char *>(BufferTemp), ADDRESS_STRING_MAXSIZE);++Index)
			fwprintf_s(FileHandle, L"%c", BufferTemp[Index]);
	}
//SRV Record(Server Selection)
	else if (ntohs(Type) == DNS_TYPE_SRV)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");

		const auto DNS_SRV_RECORD = reinterpret_cast<const dns_record_srv *>(Buffer + Location);
		fwprintf_s(FileHandle, L"Priority: %x", ntohs(DNS_SRV_RECORD->Priority));
		fwprintf_s(FileHandle, L"\n         Weight: %u", ntohs(DNS_SRV_RECORD->Weight));
		fwprintf_s(FileHandle, L"\n         Port: %u", ntohs(DNS_SRV_RECORD->Port));
		fwprintf_s(FileHandle, L"\n         Target: ");
		PrintDomainName(FileHandle, Buffer, Location + sizeof(dns_record_srv));
	}
//OPT/EDNS Record(Extension Mechanisms for Domain Name System)
	else if (ntohs(Type) == DNS_TYPE_OPT)
	{
		fwprintf_s(FileHandle, L"   Data: ");

		const auto DNS_OPT_RECORD = reinterpret_cast<const dns_record_opt *>(Buffer + Location);
		fwprintf_s(FileHandle, L"UDP Playload Size: %u", ntohs(DNS_OPT_RECORD->UDP_PayloadSize));
		fwprintf_s(FileHandle, L"\n         Extended RCode: %x", DNS_OPT_RECORD->Extended_RCode);
		fwprintf_s(FileHandle, L"\n         EDNS Version: %u", DNS_OPT_RECORD->Version);
		if (ntohs(DNS_OPT_RECORD->Z_Field) >> HIGHEST_MOVE_BIT_U16 == 0)
			fwprintf_s(FileHandle, L"\n         Server cannot handle DNSSEC security RRs.");
		else 
			fwprintf_s(FileHandle, L"\n         Server can handle DNSSEC security RRs.");

	//EDNS Option
		if (Length >= sizeof(dns_edns_option))
		{
			fwprintf_s(FileHandle, L"\n         EDNS Option:\n                         Code: ");

			const auto DNS_OPT_OPTION = reinterpret_cast<const dns_edns_option *>(Buffer + Location + sizeof(dns_record_opt));
			if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_LLQ)
				fwprintf_s(FileHandle, L"LLQ");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_UL)
				fwprintf_s(FileHandle, L"UL");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_NSID)
				fwprintf_s(FileHandle, L"ONSID");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_OWNER)
				fwprintf_s(FileHandle, L"OWNER");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_DAU)
				fwprintf_s(FileHandle, L"DAU");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_DHU)
				fwprintf_s(FileHandle, L"DHU");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_N3U)
				fwprintf_s(FileHandle, L"N3U");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_CSUBNET)
				fwprintf_s(FileHandle, L"CLIENT_SUBNET");
			else if (ntohs(DNS_OPT_OPTION->Code) == EDNS_CODE_EDNS_EXPIRE)
				fwprintf_s(FileHandle, L"EDNS_EXPIRE");
			else 
				fwprintf_s(FileHandle, L"%x", ntohs(DNS_OPT_OPTION->Code));
			fwprintf_s(FileHandle, L"\n                         Length: %x", ntohs(DNS_OPT_OPTION->Length));
		}
	}
//RRSIG Record(Resource Record digital SIGnature)
	else if (ntohs(Type) == DNS_TYPE_RRSIG)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");

		const auto DNS_RRSIG_RECORD = reinterpret_cast<const dns_record_rrsig *>(Buffer + Location);
		fwprintf_s(FileHandle, L"Type Covered: 0x%04x", ntohs(DNS_RRSIG_RECORD->TypeCovered));
		PrintTypeClassesName(FileHandle, DNS_RRSIG_RECORD->TypeCovered, 0);
		fwprintf_s(FileHandle, L"         Algorithm: ");
		if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RESERVED_0)
			fwprintf_s(FileHandle, L"Reserved");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RSA_MD5)
			fwprintf_s(FileHandle, L"RSA/MD5");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_DH)
			fwprintf_s(FileHandle, L"DH");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_DSA)
			fwprintf_s(FileHandle, L"DSA");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RESERVED_4)
			fwprintf_s(FileHandle, L"Reserved");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RSA_SHA1)
			fwprintf_s(FileHandle, L"RSA/SHA-1");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_DSA_NSEC3_SHA1)
			fwprintf_s(FileHandle, L"DSA/NSEC3/SHA-1");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RSA_SHA1_NSEC3_SHA1)
			fwprintf_s(FileHandle, L"RSA/SHA-1/NSEC3/SHA-1");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RSA_SHA256)
			fwprintf_s(FileHandle, L"RSA/SHA-256");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RESERVED_9)
			fwprintf_s(FileHandle, L"Reserved");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RSA_SHA512)
			fwprintf_s(FileHandle, L"RSA/SHA-512");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RESERVED_11)
			fwprintf_s(FileHandle, L"Reserved");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_ECC_GOST)
			fwprintf_s(FileHandle, L"ECC/GOST");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_ECDSA_P256_SHA256)
			fwprintf_s(FileHandle, L"ECDSA P256/SHA-256");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_ECDSA_P386_SHA386)
			fwprintf_s(FileHandle, L"ECDSA P386/SHA-386");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RESERVED_123)
			fwprintf_s(FileHandle, L"Reserved");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RESERVED_251)
			fwprintf_s(FileHandle, L"Reserved");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_INDIRECT)
			fwprintf_s(FileHandle, L"Indirect");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_PRIVATE_DNS)
			fwprintf_s(FileHandle, L"Private DNS");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_PRIVATE_OID)
			fwprintf_s(FileHandle, L"Private OID");
		else if (DNS_RRSIG_RECORD->Algorithm == DNSSEC_AlGORITHM_RESERVED_255)
			fwprintf_s(FileHandle, L"Reserved");
		else 
			fwprintf_s(FileHandle, L"%u", DNS_RRSIG_RECORD->Algorithm);
		fwprintf_s(FileHandle, L"\n         Labels: %u", DNS_RRSIG_RECORD->Labels);
		fwprintf_s(FileHandle, L"\n         Original TTL: %u", ntohl(DNS_RRSIG_RECORD->TTL));
		PrintSecondsInDateTime(FileHandle, ntohl(DNS_RRSIG_RECORD->TTL));
		fwprintf_s(FileHandle, L"\n         Signature Expiration: ");
		PrintDateTime(FileHandle, ntohl(DNS_RRSIG_RECORD->Expiration));
		fwprintf_s(FileHandle, L"\n         Signature Inception: ");
		PrintDateTime(FileHandle, ntohl(DNS_RRSIG_RECORD->Inception));
		fwprintf_s(FileHandle, L"\n         Key Tag: %u", ntohs(DNS_RRSIG_RECORD->KeyTag));
		fwprintf_s(FileHandle, L"\n         Signer's name: ");
		CurrentLength = PrintDomainName(FileHandle, Buffer, Location + sizeof(dns_record_rrsig)) + NULL_TERMINATE_LENGTH;
		CurrentLength += sizeof(dns_record_rrsig);
		fwprintf_s(FileHandle, L"\n         Signature: ");
		for (Index = Location + CurrentLength;Index < Location + Length;++Index)
			fwprintf_s(FileHandle, L"%02x", Buffer[Index]);
	}
//NSEC Record(Next-SECure)
	else if (ntohs(Type) == DNS_TYPE_NSEC)
	{
		fwprintf_s(FileHandle, L"\n   Data: ");

		fwprintf_s(FileHandle, L"Next Domain Name: ");
		CurrentLength = PrintDomainName(FileHandle, Buffer, Location);
		fwprintf_s(FileHandle, L"\n         List of Type Bit Map: ");
		for (Index = Location + CurrentLength;Index < Location + Length;++Index)
			fwprintf_s(FileHandle, L"%x", Buffer[Index]);
	}
//CAA Record(Certification Authority Authorization)
	else if (ntohs(Type) == DNS_TYPE_CAA)
	{
		fwprintf_s(FileHandle, L"   Data: ");

		const auto DNS_CAA_RECORD = reinterpret_cast<const dns_record_caa *>(Buffer + Location);
		fwprintf_s(FileHandle, L"Flags: %x", DNS_CAA_RECORD->Flags);
		fwprintf_s(FileHandle, L"\n         Length: %u", DNS_CAA_RECORD->Length);
		fwprintf_s(FileHandle, L"\n         Tag: \"");
		for (Index = Location + sizeof(dns_record_caa);Index < Location + sizeof(dns_record_caa) + DNS_CAA_RECORD->Length;++Index)
			fwprintf_s(FileHandle, L"%c", Buffer[Index]);
		fwprintf_s(FileHandle, L"\"");
		fwprintf_s(FileHandle, L"\n         Value: \"");
		for (Index = Location + sizeof(dns_record_caa) + DNS_CAA_RECORD->Length;Index < Location + Length;++Index)
			fwprintf_s(FileHandle, L"%c", Buffer[Index]);
		fwprintf_s(FileHandle, L"\"");
	}

	fwprintf_s(FileHandle, L"\n");
	return;
}
