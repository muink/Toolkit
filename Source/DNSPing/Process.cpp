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


#include "Process.h"

//Send process
bool SendRequestProcess(
	const sockaddr_storage &Target, 
	const bool IsLastSend)
{
//Initialization(Part 1)
	SOCKET Socket_Normal = INVALID_SOCKET;
	socklen_t AddrLen_Normal = 0;

//IPv6
	if (ConfigurationParameter.Protocol == AF_INET6)
	{
	//Socket initialization
		AddrLen_Normal = sizeof(sockaddr_in6);
		if (ConfigurationParameter.IsRawSocket && ConfigurationParameter.RawDataBuffer)
			Socket_Normal = socket(AF_INET6, SOCK_RAW, ConfigurationParameter.ServiceType);
		else 
			Socket_Normal = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);

	//Socket check
		if (Socket_Normal == INVALID_SOCKET || Socket_Normal == SOCKET_ERROR)
		{
			PrintErrorToScreen(L"[Error] Socket initialization error", WSAGetLastError());
			return false;
		}
	}
//IPv4
	else {
	//Socket initialization
		AddrLen_Normal = sizeof(sockaddr_in);
		if (ConfigurationParameter.IsRawSocket && ConfigurationParameter.RawDataBuffer)
			Socket_Normal = socket(AF_INET, SOCK_RAW, ConfigurationParameter.ServiceType);
		else 
			Socket_Normal = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	//Socket check
		if (Socket_Normal == INVALID_SOCKET || Socket_Normal == SOCKET_ERROR)
		{
			PrintErrorToScreen(L"[Error] Socket initialization error", WSAGetLastError());
			return false;
		}
	}

