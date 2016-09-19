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


#include "Process.h"

//Send DNS request process
bool SendProcess(
	const sockaddr_storage &Target, 
	const bool LastSend)
{
//Initialization(Part 1)
	SOCKET Socket_Normal = 0, Socket_SOCKS = 0, *Socket_Exchange = nullptr;
	sockaddr_storage SockAddr_SOCKS_UDP;
	memset(&SockAddr_SOCKS_UDP, 0, sizeof(SockAddr_SOCKS_UDP));
	socklen_t AddrLen_Normal = 0, AddrLen_SOCKS = 0;

//IPv6
	if (ConfigurationParameter.Protocol == AF_INET6 || ConfigurationParameter.SockAddr_SOCKS.ss_family == AF_INET6)
	{
	//Socket initialization
		AddrLen_Normal = sizeof(sockaddr_in6);
		if (ConfigurationParameter.SockAddr_SOCKS.ss_family == 0 && ConfigurationParameter.IsRawSocket && ConfigurationParameter.RawData)
		{
			Socket_Normal = socket(AF_INET6, SOCK_RAW, ConfigurationParameter.ServiceType);
			Socket_Exchange = &Socket_Normal;
		}
		else {
			if (ConfigurationParameter.SockAddr_SOCKS.ss_family > 0) //SOCKS mode
			{
				Socket_Normal = socket(AF_INET6, SOCK_STREAM, IPPROTO_TCP);
				Socket_SOCKS = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
				Socket_Exchange = &Socket_SOCKS;
			}
			else { //Normal mode
				Socket_Normal = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
				Socket_Exchange = &Socket_Normal;
			}
		}

	//Socket check
		if (Socket_Normal == INVALID_SOCKET || (ConfigurationParameter.SockAddr_SOCKS.ss_family > 0 && Socket_SOCKS == INVALID_SOCKET))
		{
			PrintErrorToScreen(L"[Error] Socket initialization error", WSAGetLastError());
			return false;
		}
	}
//IPv4
	else {
	//Socket initialization
		AddrLen_Normal = sizeof(sockaddr_in);
		if (ConfigurationParameter.SockAddr_SOCKS.ss_family == 0 && ConfigurationParameter.IsRawSocket && ConfigurationParameter.RawData)
		{
			Socket_Normal = socket(AF_INET, SOCK_RAW, ConfigurationParameter.ServiceType);
			Socket_Exchange = &Socket_Normal;
		}
		else {
			if (ConfigurationParameter.SockAddr_SOCKS.ss_family > 0) //SOCKS mode
			{
				Socket_Normal = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				Socket_SOCKS = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				Socket_Exchange = &Socket_SOCKS;
			}
			else { //Normal
				Socket_Normal = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				Socket_Exchange = &Socket_Normal;
			}
		}

	//Socket check
		if (Socket_Normal == INVALID_SOCKET || (ConfigurationParameter.SockAddr_SOCKS.ss_family > 0 && Socket_SOCKS == INVALID_SOCKET))
		{
			PrintErrorToScreen(L"[Error] Socket initialization error", WSAGetLastError());
			return false;
		}
	}

//Set socket timeout.
#if defined(PLATFORM_WIN)
	if (setsockopt(*Socket_Exchange, SOL_SOCKET, SO_SNDTIMEO, (const char *)&ConfigurationParameter.SocketTimeout, sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR || 
		setsockopt(*Socket_Exchange, SOL_SOCKET, SO_RCVTIMEO, (const char *)&ConfigurationParameter.SocketTimeout, sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR)
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	if (setsockopt(*Socket_Exchange, SOL_SOCKET, SO_SNDTIMEO, &ConfigurationParameter.SocketTimeout, sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR || 
		setsockopt(*Socket_Exchange, SOL_SOCKET, SO_RCVTIMEO, &ConfigurationParameter.SocketTimeout, sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR)
#endif
	{
		PrintErrorToScreen(L"[Error] Set UDP socket timeout error", WSAGetLastError());

		shutdown(Socket_Normal, SD_BOTH);
		shutdown(Socket_SOCKS, SD_BOTH);
		closesocket(Socket_Normal);
		closesocket(Socket_SOCKS);
		return false;
	}

//Set Internet Protocol header options.
	if (ConfigurationParameter.Protocol == AF_INET6)
	{
	#if defined(PLATFORM_WIN)
		if (ConfigurationParameter.PacketHopLimits != 0 && 
			setsockopt(*Socket_Exchange, IPPROTO_IPV6, IPV6_UNICAST_HOPS, (const char *)&ConfigurationParameter.PacketHopLimits, sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		if (ConfigurationParameter.PacketHopLimits != 0 && 
			setsockopt(*Socket_Exchange, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ConfigurationParameter.PacketHopLimits, sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
	#endif
		{
			PrintErrorToScreen(L"[Error] Set HopLimit flag error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);
			return false;
		}
	}
	else { //IPv4
	#if defined(PLATFORM_WIN)
		if (ConfigurationParameter.PacketHopLimits != 0 && 
			setsockopt(*Socket_Exchange, IPPROTO_IP, IP_TTL, (const char *)&ConfigurationParameter.PacketHopLimits, sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		if (ConfigurationParameter.PacketHopLimits != 0 && 
			setsockopt(*Socket_Exchange, IPPROTO_IP, IP_TTL, &ConfigurationParameter.PacketHopLimits, sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
	#endif
		{
			PrintErrorToScreen(L"[Error] Set TTL flag error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);
			return false;
		}

	//Set Do Not Fragment flag.
	#if (defined(PLATFORM_WIN) || defined(PLATFORM_LINUX))
		#if defined(PLATFORM_WIN)
			BOOL DoNotFragment = TRUE;
			if (ConfigurationParameter.IsDoNotFragment && 
				setsockopt(*Socket_Exchange, IPPROTO_IP, IP_DONTFRAGMENT, (const char *)&DoNotFragment, sizeof(DoNotFragment)) == SOCKET_ERROR)
		#elif defined(PLATFORM_LINUX)
			int DoNotFragment = IP_PMTUDISC_DO;
			if (ConfigurationParameter.IsDoNotFragment && 
				setsockopt(*Socket_Exchange, IPPROTO_IP, IP_MTU_DISCOVER, &DoNotFragment, sizeof(DoNotFragment)) == SOCKET_ERROR)
		#endif
		{
			PrintErrorToScreen(L"[Error] Set Do Not Fragment flag error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);
			return false;
		}
	#endif
	}

//Initialization(Part 2)
	std::shared_ptr<uint8_t> Buffer(new uint8_t[ConfigurationParameter.BufferSize]()), RecvBuffer(new uint8_t[ConfigurationParameter.BufferSize]());
	memset(Buffer.get(), 0, ConfigurationParameter.BufferSize);
	memset(RecvBuffer.get(), 0, ConfigurationParameter.BufferSize);
#if defined(PLATFORM_WIN)
	LARGE_INTEGER CPUFrequency, BeforeTime, AfterTime;
	memset(&CPUFrequency, 0, sizeof(CPUFrequency));
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	timeval BeforeTime, AfterTime;
#endif
	memset(&BeforeTime, 0, sizeof(BeforeTime));
	memset(&AfterTime, 0, sizeof(AfterTime));
	ssize_t DataLength = 0;

//Make standard packet.
	dns_hdr *pdns_hdr = nullptr;
	if (ConfigurationParameter.SockAddr_SOCKS.ss_family == 0 && ConfigurationParameter.RawData)
	{
		if (ConfigurationParameter.BufferSize >= ConfigurationParameter.RawDataLen)
		{
			memcpy_s(Buffer.get(), ConfigurationParameter.BufferSize, ConfigurationParameter.RawData.get(), ConfigurationParameter.RawDataLen);
			DataLength = ConfigurationParameter.RawDataLen;
		}
		else {
			memcpy_s(Buffer.get(), ConfigurationParameter.BufferSize, ConfigurationParameter.RawData.get(), ConfigurationParameter.BufferSize);
			DataLength = ConfigurationParameter.BufferSize;
		}
	}
	else {
	//DNS request
		memcpy_s(Buffer.get() + DataLength, ConfigurationParameter.BufferSize - DataLength, &ConfigurationParameter.Parameter_Header, sizeof(ConfigurationParameter.Parameter_Header));
		if (ConfigurationParameter.Parameter_Header.ID == 0)
		{
			pdns_hdr = (dns_hdr *)(Buffer.get() + DataLength);
		#if defined(PLATFORM_WIN)
			pdns_hdr->ID = htons((uint16_t)GetCurrentProcessId());
		#elif defined(PLATFORM_LINUX)
			pdns_hdr->ID = htons((uint16_t)pthread_self());
		#elif defined(PLATFORM_MACX)
			pdns_hdr->ID = htons(*(uint16_t *)pthread_self());
		#endif
		}
		DataLength += sizeof(dns_hdr);
		DataLength += CharToDNSQuery((const uint8_t *)ConfigurationParameter.TestDomain.c_str(), Buffer.get() + DataLength);
		memcpy_s(Buffer.get() + DataLength, ConfigurationParameter.BufferSize - DataLength, &ConfigurationParameter.Parameter_Query, sizeof(ConfigurationParameter.Parameter_Query));
		DataLength += sizeof(dns_qry);
		if (ConfigurationParameter.IsEDNS)
		{
			memcpy_s(Buffer.get() + DataLength, ConfigurationParameter.BufferSize - DataLength, &ConfigurationParameter.Parameter_EDNS, sizeof(ConfigurationParameter.Parameter_EDNS));
			DataLength += sizeof(dns_opt_record);
		}
	}

//SOCKS mode
	if (ConfigurationParameter.SockAddr_SOCKS.ss_family > 0)
	{
	//Copy socket information.
		SockAddr_SOCKS_UDP.ss_family = ConfigurationParameter.SockAddr_SOCKS.ss_family;
		if (SockAddr_SOCKS_UDP.ss_family == AF_INET6)
		{
			((PSOCKADDR_IN6)&SockAddr_SOCKS_UDP)->sin6_addr = ((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_SOCKS)->sin6_addr;
		#if defined(PLATFORM_MACX)
			((PSOCKADDR_IN6)&SockAddr_SOCKS_UDP)->sin6_port = ((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_SOCKS)->sin6_port;
		#endif
			AddrLen_SOCKS = sizeof(sockaddr_in6);
		}
		else {
			((PSOCKADDR_IN)&SockAddr_SOCKS_UDP)->sin_addr = ((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_SOCKS)->sin_addr;
		#if defined(PLATFORM_MACX)
			((PSOCKADDR_IN)&SockAddr_SOCKS_UDP)->sin_port = ((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_SOCKS)->sin_port;
		#endif
			AddrLen_SOCKS = sizeof(sockaddr_in);
		}

	//SOCKS Local UDP socket connecting and get UDP socket infomation.
		sockaddr_storage SockAddr_SOCKS_Local;
		memset(&SockAddr_SOCKS_Local, 0, sizeof(SockAddr_SOCKS_Local));
		uint16_t SOCKS_Local_Port = 0;
		if (connect(Socket_SOCKS, (PSOCKADDR)&SockAddr_SOCKS_UDP, AddrLen_SOCKS) == SOCKET_ERROR || 
			getsockname(Socket_SOCKS, (PSOCKADDR)&SockAddr_SOCKS_Local, &AddrLen_SOCKS) == SOCKET_ERROR)
		{
			PrintErrorToScreen(L"[Error] Connecting error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);
			return false;
		}
		else {
			if (SockAddr_SOCKS_UDP.ss_family == AF_INET6)
				SOCKS_Local_Port = ((PSOCKADDR_IN6)&SockAddr_SOCKS_Local)->sin6_port;
			else 
				SOCKS_Local_Port = ((PSOCKADDR_IN)&SockAddr_SOCKS_Local)->sin_port;
		}

	//SOCKS UDP-ASSOCIATE process
		if (SOCKS_UDP_ASSOCIATE(SockAddr_SOCKS_UDP, SOCKS_Local_Port, Socket_Normal, (PSOCKADDR)&ConfigurationParameter.SockAddr_SOCKS, AddrLen_Normal) == EXIT_FAILURE)
		{
			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);

			return false;
		}
		else if (connect(Socket_SOCKS, (PSOCKADDR)&SockAddr_SOCKS_UDP, AddrLen_SOCKS) == SOCKET_ERROR)
		{
			PrintErrorToScreen(L"[Error] Connecting error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);
			return false;
		}

	//Make SOCKS packet header.
		if (Target.ss_family == AF_INET6)
		{
			memmove_s(Buffer.get() + sizeof(socks_udp_relay_request) + sizeof(in6_addr) + sizeof(uint16_t), ConfigurationParameter.BufferSize - (sizeof(socks_udp_relay_request) + sizeof(in6_addr) + sizeof(uint16_t)), Buffer.get(), DataLength);
			memset(Buffer.get(), 0, sizeof(socks_udp_relay_request) + sizeof(in6_addr) + sizeof(uint16_t));
			((psocks_udp_relay_request)Buffer.get())->Address_Type = SOCKS5_ADDRESS_IPV6;
			memcpy_s(Buffer.get() + sizeof(socks_udp_relay_request), ConfigurationParameter.BufferSize - sizeof(socks_udp_relay_request), &((PSOCKADDR_IN6)&Target)->sin6_addr, sizeof(((PSOCKADDR_IN6)&Target)->sin6_addr));
			*(uint16_t *)(Buffer.get() + sizeof(socks_udp_relay_request) + sizeof(in6_addr)) = ((PSOCKADDR_IN6)&Target)->sin6_port;
			DataLength += sizeof(socks_udp_relay_request) + sizeof(in6_addr) + sizeof(uint16_t);
		}
		else if (Target.ss_family == AF_INET)
		{
			memmove_s(Buffer.get() + sizeof(socks_udp_relay_request) + sizeof(in_addr) + sizeof(uint16_t), ConfigurationParameter.BufferSize - (sizeof(socks_udp_relay_request) + sizeof(in_addr) + sizeof(uint16_t)), Buffer.get(), DataLength);
			memset(Buffer.get(), 0, sizeof(socks_udp_relay_request) + sizeof(in_addr) + sizeof(uint16_t));
			((psocks_udp_relay_request)Buffer.get())->Address_Type = SOCKS5_ADDRESS_IPV4;
			memcpy_s(Buffer.get() + sizeof(socks_udp_relay_request), ConfigurationParameter.BufferSize - sizeof(socks_udp_relay_request), &((PSOCKADDR_IN)&Target)->sin_addr, sizeof(((PSOCKADDR_IN)&Target)->sin_addr));
			*(uint16_t *)(Buffer.get() + sizeof(socks_udp_relay_request) + sizeof(in_addr)) = ((PSOCKADDR_IN)&Target)->sin_port;
			DataLength += sizeof(socks_udp_relay_request) + sizeof(in_addr) + sizeof(uint16_t);
		}
		else if (!ConfigurationParameter.TargetString_Normal.empty())
		{
			memmove_s(Buffer.get() + sizeof(socks_udp_relay_request) + sizeof(uint8_t) + ConfigurationParameter.TargetString_Normal.length() + sizeof(uint16_t), ConfigurationParameter.BufferSize - (sizeof(socks_udp_relay_request) + sizeof(uint8_t) + ConfigurationParameter.TargetString_Normal.length() + sizeof(uint16_t)), Buffer.get(), DataLength);
			memset(Buffer.get(), 0, sizeof(socks_udp_relay_request) + sizeof(uint8_t) + ConfigurationParameter.TargetString_Normal.length() + sizeof(uint16_t));
			((psocks_udp_relay_request)Buffer.get())->Address_Type = SOCKS5_ADDRESS_DOMAIN;
			*(uint8_t *)(Buffer.get() + sizeof(socks_udp_relay_request)) = (uint8_t)ConfigurationParameter.TargetString_Normal.length();
			memcpy_s(Buffer.get() + sizeof(socks_udp_relay_request) + sizeof(uint8_t), ConfigurationParameter.BufferSize - (sizeof(socks_udp_relay_request) + sizeof(uint8_t)), ConfigurationParameter.TargetString_Normal.c_str(), ConfigurationParameter.TargetString_Normal.length());
			*(uint16_t *)(Buffer.get() + sizeof(socks_udp_relay_request) + sizeof(uint8_t) + ConfigurationParameter.TargetString_Normal.length()) = ConfigurationParameter.ServiceType;
			DataLength += sizeof(socks_udp_relay_request) + sizeof(uint8_t) + ConfigurationParameter.TargetString_Normal.length() + sizeof(uint16_t);
		}
		else {
			PrintErrorToScreen(L"[Error] Make SOCKS packet header error", 0);

			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);
			return false;
		}
	}
//Normal mode
	else {
		if (connect(*Socket_Exchange, (PSOCKADDR)&Target, AddrLen_Normal) == SOCKET_ERROR)
		{
			PrintErrorToScreen(L"[Error] Connecting error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			shutdown(Socket_SOCKS, SD_BOTH);
			closesocket(Socket_Normal);
			closesocket(Socket_SOCKS);
			return false;
		}
	}

//Send request.
#if defined(PLATFORM_WIN)
	if (QueryPerformanceFrequency(
			&CPUFrequency) == 0 || 
		QueryPerformanceCounter(
			&BeforeTime) == 0)
	{
		PrintErrorToScreen(L"[Error] Get current time from High Precision Event Timer/HPET error", GetLastError());
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	if (gettimeofday(&BeforeTime, NULL) != 0)
	{
		PrintErrorToScreen(L"[Error] Get current time error", errno);
#endif

		shutdown(Socket_Normal, SD_BOTH);
		shutdown(Socket_SOCKS, SD_BOTH);
		closesocket(Socket_Normal);
		closesocket(Socket_SOCKS);
		return false;
	}
	else if (send(*Socket_Exchange, (const char *)Buffer.get(), (int)DataLength, 0) == SOCKET_ERROR)
	{
		PrintErrorToScreen(L"[Error] Send packet error", WSAGetLastError());

		shutdown(Socket_Normal, SD_BOTH);
		shutdown(Socket_SOCKS, SD_BOTH);
		closesocket(Socket_Normal);
		closesocket(Socket_SOCKS);
		return false;
	}

//Receive response.
#if defined(PLATFORM_WIN)
	DataLength = recv(*Socket_Exchange, (char *)RecvBuffer.get(), (int)ConfigurationParameter.BufferSize, 0);
	if (QueryPerformanceCounter(
			&AfterTime) == 0)
	{
		PrintErrorToScreen(L"[Error] Get current time from High Precision Event Timer/HPET error", GetLastError());

#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	#if defined(PLATFORM_LINUX)
		DataLength = recv(*Socket_Exchange, RecvBuffer.get(), ConfigurationParameter.BufferSize, MSG_NOSIGNAL);
	#elif defined(PLATFORM_MACX)
		DataLength = recv(*Socket_Exchange, RecvBuffer.get(), ConfigurationParameter.BufferSize, 0);
	#endif
	if (gettimeofday(&AfterTime, NULL) != 0)
	{
		PrintErrorToScreen(L"[Error] Get current time error", errno);
#endif

		shutdown(Socket_Normal, SD_BOTH);
		shutdown(Socket_SOCKS, SD_BOTH);
		closesocket(Socket_Normal);
		closesocket(Socket_SOCKS);
		return false;
	}

//Get waiting time.
#if defined(PLATFORM_WIN)
	auto Result = (long double)((AfterTime.QuadPart - BeforeTime.QuadPart) * (long double)MICROSECOND_TO_MILLISECOND / (long double)CPUFrequency.QuadPart);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	auto Result = (long double)(AfterTime.tv_sec - BeforeTime.tv_sec) * (long double)SECOND_TO_MILLISECOND;
	if (AfterTime.tv_sec >= BeforeTime.tv_sec)
		Result += (long double)(AfterTime.tv_usec - BeforeTime.tv_usec) / (long double)MICROSECOND_TO_MILLISECOND;
	else
		Result += (long double)(AfterTime.tv_usec + SECOND_TO_MILLISECOND * MICROSECOND_TO_MILLISECOND - BeforeTime.tv_usec) / (long double)MICROSECOND_TO_MILLISECOND;
#endif

//Print to screen.
	if (DataLength >= (ssize_t)DNS_PACKET_MINSIZE)
	{
	//SOCKS mode
		if (ConfigurationParameter.SockAddr_SOCKS.ss_family > 0)
		{
		//SOCKS reply check
			if (((psocks_udp_relay_request)RecvBuffer.get())->Reserved > 0 || ((psocks_udp_relay_request)RecvBuffer.get())->FragmentNumber > 0 || 
				DataLength < (ssize_t)sizeof(socks_udp_relay_request))
			{
				PrintErrorToScreen(L"[Error] SOCKS receive data format error", 0);

				shutdown(Socket_Normal, SD_BOTH);
				shutdown(Socket_SOCKS, SD_BOTH);
				closesocket(Socket_Normal);
				closesocket(Socket_SOCKS);
				return false;
			}
			else {
			//IPv6
				if (((psocks_udp_relay_request)RecvBuffer.get())->Address_Type == SOCKS5_ADDRESS_IPV6 && DataLength > sizeof(socks_udp_relay_request) + sizeof(in6_addr) + sizeof(uint16_t))
				{
					memmove_s(RecvBuffer.get(), ConfigurationParameter.BufferSize, RecvBuffer.get() + sizeof(socks_udp_relay_request) + sizeof(in6_addr) + sizeof(uint16_t), DataLength - (sizeof(socks_udp_relay_request) + sizeof(in6_addr) + sizeof(uint16_t)));
				}
			//IPv4
				else if (((psocks_udp_relay_request)RecvBuffer.get())->Address_Type == SOCKS5_ADDRESS_IPV4 && DataLength > sizeof(socks_udp_relay_request) + sizeof(in_addr) + sizeof(uint16_t))
				{
					memmove_s(RecvBuffer.get(), ConfigurationParameter.BufferSize, RecvBuffer.get() + sizeof(socks_udp_relay_request) + sizeof(in_addr) + sizeof(uint16_t), DataLength - (sizeof(socks_udp_relay_request) + sizeof(in_addr) + sizeof(uint16_t)));
				}
				else {
					PrintErrorToScreen(L"[Error] SOCKS receive data format error", 0);

					shutdown(Socket_Normal, SD_BOTH);
					shutdown(Socket_SOCKS, SD_BOTH);
					closesocket(Socket_Normal);
					closesocket(Socket_SOCKS);
					return false;
				}
			}

		//Print receive message to screen.
			std::wstring Message(L"Receive from %ls[%ls:%u] -> %d bytes, waiting ");
		#if defined(PLATFORM_WIN)
			Message.append(L"%lf ms.\n");
		#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
			Message.append(L"%Lf ms.\n");
		#endif
			fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.TargetString_SOCKS.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.TargetString_SOCKS.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
		}
		else {
		//Validate packet.
			if (ConfigurationParameter.IsValidate && pdns_hdr != nullptr && !ValidatePacket(RecvBuffer.get(), DataLength, pdns_hdr->ID))
			{
				std::wstring Message(L"Receive from %ls:%u -> %d bytes but validate error, waiting ");
			#if defined(PLATFORM_WIN)
				Message.append(L"%lf ms.\n");
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				Message.append(L"%Lf ms.\n");
			#endif
				fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
				if (ConfigurationParameter.OutputFile != nullptr)
					fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);

			//Try to waiting correct packet.
				for (;;)
				{
				//Timeout
				#if defined(PLATFORM_WIN)
					if (Result >= ConfigurationParameter.SocketTimeout)
						break;
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
					if (Result >= ConfigurationParameter.SocketTimeout.tv_usec / MICROSECOND_TO_MILLISECOND + ConfigurationParameter.SocketTimeout.tv_sec * SECOND_TO_MILLISECOND)
						break;
				#endif

				//Receive.
					memset(RecvBuffer.get(), 0, ConfigurationParameter.BufferSize);
				#if defined(PLATFORM_WIN)
					DataLength = recv(*Socket_Exchange, (char *)RecvBuffer.get(), (int)ConfigurationParameter.BufferSize, 0);
					if (QueryPerformanceCounter(
							&AfterTime) == 0)
					{
						PrintErrorToScreen(L"[Error] Get current time from High Precision Event Timer/HPET error", GetLastError());

						shutdown(Socket_Normal, SD_BOTH);
						shutdown(Socket_SOCKS, SD_BOTH);
						closesocket(Socket_Normal);
						closesocket(Socket_SOCKS);
						return false;
					}

				//Get waiting time.
					Result = (long double)((AfterTime.QuadPart - BeforeTime.QuadPart) * (long double)MICROSECOND_TO_MILLISECOND / (long double)CPUFrequency.QuadPart);

				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				//Receive.
					DataLength = recv(*Socket_Exchange, RecvBuffer.get(), ConfigurationParameter.BufferSize, 0);

				//Get waiting time.
					if (gettimeofday(&AfterTime, NULL) != 0)
					{
						PrintErrorToScreen(L"[Error] Get current time error", errno);

						close(Socket_Normal);
						close(Socket_SOCKS);
						return false;
					}
					Result = (long double)(AfterTime.tv_sec - BeforeTime.tv_sec) * (long double)SECOND_TO_MILLISECOND;
					if (AfterTime.tv_sec >= BeforeTime.tv_sec)
						Result += (long double)(AfterTime.tv_usec - BeforeTime.tv_usec) / (long double)MICROSECOND_TO_MILLISECOND;
					else
						Result += (long double)(AfterTime.tv_usec + SECOND_TO_MILLISECOND * MICROSECOND_TO_MILLISECOND - BeforeTime.tv_usec) / (long double)MICROSECOND_TO_MILLISECOND;
				#endif

				//SOCKET_ERROR
					if (DataLength <= 0)
						break;

				//Validate packet.
					if (!ValidatePacket(RecvBuffer.get(), DataLength, pdns_hdr->ID))
					{
						Message = (L"Receive from %ls:%u -> %d bytes but validate error, waiting ");
					#if defined(PLATFORM_WIN)
						Message.append(L"%lf ms.\n");
					#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
						Message.append(L"%Lf ms.\n");
					#endif
						fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
						if (ConfigurationParameter.OutputFile != nullptr)
							fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
					}
					else {
						break;
					}
				}

				if (DataLength <= 0)
				{
					Message = (L"Receive error: %d(%d), waiting correct answers timeout(");
				#if defined(PLATFORM_WIN)
					Message.append(L"%lf ms).\n");
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
					Message.append(L"%Lf ms).\n");
				#endif
					fwprintf_s(stderr, Message.c_str(), (int)DataLength, WSAGetLastError(), Result);
					if (ConfigurationParameter.OutputFile != nullptr)
						fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), (int)DataLength, WSAGetLastError(), Result);

					shutdown(Socket_Normal, SD_BOTH);
					shutdown(Socket_SOCKS, SD_BOTH);
					closesocket(Socket_Normal);
					closesocket(Socket_SOCKS);
					return true;
				}
				else {
					Message = (L"Receive from %ls:%u -> %d bytes, waiting ");
				#if defined(PLATFORM_WIN)
					Message.append(L"%lf ms.\n");
				#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
					Message.append(L"%Lf ms.\n");
				#endif
					fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
					if (ConfigurationParameter.OutputFile != nullptr)
						fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
				}
			}
			else {
				std::wstring Message(L"Receive from %ls:%u -> %d bytes, waiting ");
			#if defined(PLATFORM_WIN)
				Message.append(L"%lf ms.\n");
			#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
				Message.append(L"%Lf ms.\n");
			#endif
				fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
				if (ConfigurationParameter.OutputFile != nullptr)
					fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), (int)DataLength, Result);
			}
		}

	//Print response result or data.
		if (ConfigurationParameter.IsShowResponse)
		{
			PrintResponse(stderr, RecvBuffer.get(), DataLength);
			if (ConfigurationParameter.OutputFile != nullptr)
				PrintResponse(ConfigurationParameter.OutputFile, RecvBuffer.get(), DataLength);
		}
		if (ConfigurationParameter.IsShowHexResponse)
		{
			PrintHexResponse(stderr, RecvBuffer.get(), DataLength);
			if (ConfigurationParameter.OutputFile != nullptr)
				PrintHexResponse(ConfigurationParameter.OutputFile, RecvBuffer.get(), DataLength);
		}

	//Calculate time.
		ConfigurationParameter.Statistics_TotalTime += Result;
		++ConfigurationParameter.Statistics_RecvNum;

	//Mark time.
		if (ConfigurationParameter.Statistics_MaxTime == 0)
		{
			ConfigurationParameter.Statistics_MinTime = Result;
			ConfigurationParameter.Statistics_MaxTime = Result;
		}
		else if (Result < ConfigurationParameter.Statistics_MinTime)
		{
			ConfigurationParameter.Statistics_MinTime = Result;
		}
		else if (Result > ConfigurationParameter.Statistics_MaxTime)
		{
			ConfigurationParameter.Statistics_MaxTime = Result;
		}
	}
	else { //SOCKET_ERROR
		std::wstring Message(L"Receive error: %d(%d), waiting ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		Message.append(L"%Lf ms.\n");
	#endif
		fwprintf_s(stderr, Message.c_str(), (int)DataLength, WSAGetLastError(), Result);
		if (ConfigurationParameter.OutputFile != nullptr)
			fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), (int)DataLength, WSAGetLastError(), Result);
	}

//Transmission interval
	if (!LastSend)
	{
	#if defined(PLATFORM_WIN)
		if (ConfigurationParameter.TransmissionInterval != 0 && ConfigurationParameter.TransmissionInterval > Result)
			Sleep((DWORD)(ConfigurationParameter.TransmissionInterval - Result));
		else if (Result <= STANDARD_TIME_OUT)
			Sleep(STANDARD_TIME_OUT);
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		if (ConfigurationParameter.TransmissionInterval != 0 && ConfigurationParameter.TransmissionInterval > Result)
			usleep(ConfigurationParameter.TransmissionInterval - Result);
		else if (Result <= STANDARD_TIME_OUT)
			usleep(STANDARD_TIME_OUT);
	#endif
	}

	shutdown(Socket_Normal, SD_BOTH);
	shutdown(Socket_SOCKS, SD_BOTH);
	closesocket(Socket_Normal);
	closesocket(Socket_SOCKS);
	return true;
}

//SOCKS UDP-ASSOCIATE process
size_t SOCKS_UDP_ASSOCIATE(
	sockaddr_storage &SockAddr_SOCKS_UDP, 
	const uint16_t SOCKS_Local_Port, 
	const SOCKET Socket_Normal, 
	const PSOCKADDR SockAddr, 
	const socklen_t AddrLen)
{
//Connect to SOCKS server.
	if (connect(Socket_Normal, SockAddr, AddrLen) == SOCKET_ERROR)
	{
		PrintErrorToScreen(L"[Error] SOCKS server connecting error", WSAGetLastError());
		return EXIT_FAILURE;
	}

//Make SOCKS client selection packet and send.
	std::shared_ptr<uint8_t> Buffer(new uint8_t[LARGE_PACKET_MAXSIZE]());
	memset(Buffer.get(), 0, LARGE_PACKET_MAXSIZE);
	((psocks_client_selection)Buffer.get())->Version = SOCKS_VERSION_5;
	((psocks_client_selection)Buffer.get())->Methods_Number = SOCKS_METHOD_SUPPORT_NUM;
	((psocks_client_selection)Buffer.get())->Methods_A = SOCKS_METHOD_NO_AUTHENTICATION_REQUIRED;
	((psocks_client_selection)Buffer.get())->Methods_B = SOCKS_METHOD_USERNAME_PASSWORD;
	if (send(Socket_Normal, (const char *)Buffer.get(), sizeof(socks_client_selection), 0) == SOCKET_ERROR)
	{
		PrintErrorToScreen(L"[Error] Send packet error", WSAGetLastError());
		return EXIT_FAILURE;
	}
	else {
		memset(Buffer.get(), 0, LARGE_PACKET_MAXSIZE);
	}

//Receive SOCKS server selection.
	size_t DataLength = 0;
	ssize_t Result = recv(Socket_Normal, (char *)Buffer.get(), LARGE_PACKET_MAXSIZE, 0);
	if (Result > 0)
	{
		if (((psocks_server_selection)Buffer.get())->Version != SOCKS_VERSION_5)
		{
			PrintErrorToScreen(L"[Error] SOCKS server selection error", 0);
			return EXIT_FAILURE;
		}
		else if (((psocks_server_selection)Buffer.get())->Method != SOCKS_METHOD_NO_AUTHENTICATION_REQUIRED)
		{
			if (((psocks_server_selection)Buffer.get())->Method == SOCKS_METHOD_USERNAME_PASSWORD)
			{
			//Username and password authentication
				memset(Buffer.get(), 0, LARGE_PACKET_MAXSIZE);
				*((uint8_t *)Buffer.get()) = SOCKS_USERNAME_PASSWORD_VERSION;
				DataLength += sizeof(uint8_t);
				*((uint8_t *)(Buffer.get() + DataLength)) = (uint8_t)ConfigurationParameter.SOCKS_Username.length();
				DataLength += sizeof(uint8_t);
				memcpy_s(Buffer.get() + DataLength, LARGE_PACKET_MAXSIZE - DataLength, ConfigurationParameter.SOCKS_Username.c_str(), ConfigurationParameter.SOCKS_Username.length());
				DataLength += ConfigurationParameter.SOCKS_Username.length();
				if (ConfigurationParameter.SOCKS_Password.length() > 0)
				{
					*((uint8_t *)(Buffer.get() + DataLength)) = (uint8_t)ConfigurationParameter.SOCKS_Password.length();
					DataLength += sizeof(uint8_t);
					memcpy_s(Buffer.get() + DataLength, LARGE_PACKET_MAXSIZE - DataLength, ConfigurationParameter.SOCKS_Password.c_str(), ConfigurationParameter.SOCKS_Password.length());
					DataLength += ConfigurationParameter.SOCKS_Password.length();
				}

			//Send authentication request.
				if (send(Socket_Normal, (const char *)Buffer.get(), (int)DataLength, 0) == SOCKET_ERROR)
				{
					PrintErrorToScreen(L"[Error] Send packet error", WSAGetLastError());
					return EXIT_FAILURE;
				}
				else {
					Result = recv(Socket_Normal, (char *)Buffer.get(), LARGE_PACKET_MAXSIZE, 0);
				}
				if (Result > 0)
				{
					if (((psocks_server_user_authentication)Buffer.get())->Version != SOCKS_USERNAME_PASSWORD_VERSION || 
						((psocks_server_user_authentication)Buffer.get())->Status != SOCKS_USERNAME_PASSWORD_SUCCESS)
					{
						PrintErrorToScreen(L"[Error] SOCKS server authentication error", ((psocks_server_user_authentication)Buffer.get())->Status);
						return EXIT_FAILURE;
					}
				}
				else {
					PrintErrorToScreen(L"[Error] SOCKS server receive error", WSAGetLastError());
					return EXIT_FAILURE;
				}
			}
			else {
				PrintErrorToScreen(L"[Error] SOCKS server require a not support authentication", 0);
				return EXIT_FAILURE;
			}
		}

		memset(Buffer.get(), 0, LARGE_PACKET_MAXSIZE);
	}
	else {
		PrintErrorToScreen(L"[Error] SOCKS server receive error", WSAGetLastError());
		return EXIT_FAILURE;
	}

//Send UDP-ASSOCIATE request
	DataLength = sizeof(socks5_client_command_request);
	((psocks5_client_command_request)Buffer.get())->Version = SOCKS_VERSION_5;
	((psocks5_client_command_request)Buffer.get())->Command = SOCKS_COMMAND_UDP_ASSOCIATE;
	if (ConfigurationParameter.SockAddr_Normal.ss_family == AF_INET6) //IPv6
	{
		((psocks5_client_command_request)Buffer.get())->Address_Type = SOCKS5_ADDRESS_IPV6;
	//Local listening address in UDP ASSOCIATE command request is not necessary.
//		memcpy_s(Buffer.get() + DataLength, LARGE_PACKET_MAXSIZE - DataLength, &((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_addr, sizeof(((PSOCKADDR_IN6)&ConfigurationParameter.SockAddr_Normal)->sin6_addr));
		DataLength += sizeof(in6_addr);
		*((uint16_t *)(Buffer.get() + DataLength)) = SOCKS_Local_Port;
		DataLength += sizeof(uint16_t);
	}
	else { //IPv4 and domain
		((psocks5_client_command_request)Buffer.get())->Address_Type = SOCKS5_ADDRESS_IPV4;
	//Local listening address in UDP ASSOCIATE command request is not necessary.
//		memcpy_s(Buffer.get() + DataLength, LARGE_PACKET_MAXSIZE - DataLength, &((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_addr, sizeof(((PSOCKADDR_IN)&ConfigurationParameter.SockAddr_Normal)->sin_addr));
		DataLength += sizeof(in_addr);
		*((uint16_t *)(Buffer.get() + DataLength)) = SOCKS_Local_Port;
		DataLength += sizeof(uint16_t);
	}
	if (send(Socket_Normal, (const char *)Buffer.get(), (int)DataLength, 0) == SOCKET_ERROR)
	{
		PrintErrorToScreen(L"[Error] Send packet error", WSAGetLastError());
		return EXIT_FAILURE;
	}
	else {
		memset(Buffer.get(), 0, LARGE_PACKET_MAXSIZE);
		DataLength = 0;
	}

//SOCKS server reply message
	Result = recv(Socket_Normal, (char *)Buffer.get(), LARGE_PACKET_MAXSIZE, 0);
	if (Result > 0)
	{
		if (((psocks5_server_command_reply)Buffer.get())->Version != SOCKS_VERSION_5 || ((psocks5_server_command_reply)Buffer.get())->Reply != SOCKS5_REPLY_SUCCESS)
		{
			PrintErrorToScreen(L"[Error] SOCKS server reply message error", 0);
			return EXIT_FAILURE;
		}
		else {
		//Mark server connection messages.
			if (SockAddr_SOCKS_UDP.ss_family == AF_INET6 && ((psocks5_server_command_reply)Buffer.get())->Bind_Address_Type == SOCKS5_ADDRESS_IPV6)
			{
//				auto Addr = *(in6_addr *)(Buffer.get() + sizeof(socks5_server_command_reply));
				auto Port = *(uint16_t *)(Buffer.get() + sizeof(socks5_server_command_reply) + sizeof(in6_addr));
//				if (!CheckEmptyBuffer(&Addr, sizeof(Addr)))
//					((PSOCKADDR_IN6)&SockAddr_SOCKS_UDP)->sin6_addr = Addr;
				((PSOCKADDR_IN6)&SockAddr_SOCKS_UDP)->sin6_addr = ((PSOCKADDR_IN6)SockAddr)->sin6_addr;
				if (Port > 0)
					((PSOCKADDR_IN6)&SockAddr_SOCKS_UDP)->sin6_port = Port;
			}
			else if (SockAddr_SOCKS_UDP.ss_family == AF_INET && ((psocks5_server_command_reply)Buffer.get())->Bind_Address_Type == SOCKS5_ADDRESS_IPV4)
			{
//				auto Addr = *(in_addr *)(Buffer.get() + sizeof(socks5_server_command_reply));
				auto Port = *(uint16_t *)(Buffer.get() + sizeof(socks5_server_command_reply) + sizeof(in_addr));
//				if (Addr.s_addr > 0)
//					((PSOCKADDR_IN)&SockAddr_SOCKS_UDP)->sin_addr.s_addr = Addr.s_addr;
				((PSOCKADDR_IN)&SockAddr_SOCKS_UDP)->sin_addr.s_addr = ((PSOCKADDR_IN)SockAddr)->sin_addr.s_addr;
				if (Port > 0)
					((PSOCKADDR_IN)&SockAddr_SOCKS_UDP)->sin_port = Port;
			}
			else {
				PrintErrorToScreen(L"[Error] SOCKS server reply message error", 0);
				return EXIT_FAILURE;
			}
		}
	}
	else {
		PrintErrorToScreen(L"[Error] SOCKS server receive error", WSAGetLastError());
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

//Print statistics to screen(and/or output result to file)
void PrintProcess(
	const bool IsPacketStatistics, 
	const bool IsTimeStatistics)
{
//Packet statistics
	if (IsPacketStatistics)
	{
		fwprintf_s(stderr, L"\nPacket statistics for pinging %ls:\n", ConfigurationParameter.wTargetString.c_str());
		fwprintf_s(stderr, L"   Send: %u\n", (unsigned int)ConfigurationParameter.Statistics_RealSend);
		fwprintf_s(stderr, L"   Receive: %u\n", (unsigned int)ConfigurationParameter.Statistics_RecvNum);

	//Output to file.
		if (ConfigurationParameter.OutputFile != nullptr)
		{
			fwprintf_s(ConfigurationParameter.OutputFile, L"\nPacket statistics for pinging %ls:\n", ConfigurationParameter.wTargetString.c_str());
			fwprintf_s(ConfigurationParameter.OutputFile, L"   Send: %u\n", (unsigned int)ConfigurationParameter.Statistics_RealSend);
			fwprintf_s(ConfigurationParameter.OutputFile, L"   Receive: %u\n", (unsigned int)ConfigurationParameter.Statistics_RecvNum);
		}

		if (ConfigurationParameter.Statistics_RealSend >= ConfigurationParameter.Statistics_RecvNum)
		{
			fwprintf_s(stderr, L"   Lost: %u", (unsigned int)(ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum));
			if (ConfigurationParameter.Statistics_RealSend > 0)
				fwprintf_s(stderr, L" (%u%%)\n", (unsigned int)((ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum) * 100 / ConfigurationParameter.Statistics_RealSend));
			else  //Not any packets.
				fwprintf_s(stderr, L"\n");

		//Output to file.
			if (ConfigurationParameter.OutputFile != nullptr)
			{
				fwprintf_s(ConfigurationParameter.OutputFile, L"   Lost: %u", (unsigned int)(ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum));
				if (ConfigurationParameter.Statistics_RealSend > 0)
					fwprintf_s(ConfigurationParameter.OutputFile, L" (%u%%)\n", (unsigned int)((ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum) * 100 / ConfigurationParameter.Statistics_RealSend));
				else  //Not any packets.
					fwprintf_s(ConfigurationParameter.OutputFile, L"\n");
			}
		}
		else {
			fwprintf_s(stderr, L"   Lost: 0 (0%%)\n");

		//Output to file.
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, L"   Lost: 0 (0%%)\n");
		}
	}

//Time statistics
	if (IsTimeStatistics && 
		ConfigurationParameter.Statistics_RecvNum > 0 && ConfigurationParameter.Statistics_MaxTime > 0 && ConfigurationParameter.Statistics_MinTime > 0)
	{
		std::wstring Message(L"\nTime statistics for pinging %ls:\n");
		Message.append(L"   Minimum time: ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		Message.append(L"%Lf ms.\n");
	#endif
		Message.append(L"   Maximum time: ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		Message.append(L"%Lf ms.\n");
	#endif
		Message.append(L"   Average time: ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		Message.append(L"%Lf ms.\n");
	#endif
		fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ConfigurationParameter.Statistics_MinTime, ConfigurationParameter.Statistics_MaxTime, ConfigurationParameter.Statistics_TotalTime / (long double)ConfigurationParameter.Statistics_RecvNum);
		if (ConfigurationParameter.OutputFile != nullptr)
			fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.wTargetString.c_str(), ConfigurationParameter.Statistics_MinTime, ConfigurationParameter.Statistics_MaxTime, ConfigurationParameter.Statistics_TotalTime / (long double)ConfigurationParameter.Statistics_RecvNum);
	}

#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	fwprintf_s(stderr, L"\n");
	if (ConfigurationParameter.OutputFile != nullptr)
		fwprintf_s(ConfigurationParameter.OutputFile, L"\n");
#endif

	return;
}

//Output result to file
bool OutputResultToFile(
	void)
{
	ssize_t SignedResult = 0;

#if defined(PLATFORM_WIN)
	SignedResult = _wfopen_s(&ConfigurationParameter.OutputFile, ConfigurationParameter.wOutputFileName.c_str(), L"a,ccs=UTF-8");
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	errno = 0;
	ConfigurationParameter.OutputFile = fopen(ConfigurationParameter.OutputFileName.c_str(), ("a"));
#endif
	if (ConfigurationParameter.OutputFile == nullptr)
	{
	#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
		SignedResult = errno;
	#endif
		std::wstring Message(L"[Error] Create output result file ");
		Message.append(ConfigurationParameter.wOutputFileName);
		Message.append(L" error");
		PrintErrorToScreen(Message.c_str(), SignedResult);

		return false;
	}
	else {
		tm TimeStructure;
		time_t TimeValues = 0;
		memset(&TimeStructure, 0, sizeof(TimeStructure));

	//Get current time.
		time(&TimeValues);
		localtime_s(&TimeStructure, &TimeValues);
		fwprintf_s(
			ConfigurationParameter.OutputFile, 
			L"------------------------------ %d-%02d-%02d %02d:%02d:%02d ------------------------------\n", 
			TimeStructure.tm_year + 1900, 
			TimeStructure.tm_mon + 1, 
			TimeStructure.tm_mday, 
			TimeStructure.tm_hour, 
			TimeStructure.tm_min, 
			TimeStructure.tm_sec);
	}

	return true;
}

//Print header to screen. 
void PrintHeaderToScreen(
	const std::wstring wTargetAddressString, 
	const std::wstring wTestDomain)
{
	fwprintf_s(stderr, L"\n");

//Reverse lookup
	if (ConfigurationParameter.IsReverseLookup && ConfigurationParameter.SockAddr_SOCKS.ss_family == 0)
	{
		if (wTargetAddressString.empty())
		{
			uint8_t FQDN[NI_MAXHOST + 1U] = {0};
			if (getnameinfo((PSOCKADDR)&ConfigurationParameter.SockAddr_Normal, sizeof(sockaddr_in), (char *)FQDN, NI_MAXHOST, nullptr, 0, NI_NUMERICSERV) != 0)
			{
				PrintErrorToScreen(L"[Error] Resolve addresses to host names error", WSAGetLastError());
				fwprintf_s(stderr, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), wTestDomain.c_str());
				if (ConfigurationParameter.OutputFile != nullptr)
					fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), wTestDomain.c_str());
			}
			else {
				if (ConfigurationParameter.TargetString_Normal == (const char *)FQDN)
				{
					fwprintf_s(stderr, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), wTestDomain.c_str());
					if (ConfigurationParameter.OutputFile != nullptr)
						fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), wTestDomain.c_str());
				}
				else {
					std::wstring wFQDN;
					if (!MBSToWCSString(FQDN, strnlen((const char *)FQDN, NI_MAXHOST), wFQDN))
					{
						PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
						return;
					}

					fwprintf_s(stderr, L"DNSPing %ls:%u [%ls] with %ls:\n", wFQDN.c_str(), ntohs(ConfigurationParameter.ServiceType), ConfigurationParameter.wTargetString.c_str(), wTestDomain.c_str());
					if (ConfigurationParameter.OutputFile != nullptr)
						fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u [%ls] with %ls:\n", wFQDN.c_str(), ntohs(ConfigurationParameter.ServiceType), ConfigurationParameter.wTargetString.c_str(), wTestDomain.c_str());
				}
			}
		}
		else {
			fwprintf_s(stderr, L"DNSPing %ls:%u [%ls] with %ls:\n", wTargetAddressString.c_str(), ntohs(ConfigurationParameter.ServiceType), ConfigurationParameter.wTargetString.c_str(), wTestDomain.c_str());
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u [%ls] with %ls:\n", wTargetAddressString.c_str(), ntohs(ConfigurationParameter.ServiceType), ConfigurationParameter.wTargetString.c_str(), wTestDomain.c_str());
		}
	}
//Normal mode
	else {
		if (!ConfigurationParameter.TargetAddressString.empty())
		{
			fwprintf_s(stderr, L"DNSPing %ls:%u [%ls] with %ls:\n", wTargetAddressString.c_str(), ntohs(ConfigurationParameter.ServiceType), ConfigurationParameter.wTargetString.c_str(), wTestDomain.c_str());
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u [%ls] with %ls:\n", wTargetAddressString.c_str(), ntohs(ConfigurationParameter.ServiceType), ConfigurationParameter.wTargetString.c_str(), wTestDomain.c_str());
		}
		else {
			fwprintf_s(stderr, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), wTestDomain.c_str());
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.wTargetString.c_str(), ntohs(ConfigurationParameter.ServiceType), wTestDomain.c_str());
		}
	}

	return;
}

//Print more details about error code
void ErrorCodeToMessage(
	const ssize_t ErrorCode, 
	std::wstring &Message)
{
//Finish the message when there are no error codes.
	if (ErrorCode == 0)
	{
		Message.append(L".\n");
		return;
	}

//Convert error code to error message.
#if defined(PLATFORM_WIN)
	wchar_t *InnerMessage = nullptr;
	if (FormatMessageW(
		FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM|FORMAT_MESSAGE_IGNORE_INSERTS|FORMAT_MESSAGE_MAX_WIDTH_MASK, 
		nullptr, 
		(DWORD)ErrorCode, 
		MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
		(LPWSTR)&InnerMessage, 
		0, 
		nullptr) == 0)
	{
		Message.append(L", error code is %d");
	}
	else {
		Message.append(L": ");
		Message.append(InnerMessage);
		Message.pop_back(); //Delete space.
		Message.pop_back(); //Delete period.
		Message.append(L"[%d]");
	}

//Free pointer.
	if (InnerMessage != nullptr)
		LocalFree(InnerMessage);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	std::wstring InnerMessage;
	auto ErrorMessage = strerror((int)ErrorCode);
	if (ErrorMessage == nullptr || !MBSToWCSString((const uint8_t *)ErrorMessage, strnlen(ErrorMessage, FILE_BUFFER_SIZE), InnerMessage))
	{
		Message.append(L", error code is %d");
	}
	else {
		Message.append(L": ");
		Message.append(InnerMessage);
		Message.append(L"[%d]");
	}
#endif

	Message.append(L".\n");
	return;
}

//Print errors to screen
void PrintErrorToScreen(
	const wchar_t *Message, 
	const ssize_t ErrorCode)
{
	std::wstring InnerMessage(Message);
	ErrorCodeToMessage(ErrorCode, InnerMessage);
	if (ErrorCode == 0)
		fwprintf_s(stderr, InnerMessage.c_str());
	else
		fwprintf_s(stderr, InnerMessage.c_str(), (int)ErrorCode);

	return;
}

//Print description to screen
void PrintDescription(
	void)
{
//Description
	fwprintf_s(stderr, L"\n--------------------------------------------------\n");
	fwprintf_s(stderr, L"DNSPing ");
	fwprintf_s(stderr, FULL_VERSION);
#if defined(PLATFORM_WIN)
	fwprintf_s(stderr, L"(Windows)\n");
#elif defined(PLATFORM_OPENWRT)
	fwprintf(stderr, L"(OpenWrt)\n");
#elif defined(PLATFORM_LINUX)
	fwprintf(stderr, L"(Linux)\n");
#elif defined(PLATFORM_MACX)
	fwprintf(stderr, L"(Mac)\n");
#endif
	fwprintf_s(stderr, L"A useful and powerful toolkit(DNSPing)\n");
	fwprintf_s(stderr, COPYRIGHT_MESSAGE);
	fwprintf_s(stderr, L"\n--------------------------------------------------\n");

//Usage
	fwprintf_s(stderr, L"       DNSPing [-options] domain target\n");
	fwprintf_s(stderr, L"  e.g. DNSPing -a -qt AAAA -n 5 -w 500 -edns www.google.com 8.8.4.4\n");

//Supported options:
	fwprintf_s(stderr, L"\nSupported options:\n");
	fwprintf_s(stderr, L"   ?/-h              Description.\n");
	fwprintf_s(stderr, L"   -t                Pings the specified host until stopped.\n");
	fwprintf_s(stderr, L"                     To see statistics and continue type Control-Break.\n");
	fwprintf_s(stderr, L"                     To stop type Control-C.\n");
	fwprintf_s(stderr, L"   -a                Resolve addresses to host names.\n");
	fwprintf_s(stderr, L"   -n count          Set number of echo requests to send.\n");
	fwprintf_s(stderr, L"                     Count must between 1 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -f                Set the \"Do Not Fragment\" flag in outgoing packets(IPv4).\n");
	fwprintf_s(stderr, L"                     No available in Mac OS X.\n");
	fwprintf_s(stderr, L"   -i hoplimit/ttl   Specifie a Hop Limit or Time To Live for outgoing packets.\n");
	fwprintf_s(stderr, L"                     HopLimit/TTL must between 1 - 255.\n");
	fwprintf_s(stderr, L"   -w timeout        Set a long wait periods (in milliseconds) for a response.\n");
	fwprintf_s(stderr, L"                     Timeout must between 500 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -id dns_id        Specifie DNS header ID.\n");
	fwprintf_s(stderr, L"                     DNS ID must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -qr               Set DNS header QR flag.\n");
	fwprintf_s(stderr, L"   -opcode opcode    Specifie DNS header OPCode.\n");
	fwprintf_s(stderr, L"                     OPCode must between 0x0000 - 0x00FF/255.\n");
	fwprintf_s(stderr, L"   -aa               Set DNS header AA flag.\n");
	fwprintf_s(stderr, L"   -tc               Set DNS header TC flag.\n");
	fwprintf_s(stderr, L"   -rd               Set DNS header RD flag.\n");
	fwprintf_s(stderr, L"   -ra               Set DNS header RA flag.\n");
	fwprintf_s(stderr, L"   -ad               Set DNS header AD flag.\n");
	fwprintf_s(stderr, L"   -cd               Set DNS header CD flag.\n");
	fwprintf_s(stderr, L"   -rcode rcode      Specifie DNS header RCode.\n");
	fwprintf_s(stderr, L"                     RCode must between 0x0000 - 0x00FF/255.\n");
	fwprintf_s(stderr, L"   -qn count         Specifie DNS header Question count.\n");
	fwprintf_s(stderr, L"                     Question count must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -ann count        Specifie DNS header Answer count.\n");
	fwprintf_s(stderr, L"                     Answer count must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -aun count        Specifie DNS header Authority count.\n");
	fwprintf_s(stderr, L"                     Authority count must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -adn count        Specifie DNS header Additional count.\n");
	fwprintf_s(stderr, L"                     Additional count must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -ti interval_time Specifie transmission interval time(in milliseconds).\n");
	fwprintf_s(stderr, L"   -edns             Send with EDNS Label.\n");
	fwprintf_s(stderr, L"   -payload length   Specifie EDNS Label UDP Payload length.\n");
	fwprintf_s(stderr, L"                     Payload length must between 512 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"   -dnssec           Send with DNSSEC request.\n");
	fwprintf_s(stderr, L"                     EDNS Label will enable when DNSSEC is enable.\n");
	fwprintf_s(stderr, L"   -qt type          Specifie Query type.\n");
	fwprintf_s(stderr, L"                     Query type must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"                     Type: A|NS|MD|MF|CNAME|SOA|MB|MG|MR|NULL|WKS|PTR|HINFO|\n");
	fwprintf_s(stderr, L"                           MINFO|MX|TXT|RP|AFSDB|X25|ISDN|RT|NSAP|NSAPPTR|\n");
	fwprintf_s(stderr, L"                           SIG|KEY|PX|GPOS|AAAA|LOC|NXT|EID|NIMLOC|SRV|ATMA|\n");
	fwprintf_s(stderr, L"                           NAPTR|KX|A6|CERT|DNAME|SINK|OPT|APL|DS|SSHFP|\n");
	fwprintf_s(stderr, L"                           IPSECKEY|RRSIG|NSEC|DNSKEY|DHCID|NSEC3|NSEC3PARAM|\n");
	fwprintf_s(stderr, L"                           TLSA|HIP|NINFO|RKEY|TALINK|CDS|CDNSKEY|OPENPGPKEY|\n");
	fwprintf_s(stderr, L"                           SPF|UINFO|UID|GID|UNSPEC|NID|L32|L64|LP|EUI48|\n");
	fwprintf_s(stderr, L"                           EUI64|TKEY|TSIG|IXFR|AXFR|MAILB|MAILA|ANY|URI|\n");
	fwprintf_s(stderr, L"                           CAA|TA|DLV|RESERVED\n");
	fwprintf_s(stderr, L"   -qc classes       Specifie Query classes.\n");
	fwprintf_s(stderr, L"                     Query classes must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"                     Classes: IN|CSNET|CHAOS|HESIOD|NONE|ALL|ANY\n");
	fwprintf_s(stderr, L"   -p type           Specifie UDP port number/protocol(Sevice name).\n");
	fwprintf_s(stderr, L"                     UDP port must between 0x0001 - 0xFFFF/65535.\n");
	fwprintf_s(stderr, L"                     Protocol: TCPMUX|ECHO|DISCARD|SYSTAT|DAYTIME|NETSTAT|\n");
	fwprintf_s(stderr, L"                               QOTD|MSP|CHARGEN|FTP|SSH|TELNET|SMTP|\n");
	fwprintf_s(stderr, L"                               TIME|RAP|RLP|NAME|WHOIS|TACACS|DNS|XNSAUTH|MTP|\n");
	fwprintf_s(stderr, L"                               BOOTPS|BOOTPC|TFTP|RJE|FINGER|TTYLINK|SUPDUP|\n");
	fwprintf_s(stderr, L"                               SUNRPC|SQL|NTP|EPMAP|NETBIOSNS|NETBIOSDGM|\n");
	fwprintf_s(stderr, L"                               NETBIOSSSN|IMAP|BFTP|SGMP|SQLSRV|DMSP|SNMP|\n");
	fwprintf_s(stderr, L"                               SNMPTRAP|ATRTMP|ATHBP|QMTP|IPX|IMAP|IMAP3|\n");
	fwprintf_s(stderr, L"                               BGMP|TSP|IMMP|ODMR|RPC2PORTMAP|CLEARCASE|\n");
	fwprintf_s(stderr, L"                               HPALARMMGR|ARNS|AURP|LDAP|UPS|SLP|SNPP|\n");
	fwprintf_s(stderr, L"                               MICROSOFTDS|KPASSWD|TCPNETHASPSRV|RETROSPECT|\n");
	fwprintf_s(stderr, L"                               ISAKMP|BIFFUDP|WHOSERVER|SYSLOG|ROUTERSERVER|\n");
	fwprintf_s(stderr, L"                               NCP|COURIER|COMMERCE|RTSP|NNTP|HTTPRPCEPMAP|\n");
	fwprintf_s(stderr, L"                               IPP|LDAPS|MSDP|AODV|FTPSDATA|FTPS|NAS|TELNETS\n");
	fwprintf_s(stderr, L"   -rawdata raw_data Specifie Raw data to send.\n");
	fwprintf_s(stderr, L"                     RAW_Data is hex, but do not add \"0x\" before hex.\n");
	fwprintf_s(stderr, L"                     Length of RAW_Data must between 64 - 1500 bytes.\n");
	fwprintf_s(stderr, L"   -raw type         Specifie Raw socket type.\n");
	fwprintf_s(stderr, L"                     Service Name: HOPOPTS|ICMP|IGMP|GGP|IPV4|ST|TCP|CBT|EGP|\n");
	fwprintf_s(stderr, L"                                   IGP|BBNRCCMON|NVPII|PUP|ARGUS|EMCON|XNET|\n");
	fwprintf_s(stderr, L"                                   CHAOS|MUX|DCN|HMP|PRM|IDP|TRUNK_1|TRUNK_2\n");
	fwprintf_s(stderr, L"                                   LEAF_1|LEAF_2|RDP|IRTP|ISOTP4|MFE|MERIT|\n");
	fwprintf_s(stderr, L"                                   DCCP|3PC|IDPR|XTP|DDP|IDPRCMTP|TP++|IL|\n");
	fwprintf_s(stderr, L"                                   IPV6|SDRP|ROUTING|FRAGMENT|IDRP|RSVP|GRE|\n");
	fwprintf_s(stderr, L"                                   DSR|BNA|ESP|AH|NLSP|SWIPE|NARP|MOBILE|TLSP|\n");
	fwprintf_s(stderr, L"                                   SKIP|ICMPV6|NONE|DSTOPTS|AHI|CFTP|ALN|SAT|\n");
	fwprintf_s(stderr, L"                                   KRYPTOLAN|RVD|IPPC|ADF|SATMON|VISA|IPCV|\n");
	fwprintf_s(stderr, L"                                   CPNX|CPHB|WSN|PVP|BR|ND|ICLFXBM|WBEXPAK|\n");
	fwprintf_s(stderr, L"                                   ISO|VMTP|SVMTP|VINES|TTP|IPTM|NSFNET|DGP|\n");
	fwprintf_s(stderr, L"                                   TCF|EIGRP|SPRITE|LARP|MTP|AX25|IPIP|MICP|\n");
	fwprintf_s(stderr, L"                                   SCC|ETHERIP|ENCAP|APES|GMTP|IFMP|PNNI|PIM|\n");
	fwprintf_s(stderr, L"                                   ARIS|SCPS|QNX|AN|IPCOMP|SNP|COMPAQ|IPX|PGM|\n");
	fwprintf_s(stderr, L"                                   0HOP|L2TP|DDX|IATP|STP|SRP|UTI|SMP|SM|\n");
	fwprintf_s(stderr, L"                                   PTP|ISIS|FIRE|CRTP|CRUDP|SSCOPMCE|IPLT|\n");
	fwprintf_s(stderr, L"                                   SPS|PIPE|SCTP|FC|RSVPE2E|MOBILITY|UDPLITE|\n");
	fwprintf_s(stderr, L"                                   MPLS|MANET|HIP|SHIM6|WESP|ROHC|TEST-1|\n");
	fwprintf_s(stderr, L"                                   TEST-2|RAW\n");
	fwprintf_s(stderr, L"   -socks target            Specifie target of SOCKS server.\n");
	fwprintf_s(stderr, L"                            Target is Server:Port, like [::1]:1080.\n");
	fwprintf_s(stderr, L"   -socks_username username Specifie username of SOCKS server.\n");
	fwprintf_s(stderr, L"                            Length of SOCKS username must between 1 - 255 bytes.\n");
	fwprintf_s(stderr, L"   -socks_password password Specifie password of SOCKS server.\n");
	fwprintf_s(stderr, L"                            Length of SOCKS password must between 1 - 255 bytes.\n");
	fwprintf_s(stderr, L"   -buf size                Specifie receive buffer size.\n");
	fwprintf_s(stderr, L"                            Buffer size must between 512 - 4096 bytes.\n");
	fwprintf_s(stderr, L"   -dv                      Disable packets validated.\n");
	fwprintf_s(stderr, L"   -show type               Show result or hex data of responses.\n");
	fwprintf_s(stderr, L"                            Response: Result|Hex\n");
	fwprintf_s(stderr, L"   -of file_name            Output result to file.\n");
	fwprintf_s(stderr, L"                            FileName must less than 260 bytes.\n");
	fwprintf_s(stderr, L"   -6                       Using IPv6.\n");
	fwprintf_s(stderr, L"   -4                       Using IPv4.\n");
	fwprintf_s(stderr, L"   domain                   A domain name which will make request to send\n");
	fwprintf_s(stderr, L"                            to DNS server.\n");
	fwprintf_s(stderr, L"   target                   Target of DNSPing, support IPv4/IPv6 address and\n");
	fwprintf_s(stderr, L"                            domain.\n");

#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	fwprintf_s(stderr, L"\n");
#endif
	return;
}
