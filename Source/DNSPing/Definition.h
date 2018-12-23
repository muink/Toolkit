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


#ifndef TOOLKIT_DNSPING_DEFINITION_H
#define TOOLKIT_DNSPING_DEFINITION_H

#include "Structure.h"

//////////////////////////////////////////////////
// Base definitions
// 
//Character value definitions
#define ASCII_SPACE                                   32                   //" "
#define ASCII_MINUS                                   45                   //"-"
#define ASCII_PERIOD                                  46                   //"."
#define ASCII_SLASH                                   47                   //"/"
#define ASCII_ZERO                                    48                   //"0"
#define ASCII_NINE                                    57                   //"9"
#define ASCII_COLON                                   58                   //":"
#define ASCII_AT                                      64                   //"@"
#define ASCII_UPPERCASE_A                             65                   //"A"
#define ASCII_UPPERCASE_F                             70                   //"F"
#define ASCII_BRACKETS_LEFT                           91                   //"["
#define ASCII_BRACKETS_RIGHT                          93                   //"]"
#define ASCII_ACCENT                                  96                   //"`"
#define ASCII_LOWERCASE_A                             97                   //"a"
#define ASCII_LOWERCASE_F                             102                  //"f"
#define ASCII_LOWERCASE_X                             120                  //"x"
#define ASCII_BRACES_LEAD                             123                  //"{"
#define ASCII_TILDE                                   126                  //"~"


//////////////////////////////////////////////////
// Main definitions
// 
//Code definitions
#define COMMAND_MIN_COUNT                             2
#if defined(PLATFORM_WIN)
	#define MBSTOWCS_NULL_TERMINATE                       (-1)                        //MultiByteToWideChar function null-terminate
	#define WCSTOMBS_NULL_TERMINATE                       MBSTOWCS_NULL_TERMINATE     //WideCharToMultiByte function null-terminate
#endif
#define DEFAULT_SEND_TIMES                            4U                          //Default send times
#define HIGHEST_BIT_U16                               0x7FFF                      //Get highest bit in uint16_t/16 bits structure.
#define HIGHEST_MOVE_BIT_U16                          15U                         //Move 15 bits to get highest bit in uint16_t/16 bits structure.
#define LOWEST_MOVE_BIT_U16                           11U                         //Move 11 bits to make highest bit to lowest bits in uint16_t/16 bits structure.
#define NUM_HEX                                       16
#define U16_NUM_ONE                                   0x0001
#define UINT4_MAX                                     0x000F

//Time definitions
#if defined(PLATFORM_WIN)
	#define DEFAULT_TIMEOUT                               2000U                       //Default timeout, 2000 ms(2 seconds)
	#define STANDARD_TIMEOUT                              1000U                       //Standard timeout(1000 ms/1 second)
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define DEFAULT_TIMEOUT                               2U                          //Default timeout, 2 seconds
	#define STANDARD_TIMEOUT                              1000000U                    //Standard timeout, 1000000 us(1000 ms or 1 second)
#endif
#define MICROSECOND_TO_MILLISECOND                    1000U                       //1000 microseconds(1 millisecond)
#define NANOSECOND_TO_MICROSECOND                     1000U                       //1000 nanoseconds(1 microsecond)
#define SECONDS_IN_DAY                                86400U                      //86400 seconds in a day
#define SECONDS_IN_HOUR                               3600U                       //3600 seconds in an hour
#define SECONDS_IN_MINUTE                             60U                         //60 seconds in a minute
#define SECONDS_IN_MONTH                              2592000U                    //2592000 seconds in a month(30 days in a month)
#define SECONDS_IN_YEAR                               31536000U                   //31536000 seconds in a year(30 days in a month and 12 months in a year)
#define SECOND_TO_MILLISECOND                         1000U                       //1000 milliseconds(1 second)
#if defined(PLATFORM_WIN)
	#define SOCKET_MIN_TIME_INTERVAL                      1
#endif
#define SOCKET_MIN_TIMEOUT                            500U                        //The shortest socket timeout(500 ms)