//Set socket timeout.
#if defined(PLATFORM_WIN)
	if (setsockopt(Socket_Normal, SOL_SOCKET, SO_SNDTIMEO, reinterpret_cast<const char *>(&ConfigurationParameter.SocketTimeout), sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR || 
		setsockopt(Socket_Normal, SOL_SOCKET, SO_RCVTIMEO, reinterpret_cast<const char *>(&ConfigurationParameter.SocketTimeout), sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR)
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	if (setsockopt(Socket_Normal, SOL_SOCKET, SO_SNDTIMEO, &ConfigurationParameter.SocketTimeout, sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR || 
		setsockopt(Socket_Normal, SOL_SOCKET, SO_RCVTIMEO, &ConfigurationParameter.SocketTimeout, sizeof(ConfigurationParameter.SocketTimeout)) == SOCKET_ERROR)
#endif
	{
		PrintErrorToScreen(L"[Error] Set UDP socket timeout error", WSAGetLastError());

		shutdown(Socket_Normal, SD_BOTH);
		closesocket(Socket_Normal);
		return false;
	}

//Set Internet Protocol header options.
	if (ConfigurationParameter.Protocol == AF_INET6)
	{
		if (ConfigurationParameter.PacketHopLimits != 0 && 
		#if defined(PLATFORM_WIN)
			setsockopt(Socket_Normal, IPPROTO_IPV6, IPV6_UNICAST_HOPS, reinterpret_cast<const char *>(&ConfigurationParameter.PacketHopLimits), sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			setsockopt(Socket_Normal, IPPROTO_IPV6, IPV6_UNICAST_HOPS, &ConfigurationParameter.PacketHopLimits, sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
		#endif
		{
			PrintErrorToScreen(L"[Error] Set HopLimit flag error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			closesocket(Socket_Normal);
			return false;
		}
	}
	else { //IPv4
		if (ConfigurationParameter.PacketHopLimits != 0 && 
		#if defined(PLATFORM_WIN)
			setsockopt(Socket_Normal, IPPROTO_IP, IP_TTL, reinterpret_cast<const char *>(&ConfigurationParameter.PacketHopLimits), sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			setsockopt(Socket_Normal, IPPROTO_IP, IP_TTL, &ConfigurationParameter.PacketHopLimits, sizeof(ConfigurationParameter.PacketHopLimits)) == SOCKET_ERROR)
		#endif
		{
			PrintErrorToScreen(L"[Error] Set TTL flag error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			closesocket(Socket_Normal);
			return false;
		}

//Set Do Not Fragment flag.
	#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_WIN))
	#if defined(PLATFORM_WIN)
		BOOL DoNotFragment = TRUE;
		if (ConfigurationParameter.IsDoNotFragment && setsockopt(Socket_Normal, IPPROTO_IP, IP_DONTFRAGMENT, reinterpret_cast<const char *>(&DoNotFragment), sizeof(DoNotFragment)) == SOCKET_ERROR)
	#elif defined(PLATFORM_FREEBSD)
		int DoNotFragment = 1;
		if (ConfigurationParameter.IsDoNotFragment && setsockopt(Socket_Normal, IPPROTO_IP, IP_DONTFRAG, &DoNotFragment, sizeof(DoNotFragment)) == SOCKET_ERROR)
	#elif defined(PLATFORM_LINUX)
		int DoNotFragment = IP_PMTUDISC_DO;
		if (ConfigurationParameter.IsDoNotFragment && setsockopt(Socket_Normal, IPPROTO_IP, IP_MTU_DISCOVER, &DoNotFragment, sizeof(DoNotFragment)) == SOCKET_ERROR)
	#endif
		{
			PrintErrorToScreen(L"[Error] Set Do Not Fragment flag error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			closesocket(Socket_Normal);
			return false;
		}
	#endif
	}

//Initialization(Part 2)
	auto SendBuffer = std::make_unique<uint8_t[]>(ConfigurationParameter.BufferSize + MEMORY_RESERVED_BYTES);
	auto RecvBuffer = std::make_unique<uint8_t[]>(ConfigurationParameter.BufferSize + MEMORY_RESERVED_BYTES);
	memset(SendBuffer.get(), 0, ConfigurationParameter.BufferSize + MEMORY_RESERVED_BYTES);
	memset(RecvBuffer.get(), 0, ConfigurationParameter.BufferSize + MEMORY_RESERVED_BYTES);
#if defined(PLATFORM_WIN)
	LARGE_INTEGER CPU_Frequency, BeforeTime, AfterTime;
	memset(&CPU_Frequency, 0, sizeof(CPU_Frequency));
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
	timespec BeforeTime, AfterTime;
#elif defined(PLATFORM_MACOS)
	uint64_t BeforeTime = 0, AfterTime = 0;
#endif
	memset(&BeforeTime, 0, sizeof(BeforeTime));
	memset(&AfterTime, 0, sizeof(AfterTime));
	ssize_t DataLength = 0;

//Make standard packet.
	dns_hdr *DNS_Header = nullptr;
	if (ConfigurationParameter.RawDataBuffer)
	{
		if (ConfigurationParameter.BufferSize >= ConfigurationParameter.RawDataLen)
		{
			memcpy_s(SendBuffer.get(), ConfigurationParameter.BufferSize, ConfigurationParameter.RawDataBuffer.get(), ConfigurationParameter.RawDataLen);
			DataLength = ConfigurationParameter.RawDataLen;
		}
		else {
			memcpy_s(SendBuffer.get(), ConfigurationParameter.BufferSize, ConfigurationParameter.RawDataBuffer.get(), ConfigurationParameter.BufferSize);
			DataLength = ConfigurationParameter.BufferSize;
		}
	}
	else {
	//DNS packet
		memcpy_s(SendBuffer.get() + DataLength, ConfigurationParameter.BufferSize - DataLength, &ConfigurationParameter.Parameter_Header, sizeof(ConfigurationParameter.Parameter_Header));
		if (ConfigurationParameter.Parameter_Header.ID == 0)
		{
			DNS_Header = reinterpret_cast<dns_hdr *>(SendBuffer.get() + DataLength);
		#if defined(PLATFORM_WIN)
			DNS_Header->ID = hton16(static_cast<const uint16_t>(GetCurrentProcessId()));
		#elif defined(PLATFORM_LINUX)
			DNS_Header->ID = hton16(static_cast<const uint16_t>(pthread_self()));
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_MACOS))
			DNS_Header->ID = hton16(*reinterpret_cast<const uint16_t *>(pthread_self()));
		#endif
		}
		DataLength += sizeof(dns_hdr);
		DataLength += StringToPacketQuery(reinterpret_cast<const uint8_t *>(ConfigurationParameter.TestDomainString.c_str()), SendBuffer.get() + DataLength);
		memcpy_s(SendBuffer.get() + DataLength, ConfigurationParameter.BufferSize - DataLength, &ConfigurationParameter.Parameter_Query, sizeof(ConfigurationParameter.Parameter_Query));
		DataLength += sizeof(dns_qry);
		if (ConfigurationParameter.IsEDNS)
		{
			memcpy_s(SendBuffer.get() + DataLength, ConfigurationParameter.BufferSize - DataLength, &ConfigurationParameter.Parameter_EDNS, sizeof(ConfigurationParameter.Parameter_EDNS));
			DataLength += sizeof(dns_record_opt);
		}
	}

//Socket connecting
	if (connect(Socket_Normal, reinterpret_cast<const sockaddr *>(&Target), AddrLen_Normal) == SOCKET_ERROR)
	{
		PrintErrorToScreen(L"[Error] Connecting error", WSAGetLastError());

		shutdown(Socket_Normal, SD_BOTH);
		closesocket(Socket_Normal);
		return false;
	}

//Mark start time.
#if defined(PLATFORM_WIN)
	if (!MarkProcessTime(false, CPU_Frequency, BeforeTime, AfterTime))
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	if (!MarkProcessTime(false, BeforeTime, AfterTime))
#endif
	{
		shutdown(Socket_Normal, SD_BOTH);
		closesocket(Socket_Normal);
		return false;
	}

//Send request.
	ssize_t TotalSendSize = DataLength;
	DataLength = 0;
	for (;;)
	{
		ssize_t SendLength = send(Socket_Normal, reinterpret_cast<const char *>(SendBuffer.get() + DataLength), static_cast<const int>(TotalSendSize - DataLength), 0);
		if (SendLength == SOCKET_ERROR || SendLength == 0) //SOCKET_ERROR or connection closed
		{
			PrintErrorToScreen(L"[Error] Send packet error", WSAGetLastError());

			shutdown(Socket_Normal, SD_BOTH);
			closesocket(Socket_Normal);
			return false;
		}
		else {
			DataLength += SendLength;
			if (DataLength == TotalSendSize) //Send completed.
			{
				break;
			}
			else if (DataLength > TotalSendSize) //SOCKET_ERROR
			{
				PrintErrorToScreen(L"[Error] Send packet error", 0);

				shutdown(Socket_Normal, SD_BOTH);
				closesocket(Socket_Normal);
				return false;
			}
		}
	}

//Receive response and mark end time.
#if defined(PLATFORM_WIN)
	DataLength = recv(Socket_Normal, reinterpret_cast<char *>(RecvBuffer.get()), static_cast<const int>(ConfigurationParameter.BufferSize), 0);
	if (!MarkProcessTime(true, CPU_Frequency, BeforeTime, AfterTime))
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	DataLength = recv(Socket_Normal, RecvBuffer.get(), ConfigurationParameter.BufferSize, 0);
	if (!MarkProcessTime(true, BeforeTime, AfterTime))
#endif
	{
		shutdown(Socket_Normal, SD_BOTH);
		closesocket(Socket_Normal);
		return false;
	}

//Get waiting time.
#if defined(PLATFORM_WIN)
	auto ResultValue = ResultTimeCalculator(CPU_Frequency, BeforeTime, AfterTime);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	auto ResultValue = ResultTimeCalculator(BeforeTime, AfterTime);
#endif
	if (ResultValue == 0)
	{
		PrintErrorToScreen(L"[Error] Interval time calculating error", 0);

		shutdown(Socket_Normal, SD_BOTH);
		closesocket(Socket_Normal);
		return false;
	}

//Print result to screen.
	auto IsContinue = true;
	if (DataLength >= static_cast<const ssize_t>(DNS_PACKET_MINSIZE))
	{
	//Print send result.
	#if defined(PLATFORM_WIN)
		if (!PrintSendResult(Socket_Normal, DNS_Header, RecvBuffer, DataLength, ResultValue, IsContinue, CPU_Frequency, BeforeTime, AfterTime))
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		if (!PrintSendResult(Socket_Normal, DNS_Header, RecvBuffer, DataLength, ResultValue, IsContinue, BeforeTime, AfterTime))
	#endif
			return false;

	//Continue flag, break current process.
		if (!IsContinue)
			return true;
	}
	else { //SOCKET_ERROR
		std::wstring Message(L"Receive error: %d(%d), waiting ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Message.append(L"%Lf ms.\n");
	#endif
		fwprintf_s(stderr, Message.c_str(), static_cast<const int>(DataLength), WSAGetLastError(), ResultValue);
		if (ConfigurationParameter.OutputFile != nullptr)
			fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), static_cast<const int>(DataLength), WSAGetLastError(), ResultValue);
	}

//Transmission interval
	if (!IsLastSend)
	{
		if (ConfigurationParameter.TransmissionInterval != 0 && ConfigurationParameter.TransmissionInterval > ResultValue)
		#if defined(PLATFORM_WIN)
			Sleep(static_cast<const DWORD>(ConfigurationParameter.TransmissionInterval - ResultValue));
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			usleep(ConfigurationParameter.TransmissionInterval - ResultValue);
		#endif
		else if (ResultValue <= STANDARD_TIMEOUT)
		#if defined(PLATFORM_WIN)
			Sleep(STANDARD_TIMEOUT);
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			usleep(STANDARD_TIMEOUT);
		#endif
	}

	shutdown(Socket_Normal, SD_BOTH);
	closesocket(Socket_Normal);
	return true;
}

//Print result to screen
bool PrintSendResult(
	const SOCKET Socket_Normal, 
	const dns_hdr * const DNS_Header, 
	std::unique_ptr<uint8_t[]> &RecvBuffer, 
	ssize_t &DataLength, 
	long double &ResultValue, 
	bool &IsContinue, 
#if defined(PLATFORM_WIN)
	LARGE_INTEGER &CPU_Frequency, 
	LARGE_INTEGER &BeforeTime, 
	LARGE_INTEGER &AfterTime
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
	timespec &BeforeTime, 
	timespec &AfterTime
#elif defined(PLATFORM_MACOS)
	uint64_t &BeforeTime, 
	uint64_t &AfterTime
#endif
)
{
	IsContinue = false;

//Validate packet.
	if (ConfigurationParameter.IsValidated && DNS_Header != nullptr && !ValidatePacket(RecvBuffer.get(), DataLength, DNS_Header->ID))
	{
		std::wstring Message(L"Receive from %ls:%u -> %d bytes but validate error, waiting ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Message.append(L"%Lf ms.\n");
	#endif
		fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);
		if (ConfigurationParameter.OutputFile != nullptr)
			fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);

	//Try to waiting correct packet.
		for (;;)
		{
		//Timeout
		#if defined(PLATFORM_WIN)
			if (ResultValue >= ConfigurationParameter.SocketTimeout)
				break;
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			if (ResultValue >= ConfigurationParameter.SocketTimeout.tv_usec / MICROSECOND_TO_MILLISECOND + ConfigurationParameter.SocketTimeout.tv_sec * SECOND_TO_MILLISECOND)
				break;
		#endif

		//Receive response and mark end time.
			memset(RecvBuffer.get(), 0, ConfigurationParameter.BufferSize);
		#if defined(PLATFORM_WIN)
			DataLength = recv(Socket_Normal, reinterpret_cast<char *>(RecvBuffer.get()), static_cast<const int>(ConfigurationParameter.BufferSize), 0);
			if (!MarkProcessTime(true, CPU_Frequency, BeforeTime, AfterTime))
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			DataLength = recv(Socket_Normal, RecvBuffer.get(), ConfigurationParameter.BufferSize, 0);
			if (!MarkProcessTime(true, BeforeTime, AfterTime))
		#endif
			{
				shutdown(Socket_Normal, SD_BOTH);
				closesocket(Socket_Normal);
				return false;
			}

		//Get waiting time.
		#if defined(PLATFORM_WIN)
			ResultValue = ResultTimeCalculator(CPU_Frequency, BeforeTime, AfterTime);
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			ResultValue = ResultTimeCalculator(BeforeTime, AfterTime);
		#endif
			if (ResultValue == 0)
			{
				PrintErrorToScreen(L"[Error] Interval time calculating error", 0);

				shutdown(Socket_Normal, SD_BOTH);
				closesocket(Socket_Normal);
				return false;
			}

		//SOCKET_ERROR
			if (DataLength <= 0)
				break;

		//Validate packet.
			if (!ValidatePacket(RecvBuffer.get(), DataLength, DNS_Header->ID))
			{
				Message = (L"Receive from %ls:%u -> %d bytes but validate error, waiting ");
			#if defined(PLATFORM_WIN)
				Message.append(L"%lf ms.\n");
			#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
				Message.append(L"%Lf ms.\n");
			#endif
				fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);
				if (ConfigurationParameter.OutputFile != nullptr)
					fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);
			}
			else {
				break;
			}
		}

	//Receive error.
		if (DataLength <= 0)
		{
			Message = (L"Receive error: %d(%d), waiting correct answers timeout(");
		#if defined(PLATFORM_WIN)
			Message.append(L"%lf ms).\n");
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			Message.append(L"%Lf ms).\n");
		#endif
			fwprintf_s(stderr, Message.c_str(), static_cast<const int>(DataLength), WSAGetLastError(), ResultValue);
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), static_cast<const int>(DataLength), WSAGetLastError(), ResultValue);

			shutdown(Socket_Normal, SD_BOTH);
			closesocket(Socket_Normal);
			return true;
		}
		else {
			Message = (L"Receive from %ls:%u -> %d bytes, waiting ");
		#if defined(PLATFORM_WIN)
			Message.append(L"%lf ms.\n");
		#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
			Message.append(L"%Lf ms.\n");
		#endif
			fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);
		}
	}
	else {
		std::wstring Message(L"Receive from %ls:%u -> %d bytes, waiting ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Message.append(L"%Lf ms.\n");
	#endif
		fwprintf_s(stderr, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);
		if (ConfigurationParameter.OutputFile != nullptr)
			fwprintf_s(ConfigurationParameter.OutputFile, Message.c_str(), ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), static_cast<const int>(DataLength), ResultValue);
	}

//Print response result or data.
	if (ConfigurationParameter.IsShowResponse)
	{
		PrintFormattedResponse(stderr, RecvBuffer.get(), DataLength);
		if (ConfigurationParameter.OutputFile != nullptr)
			PrintFormattedResponse(ConfigurationParameter.OutputFile, RecvBuffer.get(), DataLength);
	}
	if (ConfigurationParameter.IsShowHexResponse)
	{
		PrintHexResponse(stderr, RecvBuffer.get(), DataLength);
		if (ConfigurationParameter.OutputFile != nullptr)
			PrintHexResponse(ConfigurationParameter.OutputFile, RecvBuffer.get(), DataLength);
	}

//Calculate time.
	ConfigurationParameter.Statistics_TotalTime += ResultValue;
	++ConfigurationParameter.Statistics_RecvNum;

//Mark time.
	if (ConfigurationParameter.Statistics_MaxTime == 0)
	{
		ConfigurationParameter.Statistics_MinTime = ResultValue;
		ConfigurationParameter.Statistics_MaxTime = ResultValue;
	}
	else if (ResultValue < ConfigurationParameter.Statistics_MinTime)
	{
		ConfigurationParameter.Statistics_MinTime = ResultValue;
	}
	else if (ResultValue > ConfigurationParameter.Statistics_MaxTime)
	{
		ConfigurationParameter.Statistics_MaxTime = ResultValue;
	}

	IsContinue = true;
	return true;
}

