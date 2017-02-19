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


#include "Base.h"

//Global variables
ConfigurationTable ConfigurationParameter;

//Main function of program
#if defined(PLATFORM_WIN)
int wmain(
	int argc, 
	wchar_t *argv[])
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
int main(
	int argc, 
	char *argv[])
#endif
{
//Main process
	if (argc <= COMMAND_MIN_COUNT)
	{
		PrintDescription();
	}
	else {
	//Initialization, read commands, check all parameters and settings.
		if (!ConfigurationInitialization() || !ReadCommand(argc, argv) || !ParameterCheckAndSetting())
			return EXIT_FAILURE;

	//Main send process
		if (!MainProcess())
			return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//Configuration initialization process
bool ConfigurationInitialization(
	void)
{
#if defined(PLATFORM_WIN)
//Handle the system signal.
	if (SetConsoleCtrlHandler(
			reinterpret_cast<PHANDLER_ROUTINE>(CtrlHandler), 
			TRUE) == FALSE)
	{
		PrintErrorToScreen(L"\n[Error] Set console ctrl handler error", GetLastError());
		return false;
	}

//Screen output buffer settings
	_set_errno(0);
	if (setvbuf(stderr, nullptr, _IONBF, 0) != 0)
	{
		PrintErrorToScreen(L"\n[Error] Screen output buffer setting error", errno);
		return false;
	}

//Winsock initialization
	WSAData WSAInitialization;
	memset(&WSAInitialization, 0, sizeof(WSAInitialization));
	if (WSAStartup(
			MAKEWORD(WINSOCK_VERSION_HIGH_BYTE, WINSOCK_VERSION_LOW_BYTE), 
			&WSAInitialization) != 0 || 
		LOBYTE(WSAInitialization.wVersion) != WINSOCK_VERSION_LOW_BYTE || 
		HIBYTE(WSAInitialization.wVersion) != WINSOCK_VERSION_HIGH_BYTE)
	{
		PrintErrorToScreen(L"\n[Error] Winsock initialization error", WSAGetLastError());
		return false;
	}
	else {
		ConfigurationParameter.IsInitialized_WinSock = true;
	}
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
//Handle the system signal.
	errno = 0;
	if (signal(SIGHUP, SIG_Handler) == SIG_ERR || signal(SIGINT, SIG_Handler) == SIG_ERR || 
		signal(SIGQUIT, SIG_Handler) == SIG_ERR || signal(SIGTERM, SIG_Handler) == SIG_ERR)
	{
		PrintErrorToScreen(L"\n[Error] Handle the system signal error", errno);
		return false;
	}

//Set system signal handler to ignore EPIPE signal when transport with socket.
	errno = 0;
	if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
	{
		PrintErrorToScreen(L"\n[Error] Ignore system signal error", errno);
		return false;
	}
#endif

	return true;
}

//Check all parameters and settings
bool ParameterCheckAndSetting(
	void)
{
//Socket address check
	if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET6) //IPv6
	{
		if (CheckEmptyBuffer(&(reinterpret_cast<PSOCKADDR_IN6>(&ConfigurationParameter.SockAddr_Normal))->sin6_addr, sizeof((reinterpret_cast<PSOCKADDR_IN6>(&ConfigurationParameter.SockAddr_Normal))->sin6_addr)))
		{
			PrintErrorToScreen(L"\n[Error] Target is empty", 0);
			return false;
		}
		else { //Mark port.
			if (ConfigurationParameter.ServiceType == 0)
			{
				ConfigurationParameter.ServiceType = htons(IPPORT_DNS);
				(reinterpret_cast<PSOCKADDR_IN6>(&ConfigurationParameter.SockAddr_Normal))->sin6_port = htons(IPPORT_DNS);
			}
			else {
				(reinterpret_cast<PSOCKADDR_IN6>(&ConfigurationParameter.SockAddr_Normal))->sin6_port = ConfigurationParameter.ServiceType;
			}
		}
	}
	else if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET) //IPv4
	{
		if ((reinterpret_cast<PSOCKADDR_IN>(&ConfigurationParameter.SockAddr_Normal))->sin_addr.s_addr == 0)
		{
			PrintErrorToScreen(L"\n[Error] Target is empty", 0);
			return false;
		}
		else { //Mark port.
			if (ConfigurationParameter.ServiceType == 0)
			{
				ConfigurationParameter.ServiceType = htons(IPPORT_DNS);
				(reinterpret_cast<PSOCKADDR_IN>(&ConfigurationParameter.SockAddr_Normal))->sin_port = htons(IPPORT_DNS);
			}
			else {
				(reinterpret_cast<PSOCKADDR_IN>(&ConfigurationParameter.SockAddr_Normal))->sin_port = ConfigurationParameter.ServiceType;
			}
		}
	}
	else {
		PrintErrorToScreen(L"\n[Error] Target address error", 0);
		return false;
	}

//Socket timeout settings
#if defined(PLATFORM_WIN)
#if !defined(PLATFORM_WIN_XP)
	if (!IsWindows8OrGreater())
#endif
	{
		if (ConfigurationParameter.SocketTimeout > SOCKET_MIN_TIMEOUT)
			ConfigurationParameter.SocketTimeout -= SOCKET_MIN_TIMEOUT;
		else if (ConfigurationParameter.SocketTimeout == SOCKET_MIN_TIMEOUT)
			ConfigurationParameter.SocketTimeout = 1;
	}

	ConfigurationParameter.Statistics_MinTime = ConfigurationParameter.SocketTimeout;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	ConfigurationParameter.Statistics_MinTime = ConfigurationParameter.SocketTimeout.tv_sec * SECOND_TO_MILLISECOND + ConfigurationParameter.SocketTimeout.tv_usec / MICROSECOND_TO_MILLISECOND;
#endif

//Convert multiple byte string to wide char string(Target domain and test domain).
	std::wstring WideTestDomainString, WideTargetAddressString;
	if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(ConfigurationParameter.TargetString_Normal.c_str()), ConfigurationParameter.TargetString_Normal.length(), ConfigurationParameter.WideTargetString) || 
		!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(ConfigurationParameter.TestDomainString.c_str()), ConfigurationParameter.TestDomainString.length(), WideTestDomainString))
	{
		PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
		return false;
	}

