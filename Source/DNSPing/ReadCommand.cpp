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


#include "Base.h"

//Global variables
extern ConfigurationTable ConfigurationParameter;

//Read commands
#if defined(PLATFORM_WIN)
bool ReadCommand(
	int argc, 
	wchar_t *argv[])
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
bool ReadCommand(
	int argc, 
	char *argv[])
#endif
{
//Initialization
	std::wstring Command;
	ssize_t SignedResult = 0;
	size_t UnsignedResult = 0;

//Read parameter
	for (size_t Index = 1U;Index < static_cast<size_t>(argc);++Index)
	{
	#if defined(PLATFORM_WIN)
		Command = argv[Index];
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
		{
			PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
			return false;
		}
	#endif
		SignedResult = 0;
		UnsignedResult = 0;

	//Description(Usage)
		if (Command.find(L"?") != std::string::npos || Command == L"-h" || Command == L"--help")
		{
			PrintDescription();
		}
	//Pings the specified host until stopped. To see statistics and continue type Control-Break. To stop type Control-C.
		else if (Command == L"-t" || Command == L"--until-break")
		{
			ConfigurationParameter.Statistics_PlanSend = 0;
		}
	//Resolve addresses to host names.
		else if (Command == L"-a" || Command == L"--reverse-lookup")
		{
			ConfigurationParameter.IsReverseLookup = true;
		}
	//Set number of echo requests to send.
		else if (Command == L"-n" || Command == L"--number")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Statistics_PlanSend = UnsignedResult;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-n count) error", 0);
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
		else if (Command == L"-f" || Command == L"--do-not-fragment")
		{
			ConfigurationParameter.IsDoNotFragment = true;
		}
	#endif
	//Specifie a Time To Live for outgoing packets.
		else if (Command == L"-i" || Command == L"--hop-limits")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT8_MAX)
				{
				#if defined(PLATFORM_WIN)
					ConfigurationParameter.PacketHopLimits = static_cast<DWORD>(UnsignedResult);
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
					ConfigurationParameter.PacketHopLimits = static_cast<int>(UnsignedResult);
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-i hoplimit/ttl) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Set a long wait periods (in milliseconds) for a response.
		else if (Command == L"-w" || Command == L"--waiting-time")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult >= SOCKET_MIN_TIMEOUT && UnsignedResult < UINT16_MAX)
				{
				#if defined(PLATFORM_WIN)
					ConfigurationParameter.SocketTimeout = static_cast<DWORD>(UnsignedResult);
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
					ConfigurationParameter.SocketTimeout.tv_sec = static_cast<time_t>(UnsignedResult / SECOND_TO_MILLISECOND);
					ConfigurationParameter.SocketTimeout.tv_usec = static_cast<suseconds_t>(UnsignedResult % MICROSECOND_TO_MILLISECOND * MICROSECOND_TO_MILLISECOND);
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-w timeout) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header ID.
		else if (Command == L"-id" || Command == L"--dns-id")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.ID = htons(static_cast<uint16_t>(UnsignedResult));
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-id dns_id) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Set DNS header flag: QR
		else if (Command == L"-qr" || Command == L"--flags-qr")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.QR = ~ConfigurationParameter.Parameter_Header.FlagsBits.QR;
		}
	//Specifie DNS header OPCode.
		else if (Command == L"-opcode" || Command == L"--flags-opcode")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT4_MAX)
				{
				#if BYTE_ORDER == LITTLE_ENDIAN
					auto FlagsTemp = static_cast<uint16_t>(UnsignedResult);
					FlagsTemp = htons(FlagsTemp << 11U);
					ConfigurationParameter.Parameter_Header.Flags = ConfigurationParameter.Parameter_Header.Flags | FlagsTemp;
				#else //BIG_ENDIAN
					auto FlagsTemp = static_cast<uint8_t>(UnsignedResult);
					FlagsTemp = FlagsTemp & 15; //0x00001111
					ConfigurationParameter.Parameter_Header.FlagsBits.OPCode = FlagsTemp;
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-opcode opcode) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Set DNS header flag: AA
		else if (Command == L"-aa" || Command == L"--flags-aa")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.AA = ~ConfigurationParameter.Parameter_Header.FlagsBits.AA;
		}
	//Set DNS header flag: TC
		else if (Command == L"-tc" || Command == L"--flags-tc")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.TC = ~ConfigurationParameter.Parameter_Header.FlagsBits.TC;
		}
	//Set DNS header flag: RD
		else if (Command == L"-rd" || Command == L"--flags-rd")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.RD = ~ConfigurationParameter.Parameter_Header.FlagsBits.RD;
		}
	//Set DNS header flag: RA
		else if (Command == L"-ra" || Command == L"--flags-ra")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.RA = ~ConfigurationParameter.Parameter_Header.FlagsBits.RA;
		}
	//Set DNS header flag: AD
		else if (Command == L"-ad" || Command == L"--flags-ad")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.AD = ~ConfigurationParameter.Parameter_Header.FlagsBits.AD;
		}
	//Set DNS header flag: CD
		else if (Command == L"-cd" || Command == L"--flags-cd")
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.CD = ~ConfigurationParameter.Parameter_Header.FlagsBits.CD;
		}
	//Specifie DNS header RCode.
		else if (Command == L"-rcode" || Command == L"--flags-rcode")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT4_MAX)
				{
				#if BYTE_ORDER == LITTLE_ENDIAN
					auto FlagsTemp = static_cast<uint16_t>(UnsignedResult);
					FlagsTemp = htons(FlagsTemp);
					ConfigurationParameter.Parameter_Header.Flags = ConfigurationParameter.Parameter_Header.Flags | FlagsTemp;
				#else //BIG_ENDIAN
					auto FlagsTemp = static_cast<uint8_t>(UnsignedResult);
					FlagsTemp = FlagsTemp & 15; //0x00001111
					ConfigurationParameter.Parameter_Header.FlagsBits.RCode = FlagsTemp;
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-rcode rcode) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header question count.
		else if (Command == L"-qn" || Command == L"--dns-qn")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Question = htons(static_cast<uint16_t>(UnsignedResult));
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-qn count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header Answer count.
		else if (Command == L"-ann" || Command == L"--dns-ann")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Answer = htons(static_cast<uint16_t>(UnsignedResult));
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-ann count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header Authority count.
		else if (Command == L"-aun" || Command == L"--dns-aun")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Authority = htons(static_cast<uint16_t>(UnsignedResult));
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-aun count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie DNS header Additional count.
		else if (Command == L"-adn" || Command == L"--dns-adn")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.Parameter_Header.Additional = htons(static_cast<uint16_t>(UnsignedResult));
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-adn count) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie transmission interval time(in milliseconds).
		else if (Command == L"-ti" || Command == L"--transmission-interval")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if ((UnsignedResult == 0 && errno == 0) || UnsignedResult > 0)
				{
				#if defined(PLATFORM_WIN)
					ConfigurationParameter.TransmissionInterval = UnsignedResult;
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
					ConfigurationParameter.TransmissionInterval = UnsignedResult * MICROSECOND_TO_MILLISECOND;
				#endif
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-ti interval_time) error", errno);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Send with EDNS Label.
		else if (Command == L"-edns")
		{
			ConfigurationParameter.IsEDNS = true;
		}
	//Specifie EDNS Label UDP Payload length.
		else if (Command == L"-payload")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult > DNS_PACKET_MAXSIZE_TRADITIONAL && UnsignedResult <= UINT16_MAX)
				{
					ConfigurationParameter.EDNSPayloadSize = UnsignedResult;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-payload length) error", 0);
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
		else if (Command == L"-dnssec")
		{
			ConfigurationParameter.IsEDNS = true;
			ConfigurationParameter.IsDNSSEC = true;
		}
	//Specifie Query Type.
		else if (Command == L"-qt" || Command == L"--query-type")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Type name
				UnsignedResult = DNSTypeNameToBinary(Command);
				if (UnsignedResult == 0)
				{
				//Get number.
					_set_errno(0);
					UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
					{
						ConfigurationParameter.Parameter_Query.Type = htons(static_cast<uint16_t>(UnsignedResult));
					}
					else {
						PrintErrorToScreen(L"\n[Error] Command (-qt type) error", 0);
						return false;
					}
				}
				else {
					ConfigurationParameter.Parameter_Query.Type = static_cast<uint16_t>(UnsignedResult);
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie Query Classes.
		else if (Command == L"-qc" || Command == L"--query-classes")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Classes name
				UnsignedResult = DNSClassesNameToBinary(Command);
				if (UnsignedResult == 0)
				{
				//Get number.
					_set_errno(0);
					UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
					{
						ConfigurationParameter.Parameter_Query.Classes = htons(static_cast<uint16_t>(UnsignedResult));
					}
					else {
						PrintErrorToScreen(L"\n[Error] Command (-qc classes) error", 0);
						return false;
					}
				}
				else {
					ConfigurationParameter.Parameter_Query.Classes = static_cast<uint16_t>(UnsignedResult);
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie request server name or port.
		else if (Command == L"-p" || Command == L"--port")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Server name
				UnsignedResult = ServiceNameToBinary(Command);
				if (UnsignedResult == 0)
				{
				//Get number.
					_set_errno(0);
					UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT16_MAX)
					{
						ConfigurationParameter.ServiceType = htons(static_cast<uint16_t>(UnsignedResult));
					}
					else {
						PrintErrorToScreen(L"\n[Error] Command (-p service_type/protocol) error", 0);
						return false;
					}
				}
				else {
					ConfigurationParameter.ServiceType = static_cast<uint16_t>(UnsignedResult);
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie Raw data to send.
		else if (Command == L"-rawdata")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Initialization
				std::string RawDataString;
				if (!WCS_To_MBS_String(Command.c_str(), Command.length(), RawDataString))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
				else if (RawDataString.length() < PACKET_MINSIZE && RawDataString.length() > PACKET_MAXSIZE)
				{
					PrintErrorToScreen(L"\n[Error] Command (-rawdata raw_data) error", 0);
					return false;
				}
				std::unique_ptr<uint8_t[]> RawDataTemp(new uint8_t[PACKET_MAXSIZE + PADDING_RESERVED_BYTES]());
				memset(RawDataTemp.get(), 0, PACKET_MAXSIZE + PADDING_RESERVED_BYTES);
				std::swap(ConfigurationParameter.RawDataBuffer, RawDataTemp);
				RawDataTemp.reset();
				uint8_t BufferStringTemp[5U]{0};
				BufferStringTemp[0] = ASCII_ZERO;
				BufferStringTemp[1U] = ASCII_LOWERCASE_X;

			//Read raw data.
				for (size_t InnerIndex = 0;InnerIndex < RawDataString.length();++InnerIndex)
				{
					BufferStringTemp[2U] = RawDataString[InnerIndex];
					++InnerIndex;
					BufferStringTemp[3U] = RawDataString[InnerIndex];

				//Get number.
					_set_errno(0);
					UnsignedResult = strtoul(reinterpret_cast<const char *>(BufferStringTemp), nullptr, 0);
					if (UnsignedResult > 0 && UnsignedResult <= UINT8_MAX)
					{
						ConfigurationParameter.RawDataBuffer.get()[ConfigurationParameter.RawDataLen] = static_cast<uint8_t>(UnsignedResult);
						++ConfigurationParameter.RawDataLen;
					}
					else {
						PrintErrorToScreen(L"\n[Error] Command (-rawdata raw_data) error", 0);
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
		else if (Command == L"-raw")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				ConfigurationParameter.IsRawSocket = true;
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Protocol name
				UnsignedResult = ProtocolNameToBinary(Command);
				if (UnsignedResult == 0)
				{
					_set_errno(0);

				//Get number.
					UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
					if (UnsignedResult == IPPROTO_UDP)
					{
						ConfigurationParameter.IsRawSocket = false;
					}
					else if (UnsignedResult > 0 && UnsignedResult <= UINT4_MAX)
					{
						ConfigurationParameter.ServiceType = static_cast<uint8_t>(UnsignedResult);
					}
					else {
						PrintErrorToScreen(L"\n[Error] Command (-raw service_type) error", 0);
						return false;
					}
				}
				else if (UnsignedResult == IPPROTO_UDP)
				{
					ConfigurationParameter.IsRawSocket = false;
				}
				else {
					ConfigurationParameter.ServiceType = static_cast<uint8_t>(UnsignedResult);
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Specifie buffer size.
		else if (Command == L"-buf" || Command == L"--buffer-size")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get number.
				_set_errno(0);
				UnsignedResult = wcstoul(Command.c_str(), nullptr, 0);
				if (UnsignedResult >= DNS_PACKET_MAXSIZE_TRADITIONAL && UnsignedResult <= DEFAULT_LARGE_BUFFER_SIZE)
				{
					ConfigurationParameter.BufferSize = UnsignedResult;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-buf size) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Disable packets validated.
		else if (Command == L"-dv" || Command == L"--disable-validate")
		{
			ConfigurationParameter.IsValidated = false;
		}
	//Show response.
		else if (Command == L"-show" || Command == L"--show-response")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Get showing type.
				CaseConvert(Command, true);
				if (Command == L"RESULT")
				{
					ConfigurationParameter.IsShowResponse = true;
				}
				else if (Command == L"HEX" || Command == L"HEXADECIMAL")
				{
					ConfigurationParameter.IsShowHexResponse = true;
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-show type) error", 0);
					return false;
				}
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Output result to file.
		else if (Command == L"-of" || Command == L"--output-file")
		{
			if (Index + 1U < static_cast<size_t>(argc))
			{
				++Index;
			#if defined(PLATFORM_WIN)
				Command = argv[Index];
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(argv[Index]), strnlen(argv[Index], FILE_BUFFER_SIZE), Command))
				{
					PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
					return false;
				}
			#endif

			//Mark file name.
				ConfigurationParameter.WideOutputFileName = Command;
			#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				if (Command.length() < PATH_MAX)
				{
					std::unique_ptr<uint8_t[]> OutputFileNameBuffer(new uint8_t[Command.length() + PADDING_RESERVED_BYTES]);
					memset(OutputFileNameBuffer.get(), 0, Command.length() + PADDING_RESERVED_BYTES);
					if (wcstombs(reinterpret_cast<char *>(OutputFileNameBuffer.get()), Command.c_str(), Command.length()) == static_cast<size_t>(RETURN_ERROR))
					{
						PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", 0);
						return false;
					}
					else {
						ConfigurationParameter.OutputFileName = reinterpret_cast<const char *>(OutputFileNameBuffer.get());
					}
				}
				else {
					PrintErrorToScreen(L"\n[Error] Command (-of file_name) error", 0);
					return false;
				}
			#endif
			}
			else {
				PrintErrorToScreen(L"\n[Error] Not enough parameters error", 0);
				return false;
			}
		}
	//Using IPv6.
		else if (Command == L"-6" || Command == L"--ipv6")
		{
			ConfigurationParameter.Protocol = AF_INET6;
		}
	//Using IPv4.
		else if (Command == L"-4" || Command == L"--ipv4")
		{
			ConfigurationParameter.Protocol = AF_INET;
		}
	//Specifie Query Domain.
		else if (!ConfigurationParameter.RawDataBuffer && ConfigurationParameter.TestDomainString.empty() && Index == static_cast<size_t>(argc - 2))
		{
		//Check parameter.
			if (Command.length() <= DOMAIN_MINSIZE || Command.length() > DOMAIN_MAXSIZE)
			{
				PrintErrorToScreen(L"\n[Error] Test domain length error", 0);
				return false;
			}

		//Initialization
			if (!WCS_To_MBS_String(Command.c_str(), Command.length(), ConfigurationParameter.TestDomainString))
			{
				PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
				return false;
			}
		}
	//Specifie target.
		else if (Index == static_cast<size_t>(argc - 1))
		{
		//Check parameter.
			if (Command.length() < DOMAIN_MINSIZE || Command.length() > DOMAIN_MAXSIZE)
			{
				PrintErrorToScreen(L"\n[Error] Target length error", 0);
				return false;
			}

		//Initialization
			std::string CommandString;
			if (!WCS_To_MBS_String(Command.c_str(), Command.length(), CommandString))
			{
				PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
				return false;
			}

		//IPv6
			if (CommandString.find(ASCII_COLON) != std::string::npos)
			{
			//Check parameter.
				if (Command.length() < IPV6_SHORTEST_ADDR_STRING || Command.length() > IPV6_LONGEST_ADDR_STRING) //IPv6 format
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
				if (!AddressStringToBinary(AF_INET6, reinterpret_cast<const uint8_t *>(CommandString.c_str()), &reinterpret_cast<sockaddr_in6 *>(&ConfigurationParameter.SockAddr_Normal)->sin6_addr, &SignedResult))
				{
					PrintErrorToScreen(L"\n[Error] Target format error", SignedResult);
					return false;
				}

				ConfigurationParameter.TargetString_Normal.append("[");
				ConfigurationParameter.TargetString_Normal.append(CommandString);
				ConfigurationParameter.TargetString_Normal.append("]");
			}
			else {
				for (auto StringIter = CommandString.begin();StringIter != CommandString.end();++StringIter)
				{
				//Domain
					if (*StringIter < ASCII_PERIOD || *StringIter == ASCII_SLASH || *StringIter > ASCII_NINE)
					{
					//Check parameter.
						if (Command.length() <= DOMAIN_MINSIZE)
						{
							PrintErrorToScreen(L"\n[Error] Target length error", 0);
							return false;
						}

					//Get address.
						addrinfo AddrInfoHints, *AddrInfo = nullptr;
						memset(&AddrInfoHints, 0, sizeof(AddrInfoHints));
						AddrInfoHints.ai_family = ConfigurationParameter.Protocol;
						if (getaddrinfo(CommandString.c_str(), nullptr, &AddrInfoHints, &AddrInfo) != 0)
						{
							PrintErrorToScreen(L"\n[Error] Resolve domain name error", WSAGetLastError());
							return false;
						}

					//Get address from result.
						if (AddrInfo != nullptr)
						{
							for (auto AddrInfoIter = AddrInfo;AddrInfoIter != nullptr;AddrInfoIter = AddrInfoIter->ai_next)
							{
							//IPv6
								if (AddrInfoIter->ai_family == AF_INET6 && !IN6_IS_ADDR_LINKLOCAL(reinterpret_cast<in6_addr *>(AddrInfoIter->ai_addr)) && !(reinterpret_cast<sockaddr_in6 *>(AddrInfoIter->ai_addr)->sin6_scope_id == 0)) //Get port from first(Main) IPv6 device
								{
									ConfigurationParameter.Protocol = AF_INET6;
									ConfigurationParameter.SockAddr_Normal.ss_family = AF_INET6;
									reinterpret_cast<sockaddr_in6 *>(&ConfigurationParameter.SockAddr_Normal)->sin6_addr = reinterpret_cast<sockaddr_in6 *>(AddrInfoIter->ai_addr)->sin6_addr;

								//Convert binary to address string.
									ConfigurationParameter.TargetAddressString = CommandString;
									uint8_t AddrBuffer[ADDRESS_STRING_MAXSIZE]{0};
									if (!BinaryToAddressString(AF_INET6, &reinterpret_cast<sockaddr_in6 *>(&ConfigurationParameter.SockAddr_Normal)->sin6_addr, AddrBuffer, ADDRESS_STRING_MAXSIZE, &SignedResult))
									{
										PrintErrorToScreen(L"\n[Error] IPv6 address format error error", SignedResult);
										return false;
									}

									CaseConvert(true, AddrBuffer, strnlen_s(reinterpret_cast<const char *>(AddrBuffer), ADDRESS_STRING_MAXSIZE));
									ConfigurationParameter.TargetString_Normal.append("[");
									ConfigurationParameter.TargetString_Normal.append(reinterpret_cast<const char *>(AddrBuffer));
									ConfigurationParameter.TargetString_Normal.append("]");
									break;
								}
							//IPv4
								else if (AddrInfoIter->ai_family == AF_INET && reinterpret_cast<sockaddr_in *>(AddrInfoIter->ai_addr)->sin_addr.s_addr != INADDR_LOOPBACK && reinterpret_cast<sockaddr_in *>(AddrInfoIter->ai_addr)->sin_addr.s_addr != INADDR_BROADCAST)
								{
									ConfigurationParameter.Protocol = AF_INET;
									ConfigurationParameter.SockAddr_Normal.ss_family = AF_INET;
									reinterpret_cast<sockaddr_in *>(&ConfigurationParameter.SockAddr_Normal)->sin_addr = reinterpret_cast<sockaddr_in *>(AddrInfoIter->ai_addr)->sin_addr;

								//Convert binary to address string.
									ConfigurationParameter.TargetAddressString = CommandString;
									uint8_t AddrBuffer[ADDRESS_STRING_MAXSIZE]{0};
									if (!BinaryToAddressString(AF_INET, &reinterpret_cast<sockaddr_in *>(&ConfigurationParameter.SockAddr_Normal)->sin_addr, AddrBuffer, ADDRESS_STRING_MAXSIZE, &SignedResult))
									{
										PrintErrorToScreen(L"\n[Error] IPv4 address format error error", SignedResult);
										return false;
									}

									ConfigurationParameter.TargetString_Normal = reinterpret_cast<const char *>(AddrBuffer);
									break;
								}
							}

							freeaddrinfo(AddrInfo);
						}
						else {
							PrintErrorToScreen(L"\n[Error] Resolve domain name error", WSAGetLastError());
							return false;
						}


						break;
					}

				//IPv4
					if (StringIter == CommandString.end() - 1U)
					{
					//Check parameter.
						if (Command.length() < IPV4_SHORTEST_ADDR_STRING || Command.length() > IPV4_LONGEST_ADDR_STRING) //IPv4 format
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
						if (!AddressStringToBinary(AF_INET, reinterpret_cast<const uint8_t *>(CommandString.c_str()), &reinterpret_cast<sockaddr_in *>(&ConfigurationParameter.SockAddr_Normal)->sin_addr, &SignedResult))
						{
							PrintErrorToScreen(L"\n[Error] Target format error", SignedResult);
							return false;
						}

						ConfigurationParameter.TargetString_Normal = CommandString;
					}
				}
			}
		}
	}

	return true;
}