//Mark process time
bool MarkProcessTime(
	const bool IsFinished, 
#if defined(PLATFORM_WIN)
	LARGE_INTEGER &CPU_Frequency, 
	LARGE_INTEGER &BeforeTime, 
	LARGE_INTEGER &AfterTime
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
	timespec &BeforeTime, 
	timespec &AfterTime
#elif defined(PLATFORM_MACOS)
	uint64_t &BeforeTime, 
	uint64_t &AfterTime
#endif
)
{
//Mark start time.
	if (!IsFinished)
	{
	#if defined(PLATFORM_WIN)
		if (QueryPerformanceFrequency(
				&CPU_Frequency) == 0 || 
			QueryPerformanceCounter(
				&BeforeTime) == 0)
		{
			PrintErrorToScreen(L"[Error] Get current time from High Precision Event Timer/HPET error", GetLastError());
			return false;
		}
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
		if (clock_gettime(CLOCK_MONOTONIC, &BeforeTime) != 0)
		{
			PrintErrorToScreen(L"[Error] Get current time error", errno);
			return false;
		}
	#elif defined(PLATFORM_MACOS)
	//Convert to nanoseconds.
		mach_timebase_info_data_t TimeBaseInfo;
		memset(&TimeBaseInfo, 0, sizeof(TimeBaseInfo));
		if (mach_timebase_info(&TimeBaseInfo) != KERN_SUCCESS)
		{
			PrintErrorToScreen(L"[Error] Get current time error", errno);
			return false;
		}

	//Mark time.
		BeforeTime = mach_absolute_time();
		if (BeforeTime == 0)
		{
			PrintErrorToScreen(L"[Error] Get current time error", errno);
			return false;
		}
		else {
			BeforeTime = BeforeTime * TimeBaseInfo.numer / TimeBaseInfo.denom;
		}
	#endif
	}
//Mark end time.
	else {
	#if defined(PLATFORM_WIN)
		if (QueryPerformanceCounter(
				&AfterTime) == 0)
		{
			PrintErrorToScreen(L"[Error] Get current time from High Precision Event Timer/HPET error", GetLastError());
			return false;
		}
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
		if (clock_gettime(CLOCK_MONOTONIC, &AfterTime) != 0)
		{
			PrintErrorToScreen(L"[Error] Get current time error", errno);
			return false;
		}
	#elif defined(PLATFORM_MACOS)
	//Mark time.
		AfterTime = mach_absolute_time();
		if (AfterTime == 0)
		{
			PrintErrorToScreen(L"[Error] Get current time error", errno);
			return false;
		}

	//Convert to nanoseconds.
		mach_timebase_info_data_t TimeBaseInfo;
		memset(&TimeBaseInfo, 0, sizeof(TimeBaseInfo));
		if (mach_timebase_info(&TimeBaseInfo) != KERN_SUCCESS)
		{
			PrintErrorToScreen(L"[Error] Get current time error", errno);
			return false;
		}
		else {
			AfterTime = AfterTime * TimeBaseInfo.numer / TimeBaseInfo.denom;
		}
	#endif
	}

	return true;
}

