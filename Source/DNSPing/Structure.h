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


//////////////////////////////////////////////////
// Operating system
// 
/* This code is from Qt source, which is in qglobal.h header file.
// See https://www.qt.io/developers

	The operating system, must be one of: (PLATFORM_x)

	MACX       - Mac OS X
	MAC9       - Mac OS 9
	DARWIN     - Darwin OS (Without Mac OS X)
	MSDOS      - MS-DOS and Windows
	OS2        - OS/2
	OS2EMX     - XFree86 on OS/2 (not PM)
	WIN32      - Win32 (Windows 95/98/ME and Windows NT/2000/XP or newer versions)
	CYGWIN     - Cygwin
	SOLARIS    - Sun Solaris
	HPUX       - HP-UX
	ULTRIX     - DEC Ultrix
	LINUX      - Linux
	FREEBSD    - FreeBSD
	NETBSD     - NetBSD
	OPENBSD    - OpenBSD
	BSDI       - BSD/OS
	IRIX       - SGI Irix
	OSF        - HP Tru64 UNIX
	SCO        - SCO OpenServer 5
	UNIXWARE   - UnixWare 7, Open UNIX 8
	AIX        - AIX
	HURD       - GNU Hurd
	DGUX       - DG/UX
	RELIANT    - Reliant UNIX
	DYNIX      - DYNIX/ptx
	QNX        - QNX
	QNX6       - QNX RTP 6.1
	LYNX       - LynxOS
	BSD4       - Any BSD 4.4 system
	UNIX       - Any UNIX BSD/SYSV system
*/

#if defined(__DARWIN_X11__)
#  define PLATFORM_DARWIN
#elif defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__))
#  define PLATFORM_MACX
#elif defined(__MACOSX__)
#  define PLATFORM_MACX
#elif defined(macintosh)
#  define PLATFORM_MAC9
#elif defined(__CYGWIN__)
#  define PLATFORM_CYGWIN
#elif defined(MSDOS) || defined(_MSDOS)
#  define PLATFORM_MSDOS
#elif defined(__OS2__)
#  if defined(__EMX__)
#    define PLATFORM_OS2EMX
#  else
#    define PLATFORM_OS2
#  endif
#elif !defined(SAG_COM) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define PLATFORM_WIN32
#  define PLATFORM_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  define PLATFORM_WIN32
#elif defined(__MWERKS__) && defined(__INTEL__)
#  define PLATFORM_WIN32
#elif defined(__sun) || defined(sun)
#  define PLATFORM_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define PLATFORM_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define PLATFORM_ULTRIX
#elif defined(sinix)
#  define PLATFORM_RELIANT
#elif defined(__linux__) || defined(__linux)
#  define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__)
#  define PLATFORM_FREEBSD
#  define PLATFORM_BSD4
#elif defined(__NetBSD__)
#  define PLATFORM_NETBSD
#  define PLATFORM_BSD4
#elif defined(__OpenBSD__)
#  define PLATFORM_OPENBSD
#  define PLATFORM_BSD4
#elif defined(__bsdi__)
#  define PLATFORM_BSDI
#  define PLATFORM_BSD4
#elif defined(__sgi)
#  define PLATFORM_IRIX
#elif defined(__osf__)
#  define PLATFORM_OSF
#elif defined(_AIX)
#  define PLATFORM_AIX
#elif defined(__Lynx__)
#  define PLATFORM_LYNX
#elif defined(__GNU_HURD__)
#  define PLATFORM_HURD
#elif defined(__DGUX__)
#  define PLATFORM_DGUX
#elif defined(__QNXNTO__)
#  define PLATFORM_QNX6
#elif defined(__QNX__)
#  define PLATFORM_QNX
#elif defined(_SEQUENT_)
#  define PLATFORM_DYNIX
#elif defined(_SCO_DS)                   /* SCO OpenServer 5 + GCC */
#  define PLATFORM_SCO
#elif defined(__USLC__)                  /* all SCO platforms + UDK or OUDK */
#  define PLATFORM_UNIXWARE
#  define PLATFORM_UNIXWARE7
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define PLATFORM_UNIXWARE
#  define PLATFORM_UNIXWARE7
#elif defined(__MAKEDEPEND__)
#else
#  error "Qt has not been ported to this OS - talk to qt-bugs@trolltech.com"
#endif

//System series defines
#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64)
#  define PLATFORM_WIN
#endif
#if defined(PLATFORM_MAC9) || defined(PLATFORM_MACX)
#  define PLATFORM_MAC
#endif
#if defined(PLATFORM_MAC9) || defined(PLATFORM_MSDOS) || defined(PLATFORM_OS2) || defined(PLATFORM_WIN)
#  undef PLATFORM_UNIX
#elif !defined(PLATFORM_UNIX)
#  define PLATFORM_UNIX
#endif
/* Xcode support
#if defined(PLATFORM_MACX)
#  ifdef MAC_OS_X_VERSION_MIN_REQUIRED
#    undef MAC_OS_X_VERSION_MIN_REQUIRED
#  endif
#  define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_2
#  include <AvailabilityMacros.h>
#  if !defined(MAC_OS_X_VERSION_10_3)
#     define MAC_OS_X_VERSION_10_3 MAC_OS_X_VERSION_10_2 + 1
#  endif
#  if !defined(MAC_OS_X_VERSION_10_4)
#       define MAC_OS_X_VERSION_10_4 MAC_OS_X_VERSION_10_3 + 1
#  endif
#  if (MAC_OS_X_VERSION_MAX_ALLOWED > MAC_OS_X_VERSION_10_4)
#    error "This version of Mac OS X is unsupported"
#  endif
#endif
*/


//////////////////////////////////////////////////
// Base header
// 
//C Standard Library and C++ Standard Template Library/STL Headers
#include <cstring>
#include <ctime>
#include <memory>
#include <string>

#if defined(PLATFORM_WIN)
//Part 1 including files
	#include <winsock2.h>                //WinSock 2.0+

//Part 2 including files(MUST be including after Part 1)
	#include <windns.h>                  //Windows DNS definitions and DNS API
	#include <ws2tcpip.h>                //WinSock 2.0+ Extension for TCP/IP protocols

//Part 3 including files(MUST be including after Part 2)
//	#include <windows.h>                 //Master include file

//Part 4 including files(MUST be including after Part 3)
	#if !defined(PLATFORM_WIN_XP)
		#include <versionhelpers.h>          //Windows Version Helpers, minimum supported system of Windows Version Helpers is Windows Vista.
	#endif
	#pragma comment(lib, "ws2_32.lib")   //WinSock 2.0+