//Convert multiple byte string to wide char string(Target address).
	if (!ConfigurationParameter.TargetAddressString.empty())
	{
		if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(ConfigurationParameter.TargetAddressString.c_str()), ConfigurationParameter.TargetAddressString.length(), WideTargetAddressString))
		{
			PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
			return false;
		}
	}

//Check DNS packet data.
	if (ConfigurationParameter.Parameter_Header.Flags == 0)
		ConfigurationParameter.Parameter_Header.Flags = htons(DNS_STANDARD);
	if (ConfigurationParameter.Parameter_Header.Question == 0)
		ConfigurationParameter.Parameter_Header.Question = htons(U16_NUM_ONE);
	if (ConfigurationParameter.Parameter_Query.Classes == 0)
		ConfigurationParameter.Parameter_Query.Classes = htons(DNS_CLASS_INTERNET);
	if (ConfigurationParameter.Parameter_Query.Type == 0)
	{
		if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET6)
			ConfigurationParameter.Parameter_Query.Type = htons(DNS_TYPE_AAAA);
		else //IPv4
			ConfigurationParameter.Parameter_Query.Type = htons(DNS_TYPE_A);
	}

//Check EDNS Label.
	if (ConfigurationParameter.IsDNSSEC)
		ConfigurationParameter.IsEDNS = true;
	if (ConfigurationParameter.IsEDNS)
	{
		ConfigurationParameter.Parameter_Header.Additional = htons(U16_NUM_ONE);
		ConfigurationParameter.Parameter_EDNS.Type = htons(DNS_TYPE_OPT);
		if (ConfigurationParameter.EDNSPayloadSize == 0)
			ConfigurationParameter.Parameter_EDNS.UDPPayloadSize = htons(EDNS_PACKET_MINSIZE);
		else 
			ConfigurationParameter.Parameter_EDNS.UDPPayloadSize = htons(static_cast<uint16_t>(ConfigurationParameter.EDNSPayloadSize));
		if (ConfigurationParameter.IsDNSSEC)
		{
			ConfigurationParameter.Parameter_Header.FlagsBits.AD = ~ConfigurationParameter.Parameter_Header.FlagsBits.AD; //Local DNSSEC Server validated
//			ConfigurationParameter.Parameter_Header.FlagsBits.CD = ~ConfigurationParameter.Parameter_Header.FlagsBits.CD; //Checking Disabled
			ConfigurationParameter.Parameter_EDNS.Z_Bits.DO = ~ConfigurationParameter.Parameter_EDNS.Z_Bits.DO; //Accepts DNSSEC security RRs
		}
	}

//Output result to file.
	if (!ConfigurationParameter.WideOutputFileName.empty() && !OutputResultToFile())
		return false;

//Print to screen before sending and send request.
	PrintHeaderToScreen(WideTargetAddressString, WideTestDomainString);

	return true;
}

//Main send process
bool MainProcess(
	void)
{
	if (ConfigurationParameter.Statistics_PlanSend == 0)
	{
		for (;;)
		{
			if (ConfigurationParameter.Statistics_RealSend <= UINT16_MAX)
			{
				++ConfigurationParameter.Statistics_RealSend;
				if (!SendRequestProcess(ConfigurationParameter.SockAddr_Normal, false))
					return false;
			}
			else {
			//Statistics is full.
				fwprintf_s(stderr, L"\n[Notice] Statistics is full.\n");
				if (ConfigurationParameter.OutputFile != nullptr)
					fwprintf_s(ConfigurationParameter.OutputFile, L"\n[Notice] Statistics is full.\n");
				PrintProcess(true, true);

			//Close file handle.
				if (ConfigurationParameter.OutputFile != nullptr)
				{
					fclose(ConfigurationParameter.OutputFile);
					ConfigurationParameter.OutputFile = nullptr;
				}

				return false;
			}
		}
	}
	else {
		auto IsLastSend = false;
		for (size_t Index = 0;Index < ConfigurationParameter.Statistics_PlanSend;++Index)
		{
			++ConfigurationParameter.Statistics_RealSend;
			if (Index == ConfigurationParameter.Statistics_PlanSend - 1U)
				IsLastSend = true;
			if (!SendRequestProcess(ConfigurationParameter.SockAddr_Normal, IsLastSend))
			{
			//Close file handle.
				if (ConfigurationParameter.OutputFile != nullptr)
				{
					fclose(ConfigurationParameter.OutputFile);
					ConfigurationParameter.OutputFile = nullptr;
				}

				return false;
			}
		}
	}

//Print to screen before finished and close file handle.
	PrintProcess(true, true);
	if (ConfigurationParameter.OutputFile != nullptr)
	{
		fclose(ConfigurationParameter.OutputFile);
		ConfigurationParameter.OutputFile = nullptr;
	}

	return true;
}