//Calculate all process result time
long double ResultTimeCalculator(
#if defined(PLATFORM_WIN)
	const LARGE_INTEGER CPU_Frequency, 
	const LARGE_INTEGER BeforeTime, 
	const LARGE_INTEGER AfterTime)
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
	const timespec BeforeTime, 
	const timespec AfterTime)
#elif defined(PLATFORM_MACOS)
	const uint64_t BeforeTime, 
	const uint64_t AfterTime)
#endif
{
#if defined(PLATFORM_WIN)
	if (AfterTime.QuadPart >= BeforeTime.QuadPart)
		return static_cast<const long double>((AfterTime.QuadPart - BeforeTime.QuadPart) * static_cast<const LONGLONG>(MICROSECOND_TO_MILLISECOND)) / static_cast<const long double>(CPU_Frequency.QuadPart);
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX))
	if (AfterTime.tv_sec >= BeforeTime.tv_sec)
		return static_cast<const long double>((AfterTime.tv_sec - BeforeTime.tv_sec) * static_cast<const time_t>(SECOND_TO_MILLISECOND)) + static_cast<const long double>(AfterTime.tv_nsec - BeforeTime.tv_nsec) / static_cast<const long double>(NANOSECOND_TO_MICROSECOND * MICROSECOND_TO_MILLISECOND);