//Endian definitions
	#define __LITTLE_ENDIAN            1234                         //Little Endian
	#define __BIG_ENDIAN               4321                         //Big Endian
	#define __BYTE_ORDER               __LITTLE_ENDIAN              //x86 and x86-64/x64 is Little Endian in Windows.
	#define LITTLE_ENDIAN              __LITTLE_ENDIAN
	#define BIG_ENDIAN                 __BIG_ENDIAN
	#define BYTE_ORDER                 __BYTE_ORDER

//Windows compatible definitions
	typedef SSIZE_T                   ssize_t;
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
//Portable Operating System Interface/POSIX and Unix system header
	#include <cerrno>                 //Error report
	#include <cstdio>                 //File Input/Output
	#include <cstdlib>                //C Standard Library
	#include <climits>                //Limits
	#include <netdb.h>                //Network database operations
	#include <pthread.h>              //Threads
	#include <signal.h>               //Signals
	#include <unistd.h>               //Standard library API
	#include <arpa/inet.h>            //Internet operations
	#include <sys/time.h>             //Date and time

//Windows compatible
	#define FALSE                    0
	#define INVALID_SOCKET           (-1)
	#define SOCKET_ERROR             (-1)
	#define RETURN_ERROR             (-1)
	#define MAX_PATH                 PATH_MAX
	#define in_addr                  in_addr_Windows
	typedef int                      SOCKET;
	typedef sockaddr                 *PSOCKADDR;
	typedef sockaddr_in              *PSOCKADDR_IN;
	typedef sockaddr_in6             *PSOCKADDR_IN6;

//Internet Protocol version 4/IPv4 Address(Linux, from Microsoft Windows)
	typedef struct _in_addr_windows_
	{
		union {
			union {
				struct {
					uint8_t    s_b1, s_b2, s_b3, s_b4;
				}S_un_b;
				struct {
					uint16_t   s_w1, s_w2;
				}S_un_w;
				uint32_t       S_addr;
			}S_un;
			uint32_t           s_addr;
		};
	}in_addr_Windows;
	#define s_host                                  S_un.S_un_b.s_b2
	#define s_net                                   S_un.S_un_b.s_b1
	#define s_imp                                   S_un.S_un_w.s_w2
	#define s_impno                                 S_un.S_un_b.s_b4
	#define s_lh                                    S_un.S_un_b.s_b3
#endif


//////////////////////////////////////////////////
// Base defines
// 
#pragma pack(1)                                        //Memory alignment: 1 bytes/8 bits

//ASCII values definitions
#define ASCII_SPACE               32                   //" "
#define ASCII_PERIOD              46                   //"."
#define ASCII_SLASH               47                   //"/"
#define ASCII_ZERO                48                   //"0"
#define ASCII_NINE                57                   //"9"
#define ASCII_COLON               58                   //":"
#define ASCII_AT                  64                   //"@"
#define ASCII_UPPERCASE_A         65                   //"A"
#define ASCII_UPPERCASE_F         70                   //"F"
#define ASCII_BRACKETS_LEFT       91                   //"["
#define ASCII_BRACKETS_RIGHT      93                   //"]"
#define ASCII_ACCENT              96                   //"`"
#define ASCII_LOWERCASE_A         97                   //"a"
#define ASCII_LOWERCASE_F         102                  //"f"
#define ASCII_LOWERCASE_X         120                  //"x"
#define ASCII_BRACES_LEAD         123                  //"{"
#define ASCII_TILDE               126                  //"~"


