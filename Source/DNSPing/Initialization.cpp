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

//GlobalStatus class constructor
ConfigurationTable::ConfigurationTable(
	void)
{
//Global status
	OutputFile = nullptr;
#if defined(PLATFORM_WIN)
	IsWinSockInitialized = false;
#endif

//C-Syle type parameter block
	TransmissionInterval = 0;
	BufferSize = PACKET_MAXSIZE;
	RawDataLen = 0;
	EDNSPayloadSize = 0;
	Protocol = 0;
	ServiceType = 0;
	IsReverseLookup = false;
	IsRawSocket = false;
	IsEDNS = false;
	IsDNSSEC = false;
	IsValidate = true;
	IsShowResponse = false;
	IsShowHexResponse = false;
#if (defined(PLATFORM_WIN) || defined(PLATFORM_LINUX))
	IsDoNotFragment = false;
#endif
#if defined(PLATFORM_WIN)
	PacketHopLimits = 0;
	SocketTimeout = DEFAULT_TIME_OUT;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	PacketHopLimits = 0;
	SocketTimeout.tv_sec = DEFAULT_TIME_OUT;
#endif
	memset(&SockAddr_Normal, 0, sizeof(SockAddr_Normal));
	memset(&SockAddr_SOCKS, 0, sizeof(SockAddr_SOCKS));
	memset(&Parameter_Header, 0, sizeof(Parameter_Header));
	memset(&Parameter_Query, 0, sizeof(Parameter_Query));
	memset(&Parameter_EDNS, 0, sizeof(Parameter_EDNS));

//C-Syle type result block
	Statistics_Send = DEFAULT_SEND_TIMES;
	Statistics_RealSend = 0;
	Statistics_RecvNum = 0;
	Statistics_TotalTime = 0;
	Statistics_MaxTime = 0;
	Statistics_MinTime = 0;

	return;
}

//GlobalStatus class destructor
ConfigurationTable::~ConfigurationTable(
	void)
{
//Close all file handles and WinSock cleanup.
#if defined(PLATFORM_WIN)
	_fcloseall();
	if (IsWinSockInitialized)
		WSACleanup();
#elif (defined(PLATFORM_LINUX) && !defined(PLATFORM_OPENWRT))
	fcloseall();
#endif

	return;
}
