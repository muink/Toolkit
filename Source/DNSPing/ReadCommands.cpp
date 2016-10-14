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


#include "Base.h"

//Global variables
extern ConfigurationTable ConfigurationParameter;

//Read commands
#if defined(PLATFORM_WIN)
bool ReadCommands(
	int argc, 
	wchar_t *argv[])
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
bool ReadCommands(
	int argc, 
	char *argv[])
#endif
{
//Initialization
	std::wstring Parameter;
	ssize_t SignedResult = 0;
	size_t UnsignedResult = 0;

//Read parameter
	for (size_t Index = 1U;Index < (size_t)argc;++Index)
	{
	#if defined(PLATFORM_WIN)
		Parameter = argv[Index];
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
		{
			PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
			return false;
		}
	#endif
		SignedResult = 0;
		UnsignedResult = 0;

	//Description(Usage)
		if (Parameter.find(L"?") != std::string::npos || Parameter == L"-h" || Parameter == L"--help")
		{
			PrintDescription();
		}
	//Pings the specified host until stopped. To see statistics and continue type Control-Break. To stop type Control-C.
		else if (Parameter == L"-t" || Parameter == L"--until-break")
		{
			ConfigurationParameter.Statistics_Send = 0;
		}
	//Resolve addresses to host names.
		else if (Parameter == L"-a" || Parameter == L"--reverse-lookup")
		{
			ConfigurationParameter.IsReverseLookup = true;
		}
	//Set number of echo requests to send.
		else if (Parameter == L"-n" || Parameter == L"--number")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Statistics_Send = UnsignedResult;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-n count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Set the "Do Not Fragment" flag in outgoing packets.
	#if (defined(PLATFORM_WIN) || defined(PLATFORM_LINUX))
		else if (Parameter == L"-f" || Parameter == L"--do-not-fragment")
		{
			ConfigurationParameter.IsDoNotFragment = true;
		}
	#endif
	//Specifie a Time To Live for outgoing packets.
		else if (Parameter == L"-i" || Parameter == L"--hop-limits")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT8_MAX)
				{
				#if defined(PLATFORM_WIN)
					ConfigurationParameter.PacketHopLimits = (DWORD)UnsignedResult;
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
					ConfigurationParameter.PacketHopLimits = (int)UnsignedResult;
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-i hoplimit/ttl) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Set a long wait periods (in milliseconds) for a response.
		else if (Parameter == L"-w" || Parameter == L"--waiting-time")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
			}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult >= TIME_OUT_MIN && UnsignedResult < UINT16_MAX)
				{
				#if defined(PLATFORM_WIN)
					ConfigurationParameter.SocketTimeout = (DWORD)UnsignedResult;
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
					ConfigurationParameter.SocketTimeout.tv_sec = (time_t)(UnsignedResult / SECOND_TO_MILLISECOND);
					ConfigurationParameter.SocketTimeout.tv_usec = (suseconds_t)(UnsignedResult % MICROSECOND_TO_MILLISECOND * MICROSECOND_TO_MILLISECOND);
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-w timeout) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header ID.
		else if (Parameter == L"-id" || Parameter == L"--dns-id")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.ID = htons((uint16_t)UnsignedResult);
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-id dns_id) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Set DNS header flag: QR
		else if (Parameter == L"-qr" || Parameter == L"--flags-qr")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.QR = ~ConfigurationParameter.Parameter_Header.FlagsBits.QR;
		}
	//Specifie DNS header OPCode.
		else if (Parameter == L"-opcode" || Parameter == L"--flags-opcode")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT4_MAX)
				{
				#if BYTE_ORDER == LITTLE_ENDIAN
					auto TempFlags = (uint16_t)UnsignedResult;
					TempFlags = htons(TempFlags << 11U);
					ConfigurationParameter.Parameter_Header.Flags = ConfigurationParameter.Parameter_Header.Flags | TempFlags;
				#else //BIG_ENDIAN
					auto TempFlags = (uint8_t)UnsignedResult;
					TempFlags = TempFlags & 15;//0x00001111
					ConfigurationParameter.Parameter_Header.FlagsBits.OPCode = TempFlags;
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-opcode opcode) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Set DNS header flag: AA
		else if (Parameter == L"-aa" || Parameter == L"--flags-aa")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.AA = ~ConfigurationParameter.Parameter_Header.FlagsBits.AA;
		}
	//Set DNS header flag: TC
		else if (Parameter == L"-tc" || Parameter == L"--flags-tc")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.TC = ~ConfigurationParameter.Parameter_Header.FlagsBits.TC;
		}
	//Set DNS header flag: RD
		else if (Parameter == L"-rd" || Parameter == L"--flags-rd")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.RD = ~ConfigurationParameter.Parameter_Header.FlagsBits.RD;
		}
	//Set DNS header flag: RA
		else if (Parameter == L"-ra" || Parameter == L"--flags-ra")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.RA = ~ConfigurationParameter.Parameter_Header.FlagsBits.RA;
		}
	//Set DNS header flag: AD
		else if (Parameter == L"-ad" || Parameter == L"--flags-ad")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.AD = ~ConfigurationParameter.Parameter_Header.FlagsBits.AD;
		}
	//Set DNS header flag: CD
		else if (Parameter == L"-cd" || Parameter == L"--flags-cd")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.CD = ~ConfigurationParameter.Parameter_Header.FlagsBits.CD;
		}
	//Specifie DNS header RCode.
		else if (Parameter == L"-rcode" || Parameter == L"--flags-rcode")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT4_MAX)
				{
				#if BYTE_ORDER == LITTLE_ENDIAN
					auto TempFlags = (uint16_t)UnsignedResult;
					TempFlags = htons(TempFlags);
					ConfigurationParameter.Parameter_Header.Flags = ConfigurationParameter.Parameter_Header.Flags | TempFlags;
				#else //BIG_ENDIAN
					auto TempFlags = (uint8_t)UnsignedResult;
					TempFlags = TempFlags & 15; //0x00001111
					ConfigurationParameter.Parameter_Header.FlagsBits.RCode = TempFlags;
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-rcode rcode) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header question count.
		else if (Parameter == L"-qn" || Parameter == L"--dns-qn")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Questions = htons((uint16_t)UnsignedResult);
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-qn count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header Answer count.
		else if (Parameter == L"-ann" || Parameter == L"--dns-ann")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Answer = htons((uint16_t)UnsignedResult);
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-ann count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header Authority count.
		else if (Parameter == L"-aun" || Parameter == L"--dns-aun")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Authority = htons((uint16_t)UnsignedResult);
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-aun count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header Additional count.
		else if (Parameter == L"-adn" || Parameter == L"--dns-adn")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Additional = htons((uint16_t)UnsignedResult);
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-adn count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie transmission interval time(in milliseconds).
		else if (Parameter == L"-ti" || Parameter == L"--transmission-interval")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if ((UnsignedResult == 0 && errno == 0) || UnsignedResult > 0)
				{
				#if defined(PLATFORM_WIN)
					ConfigurationParameter.TransmissionInterval = UnsignedResult;
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
					ConfigurationParameter.TransmissionInterval = UnsignedResult * MICROSECOND_TO_MILLISECOND;
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-ti interval_time) error", errno);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Send with EDNS Label.
		else if (Parameter == L"-edns")
		{
			ConfigurationParameter.IsEDNS = true;
		}
	//Specifie EDNS Label UDP Payload length.
		else if (Parameter == L"-payload")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult > OLD_DNS_MAXSIZE && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.EDNSPayloadSize = UnsignedResult;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-payload length) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}

			ConfigurationParameter.IsEDNS = true;
		}
	//Send with DNSSEC request.
		else if (Parameter == L"-dnssec")
		{
			ConfigurationParameter.IsEDNS = true;
			ConfigurationParameter.IsDNSSEC = true;
		}
	//Specifie Query Type.
		else if (Parameter == L"-qt" || Parameter == L"--query-type")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Type name
				UnsignedResult = DNSTypeNameToBinary(Parameter);
				if (UnsignedResult == 0)
				{
					_set_errno(0);

				//Type number
					UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
					{
						ConfigurationParameter.Parameter_Query.Type = htons((uint16_t)UnsignedResult);
					}
					else {
						PrintErrorToScreen(L"\n[Error] Parameter (-qt type) error", 0);
						return false;
					}
				}
				else {
					ConfigurationParameter.Parameter_Query.Type = (uint16_t)UnsignedResult;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie Query Classes.
		else if (Parameter == L"-qc" || Parameter == L"--query-classes")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Classes name
				UnsignedResult = DNSClassesNameToBinary(Parameter);
				if (UnsignedResult == 0)
				{
					_set_errno(0);

				//Classes number
					UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
					{
						ConfigurationParameter.Parameter_Query.Classes = htons((uint16_t)UnsignedResult);
					}
					else {
						PrintErrorToScreen(L"\n[Error] Parameter (-qc classes) error", 0);
						return false;
					}
				}
				else {
					ConfigurationParameter.Parameter_Query.Classes = (uint16_t)UnsignedResult;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie request server name or port.
		else if (Parameter == L"-p" || Parameter == L"--port")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Server name
				UnsignedResult = ServiceNameToBinary(Parameter);
				if (UnsignedResult == 0)
				{
					_set_errno(0);

				//Number port
					UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
					{
						ConfigurationParameter.ServiceType = htons((uint16_t)UnsignedResult);
					}
					else {
						PrintErrorToScreen(L"\n[Error] Parameter (-p service_type/protocol) error", 0);
						return false;
					}
				}
				else {
					ConfigurationParameter.ServiceType = (uint16_t)UnsignedResult;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie Raw data to send.
		else if (Parameter == L"-rawdata")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Initialization
				std::string RawDataString;
				if (!WCSToMBSString(Parameter.c_str(), Parameter.length(), RawDataString))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
				else if (RawDataString.length() < PACKET_MINSIZE && RawDataString.length() > PACKET_MAXSIZE)
				{
					PrintErrorToScreen(L"\n[Error] Parameter (-rawdata raw_data) error", 0);
					return false;
				}
				std::shared_ptr<uint8_t> TempRawData(new uint8_t[PACKET_MAXSIZE]());
				memset(TempRawData.get(), 0, PACKET_MAXSIZE);
				ConfigurationParameter.RawData.swap(TempRawData);
				TempRawData.reset();
				uint8_t TempString[5U]{0};
				TempString[0] = ASCII_ZERO;
				TempString[1U] = ASCII_LOWERCASE_X;

			//Read raw data.
				for (size_t InnerIndex = 0;InnerIndex < RawDataString.length();++InnerIndex)
				{
					TempString[2U] = RawDataString[InnerIndex];
					++InnerIndex;
					TempString[3U] = RawDataString[InnerIndex];

				//Read number.
					_set_errno(0);
					UnsignedResult = strtoul((const char *)TempString, nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT8_MAX)
					{
						ConfigurationParameter.RawData.get()[ConfigurationParameter.RawDataLen] = (uint8_t)UnsignedResult;
						++ConfigurationParameter.RawDataLen;
					}
					else {
						PrintErrorToScreen(L"\n[Error] Parameter (-rawdata raw_data) error", 0);
						return false;
					}
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Send RAW data with Raw Socket.
		else if (Parameter == L"-raw")
		{
			if (Index + 1U < (size_t)argc)
			{
				ConfigurationParameter.IsRawSocket = true;
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Protocol name
				UnsignedResult = ProtocolNameToBinary(Parameter);
				if (UnsignedResult == 0)
				{
					_set_errno(0);

				//Protocol number
					UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
					if (UnsignedResult == IPPROTO_UDP)
					{
						ConfigurationParameter.IsRawSocket = false;
					}
					else if (UnsignedResult > 0 && UnsignedResult <= UINT4_MAX)
					{
						ConfigurationParameter.ServiceType = (uint8_t)UnsignedResult;
					}
					else {
						PrintErrorToScreen(L"\n[Error] Parameter (-raw service_type) error", 0);
						return false;
					}
				}
				else if (UnsignedResult == IPPROTO_UDP)
				{
					ConfigurationParameter.IsRawSocket = false;
				}
				else {
					ConfigurationParameter.ServiceType = (uint8_t)UnsignedResult;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie SOCKS target.
		else if (Parameter == L"-socks")
		{
			if (Index + 1U < (size_t)argc)
			{
			//Initialization
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif
				std::string ParameterString;
				if (!WCSToMBSString(Parameter.c_str(), Parameter.length(), ParameterString))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
				else {
					ConfigurationParameter.TargetString_SOCKS = Parameter;
				}

			//IPv6 address
				if (ParameterString.find(ASCII_COLON) == std::string::npos)
				{
					PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", 0);
					return false;
				}
				else if (ParameterString.find(ASCII_BRACKETS_LEFT) != std::string::npos || ParameterString.find(ASCII_BRACKETS_RIGHT) != std::string::npos)
				{
				//Check parameter.
					if (Parameter.length() < 6U || Parameter.length() > 47U || //IPv6 format
						ParameterString.find("]:") == std::string::npos || ParameterString.find(ASCII_BRACKETS_RIGHT) != ParameterString.find("]:"))
					{
						PrintErrorToScreen(L"\n[Error] Target length error", 0);
						return false;
					}
					else {
						ConfigurationParameter.SockAddr_SOCKS.ss_family = AF_INET6;
					}

				//Mark port.
					if (Parameter.length() > Parameter.find(L"]:"))
					{
						std::wstring Port;
						Port.append(Parameter, Parameter.find(L"]:") + 2U, Parameter.length() - Parameter.find(L"]:"));

					//Server name
						UnsignedResult = ServiceNameToBinary(Port);
						if (UnsignedResult == 0)
						{
							_set_errno(0);

						//Number port
							UnsignedResult = wcstoul(Port.c_str(), nullptr, 0);
							if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
							{
								((sockaddr_in6 *)&ConfigurationParameter.SockAddr_SOCKS)->sin6_port = htons((uint16_t)UnsignedResult);
							}
							else {
								PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", 0);
								return false;
							}
						}
						else {
							((sockaddr_in6 *)&ConfigurationParameter.SockAddr_SOCKS)->sin6_port = htons((uint16_t)UnsignedResult);
						}

						ParameterString.erase(0, 1U);
						ParameterString.erase(ParameterString.find("]:"), ParameterString.length() - ParameterString.find("]:"));
					}
					else {
						PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", 0);
						return false;
					}

				//Mark address.
					if (!AddressStringToBinary(AF_INET6, (const uint8_t *)ParameterString.c_str(), &((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_SOCKS)->sin6_addr, SignedResult))
					{
						PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", SignedResult);
						return false;
					}
				}
				else {
				//Mark port.
					if (ParameterString.find(ASCII_COLON) != std::string::npos && Parameter.length() > Parameter.find(ASCII_COLON))
					{
						std::wstring Port;
						Port.append(Parameter, Parameter.find(ASCII_COLON) + 1U, Parameter.length() - Parameter.find(ASCII_COLON));

					//Server name
						UnsignedResult = ServiceNameToBinary(Port);
						if (UnsignedResult == 0)
						{
							_set_errno(0);

						//Number port
							UnsignedResult = wcstoul(Port.c_str(), nullptr, 0);
							if (UnsignedResult <= 0 || UnsignedResult > UINT16_MAX)
							{
								PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", 0);
								return false;
							}
						}

						ParameterString.erase(ParameterString.find(ASCII_COLON), ParameterString.length() - ParameterString.find(ASCII_COLON));
					}
					else {
						PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", 0);
						return false;
					}

				//Mark target(Domain or IPv4 address)
					for (auto StringIter = ParameterString.begin();StringIter != ParameterString.end();++StringIter)
					{
					//Domain
						if (*StringIter < ASCII_PERIOD || *StringIter == ASCII_SLASH || *StringIter > ASCII_NINE)
						{
							if (Parameter.length() < DOMAIN_MINSIZE + 2U || Parameter.length() > DOMAIN_MAXSIZE + 6U)
							{
								PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", 0);
								return false;
							}
							else {
							//Get address.
								addrinfo AddrInfoHints, *AddrInfo = nullptr;
								memset(&AddrInfoHints, 0, sizeof(AddrInfoHints));
								AddrInfoHints.ai_family = ConfigurationParameter.Protocol;
								if (getaddrinfo(ParameterString.c_str(), nullptr, &AddrInfoHints, &AddrInfo) != 0)
								{
									PrintErrorToScreen(L"\n[Error] Resolve domain name error", WSAGetLastError());
									return false;
								}

							//Get address from result.
								if (AddrInfo != nullptr)
								{
									for (auto PTR = AddrInfo;PTR != nullptr;PTR = PTR->ai_next)
									{
									//IPv6
										if (PTR->ai_family == AF_INET6 && !IN6_IS_ADDR_LINKLOCAL((in6_addr *)(PTR->ai_addr)) && !(((PSOCKADDR_IN6)(PTR->ai_addr))->sin6_scope_id == 0)) //Get port from first(Main) IPv6 device
										{
											ConfigurationParameter.SockAddr_SOCKS.ss_family = AF_INET6;
											((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_SOCKS)->sin6_addr = ((PSOCKADDR_IN6)(PTR->ai_addr))->sin6_addr;
											((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_SOCKS)->sin6_port = htons((uint16_t)UnsignedResult);

										//Convert binary to address string.
											uint8_t Buffer[ADDRESS_STRING_MAXSIZE]{0};
											if (!BinaryToAddressString(AF_INET6, &((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_SOCKS)->sin6_addr, Buffer, ADDRESS_STRING_MAXSIZE, &SignedResult))
											{
												PrintErrorToScreen(L"\n[Error] IPv6 address format error error", SignedResult);
												return false;
											}

											break;
										}
									//IPv4
										else if (PTR->ai_family == AF_INET && ((PSOCKADDR_IN)(PTR->ai_addr))->sin_addr.s_addr != INADDR_LOOPBACK && ((PSOCKADDR_IN)(PTR->ai_addr))->sin_addr.s_addr != INADDR_BROADCAST)
										{
											ConfigurationParameter.SockAddr_SOCKS.ss_family = AF_INET;
											((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_SOCKS)->sin_addr = ((PSOCKADDR_IN)(PTR->ai_addr))->sin_addr;
											((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_SOCKS)->sin_port = htons((uint16_t)UnsignedResult);

										//Convert binary to address string.
											uint8_t Buffer[ADDRESS_STRING_MAXSIZE]{0};
											if (!BinaryToAddressString(AF_INET, &((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_SOCKS)->sin_addr, Buffer, ADDRESS_STRING_MAXSIZE, &SignedResult))
											{
												PrintErrorToScreen(L"\n[Error] IPv4 address format error error", SignedResult);
												return false;
											}

											break;
										}
									}

									freeaddrinfo(AddrInfo);
								}
								else {
									PrintErrorToScreen(L"\n[Error] Resolve domain name error", WSAGetLastError());
									return false;
								}
							}

							break;
						}

					//IPv4
						if (StringIter == ParameterString.end() - 1U)
						{
						//Check parameter.
							if (Parameter.length() < 9U || Parameter.length() > 21U) //IPv4 format
							{
								PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", 0);
								return false;
							}
							else {
								((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_SOCKS)->sin_port = htons((uint16_t)UnsignedResult);
							}

						//Mark address.
							ConfigurationParameter.SockAddr_SOCKS.ss_family = AF_INET;
							if (!AddressStringToBinary(AF_INET, (const uint8_t *)ParameterString.c_str(), &((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_SOCKS)->sin_addr, SignedResult))
							{
								PrintErrorToScreen(L"\n[Error] Parameter (-socks target) error", SignedResult);
								return false;
							}
						}
					}
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie SOCKS username.
		else if (Parameter == L"-socks_username")
		{
			if (Index + 1U < (size_t)argc)
			{
			//Initialization
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif
				if (!WCSToMBSString(Parameter.c_str(), Parameter.length(), ConfigurationParameter.SOCKS_Username))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}

			//SOCKS username check
				if (ConfigurationParameter.SOCKS_Username.length() > SOCKS_USERNAME_PASSWORD_MAXNUM)
				{
					PrintErrorToScreen(L"\n[Error] Parameter (-socks_username) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie SOCKS password.
		else if (Parameter == L"-socks_password")
		{
			if (Index + 1U < (size_t)argc)
			{
			//Initialization
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif
				if (!WCSToMBSString(Parameter.c_str(), Parameter.length(), ConfigurationParameter.SOCKS_Password))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}

			//SOCKS password check
				if (ConfigurationParameter.SOCKS_Password.length() > SOCKS_USERNAME_PASSWORD_MAXNUM)
				{
					PrintErrorToScreen(L"\n[Error] Parameter (-socks_password) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie buffer size.
		else if (Parameter == L"-buf" || Parameter == L"--buffer-size")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				_set_errno(0);
				UnsignedResult = wcstoul(Parameter.c_str(), nullptr, 0);
				if (UnsignedResult >= OLD_DNS_MAXSIZE && UnsignedResult <= LARGE_PACKET_MAXSIZE)
				{
					ConfigurationParameter.BufferSize = UnsignedResult;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-buf size) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Disable packets validated.
		else if (Parameter == L"-dv" || Parameter == L"--disable-validate")
		{
			ConfigurationParameter.IsValidate = false;
		}
	//Show response.
		else if (Parameter == L"-show" || Parameter == L"--show-response")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				if (Parameter == L"result")
				{
					ConfigurationParameter.IsShowResponse = true;
				}
				else if (Parameter == L"hex" || Parameter == L"hexadecimal")
				{
					ConfigurationParameter.IsShowHexResponse = true;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-show type) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Output result to file.
		else if (Parameter == L"-of" || Parameter == L"--output-file")
		{
			if (Index + 1U < (size_t)argc)
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Parameter = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				if (!MBSToWCSString((const uint8_t *)argv[Index], strnlen(argv[Index], FILE_BUFFER_SIZE), Parameter))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

				if (Parameter.length() <= MAX_PATH)
				{
					ConfigurationParameter.wOutputFileName = Parameter;

				#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
					std::shared_ptr<uint8_t> OutputFileNamePTR(new uint8_t[Parameter.length() + 1U]);
					memset(OutputFileNamePTR.get(), 0, Parameter.length() + 1U);
					if (wcstombs((char *)OutputFileNamePTR.get(), Parameter.c_str(), Parameter.length()) == (size_t)RETURN_ERROR)
					{
						PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
						return false;
					}
					else {
						ConfigurationParameter.OutputFileName = (const char *)OutputFileNamePTR.get();
					}
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Parameter (-of file_name) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Using IPv6.
		else if (Parameter == L"-6" || Parameter == L"--ipv6")
		{
			ConfigurationParameter.Protocol = AF_INET6;
		}
	//Using IPv4.
		else if (Parameter == L"-4" || Parameter == L"--ipv4")
		{
			ConfigurationParameter.Protocol = AF_INET;
		}
	//Specifie Query Domain.
		else if (!ConfigurationParameter.RawData && ConfigurationParameter.TestDomain.empty() && Index == (size_t)(argc - 2))
		{
		//Check parameter.
			if (Parameter.length() <= DOMAIN_MINSIZE || Parameter.length() > DOMAIN_MAXSIZE)
			{
				PrintErrorToScreen(L"\n[Error] Test domain length error", 0);
				return false;
			}

		//Initialization
			if (!WCSToMBSString(Parameter.c_str(), Parameter.length(), ConfigurationParameter.TestDomain))
			{
				PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
				return false;
			}
		}
	//Specifie target.
		else if (Index == (size_t)(argc - 1))
		{
		//Check parameter.
			if (Parameter.length() < DOMAIN_MINSIZE || Parameter.length() > DOMAIN_MAXSIZE)
			{
				PrintErrorToScreen(L"\n[Error] Target length error", 0);
				return false;
			}

		//Initialization
			std::string ParameterString;
			if (!WCSToMBSString(Parameter.c_str(), Parameter.length(), ParameterString))
			{
				PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
				return false;
			}

		//IPv6
			if (ParameterString.find(ASCII_COLON) != std::string::npos)
			{
			//Check parameter.
				if (Parameter.length() < 2U || Parameter.length() > 40U) //IPv6 format
				{
					PrintErrorToScreen(L"\n[Error] Target length error", 0);
					return false;
				}
				else if (ConfigurationParameter.Protocol == AF_INET)
				{
					PrintErrorToScreen(L"\n[Error] Target protocol error", 0);
					return false;
				}

			//Mark address.
				ConfigurationParameter.Protocol = AF_INET6;
				ConfigurationParameter.SockAddr_Normal.ss_family = AF_INET6;
				if (!AddressStringToBinary(AF_INET6, (const uint8_t *)ParameterString.c_str(), &((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_addr, SignedResult))
				{
					PrintErrorToScreen(L"\n[Error] Target format error", SignedResult);
					return false;
				}

				ConfigurationParameter.TargetString_Normal.append("[");
				ConfigurationParameter.TargetString_Normal.append(ParameterString);
				ConfigurationParameter.TargetString_Normal.append("]");
			}
			else {
				for (auto StringIter = ParameterString.begin();StringIter != ParameterString.end();++StringIter)
				{
				//Domain
					if (*StringIter < ASCII_PERIOD || *StringIter == ASCII_SLASH || *StringIter > ASCII_NINE)
					{
					//Check parameter.
						if (Parameter.length() <= DOMAIN_MINSIZE)
						{
							PrintErrorToScreen(L"\n[Error] Target length error", 0);
							return false;
						}

					//SOCKS mode
						if (ConfigurationParameter.SockAddr_SOCKS.ss_family > 0)
						{
							ConfigurationParameter.TargetString_Normal.append(ParameterString);
						}
					//Normal mode
						else {
						//Get address.
							addrinfo AddrInfoHints, *AddrInfo = nullptr;
							memset(&AddrInfoHints, 0, sizeof(AddrInfoHints));
							AddrInfoHints.ai_family = ConfigurationParameter.Protocol;
							if (getaddrinfo(ParameterString.c_str(), nullptr, &AddrInfoHints, &AddrInfo) != 0)
							{
								PrintErrorToScreen(L"\n[Error] Resolve domain name error", WSAGetLastError());
								return false;
							}

						//Get address from result.
							if (AddrInfo != nullptr)
							{
								for (auto PTR = AddrInfo;PTR != nullptr;PTR = PTR->ai_next)
								{
								//IPv6
									if (PTR->ai_family == AF_INET6 && !IN6_IS_ADDR_LINKLOCAL((in6_addr *)(PTR->ai_addr)) && !(((PSOCKADDR_IN6)(PTR->ai_addr))->sin6_scope_id == 0)) //Get port from first(Main) IPv6 device
									{
										ConfigurationParameter.Protocol = AF_INET6;
										ConfigurationParameter.SockAddr_Normal.ss_family = AF_INET6;
										((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_addr = ((PSOCKADDR_IN6)(PTR->ai_addr))->sin6_addr;

									//Convert binary to address string.
										ConfigurationParameter.TargetAddressString = ParameterString;
										uint8_t Buffer[ADDRESS_STRING_MAXSIZE]{0};
										if (!BinaryToAddressString(AF_INET6, &((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_addr, Buffer, ADDRESS_STRING_MAXSIZE, &SignedResult))
										{
											PrintErrorToScreen(L"\n[Error] IPv6 address format error error", SignedResult);
											return false;
										}

										CaseConvert(true, Buffer, strnlen_s((const char *)Buffer, ADDRESS_STRING_MAXSIZE));
										ConfigurationParameter.TargetString_Normal.append("[");
										ConfigurationParameter.TargetString_Normal.append((const char *)Buffer);
										ConfigurationParameter.TargetString_Normal.append("]");
										break;
									}
								//IPv4
									else if (PTR->ai_family == AF_INET && ((PSOCKADDR_IN)(PTR->ai_addr))->sin_addr.s_addr != INADDR_LOOPBACK && ((PSOCKADDR_IN)(PTR->ai_addr))->sin_addr.s_addr != INADDR_BROADCAST)
									{
										ConfigurationParameter.Protocol = AF_INET;
										ConfigurationParameter.SockAddr_Normal.ss_family = AF_INET;
										((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_addr = ((PSOCKADDR_IN)(PTR->ai_addr))->sin_addr;

									//Convert binary to address string.
										ConfigurationParameter.TargetAddressString = ParameterString;
										uint8_t Buffer[ADDRESS_STRING_MAXSIZE]{0};
										if (!BinaryToAddressString(AF_INET, &((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_addr, Buffer, ADDRESS_STRING_MAXSIZE, &SignedResult))
										{
											PrintErrorToScreen(L"\n[Error] IPv4 address format error error", SignedResult);
											return false;
										}

										ConfigurationParameter.TargetString_Normal = (const char *)Buffer;
										break;
									}
								}

								freeaddrinfo(AddrInfo);
							}
							else {
								PrintErrorToScreen(L"\n[Error] Resolve domain name error", WSAGetLastError());
								return false;
							}
						}

						break;
					}

				//IPv4
					if (StringIter == ParameterString.end() - 1U)
					{
					//Check parameter.
						if (Parameter.length() < 7U || Parameter.length() > 15U) //IPv4 format
						{
							PrintErrorToScreen(L"\n[Error] Target length error", 0);
							return false;
						}
						else if (ConfigurationParameter.Protocol == AF_INET6)
						{
							PrintErrorToScreen(L"\n[Error] Target protocol error", 0);
							return false;
						}

					//Mark address.
						ConfigurationParameter.Protocol = AF_INET;
						ConfigurationParameter.SockAddr_Normal.ss_family = AF_INET;
						if (!AddressStringToBinary(AF_INET, (const uint8_t *)ParameterString.c_str(), &((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_addr, SignedResult))
						{
							PrintErrorToScreen(L"\n[Error] Target format error", SignedResult);
							return false;
						}

						ConfigurationParameter.TargetString_Normal = ParameterString;
					}
				}
			}
		}
	}

	return true;
}