//////////////////////////////////////////////////
// Protocol header structures
// 
//Domain Name System/DNS Part
/* About RFC standards
RFC 920(https://tools.ietf.org/html/rfc920), Domain Requirements – Specified original top-level domains
RFC 1032(https://tools.ietf.org/html/rfc1032), Domain Administrators Guide
RFC 1033(https://tools.ietf.org/html/rfc1033), Domain Administrators Operations Guide
RFC 1034(https://tools.ietf.org/html/rfc1034), Domain Names - Concepts and Facilities
RFC 1035(https://tools.ietf.org/html/rfc1035), Domain Names - Implementation and Specification
RFC 1101(https://tools.ietf.org/html/rfc1101), DNS Encodings of Network Names and Other Types
RFC 1123(https://tools.ietf.org/html/rfc1123), Requirements for Internet Hosts—Application and Support
RFC 1178(https://tools.ietf.org/html/rfc1178), Choosing a Name for Your Computer (FYI 5)
RFC 1183(https://tools.ietf.org/html/rfc1183), New DNS RR Definitions
RFC 1348(https://tools.ietf.org/html/rfc1348), DNS NSAP RRs
RFC 1591(https://tools.ietf.org/html/rfc1591), Domain Name System Structure and Delegation (Informational)
RFC 1664(https://tools.ietf.org/html/rfc1664), Using the Internet DNS to Distribute RFC1327 Mail Address Mapping Tables
RFC 1706(https://tools.ietf.org/html/rfc1706), DNS NSAP Resource Records
RFC 1712(https://tools.ietf.org/html/rfc1712), DNS Encoding of Geographical Location
RFC 1876(https://tools.ietf.org/html/rfc1876), A Means for Expressing Location Information in the Domain Name System
RFC 1886(https://tools.ietf.org/html/rfc1886), DNS Extensions to support IP version 6
RFC 1912(https://tools.ietf.org/html/rfc1912), Common DNS Operational and Configuration Errors
RFC 1995(https://tools.ietf.org/html/rfc1995), Incremental Zone Transfer in DNS
RFC 1996(https://tools.ietf.org/html/rfc1996), A Mechanism for Prompt Notification of Zone Changes (DNS NOTIFY)
RFC 2052(https://tools.ietf.org/html/rfc2052), A DNS RR for specifying the location of services (DNS SRV)
RFC 2100(https://tools.ietf.org/html/rfc2100), The Naming of Hosts (Informational)
RFC 2136(https://tools.ietf.org/html/rfc2136), Dynamic Updates in the domain name system (DNS UPDATE)
RFC 2181(https://tools.ietf.org/html/rfc2181), Clarifications to the DNS Specification
RFC 2182(https://tools.ietf.org/html/rfc2182), Selection and Operation of Secondary DNS Servers
RFC 2230(https://tools.ietf.org/html/rfc2230), Key Exchange Delegation Record for the DNS
RFC 2308(https://tools.ietf.org/html/rfc2308), Negative Caching of DNS Queries (DNS NCACHE)
RFC 2317(https://tools.ietf.org/html/rfc2317), Classless IN-ADDR.ARPA delegation (BCP 20)
RFC 2535(https://tools.ietf.org/html/rfc2535), Domain Name System Security Extensions
RFC 2536(https://tools.ietf.org/html/rfc2536), DSA KEYs and SIGs in the Domain Name System (DNS)
RFC 2537(https://tools.ietf.org/html/rfc2537), RSA/MD5 KEYs and SIGs in the Domain Name System (DNS)
RFC 2539(https://tools.ietf.org/html/rfc2539), Storage of Diffie-Hellman Keys in the Domain Name System (DNS)
RFC 2671(https://tools.ietf.org/html/rfc2671), Extension Mechanisms for DNS (EDNS0)
RFC 2672(https://tools.ietf.org/html/rfc2672), Non-Terminal DNS Name Redirection
RFC 2845(https://tools.ietf.org/html/rfc2845), Secret Key Transaction Authentication for DNS (TSIG)
RFC 2874(https://tools.ietf.org/html/rfc2874), DNS Extensions to Support IPv6 Address Aggregation and Renumbering
RFC 2930(https://tools.ietf.org/html/rfc2930), Secret Key Establishment for DNS (TKEY RR)
RFC 3110(https://tools.ietf.org/html/rfc3110), RSA/SHA-1 SIGs and RSA KEYs in the Domain Name System (DNS)
RFC 3123(https://tools.ietf.org/html/rfc3123), A DNS RR Type for Lists of Address Prefixes (APL RR)
RFC 3225(https://tools.ietf.org/html/rfc3225), Indicating Resolver Support of DNSSEC
RFC 3226(https://tools.ietf.org/html/rfc3226), DNSSEC and IPv6 A6 aware server/resolver message size requirements
RFC 3403(https://tools.ietf.org/html/rfc3403), Dynamic Delegation Discovery System (DDDS) Part Three: The Domain Name System (DNS) Database
RFC 3597(https://tools.ietf.org/html/rfc3597), Handling of Unknown DNS Resource Record (RR) Types
RFC 3696(https://tools.ietf.org/html/rfc3696), Application Techniques for Checking and Transformation of Names (Informational)
RFC 4025(https://tools.ietf.org/html/rfc4025), A Method for Storing IPsec Keying Material in DNS
RFC 4034(https://tools.ietf.org/html/rfc4034), Resource Records for the DNS Security Extensions
RFC 4255(https://tools.ietf.org/html/rfc4255), Using DNS to Securely Publish Secure Shell (SSH) Key Fingerprints
RFC 4343(https://tools.ietf.org/html/rfc4343), Domain Name System (DNS) Case Insensitivity Clarification
RFC 4398(https://tools.ietf.org/html/rfc4398), Storing Certificates in the Domain Name System (DNS)
RFC 4408(https://tools.ietf.org/html/rfc4408), Sender Policy Framework (SPF) for Authorizing Use of Domains in E-Mail, Version 1
RFC 4431(https://tools.ietf.org/html/rfc4431), The DNSSEC Lookaside Validation (DLV) DNS Resource Record
RFC 4592(https://tools.ietf.org/html/rfc4592), The Role of Wildcards in the Domain Name System
RFC 4635(https://tools.ietf.org/html/rfc4635), HMAC SHA TSIG Algorithm Identifiers
RFC 4701(https://tools.ietf.org/html/rfc4701), A DNS Resource Record (RR) for Encoding Dynamic Host Configuration Protocol (DHCP) Information (DHCID RR)
RFC 4892(https://tools.ietf.org/html/rfc4892), Requirements for a Mechanism Identifying a Name Server Instance (Informational)
RFC 5001(https://tools.ietf.org/html/rfc5001), DNS Name Server Identifier (NSID) Option
RFC 5155(https://tools.ietf.org/html/rfc5155), DNS Security (DNSSEC) Hashed Authenticated Denial of Existence
RFC 5205(https://tools.ietf.org/html/rfc5205), Host Identity Protocol (HIP) Domain Name System (DNS) Extension
RFC 5452(https://tools.ietf.org/html/rfc5452), Measures for Making DNS More Resilient against Forged Answers
RFC 5625(https://tools.ietf.org/html/rfc5625), DNS Proxy Implementation Guidelines (BCP 152)
RFC 5890(https://tools.ietf.org/html/rfc5890), Internationalized Domain Names for Applications (IDNA):Definitions and Document Framework
RFC 5891(https://tools.ietf.org/html/rfc5891), Internationalized Domain Names in Applications (IDNA): Protocol
RFC 5892(https://tools.ietf.org/html/rfc5892), The Unicode Code Points and Internationalized Domain Names for Applications (IDNA)
RFC 5893(https://tools.ietf.org/html/rfc5893), Right-to-Left Scripts for Internationalized Domain Names for Applications (IDNA)
RFC 5894(https://tools.ietf.org/html/rfc5894), Internationalized Domain Names for Applications (IDNA):Background, Explanation, and Rationale (Informational)
RFC 5895(https://tools.ietf.org/html/rfc5895), Mapping Characters for Internationalized Domain Names in Applications (IDNA) 2008 (Informational)
RFC 5936(https://tools.ietf.org/html/rfc5936), DNS Zone Transfer Protocol (AXFR)
RFC 5966(https://tools.ietf.org/html/rfc5966), DNS Transport over TCP - Implementation Requirements
RFC 6195(https://tools.ietf.org/html/rfc6195), Domain Name System (DNS) IANA Considerations (BCP 42)
RFC 6698(https://tools.ietf.org/html/rfc6698), The DNS-Based Authentication of Named Entities (DANE) Transport Layer Security (TLS) Protocol: TLSA
RFC 6742(https://tools.ietf.org/html/rfc6742), DNS Resource Records for the Identifier-Locator Network Protocol (ILNP)
RFC 6844(https://tools.ietf.org/html/rfc6844), DNS Certification Authority Authorization (CAA) Resource Record
RFC 6975(https://tools.ietf.org/html/rfc6975), Signaling Cryptographic Algorithm Understanding in DNS Security Extensions (DNSSEC)
RFC 7043(https://tools.ietf.org/html/rfc7043), Resource Records for EUI-48 and EUI-64 Addresses in the DNS
RFC 7314(https://tools.ietf.org/html/rfc7314), Extension Mechanisms for DNS (EDNS) EXPIRE Option
*/

//About this list, see https://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml
//Port and Flags definitions
#ifndef IPPORT_DNS
	#define IPPORT_DNS              53U        //Standard DNS(TCP and UDP) Port
#endif
#ifndef IPPORT_MDNS
	#define IPPORT_MDNS             5353U      //Multicast Domain Name System/mDNS Port
#endif
#ifndef IPPORT_LLMNR
	#define IPPORT_LLMNR            5355U      //Link-Local Multicast Name Resolution/LLMNR Port
