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
ConfigurationTable ConfigurationParameter;

//Main function of program
#if defined(PLATFORM_WIN)
int wmain(
	int argc, 
	wchar_t *argv[])
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
int main(
	int argc, 
	char *argv[])
#endif
{
//Main process
	if (argc <= 2)
	{
		PrintDescription();
	}
	else {
	//Initialization and read commands.
		if (!ConfigurationInitialization() || !ReadCommands(argc, argv))
			return EXIT_FAILURE;

	//Check parameter reading.
		if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET6)
		{
			if (CheckEmptyBuffer(&((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_addr, sizeof(((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_addr)))
			{
				PrintErrorToScreen(L"\n[Error] Target is empty", 0);
				return EXIT_FAILURE;
			}
			else {
			//Mark port.
				if (ConfigurationParameter.ServiceType == 0)
				{
					ConfigurationParameter.ServiceType = htons(IPPORT_DNS);
					((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_port = htons(IPPORT_DNS);
				}
				else {
					((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_port = ConfigurationParameter.ServiceType;
				}
			}
		}
		else if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET)
		{
			if (((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_addr.s_addr == 0)
			{
				PrintErrorToScreen(L"\n[Error] Target is empty", 0);
				return EXIT_FAILURE;
			}
			else {
			//Mark port.
				if (ConfigurationParameter.ServiceType == 0)
				{
					ConfigurationParameter.ServiceType = htons(IPPORT_DNS);
					((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_port = htons(IPPORT_DNS);
				}
				else {
					((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_port = ConfigurationParameter.ServiceType;
				}
			}
		}
		else { //SOCKS mode
			if (ConfigurationParameter.SockAddr_SOCKS.ss_family == 0)
			{
				PrintErrorToScreen(L"\n[Error] Target is empty", 0);
				return EXIT_FAILURE;
			}
			else {
			//Mark port.
				if (ConfigurationParameter.ServiceType == 0)
					ConfigurationParameter.ServiceType = htons(IPPORT_DNS);
			}
		}

	//Check parameter.
	#if defined(PLATFORM_WIN)
		//Minimum supported system of Windows Version Helpers is Windows Vista.
		#if !defined(PLATFORM_WIN_XP)
			if (!IsWindows8OrGreater())
		#endif
			{
				if (ConfigurationParameter.SocketTimeout > TIME_OUT_MIN)
					ConfigurationParameter.SocketTimeout -= 500;
				else if (ConfigurationParameter.SocketTimeout == TIME_OUT_MIN)
					ConfigurationParameter.SocketTimeout = 1;
			}

		ConfigurationParameter.Statistics_MinTime = ConfigurationParameter.SocketTimeout;
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		ConfigurationParameter.Statistics_MinTime = ConfigurationParameter.SocketTimeout.tv_sec * SECOND_TO_MILLISECOND + ConfigurationParameter.SocketTimeout.tv_usec / MICROSECOND_TO_MILLISECOND;
	#endif

	//Convert multiple byte string to wide char string.
		std::wstring wTestDomain, wTargetAddressString;
		if (!MBSToWCSString((const uint8_t *)ConfigurationParameter.TargetString_Normal.c_str(), ConfigurationParameter.TargetString_Normal.length(), ConfigurationParameter.wTargetString) || 
			!MBSToWCSString((const uint8_t *)ConfigurationParameter.TestDomain.c_str(), ConfigurationParameter.TestDomain.length(), wTestDomain))
		{
			PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
			return EXIT_FAILURE;
		}
		if (!ConfigurationParameter.TargetAddressString.empty())
		{
			if (!MBSToWCSString((const uint8_t *)ConfigurationParameter.TargetAddressString.c_str(), ConfigurationParameter.TargetAddressString.length(), wTargetAddressString))
			{
				PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
				return EXIT_FAILURE;
			}
		}

	//Check DNS header.
		if (ConfigurationParameter.Parameter_Header.Flags == 0)
			ConfigurationParameter.Parameter_Header.Flags = htons(DNS_STANDARD);
		if (ConfigurationParameter.Parameter_Header.Questions == 0)
			ConfigurationParameter.Parameter_Header.Questions = htons(U16_NUM_ONE);

	//Check DNS query.
		if (ConfigurationParameter.Parameter_Query.Classes == 0)
			ConfigurationParameter.Parameter_Query.Classes = htons(DNS_CLASS_IN);
		if (ConfigurationParameter.Parameter_Query.Type == 0)
		{
			if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET6)
				ConfigurationParameter.Parameter_Query.Type = htons(DNS_RECORD_AAAA);
			else //IPv4
				ConfigurationParameter.Parameter_Query.Type = htons(DNS_RECORD_A);
		}

	//Check EDNS Label.
		if (ConfigurationParameter.IsDNSSEC)
			ConfigurationParameter.IsEDNS = true;
		if (ConfigurationParameter.IsEDNS)
		{
			ConfigurationParameter.Parameter_Header.Additional = htons(U16_NUM_ONE);
			ConfigurationParameter.Parameter_EDNS.Type = htons(DNS_RECORD_OPT);
			if (ConfigurationParameter.EDNSPayloadSize == 0)
				ConfigurationParameter.Parameter_EDNS.UDPPayloadSize = htons(EDNS0_MINSIZE);
			else 
				ConfigurationParameter.Parameter_EDNS.UDPPayloadSize = htons((uint16_t)ConfigurationParameter.EDNSPayloadSize);
			if (ConfigurationParameter.IsDNSSEC)
			{
				ConfigurationParameter.Parameter_Header.FlagsBits.AD = ~ConfigurationParameter.Parameter_Header.FlagsBits.AD; //Local DNSSEC Server validate
//				ConfigurationParameter.Parameter_Header.FlagsBits.CD = ~ConfigurationParameter.Parameter_Header.FlagsBits.CD; //Checking Disabled
				ConfigurationParameter.Parameter_EDNS.Z_Bits.DO = ~ConfigurationParameter.Parameter_EDNS.Z_Bits.DO; //Accepts DNSSEC security RRs
			}
		}

	//Output result to file.
		if (!ConfigurationParameter.wOutputFileName.empty() && !OutputResultToFile())
			return EXIT_FAILURE;

	//Print to screen before sending and send request.
		PrintHeaderToScreen(wTargetAddressString, wTestDomain);
		if (ConfigurationParameter.Statistics_Send == 0)
		{
			for (;;)
			{
				if (ConfigurationParameter.Statistics_RealSend <= UINT16_MAX)
				{
					++ConfigurationParameter.Statistics_RealSend;
					if (!SendProcess(ConfigurationParameter.SockAddr_Normal, false))
						return EXIT_FAILURE;
				}
				else {
					fwprintf_s(stderr, L"\n[Notice] Statistics is full.\n");
					if (ConfigurationParameter.OutputFile != nullptr)
						fwprintf_s(ConfigurationParameter.OutputFile, L"\n[Notice] Statistics is full.\n");
					PrintProcess(true, true);

				//Close file handle.
					if (ConfigurationParameter.OutputFile != nullptr)
						fclose(ConfigurationParameter.OutputFile);

					return EXIT_SUCCESS;
				}
			}
		}
		else {
			auto LastSend = false;
			for (size_t Index = 0;Index < ConfigurationParameter.Statistics_Send;++Index)
			{
				++ConfigurationParameter.Statistics_RealSend;
				if (Index == ConfigurationParameter.Statistics_Send - 1U)
					LastSend = true;
				if (!SendProcess(ConfigurationParameter.SockAddr_Normal, LastSend))
				{
				//Close file handle.
					if (ConfigurationParameter.OutputFile != nullptr)
						fclose(ConfigurationParameter.OutputFile);

					return EXIT_FAILURE;
				}
			}
		}

	//Print to screen before finished and close file handle.
		PrintProcess(true, true);
		if (ConfigurationParameter.OutputFile != nullptr)
			fclose(ConfigurationParameter.OutputFile);
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
			(PHANDLER_ROUTINE)CtrlHandler, 
			TRUE) == FALSE)
	{
		PrintErrorToScreen(L"\n[Error] Set console ctrl handler error", GetLastError());
		return false;
	}

//Winsock initialization
	WSAData WSAInitialization;
	memset(&WSAInitialization, 0, sizeof(WSAInitialization));
	if (WSAStartup(MAKEWORD(2, 2), &WSAInitialization) != 0 || LOBYTE(WSAInitialization.wVersion) != 2 || HIBYTE(WSAInitialization.wVersion) != 2)
	{
		PrintErrorToScreen(L"\n[Error] Winsock initialization error", WSAGetLastError());
		return false;
	}
	else {
		ConfigurationParameter.IsWinSockInitialized = true;
	}
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
//Handle the system signal.
	if (signal(SIGHUP, SIG_Handler) == SIG_ERR || signal(SIGINT, SIG_Handler) == SIG_ERR || 
		signal(SIGQUIT, SIG_Handler) == SIG_ERR || signal(SIGTERM, SIG_Handler) == SIG_ERR)
	{
		PrintErrorToScreen(L"\n[Error] Handle the system signal error", errno);
		return false;
	}
#endif

	return true;
}
