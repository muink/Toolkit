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


#ifndef TOOLKIT_DNSPING_TYPE_H
#define TOOLKIT_DNSPING_TYPE_H

#include "Definition.h"

//////////////////////////////////////////////////
// Class and structure definitions
class ConfigurationTable
{
public:
//Global status
	FILE                                 *OutputFile;
#if defined(PLATFORM_WIN)
	bool                                 IsInitialized_WinSock;
#endif

//C-Syle type parameter block
	size_t                               TransmissionInterval;
	size_t                               BufferSize;
	size_t                               RawDataLen;
	size_t                               EDNSPayloadSize;
	uint16_t                             Protocol;
	uint16_t                             ServiceType;
	bool                                 IsReverseLookup;
	bool                                 IsRawSocket;
	bool                                 IsEDNS;
	bool                                 IsDNSSEC;
	bool                                 IsValidated;
	bool                                 IsShowResponse;
	bool                                 IsShowHexResponse;
#if (defined(PLATFORM_WIN) || defined(PLATFORM_LINUX))
	bool                                 IsDoNotFragment;
#endif
#if defined(PLATFORM_WIN)
	DWORD                                PacketHopLimits;
	DWORD                                SocketTimeout;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	int                                  PacketHopLimits;
	timeval                              SocketTimeout;
#endif
	sockaddr_storage                     SockAddr_Normal;
	dns_hdr                              Parameter_Header;
	dns_qry                              Parameter_Query;
	dns_record_opt                       Parameter_EDNS;

//C-Syle type result block
	size_t                               Statistics_PlanSend;
	size_t                               Statistics_RealSend;
	size_t                               Statistics_RecvNum;
	long double                          Statistics_TotalTime;
	long double                          Statistics_MaxTime;
	long double                          Statistics_MinTime;

//C++ STL type block
	std::string                          TestDomainString;
	std::string                          TargetAddressString;
	std::string                          TargetString_Normal;
#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string                          OutputFileName;
#endif
	std::wstring                         WideTargetString;
	std::wstring                         WideOutputFileName;
	std::unique_ptr<uint8_t[]>           RawDataBuffer;

//Redefine operator functions
//	ConfigurationTable() = default;
	ConfigurationTable(const ConfigurationTable &) = delete;
	ConfigurationTable & operator=(const ConfigurationTable &) = delete;

//Member functions
	ConfigurationTable(
		void);
	~ConfigurationTable(
		void);
};
#endif