#endif
#define DNS_STANDARD            0x0100   //System Standard query
#define DNS_SQR_NE              0x8180   //Standard query response and no error.
#define DNS_SQR_NETC            0x8380   //Standard query response and no error, but Truncated.
#define DNS_SQR_FE              0x8181   //Standard query response, Format Error
#define DNS_SQR_SF              0x8182   //Standard query response, Server failure
#define DNS_SQR_SNH             0x8183   //Standard query response, but No Such Name.
#define DNS_QUERY_PTR           0xC00C   //Pointer of first query

//OPCode definitions
#ifndef DNS_OPCODE_QUERY
	#define DNS_OPCODE_QUERY           0                //OPCode Query is 0.
#endif
#ifndef DNS_OPCODE_IQUERY
	#define DNS_OPCODE_IQUERY          1U               //OPCode Inverse Query(Obsolete) is 1.
#endif
#ifndef DNS_OPCODE_SERVER_STATUS
	#define DNS_OPCODE_SERVER_STATUS   2U               //OPCode Status is 2.
#endif
#ifndef DNS_OPCODE_UNKNOWN
	#define DNS_OPCODE_UNKNOWN         3U               //OPCode Unknown is 3.
#endif
#ifndef DNS_OPCODE_NOTIFY
	#define DNS_OPCODE_NOTIFY          4U               //OPCode Notify is 4.
#endif
#ifndef DNS_OPCODE_UPDATE
	#define DNS_OPCODE_UPDATE          5U               //OPCode Update is 5.
#endif
#ifndef DNS_OPCODE_RESERVED
	#define DNS_OPCODE_RESERVED        0xFFFF           //DNS Reserved OPCode is 65535.
#endif

//Classes definitions
#ifndef DNS_CLASS_INTERNET
	#define DNS_CLASS_INTERNET      0x0001           //DNS INTERNET Classes is 1.
#endif
#ifndef DNS_CLASS_CSNET
	#define DNS_CLASS_CSNET         0x0002           //DNS CSNET Classes is 2.
#endif
#ifndef DNS_CLASS_CHAOS
	#define DNS_CLASS_CHAOS         0x0003           //DNS CHAOS Classes is 3.
#endif
#ifndef DNS_CLASS_HESIOD
	#define DNS_CLASS_HESIOD        0x0004           //DNS HESIOD Classes is 4.
#endif
#ifndef DNS_CLASS_NONE
	#define DNS_CLASS_NONE          0x00FE           //DNS NONE Classes is 254.
#endif
#ifndef DNS_CLASS_ALL
	#define DNS_CLASS_ALL           0x00FF           //DNS ALL Classes is 255.
#endif
#ifndef DNS_CLASS_ANY
	#define DNS_CLASS_ANY           0x00FF           //DNS ANY Classes is 255.
#endif
#ifndef DNS_RCLASS_INTERNET
	#define DNS_RCLASS_INTERNET     0x0100           //DNS INTERNET Classes is 1.
#endif
#ifndef DNS_RCLASS_CSNET
	#define DNS_RCLASS_CSNET        0x0200           //DNS CSNET Classes is 2.
#endif
#ifndef DNS_RCLASS_CHAOS
	#define DNS_RCLASS_CHAOS        0x0300           //DNS CHAOS Classes is 3.
#endif
#ifndef DNS_RCLASS_HESIOD
	#define DNS_RCLASS_HESIOD       0x0400           //DNS HESIOD Classes is 4.
#endif
#ifndef DNS_RCLASS_NONE
	#define DNS_RCLASS_NONE         0xFE00           //DNS NONE Classes is 254.
#endif
#ifndef DNS_RCLASS_ALL
	#define DNS_RCLASS_ALL          0xFF00           //DNS ALL Classes is 255.
#endif
#ifndef DNS_RCLASS_ANY
	#define DNS_RCLASS_ANY          0xFF00           //DNS ANY Classes is 255.
#endif

//RCode definitions
#ifndef DNS_RCODE_NOERROR
	#define DNS_RCODE_NOERROR       0                //RCode No Error is 0.
#endif
#ifndef DNS_RCODE_FORMERR
	#define DNS_RCODE_FORMERR       0x0001           //RCode Format Error is 1.
#endif
#ifndef DNS_RCODE_SERVFAIL
	#define DNS_RCODE_SERVFAIL      0x0002           //RCode Server Failure is 2.
#endif
#ifndef DNS_RCODE_NXDOMAIN
	#define DNS_RCODE_NXDOMAIN      0x0003           //RCode Non-Existent Domain is 3.
#endif
#ifndef DNS_RCODE_NOTIMPL
	#define DNS_RCODE_NOTIMPL       0x0004           //RCode Not Implemented is 4.
#endif
#ifndef DNS_RCODE_REFUSED
	#define DNS_RCODE_REFUSED       0x0005           //RCode Query Refused is 5.
#endif
#ifndef DNS_RCODE_YXDOMAIN
	#define DNS_RCODE_YXDOMAIN      0x0006           //RCode Name Exists when it should not is 6.
#endif
#ifndef DNS_RCODE_YXRRSET
	#define DNS_RCODE_YXRRSET       0x0007           //RCode RR Set Exists when it should not is 7.
#endif
#ifndef DNS_RCODE_NXRRSET
	#define DNS_RCODE_NXRRSET       0x0008           //RCode RR Set that should exist does not is 8.
#endif
#ifndef DNS_RCODE_NOTAUTH
	#define DNS_RCODE_NOTAUTH       0x0009           //RCode Server Not Authoritative for zone/Not Authorized is 9.
#endif
#ifndef DNS_RCODE_NOTZONE
	#define DNS_RCODE_NOTZONE       0x000A           //RCode Name not contained in zone is 10.
#endif
#ifndef DNS_RCODE_BADVERS
	#define DNS_RCODE_BADVERS       0x0010           //RCode Bad OPT Version/TSIG Signature Failure is 16.
#endif
#ifndef DNS_RCODE_BADKEY
	#define DNS_RCODE_BADKEY        0x0011           //RCode Key not recognized is 17.
#endif
#ifndef DNS_RCODE_BADTIME
	#define DNS_RCODE_BADTIME       0x0012           //RCode Signature out of time window is 18.
#endif
#ifndef DNS_RCODE_BADMODE
	#define DNS_RCODE_BADMODE       0x0013           //RCode Bad TKEY Mode is 19.
#endif
#ifndef DNS_RCODE_BADNAME
	#define DNS_RCODE_BADNAME       0x0014           //RCode Duplicate key name is 20.
