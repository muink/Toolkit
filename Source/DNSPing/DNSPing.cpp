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


#include "DNSPing.h"

//Main function of program
#if defined(PLATFORM_WIN)
int wmain(
	int argc, 
	wchar_t *argv[])
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
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
		if (!ConfigurationInitialization() || !ReadCommand(argc, argv) || !Parameter_CheckSetting())
			return EXIT_FAILURE;

	//Main send process
		if (!RequestLoopProcess())
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
			reinterpret_cast<PHANDLER_ROUTINE>(SignalHandler), 
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
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
//Handle the system signal.
	errno = 0;
	if (signal(SIGHUP, SignalHandler) == SIG_ERR || 
		signal(SIGINT, SignalHandler) == SIG_ERR || 
		signal(SIGQUIT, SignalHandler) == SIG_ERR || 
		signal(SIGTERM, SignalHandler) == SIG_ERR)
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
bool Parameter_CheckSetting(
	void)
{
//Socket address check
	if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET6) //IPv6
	{
		if (CheckEmptyBuffer(&reinterpret_cast<const sockaddr_in6 *>(&ConfigurationParameter.SockAddr_Normal)->sin6_addr, sizeof(reinterpret_cast<const sockaddr_in6 *>(&ConfigurationParameter.SockAddr_Normal)->sin6_addr)))
		{
			PrintErrorToScreen(L"\n[Error] Target is empty", 0);
			return false;
		}
	//Register port.
		else {
			if (ConfigurationParameter.ServiceType == 0)
			{
				ConfigurationParameter.ServiceType = hton16(IPPORT_DNS);
				reinterpret_cast<sockaddr_in6 *>(&ConfigurationParameter.SockAddr_Normal)->sin6_port = hton16(IPPORT_DNS);
			}
			else {
				reinterpret_cast<sockaddr_in6 *>(&ConfigurationParameter.SockAddr_Normal)->sin6_port = ConfigurationParameter.ServiceType;
			}
		}
	}
	else if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET) //IPv4
	{
		if (reinterpret_cast<const sockaddr_in *>(&ConfigurationParameter.SockAddr_Normal)->sin_addr.s_addr == 0)
		{
			PrintErrorToScreen(L"\n[Error] Target is empty", 0);
			return false;
		}
	//Register port.
		else {
			if (ConfigurationParameter.ServiceType == 0)
			{
				ConfigurationParameter.ServiceType = hton16(IPPORT_DNS);
				reinterpret_cast<sockaddr_in *>(&ConfigurationParameter.SockAddr_Normal)->sin_port = hton16(IPPORT_DNS);
			}
			else {
				reinterpret_cast<sockaddr_in *>(&ConfigurationParameter.SockAddr_Normal)->sin_port = ConfigurationParameter.ServiceType;
			}
		}
	}
	else {
		PrintErrorToScreen(L"\n[Error] Target address error", 0);
		return false;
	}

//Socket timeout settings
#if defined(PLATFORM_WIN)
	if (!IsWindows8OrGreater())
	{
		if (ConfigurationParameter.SocketTimeout > SOCKET_MIN_TIMEOUT)
			ConfigurationParameter.SocketTimeout -= SOCKET_MIN_TIMEOUT;
		else if (ConfigurationParameter.SocketTimeout == SOCKET_MIN_TIMEOUT)
			ConfigurationParameter.SocketTimeout = SOCKET_MIN_TIME_INTERVAL;
	}

	ConfigurationParameter.Statistics_MinTime = ConfigurationParameter.SocketTimeout;
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	ConfigurationParameter.Statistics_MinTime = ConfigurationParameter.SocketTimeout.tv_sec * SECOND_TO_MILLISECOND + ConfigurationParameter.SocketTimeout.tv_usec / MICROSECOND_TO_MILLISECOND;
#endif

//Convert multiple byte string to wide char string, target domain and test domain.
	std::wstring WideTestDomainString, WideTargetAddressString;
	if (!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(ConfigurationParameter.TargetString_Normal.c_str()), ConfigurationParameter.TargetString_Normal.length(), ConfigurationParameter.WideTargetString) || 
		!MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(ConfigurationParameter.TestDomainString.c_str()), ConfigurationParameter.TestDomainString.length(), WideTestDomainString))
	{
		PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
		return false;
	}

//Convert multiple byte string to wide char string, target address.
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
		ConfigurationParameter.Parameter_Header.Flags = hton16(DNS_FLAG_REQUEST_STANDARD);
	if (ConfigurationParameter.Parameter_Header.Question == 0)
		ConfigurationParameter.Parameter_Header.Question = hton16(U16_NUM_ONE);
	if (ConfigurationParameter.Parameter_Query.Classes == 0)
		ConfigurationParameter.Parameter_Query.Classes = hton16(DNS_CLASS_INTERNET);
	if (ConfigurationParameter.Parameter_Query.Type == 0)
	{
		if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET6)
			ConfigurationParameter.Parameter_Query.Type = hton16(DNS_TYPE_AAAA);
		else //IPv4
			ConfigurationParameter.Parameter_Query.Type = hton16(DNS_TYPE_A);
	}

//Check EDNS Label.
	if (ConfigurationParameter.IsDNSSEC)
		ConfigurationParameter.IsEDNS = true;
	if (ConfigurationParameter.IsEDNS)
	{
		ConfigurationParameter.Parameter_Header.Additional = hton16(U16_NUM_ONE);
		ConfigurationParameter.Parameter_EDNS.Type = hton16(DNS_TYPE_OPT);
		if (ConfigurationParameter.EDNSPayloadSize == 0)
			ConfigurationParameter.Parameter_EDNS.UDP_PayloadSize = hton16(EDNS_PACKET_MINSIZE);
		else 
			ConfigurationParameter.Parameter_EDNS.UDP_PayloadSize = hton16(static_cast<const uint16_t>(ConfigurationParameter.EDNSPayloadSize));
		if (ConfigurationParameter.IsDNSSEC)
		{
			ConfigurationParameter.Parameter_Header.Flags = hton16(ntoh16(ConfigurationParameter.Parameter_Header.Flags) | DNS_FLAG_GET_BIT_AD); //Set Authentic Data bit.
			ConfigurationParameter.Parameter_Header.Flags = hton16(ntoh16(ConfigurationParameter.Parameter_Header.Flags) | DNS_FLAG_GET_BIT_CD); //Set Checking Disabled bit.
			ConfigurationParameter.Parameter_EDNS.Z_Field = hton16(ntoh16(ConfigurationParameter.Parameter_EDNS.Z_Field) | EDNS_FLAG_GET_BIT_DO); //Set Accepts DNSSEC security resource records bit.
		}
	}

//Output result to file.
	if (!ConfigurationParameter.WideOutputFileName.empty() && !OutputResultToFile())
		return false;

//Print to screen before sending and send request.
	PrintHeaderToScreen(WideTargetAddressString, WideTestDomainString);
	return true;
}

//Main request process
bool RequestLoopProcess(
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