#elif defined(PLATFORM_MACOS)
	if (AfterTime >= BeforeTime)
		return static_cast<const long double>(AfterTime - BeforeTime) / static_cast<const long double>(NANOSECOND_TO_MICROSECOND * MICROSECOND_TO_MILLISECOND);
#endif

	return 0;
}

//Print statistics to screen and/or output result to file
void PrintProcess(
	const bool IsPacketStatistics, 
	const bool IsTimeStatistics)
{
//Packet statistics
	if (IsPacketStatistics)
	{
		fwprintf_s(stderr, L"\nPacket statistics for pinging %ls:\n", ConfigurationParameter.WideTargetString.c_str());
		fwprintf_s(stderr, L"   Send: %u\n", static_cast<const unsigned int>(ConfigurationParameter.Statistics_RealSend));
		fwprintf_s(stderr, L"   Receive: %u\n", static_cast<const unsigned int>(ConfigurationParameter.Statistics_RecvNum));

	//Output to file.
		if (ConfigurationParameter.OutputFile != nullptr)
		{
			fwprintf_s(ConfigurationParameter.OutputFile, L"\nPacket statistics for pinging %ls:\n", ConfigurationParameter.WideTargetString.c_str());
			fwprintf_s(ConfigurationParameter.OutputFile, L"   Send: %u\n", static_cast<const unsigned int>(ConfigurationParameter.Statistics_RealSend));
			fwprintf_s(ConfigurationParameter.OutputFile, L"   Receive: %u\n", static_cast<const unsigned int>(ConfigurationParameter.Statistics_RecvNum));
		}

		if (ConfigurationParameter.Statistics_RealSend >= ConfigurationParameter.Statistics_RecvNum)
		{
			fwprintf_s(stderr, L"   Lost: %u", static_cast<const unsigned int>(ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum));
			if (ConfigurationParameter.Statistics_RealSend > 0)
				fwprintf_s(stderr, L" (%u%%)\n", static_cast<const unsigned int>((ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum) * 100 / ConfigurationParameter.Statistics_RealSend));
			else  //Not any packets.
				fwprintf_s(stderr, L"\n");

		//Output to file.
			if (ConfigurationParameter.OutputFile != nullptr)
			{
				fwprintf_s(ConfigurationParameter.OutputFile, L"   Lost: %u", static_cast<const unsigned int>(ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum));
				if (ConfigurationParameter.Statistics_RealSend > 0)
					fwprintf_s(ConfigurationParameter.OutputFile, L" (%u%%)\n", static_cast<const unsigned int>((ConfigurationParameter.Statistics_RealSend - ConfigurationParameter.Statistics_RecvNum) * 100 / ConfigurationParameter.Statistics_RealSend));
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
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Message.append(L"%Lf ms.\n");
	#endif
		Message.append(L"   Maximum time: ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Message.append(L"%Lf ms.\n");
	#endif
		Message.append(L"   Average time: ");
	#if defined(PLATFORM_WIN)
		Message.append(L"%lf ms.\n");
	#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		Message.append(L"%Lf ms.\n");
	#endif
		fwprintf_s(
			stderr, 
			Message.c_str(), 
			ConfigurationParameter.WideTargetString.c_str(), 
			ConfigurationParameter.Statistics_MinTime, 
			ConfigurationParameter.Statistics_MaxTime, 
			ConfigurationParameter.Statistics_TotalTime / static_cast<const long double>(ConfigurationParameter.Statistics_RecvNum));
		if (ConfigurationParameter.OutputFile != nullptr)
			fwprintf_s(
				ConfigurationParameter.OutputFile, 
				Message.c_str(), 
				ConfigurationParameter.WideTargetString.c_str(), 
				ConfigurationParameter.Statistics_MinTime, 
				ConfigurationParameter.Statistics_MaxTime, 
				ConfigurationParameter.Statistics_TotalTime / static_cast<const long double>(ConfigurationParameter.Statistics_RecvNum));
	}

#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
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
	SignedResult = _wfopen_s(&ConfigurationParameter.OutputFile, ConfigurationParameter.WideOutputFileName.c_str(), L"a,ccs=UTF-8");
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	errno = 0;
	ConfigurationParameter.OutputFile = fopen(ConfigurationParameter.OutputFileName.c_str(), "a");
#endif
	if (ConfigurationParameter.OutputFile == nullptr)
	{
	#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
		SignedResult = errno;
	#endif
		std::wstring Message(L"[Error] Create output result file ");
		Message.append(ConfigurationParameter.WideOutputFileName);
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
	const std::wstring WideTargetAddressString, 
	const std::wstring WideTestDomainString)
{
	fwprintf_s(stderr, L"\n");

//Reverse lookup
	if (ConfigurationParameter.IsReverseLookup)
	{
		if (WideTargetAddressString.empty())
		{
			auto FQDN_String = std::make_unique<uint8_t[]>(NI_MAXHOST + MEMORY_RESERVED_BYTES);
			memset(FQDN_String.get(), 0, NI_MAXHOST + MEMORY_RESERVED_BYTES);
			if (getnameinfo(reinterpret_cast<const sockaddr *>(&ConfigurationParameter.SockAddr_Normal), sizeof(sockaddr_in), reinterpret_cast<char *>(FQDN_String.get()), NI_MAXHOST, nullptr, 0, NI_NUMERICSERV) != 0)
			{
				PrintErrorToScreen(L"[Error] Resolve addresses to host names error", WSAGetLastError());
				fwprintf_s(stderr, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), WideTestDomainString.c_str());
				if (ConfigurationParameter.OutputFile != nullptr)
					fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), WideTestDomainString.c_str());
			}
			else {
				if (ConfigurationParameter.TargetString_Normal == reinterpret_cast<const char *>(FQDN_String.get()))
				{
					fwprintf_s(stderr, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), WideTestDomainString.c_str());
					if (ConfigurationParameter.OutputFile != nullptr)
						fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), WideTestDomainString.c_str());
				}
				else {
					std::wstring Wide_FQDN;
					if (!MBS_To_WCS_String(FQDN_String.get(), strnlen_s(reinterpret_cast<const char *>(FQDN_String.get()), NI_MAXHOST), Wide_FQDN))
					{
						PrintErrorToScreen(L"\n[Error] Convert multiple byte or wide char string error", GetLastError());
						return;
					}
					else {
						fwprintf_s(stderr, L"DNSPing %ls:%u [%ls] with %ls:\n", Wide_FQDN.c_str(), ntoh16(ConfigurationParameter.ServiceType), ConfigurationParameter.WideTargetString.c_str(), WideTestDomainString.c_str());
						if (ConfigurationParameter.OutputFile != nullptr)
							fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u [%ls] with %ls:\n", Wide_FQDN.c_str(), ntoh16(ConfigurationParameter.ServiceType), ConfigurationParameter.WideTargetString.c_str(), WideTestDomainString.c_str());
					}
				}
			}
		}
		else {
			fwprintf_s(stderr, L"DNSPing %ls:%u [%ls] with %ls:\n", WideTargetAddressString.c_str(), ntoh16(ConfigurationParameter.ServiceType), ConfigurationParameter.WideTargetString.c_str(), WideTestDomainString.c_str());
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u [%ls] with %ls:\n", WideTargetAddressString.c_str(), ntoh16(ConfigurationParameter.ServiceType), ConfigurationParameter.WideTargetString.c_str(), WideTestDomainString.c_str());
		}
	}