#endif
#ifndef DNS_RCODE_BADALG
	#define DNS_RCODE_BADALG        0x0015           //RCode Algorithm not supported is 21.
#endif
#ifndef DNS_RCODE_BADTRUNC
	#define DNS_RCODE_BADTRUNC      0x0016           //RCode Bad Truncation is 22.
#endif
#ifndef DNS_RCODE_PRIVATE_A
	#define DNS_RCODE_PRIVATE_A     0xFF00           //DNS Reserved Private use RCode is begin at 3841.
#endif
#ifndef DNS_RCODE_PRIVATE_B
	#define DNS_RCODE_PRIVATE_B     0xFFFE           //DNS Reserved Private use RCode is end at 4095.
#endif

//Record Types definitions
#ifndef DNS_TYPE_A
	#define DNS_TYPE_A            0x0001             //DNS Type A is 1.
#endif
#ifndef DNS_TYPE_NS
	#define DNS_TYPE_NS           0x0002             //DNS Type NS is 2.
#endif
#ifndef DNS_TYPE_MD
	#define DNS_TYPE_MD           0x0003             //DNS Type MD is 3(Obsolete).
#endif
#ifndef DNS_TYPE_MF
	#define DNS_TYPE_MF           0x0004             //DNS Type MF is 4(Obsolete).
#endif
#ifndef DNS_TYPE_CNAME
	#define DNS_TYPE_CNAME        0x0005             //DNS Type CNAME is 5.
#endif
#ifndef DNS_TYPE_SOA
	#define DNS_TYPE_SOA          0x0006             //DNS Type SOA is 6.
#endif
#ifndef DNS_TYPE_MB
	#define DNS_TYPE_MB           0x0007             //DNS Type MB is 7(Experimental).
#endif
#ifndef DNS_TYPE_MG
	#define DNS_TYPE_MG           0x0008             //DNS Type MG is 8(Experimental).
#endif
#ifndef DNS_TYPE_MR
	#define DNS_TYPE_MR           0x0009             //DNS Type MR is 9(Experimental).
#endif
#ifndef DNS_TYPE_NULL
	#define DNS_TYPE_NULL         0x000A             //DNS Type NULL is 10(Experimental).
#endif
#ifndef DNS_TYPE_WKS
	#define DNS_TYPE_WKS          0x000B             //DNS Type WKS is 11.
#endif
#ifndef DNS_TYPE_PTR
	#define DNS_TYPE_PTR          0x000C             //DNS Type PTR is 12.
#endif
#ifndef DNS_TYPE_HINFO
	#define DNS_TYPE_HINFO        0x000D             //DNS Type HINFO is 13.
#endif
#ifndef DNS_TYPE_MINFO
	#define DNS_TYPE_MINFO        0x000E             //DNS Type MINFO is 14.
#endif
#ifndef DNS_TYPE_MX
	#define DNS_TYPE_MX           0x000F             //DNS Type MX is 15.
#endif
#ifndef DNS_TYPE_TEXT
	#define DNS_TYPE_TEXT          0x0010             //DNS Type TXT is 16.
#endif
#ifndef DNS_TYPE_RP
	#define DNS_TYPE_RP           0x0011             //DNS Type RP is 17.
#endif
#ifndef DNS_TYPE_AFSDB
	#define DNS_TYPE_AFSDB        0x0012             //DNS Type AFSDB is 18.
#endif
#ifndef DNS_TYPE_X25
	#define DNS_TYPE_X25          0x0013             //DNS Type X25 is 19.
#endif
#ifndef DNS_TYPE_ISDN
	#define DNS_TYPE_ISDN         0x0014             //DNS Type ISDN is 20.
#endif
#ifndef DNS_TYPE_RT
	#define DNS_TYPE_RT           0x0015             //DNS Type RT is 21.
#endif
#ifndef DNS_TYPE_NSAP
	#define DNS_TYPE_NSAP         0x0016             //DNS Type NSAP is 22.
#endif
#ifndef DNS_TYPE_NSAPPTR
	#define DNS_TYPE_NSAPPTR     0x0017             //DNS Type NSAPPTR is 23(Obsolete).
#endif
#ifndef DNS_TYPE_SIG
	#define DNS_TYPE_SIG          0x0018             //DNS Type SIG is 24.
#endif
#ifndef DNS_TYPE_KEY
	#define DNS_TYPE_KEY          0x0019             //DNS Type KEY is 25.
#endif
#ifndef DNS_TYPE_PX
	#define DNS_TYPE_PX           0x001A             //DNS Type PX is 26.
#endif
#ifndef DNS_TYPE_GPOS
	#define DNS_TYPE_GPOS         0x001B             //DNS Type GPOS is 27.
#endif
#ifndef DNS_TYPE_AAAA
	#define DNS_TYPE_AAAA         0x001C             //DNS Type AAAA is 28.
#endif
#ifndef DNS_TYPE_LOC
	#define DNS_TYPE_LOC          0x001D             //DNS Type LOC is 29.
#endif
#ifndef DNS_TYPE_NXT
	#define DNS_TYPE_NXT          0x001E             //DNS Type NXT is 30.
#endif
#ifndef DNS_TYPE_EID
	#define DNS_TYPE_EID          0x001F             //DNS Type EID is 31.
#endif
#ifndef DNS_TYPE_NIMLOC
	#define DNS_TYPE_NIMLOC       0x0020             //DNS Type NIMLOC is 32.
#endif
#ifndef DNS_TYPE_SRV
	#define DNS_TYPE_SRV          0x0021             //DNS Type SRV is 33.
#endif
#ifndef DNS_TYPE_ATMA
	#define DNS_TYPE_ATMA         0x0022             //DNS Type ATMA is 34.
#endif
#ifndef DNS_TYPE_NAPTR
	#define DNS_TYPE_NAPTR        0x0023             //DNS Type NAPTR is 35.
#endif
#ifndef DNS_TYPE_KX
	#define DNS_TYPE_KX           0x0024             //DNS Type KX is 36.
#endif
#ifndef DNS_TYPE_CERT
	#define DNS_TYPE_CERT         0x0025             //DNS Type CERT is 37.
#endif
#ifndef DNS_TYPE_A6
	#define DNS_TYPE_A6           0x0026             //DNS Type A6 is 38(Obsolete).
#endif
#ifndef DNS_TYPE_DNAME
	#define DNS_TYPE_DNAME        0x0027             //DNS Type DNAME is 39.
#endif
#ifndef DNS_TYPE_SINK
	#define DNS_TYPE_SINK         0x0028             //DNS Type SINK is 40.
#endif
#ifndef DNS_TYPE_OPT
	#define DNS_TYPE_OPT          0x0029             //DNS Type OPT/EDNS is 41.