//Size and length definitions
#define ADDRESS_STRING_MAXSIZE                        64U                                                           //Maximum size of addresses(IPv4/IPv6) words(64 bytes)
#define BYTES_TO_BITS                                 8U
#define DEFAULT_LARGE_BUFFER_SIZE                     4096U                                                         //Default size of large buffer(4KB/4096 bytes)
#define DOMAIN_MAXSIZE                                256U                                                          //Maximum size of whole level domain is 256 bytes(Section 2.3.1 in RFC 1035).
#define DOMAIN_MINSIZE                                2U                                                            //Minimum size of whole level domain is 3 bytes(Section 2.3.1 in RFC 1035).
#define FILE_BUFFER_SIZE                              DEFAULT_LARGE_BUFFER_SIZE                                     //Maximum size of file buffer(4KB/4096 bytes)
#define IPV4_SHORTEST_ADDR_STRING                     6U                                                            //The shortest IPv4 address strings(*.*.*.*)
#define IPV4_LONGEST_ADDR_STRING                      15U                                                           //The longest IPv6 address strings(***.***.***.***)
#define IPV6_SHORTEST_ADDR_STRING                     2U                                                            //The shortest IPv6 address strings(::)
#define IPV6_LONGEST_ADDR_STRING                      46U                                                           //The longest IPv6 address strings(xxxx::xxxx::xxxx::xxxx::xxxx::xxxx::xxxx::xxxx)
#define MEMORY_RESERVED_BYTES                         2U                                                            //Memory reserved bytes(2 bytes)
#define NULL_TERMINATE_LENGTH                         1U                                                            //Length of C style string null
#define PACKET_MAXSIZE                                2048U                                                         //Maximum size of packets, 2048 bytes
#define PACKET_MINSIZE                                64U                                                           //Minimum size of packets in Ethernet network.
#define DNS_PACKET_MINSIZE                            (sizeof(dns_hdr) + NULL_TERMINATE_LENGTH + sizeof(dns_qry))   //Minimum DNS packet size(DNS Header + Minimum Domain<ROOT> + DNS Query)

//Version definitions
#define FULL_VERSION                                  L"0.4.0.1"
#define COPYRIGHT_MESSAGE                             L"Copyright (C) 2014-2019 Chengr28"

//Function definitions
#define hton16_Force(Value)                           (static_cast<const uint16_t>((reinterpret_cast<const uint8_t *>(&(Value)))[0] <<                                      \
														(sizeof(uint8_t) * BYTES_TO_BITS) | (reinterpret_cast<const uint8_t *>(&(Value)))[1U]))
#define ntoh16_Force                                  hton16_Force
#define hton16                                        htons
#define ntoh16                                        ntohs
#define hton32_Force(Value)                           (static_cast<const uint32_t>((reinterpret_cast<const uint8_t *>(&(Value)))[0] <<                                      \
														((sizeof(uint16_t) + sizeof(uint8_t)) * BYTES_TO_BITS) |                                                            \
														(reinterpret_cast<const uint8_t *>(&(Value)))[1U] << (sizeof(uint16_t) * BYTES_TO_BITS) |                           \
														(reinterpret_cast<const uint8_t *>(&(Value)))[2U] << (sizeof(uint8_t) * BYTES_TO_BITS) |                            \
														(reinterpret_cast<const uint8_t *>(&(Value)))[3U]))
#define ntoh32_Force                                  hton32_Force
#define hton32                                        htonl
#define ntoh32                                        ntohl
#define hton64_Force(Value)                           (static_cast<const uint64_t>(((static_cast<const uint64_t>(hton32(static_cast<const uint32_t>                         \
														(((Value) << (sizeof(uint32_t) * BYTES_TO_BITS)) >>                                                                 \
														(sizeof(uint32_t) * BYTES_TO_BITS))))) <<                                                                           \
														(sizeof(uint32_t) * BYTES_TO_BITS)) | static_cast<const uint32_t>(hton32(static_cast<const uint32_t>((Value) >>     \
														(sizeof(uint32_t) * BYTES_TO_BITS))))))
#define ntoh64_Force                                  hton64_Force
#if defined(PLATFORM_WIN)
	#define hton64                                        htonll
	#define ntoh64                                        ntohll
#elif (defined(PLATFORM_FREEBSD) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
#if BYTE_ORDER == LITTLE_ENDIAN
	#define hton64(Value)                                 hton64_Force(Value)
#else //BIG_ENDIAN
	#define hton64(Value)                                 (Value)
#endif
	#define ntoh64                                        hton64
#endif
#endif