//Normal mode
	else {
		if (!ConfigurationParameter.TargetAddressString.empty())
		{
			fwprintf_s(stderr, L"DNSPing %ls:%u [%ls] with %ls:\n", WideTargetAddressString.c_str(), ntoh16(ConfigurationParameter.ServiceType), ConfigurationParameter.WideTargetString.c_str(), WideTestDomainString.c_str());
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u [%ls] with %ls:\n", WideTargetAddressString.c_str(), ntoh16(ConfigurationParameter.ServiceType), ConfigurationParameter.WideTargetString.c_str(), WideTestDomainString.c_str());
		}
		else {
			fwprintf_s(stderr, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), WideTestDomainString.c_str());
			if (ConfigurationParameter.OutputFile != nullptr)
				fwprintf_s(ConfigurationParameter.OutputFile, L"DNSPing %ls:%u with %ls:\n", ConfigurationParameter.WideTargetString.c_str(), ntoh16(ConfigurationParameter.ServiceType), WideTestDomainString.c_str());
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
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK, 
			nullptr, 
			static_cast<const DWORD>(ErrorCode), 
			MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), 
			reinterpret_cast<const LPWSTR>(&InnerMessage), 
			0, 
			nullptr) == 0)
	{
		Message.append(L", error code is %d");

	//Free pointer.
		if (InnerMessage != nullptr)
		{
			LocalFree(InnerMessage);
			InnerMessage = nullptr;
		}
	}
	else {
		Message.append(L": ");
		Message.append(InnerMessage);
		if (Message.back() == ASCII_SPACE)
			Message.pop_back(); //Remove space.
		if (Message.back() == ASCII_PERIOD)
			Message.pop_back(); //Remove period.
		Message.append(L"[%d]");

	//Free pointer.
		LocalFree(InnerMessage);
		InnerMessage = nullptr;
	}
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::wstring InnerMessage;
	const auto ErrorMessage = strerror(static_cast<const int>(ErrorCode));
	if (ErrorMessage == nullptr || !MBS_To_WCS_String(reinterpret_cast<const uint8_t *>(ErrorMessage), strnlen(ErrorMessage, FILE_BUFFER_SIZE), InnerMessage))
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
	const wchar_t * const Message, 
	const ssize_t ErrorCode)
{
	std::wstring InnerMessage(Message);
	ErrorCodeToMessage(ErrorCode, InnerMessage);
	if (ErrorCode == 0)
		fwprintf_s(stderr, InnerMessage.c_str());
	else 
		fwprintf_s(stderr, InnerMessage.c_str(), static_cast<const int>(ErrorCode));

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
#if defined(PLATFORM_FREEBSD)
	fwprintf(stderr, L"(FreeBSD)\n");
#elif defined(PLATFORM_OPENWRT)
	fwprintf(stderr, L"(OpenWrt)\n");
#elif defined(PLATFORM_LINUX)
	fwprintf(stderr, L"(Linux)\n");
#elif defined(PLATFORM_MACOS)
	fwprintf(stderr, L"(macOS)\n");
#elif defined(PLATFORM_WIN)
	fwprintf_s(stderr, L"(Windows)\n");
#endif
	fwprintf_s(stderr, L"DNSPing, a useful and powerful toolkit\n");
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
#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_WIN))
	fwprintf_s(stderr, L"   -f                Set the \"Do Not Fragment\" flag in outgoing packets(IPv4).\n");
	fwprintf_s(stderr, L"                     No available in macOS.\n");
#endif
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
	fwprintf_s(stderr, L"                           EUI64|ADDRS|TKEY|TSIG|IXFR|AXFR|MAILB|MAILA|ANY|\n");
	fwprintf_s(stderr, L"                           URI|CAA|TA|DLV|RESERVED\n");
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
	fwprintf_s(stderr, L"   -buf size         Specifie receive buffer size.\n");
	fwprintf_s(stderr, L"                     Buffer size must between 512 - 4096 bytes.\n");
	fwprintf_s(stderr, L"   -dv               Disable packets validated.\n");
	fwprintf_s(stderr, L"   -show type        Show result or hex data of responses.\n");
	fwprintf_s(stderr, L"                     Response: Result|Hex\n");
	fwprintf_s(stderr, L"   -of file_name     Output result to file.\n");
	fwprintf_s(stderr, L"   -6                Using IPv6.\n");
	fwprintf_s(stderr, L"   -4                Using IPv4.\n");
	fwprintf_s(stderr, L"   domain            A domain name which will make request to send to DNS server.\n");
	fwprintf_s(stderr, L"   target            Target, support IPv4/IPv6 address and domain.\n");

#if (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	fwprintf_s(stderr, L"\n");
#endif
	return;
}