#endif
#ifndef DNS_TYPE_APL
	#define DNS_TYPE_APL          0x002A             //DNS Type APL is 42.
#endif
#ifndef DNS_TYPE_DS
	#define DNS_TYPE_DS           0x002B             //DNS Type DS is 43.
#endif
#ifndef DNS_TYPE_SSHFP
	#define DNS_TYPE_SSHFP        0x002C             //DNS Type SSHFP is 44.
#endif
#ifndef DNS_TYPE_IPSECKEY
	#define DNS_TYPE_IPSECKEY     0x002D             //DNS Type IPSECKEY is 45.
#endif
#ifndef DNS_TYPE_RRSIG
	#define DNS_TYPE_RRSIG        0x002E             //DNS Type RRSIG is 46.
#endif
#ifndef DNS_TYPE_NSEC
	#define DNS_TYPE_NSEC         0x002F             //DNS Type NSEC is 47.
#endif
#ifndef DNS_TYPE_DNSKEY
	#define DNS_TYPE_DNSKEY       0x0030             //DNS Type DNSKEY is 48.
#endif
#ifndef DNS_TYPE_DHCID
	#define DNS_TYPE_DHCID        0x0031             //DNS Type DHCID is 49.
#endif
#ifndef DNS_TYPE_NSEC3
	#define DNS_TYPE_NSEC3        0x0032             //DNS Type NSEC3 is 50.
#endif
#ifndef DNS_TYPE_NSEC3PARAM
	#define DNS_TYPE_NSEC3PARAM   0x0033             //DNS Type NSEC3PARAM is 51.
#endif
#ifndef DNS_TYPE_TLSA
	#define DNS_TYPE_TLSA         0x0034             //DNS Type Record TLSA is 52.
#endif
#ifndef DNS_TYPE_HIP
	#define DNS_TYPE_HIP          0x0037             //DNS Type HIP is 55.
#endif
#ifndef DNS_TYPE_NINFO
	#define DNS_TYPE_NINFO        0x0038             //DNS Type NINFO is 56.
#endif
#ifndef DNS_TYPE_RKEY
	#define DNS_TYPE_RKEY         0x0039             //DNS Type RKEY is 57.
#endif
#ifndef DNS_TYPE_TALINK
	#define DNS_TYPE_TALINK       0x003A             //DNS Type TALINK is 58.
#endif
#ifndef DNS_TYPE_CDS
	#define DNS_TYPE_CDS          0x003B             //DNS Type CDS is 59.
#endif
#ifndef DNS_TYPE_CDNSKEY
	#define DNS_TYPE_CDNSKEY      0x003C             //DNS Type CDNSKEY is 60.
#endif
#ifndef DNS_TYPE_OPENPGPKEY
	#define DNS_TYPE_OPENPGPKEY   0x003D             //DNS Type OPENPGPKEY is 61.
#endif
#ifndef DNS_TYPE_SPF
	#define DNS_TYPE_SPF          0x0063             //DNS Type SPF is 99.
#endif
#ifndef DNS_TYPE_UINFO
	#define DNS_TYPE_UINFO        0x0064             //DNS Type UINFO is 100.
#endif
#ifndef DNS_TYPE_UID
	#define DNS_TYPE_UID          0x0065             //DNS Type UID is 101.
#endif
#ifndef DNS_TYPE_GID
	#define DNS_TYPE_GID          0x0066             //DNS Type GID is 102.
#endif
#ifndef DNS_TYPE_UNSPEC
	#define DNS_TYPE_UNSPEC       0x0067             //DNS Type UNSPEC is 103.
#endif
#ifndef DNS_TYPE_NID
	#define DNS_TYPE_NID          0x0068             //DNS Type NID is 104.
#endif
#ifndef DNS_TYPE_L32
	#define DNS_TYPE_L32          0x0069             //DNS Type L32 is 105.
#endif
#ifndef DNS_TYPE_L64
	#define DNS_TYPE_L64          0x006A             //DNS Type L64 is 106.
#endif
#ifndef DNS_TYPE_LP
	#define DNS_TYPE_LP           0x006B             //DNS Type LP is 107.
#endif
#ifndef DNS_TYPE_EUI48
	#define DNS_TYPE_EUI48        0x006C             //DNS Type EUI48 is 108.
#endif
#ifndef DNS_TYPE_EUI64
	#define DNS_TYPE_EUI64        0x006D             //DNS Type EUI64 is 109.
#endif
#ifndef DNS_TYPE_ADDRS
	#define DNS_TYPE_ADDRS        0x00F8             //DNS Type TKEY is 248.
#endif
#ifndef DNS_TYPE_TKEY
	#define DNS_TYPE_TKEY         0x00F9             //DNS Type TKEY is 249.
#endif
#ifndef DNS_TYPE_TSIG
	#define DNS_TYPE_TSIG         0x00FA             //DNS Type TSIG is 250.
#endif
#ifndef DNS_TYPE_IXFR
	#define DNS_TYPE_IXFR         0x00FB             //DNS Type IXFR is 251.
#endif
#ifndef DNS_TYPE_AXFR
	#define DNS_TYPE_AXFR         0x00FC             //DNS Type AXFR is 252.
#endif
#ifndef DNS_TYPE_MAILB
	#define DNS_TYPE_MAILB        0x00FD             //DNS Type MAILB is 253.
#endif
#ifndef DNS_TYPE_MAILA
	#define DNS_TYPE_MAILA        0x00FE             //DNS Type MAILA is 254.
#endif
#ifndef DNS_TYPE_ANY
	#define DNS_TYPE_ANY          0x00FF             //DNS Type ANY is 255.
#endif
#ifndef DNS_TYPE_URI
	#define DNS_TYPE_URI          0x0100             //DNS Type URI is 256.
#endif
#ifndef DNS_TYPE_CAA
	#define DNS_TYPE_CAA          0x0101             //DNS Type CAA is 257.
#endif
#ifndef DNS_TYPE_TA
	#define DNS_TYPE_TA           0x8000             //DNS Type TA is 32768.
#endif
#ifndef DNS_TYPE_DLV
	#define DNS_TYPE_DLV          0x8001             //DNS Type DLVS is 32769.
#endif
#ifndef DNS_TYPE_PRIVATE_A
	#define DNS_TYPE_PRIVATE_A    0xFF00           //DNS Reserved Private use records is begin at 65280.
#endif
#ifndef DNS_TYPE_PRIVATE_B
	#define DNS_TYPE_PRIVATE_B    0xFFFE           //DNS Reserved Private use records is end at 65534.
#endif
#ifndef DNS_TYPE_RESERVED
	#define DNS_TYPE_RESERVED     0xFFFF           //DNS Reserved records is 65535.
#endif


/* Domain Name System/DNS Header
// With User Datagram Protocol/UDP

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        Identification         |Q|OPCode |A|T|R|R|Z|A|C| RCode |  QR/Query and Response, AA/Authoritative Answer, TC/Truncated, RD/Recursion Desired, RA/Recursion Available
|                               |R|       |A|C|D|A| |D|D|       |  Z/Zero, AD/Authenticated Data, CD/Checking Disabled, RCode/Return Code
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        Total Questions        |       Total Answer RRs        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|      Total Authority RRs      |     Total Additional RRs      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
#define OLD_DNS_MAXSIZE 512U
typedef struct _dns_hdr_
{
	uint16_t              ID;
	union {
		uint16_t          Flags;
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       RD:1;
			uint8_t       TC:1;
			uint8_t       AA:1;
			uint8_t       OPCode_Second:1;
			uint8_t       OPCode_First:3;
			uint8_t       QR:1;
			uint8_t       RCode:4;
			uint8_t       AD:1;
			uint8_t       CD:1;
			uint8_t       Zero:1;
			uint8_t       RA:1;
		#else //BIG_ENDIAN
			uint8_t       QR:1;
			uint8_t       OPCode:4;
			uint8_t       AA:1;
			uint8_t       TC:1;
			uint8_t       RD:1;
			uint8_t       RA:1;
			uint8_t       Zero:1;
			uint8_t       AD:1;
			uint8_t       CD:1;
			uint8_t       RCode:4;
		#endif
		}FlagsBits;
	};
	uint16_t              Questions;
	uint16_t              Answer;
	uint16_t              Authority;
	uint16_t              Additional;
}dns_hdr;

/* Domain Name System/DNS Query

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                             Name                              /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Type              |            Classes            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_qry_
{
//	uint8_t               *Name;
	uint16_t              Type;
	uint16_t              Classes;
}dns_qry;

/* Domain Name System/DNS Standard Resource Record

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                             Name                              /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Type              |           Classes             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Time To Live          |            Length             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                             Data                              /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_standard_
{
//	uint8_t               *Name;
	uint16_t              Type;
	uint16_t              Classes;
	uint32_t              TTL;
	uint16_t              Length;
//	uint8_t               *Data;
}dns_standard_record;

/* Start Of a zone of Authority/SOA Resource Record

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                         Primary Name                          /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                         Mailbox Name                          /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                            Serial                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                        Refresh Interval                       |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         Retry Interval                        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          Expire Limit                         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                          Minimum TTL                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_soa_
{
//	uint8_t               *PrimaryName;
//	uint8_t               *MailboxName;
	uint32_t              Serial;
	uint32_t              RefreshInterval;
	uint32_t              RetryInterval;
	uint32_t              ExpireLimit;
	uint32_t              MinimumTTL;
}dns_soa_record;

/* Mail eXchange/MX Resource Record

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Preference           |                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               /
/                                                               /
/                      Mail Exchange Name                       /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_mx_
{
	uint16_t              Preference;
//	uint8_t               *MailExchangeName;
}dns_mx_record;

/* Text strings/TXT Resource Record

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Length     |                                               /
+-+-+-+-+-+-+-+-+                                               /
/                              TXT                              /
/                                                               /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_txt_
{
	uint8_t              Length;
//	uint8_t              *TXT;

}dns_txt_record;

/* Server Selection/SRV Resource Record

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Priority            |            Weight             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Port              |                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               /
/                                                               /
/                            Target                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_srv_
{
	uint16_t             Priority;
	uint16_t             Weight;
	uint16_t             Port;
//	uint8_t              *Target;
}dns_srv_record;

// Option/OPT Resource Record(Extension Mechanisms for Domain Name System/EDNS, EDNS Label)
#define EDNS0_CODE_LLQ                 0x0001   //Long-lived query
#define EDNS0_CODE_UL                  0x0002   //Update lease
#define EDNS0_CODE_NSID                0x0003   //Name Server Identifier (RFC 5001)
#define EDNS0_CODE_OWNER               0x0004   //Owner, reserved
#define EDNS0_CODE_DAU                 0x0005   //DNSSEC Algorithm Understood (RFC6975)
#define EDNS0_CODE_DHU                 0x0006   //DS Hash Understood (RFC6975)
#define EDNS0_CODE_N3U                 0x0007   //DSEC3 Hash Understood (RFC6975)
#define EDNS0_CODE_CLIENT_SUBNET       0x0008   //Client subnet as assigned by IANA
#define EDNS0_CODE_EDNS_EXPIRE         0x0009   //EDNS Expire (RFC7314)
#define EDNS0_CODE_CLIENT_SUBNET_EXP   0x50FA   //Client subnet, ID is 20730

/*
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Domain                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Type              |       UDP Payload Size        |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Extended RCode | EDNS0 Version |D|          Reserved           |  Extended RCode/Higher bits in extended Return Code, D/DO bit
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Length             |\---------- Z Field -----------/
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
#define EDNS0_MINSIZE 1220U
typedef struct _dns_opt_
{
	uint8_t               RootName;
	uint16_t              Type;              //Additional RRs Type
	uint16_t              UDPPayloadSize;
	uint8_t               Extended_RCode;
	uint8_t               Version;           //EDNS0 Version
	union {
		uint16_t          Z_Field;
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       Reserved_First:7;
			uint8_t       DO:1;              //DO bit
		#else //BIG_ENDIAN
			uint8_t       DO:1;              //DO bit
			uint8_t       Reserved_First:7;
		#endif
			uint8_t       Reserved_Second;
		}Z_Bits;
	};
	uint16_t              DataLength;
}dns_opt_record;

/* Extension Mechanisms for Domain Name System/EDNS0 Option

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                             Code                              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                            Length                             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                             Data                              /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_edns0_option_
{
	uint16_t              Code;
	uint16_t              Length;
//	uint8_t               *Data;
}dns_edns0_option;

// RRSIG Record(Resource Record Digital Signature)
#define DNSSEC_AlGORITHM_RSA_MD5               1U
#define DNSSEC_AlGORITHM_DH                    2U
#define DNSSEC_AlGORITHM_DSA                   3U
#define DNSSEC_AlGORITHM_ECC                   4U
#define DNSSEC_AlGORITHM_RSA_SHA1              5U
#define DNSSEC_AlGORITHM_DSA_NSEC3_SHA1        6U
#define DNSSEC_AlGORITHM_RSA_SHA1_NSEC3_SHA1   7U
#define DNSSEC_AlGORITHM_RSA_SHA256            8U
#define DNSSEC_AlGORITHM_RSA_SHA512            10U
#define DNSSEC_AlGORITHM_ECC_GOST              12U
#define DNSSEC_AlGORITHM_ECDSA_P256_SHA256     13U
#define DNSSEC_AlGORITHM_ECDSA_P386_SHA386     14U
#define DNSSEC_AlGORITHM_HMAC_MD5              157U
#define DNSSEC_AlGORITHM_INDIRECT              252U
#define DNSSEC_AlGORITHM_PRIVATE_DNS           253U
#define DNSSEC_AlGORITHM_PRIVATE_OID           254U

/*
                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|        Type Covered           |   Algorithm   |    Labels     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         Original TTL                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                      Signature Expiration                     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                      Signature Inception                      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Key Tag            |                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+         Signer's Name         /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Signature                          /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_rrsig_
{
	uint16_t              TypeCovered;
	uint8_t               Algorithm;
	uint8_t               Labels;
	uint32_t              TTL;
	uint32_t              Expiration;
	uint32_t              Inception;
	uint16_t              KeyTag;
//	uint8_t               *SignerName;
//	uint8_t               *Signature;
}dns_rrsig_record;

/* Certification Authority Authorization/CAA Resource Record

                    1                   2                   3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Flags     |    Length     |                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               /
/                             Tag                               /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Value                              /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_caa_
{
	uint8_t              Flags;
	uint8_t              Length;
//	uint8_t              *Tag;
//	uint8_t              *Value;
}dns_caa_record;


//SOCKS protocol part
//Version, Method, Command and Reply definitions
#define SOCKS_VERSION_5                            5U
#define SOCKS_METHOD_NO_AUTHENTICATION_NUM         1U
#define SOCKS_METHOD_SUPPORT_NUM                   2U
#define SOCKS_METHOD_NO_AUTHENTICATION_REQUIRED    0
#define SOCKS_METHOD_GSSAPI                        1U
#define SOCKS_METHOD_USERNAME_PASSWORD             2U
#define SOCKS_METHOD_IANA_ASSIGNED_A               3U
#define SOCKS_METHOD_IANA_ASSIGNED_B               0x7F
#define SOCKS_METHOD_RESERVED_FOR_PRIVATE_A        0x80
#define SOCKS_METHOD_RESERVED_FOR_PRIVATE_B        0xFE
#define SOCKS_METHOD_NO_ACCEPTABLE_METHODS         0xFF
#define SOCKS_USERNAME_PASSWORD_VERSION            1U
#define SOCKS_USERNAME_PASSWORD_MAXNUM             255U
#define SOCKS_USERNAME_PASSWORD_SUCCESS            0
#define SOCKS_COMMAND_CONNECT                      1U
#define SOCKS_COMMAND_BIND                         2U
#define SOCKS_COMMAND_UDP_ASSOCIATE                3U
#define SOCKS5_ADDRESS_IPV4                        1U
#define SOCKS5_ADDRESS_DOMAIN                      3U
#define SOCKS5_ADDRESS_IPV6                        4U
#define SOCKS5_REPLY_SUCCESS                       0
#define SOCKS5_REPLY_SERVER_FAILURE                1U
#define SOCKS5_REPLY_NOT_ALLOWED                   2U
#define SOCKS5_REPLY_NETWORK_UNREACHABLE           3U
#define SOCKS5_REPLY_HOST_UNREACHABLE              4U
#define SOCKS5_REPLY_REFUSED                       5U
#define SOCKS5_REPLY_TTL_EXPORED                   6U
#define SOCKS5_REPLY_COMMAND_NOT_SUPPORTED         7U
#define SOCKS5_REPLY_ADDRESS_TYPE_NOT_SUPPORTED    8U
#define SOCKS5_REPLY_UNASSIGNED_A                  9U
#define SOCKS5_REPLY_UNASSIGNED_B                  0xFF

//SOCKS client version identifier and method selection message
/*
                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Version    | Method Number |            Methods            /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _socks_client_selection_message_
{
	uint8_t               Version;
	uint8_t               Methods_Number;
	uint8_t               Methods_A;               //Usually is 0(NO_AUTHENTICATION_REQUIRED)
	uint8_t               Methods_B;               //Usually is 2(USERNAME/PASSWORD)
}socks_client_selection, *psocks_client_selection;

//SOCKS server method selection message
/*
                    1 1 1 1 1 1 1
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Version    |    Method     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _socks_server_selection_message_
{
	uint8_t               Version;
	uint8_t               Method;
}socks_server_selection, *psocks_server_selection;

//SOCKS client Username/Password authentication message
/*
                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Version    |UserName Length|           User Name           /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Password Length|                   Password                    /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

typedef struct _socks_client_user_authentication_
{
	uint8_t               Version;
	uint8_t               UserName_Length;
	uint8_t               *UserName;
	uint8_t               Password_Length;
	uint8_t               *Password;
}socks_client_user_authentication, *psocks_client_user_authentication;
*/

//SOCKS server Username/Password authentication message
/*
                    1 1 1 1 1 1 1
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Version    |    Status     |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _socks_server_user_authentication_
{
	uint8_t               Version;
	uint8_t               Status;
}socks_server_user_authentication, *psocks_server_user_authentication;

//SOCKS version 5 client request message
/*
                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Version    |    Command    |   Reserved    | Address Type  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                        Remote Address                         /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Remote Port           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _socks5_client_command_request_
{
	uint8_t               Version;
	uint8_t               Command;
	uint8_t               Reserved;
	uint8_t               Address_Type;
//	uint8_t               *Remote_Address;
//	uint16_t              Remote_Port;
}socks5_client_command_request, *psocks5_client_command_request;

//SOCKS version 5 server reply message
/*
                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|    Version    |     Reply     |   Reserved    | Address Type  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                         Bind Address                          /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Bind Port           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _socks5_server_command_reply_
{
	uint8_t               Version;
	uint8_t               Reply;
	uint8_t               Reserved;
	uint8_t               Bind_Address_Type;
//	uint8_t               *Bind_Address;
//	uint16_t              Bind_Port;
}socks5_server_command_reply, *psocks5_server_command_reply;

//SOCKS UDP relay request
/*
                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|           Reserved            |Fragment Number| Address Type  |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                        Remote Address                         /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Remote Port           |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _socks_udp_relay_request_
{
	uint16_t              Reserved;
	uint8_t               FragmentNumber;
	uint8_t               Address_Type;
//	uint8_t               *Remote_Address;
//	uint16_t              Remote_Port;
}socks_udp_relay_request, *psocks_udp_relay_request;
