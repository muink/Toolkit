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


#ifndef TOOLKIT_DNSPING_STRUCTURE_H
#define TOOLKIT_DNSPING_STRUCTURE_H

#include "Platform.h"

//Memory alignment settings(Part 1)
#pragma pack(push) //Push current alignment to stack.
#pragma pack(1) //Set alignment to 1 byte boundary.

//////////////////////////////////////////////////
// Protocol header structures
// 
//Domain Name System/DNS part
/* About RFC standards
* RFC 920, Domain Requirements – Specified original top-level domains(https://tools.ietf.org/html/rfc920)
* RFC 1032, Domain Administrators Guide(https://tools.ietf.org/html/rfc1032)
* RFC 1033, Domain Administrators Operations Guide(https://tools.ietf.org/html/rfc1033)
* RFC 1034, Domain Names - Concepts and Facilities(https://tools.ietf.org/html/rfc1034)
* RFC 1035, Domain Names - Implementation and Specification(https://tools.ietf.org/html/rfc1035)
* RFC 1101, DNS Encodings of Network Names and Other Types(https://tools.ietf.org/html/rfc1101)
* RFC 1123, Requirements for Internet Hosts—Application and Support(https://tools.ietf.org/html/rfc1123)
* RFC 1178, Choosing a Name for Your Computer (FYI 5)(https://tools.ietf.org/html/rfc1178)
* RFC 1183, New DNS RR Definitions(https://tools.ietf.org/html/rfc1183)
* RFC 1348, DNS NSAP RRs(https://tools.ietf.org/html/rfc1348)
* RFC 1591, Domain Name System Structure and Delegation (Informational)(https://tools.ietf.org/html/rfc1591)
* RFC 1664, Using the Internet DNS to Distribute RFC1327 Mail Address Mapping Tables(https://tools.ietf.org/html/rfc1664)
* RFC 1706, DNS NSAP Resource Records(https://tools.ietf.org/html/rfc1706)
* RFC 1712, DNS Encoding of Geographical Location(https://tools.ietf.org/html/rfc1712)
* RFC 1876, A Means for Expressing Location Information in the Domain Name System(https://tools.ietf.org/html/rfc1876)
* RFC 1886, DNS Extensions to support IP version 6(https://tools.ietf.org/html/rfc1886)
* RFC 1912, Common DNS Operational and Configuration Errors(https://tools.ietf.org/html/rfc1912)
* RFC 1995, Incremental Zone Transfer in DNS(https://tools.ietf.org/html/rfc1995)
* RFC 1996, A Mechanism for Prompt Notification of Zone Changes (DNS NOTIFY)(https://tools.ietf.org/html/rfc1996)
* RFC 2052, A DNS RR for specifying the location of services (DNS SRV)(https://tools.ietf.org/html/rfc2052)
* RFC 2100, The Naming of Hosts (Informational)(https://tools.ietf.org/html/rfc2100)
* RFC 2136, Dynamic Updates in the domain name system (DNS UPDATE)(https://tools.ietf.org/html/rfc2136)
* RFC 2181, Clarifications to the DNS Specification(https://tools.ietf.org/html/rfc2181)
* RFC 2182, Selection and Operation of Secondary DNS Servers(https://tools.ietf.org/html/rfc2182)
* RFC 2230, Key Exchange Delegation Record for the DNS(https://tools.ietf.org/html/rfc2230)
* RFC 2308, Negative Caching of DNS Queries (DNS NCACHE)(https://tools.ietf.org/html/rfc2308)
* RFC 2317, Classless IN-ADDR.ARPA delegation (BCP 20)(https://tools.ietf.org/html/rfc2317)
* RFC 2535, Domain Name System Security Extensions(https://tools.ietf.org/html/rfc2535)
* RFC 2536, DSA KEYs and SIGs in the Domain Name System (DNS)(https://tools.ietf.org/html/rfc2536)
* RFC 2537, RSA/MD5 KEYs and SIGs in the Domain Name System (DNS)(https://tools.ietf.org/html/rfc2537)
* RFC 2539, Storage of Diffie-Hellman Keys in the Domain Name System (DNS)(https://tools.ietf.org/html/rfc2539)
* RFC 2671, Extension Mechanisms for DNS (EDNS)(https://tools.ietf.org/html/rfc2671)
* RFC 2672, Non-Terminal DNS Name Redirection(https://tools.ietf.org/html/rfc2672)
* RFC 2845, Secret Key Transaction Authentication for DNS (TSIG)(https://tools.ietf.org/html/rfc2845)
* RFC 2874, DNS Extensions to Support IPv6 Address Aggregation and Renumbering(https://tools.ietf.org/html/rfc2874)
* RFC 2930, Secret Key Establishment for DNS (TKEY RR)(https://tools.ietf.org/html/rfc2930)
* RFC 3110, RSA/SHA-1 SIGs and RSA KEYs in the Domain Name System (DNS)(https://tools.ietf.org/html/rfc3110)
* RFC 3123, A DNS RR Type for Lists of Address Prefixes (APL RR)(https://tools.ietf.org/html/rfc3123)
* RFC 3225, Indicating Resolver Support of DNSSEC(https://tools.ietf.org/html/rfc3225)
* RFC 3226, DNSSEC and IPv6 A6 aware server/resolver message size requirements(https://tools.ietf.org/html/rfc3226)
* RFC 3403, Dynamic Delegation Discovery System (DDDS) Part Three: The Domain Name System (DNS Database)(https://tools.ietf.org/html/rfc3403)
* RFC 3597, Handling of Unknown DNS Resource Record (RR Types)(https://tools.ietf.org/html/rfc3597)
* RFC 3696, Application Techniques for Checking and Transformation of Names (Informational)(https://tools.ietf.org/html/rfc3696)
* RFC 4025, A Method for Storing IPsec Keying Material in DNS(https://tools.ietf.org/html/rfc4025)
* RFC 4034, Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
* RFC 4255, Using DNS to Securely Publish Secure Shell (SSH Key Fingerprints)(https://tools.ietf.org/html/rfc4225)
* RFC 4343, Domain Name System (DNS) Case Insensitivity Clarification(https://tools.ietf.org/html/rfc4343)
* RFC 4398, Storing Certificates in the Domain Name System (DNS)(https://tools.ietf.org/html/rfc4398)
* RFC 4408, Sender Policy Framework (SPF) for Authorizing Use of Domains in E-Mail, Version 1(https://tools.ietf.org/html/rfc4408)
* RFC 4431, The DNSSEC Lookaside Validation (DLV) DNS Resource Record(https://tools.ietf.org/html/rfc4431)
* RFC 4592, The Role of Wildcards in the Domain Name System(https://tools.ietf.org/html/rfc4592)
* RFC 4635, HMAC SHA TSIG Algorithm Identifiers(https://tools.ietf.org/html/rfc4635)
* RFC 4701, A DNS Resource Record (RR) for Encoding Dynamic Host Configuration Protocol (DHCP Information)(DHCID RR)(https://tools.ietf.org/html/rfc4701)
* RFC 4892, Requirements for a Mechanism Identifying a Name Server Instance (Informational)(https://tools.ietf.org/html/rfc4892)
* RFC 5001, DNS Name Server Identifier (NSID) Option(https://tools.ietf.org/html/rfc5001)
* RFC 5155, DNS Security (DNSSEC) Hashed Authenticated Denial of Existence(https://tools.ietf.org/html/rfc5155)
* RFC 5205, Host Identity Protocol (HIP Domain Name System (DNS Extension(https://tools.ietf.org/html/rfc5205)
* RFC 5452, Measures for Making DNS More Resilient against Forged Answers(https://tools.ietf.org/html/rfc5452)
* RFC 5625, DNS Proxy Implementation Guidelines (BCP 152)(https://tools.ietf.org/html/rfc5625)
* RFC 5890, Internationalized Domain Names for Applications (IDNA):Definitions and Document Framework(https://tools.ietf.org/html/rfc5890)
* RFC 5891, Internationalized Domain Names in Applications (IDNA): Protocol(https://tools.ietf.org/html/rfc5891)
* RFC 5892, The Unicode Code Points and Internationalized Domain Names for Applications (IDNA)(https://tools.ietf.org/html/rfc5892)
* RFC 5893, Right-to-Left Scripts for Internationalized Domain Names for Applications (IDNA)(https://tools.ietf.org/html/rfc5893)
* RFC 5894, Internationalized Domain Names for Applications (IDNA):Background, Explanation, and Rationale (Informational)(https://tools.ietf.org/html/rfc594)
* RFC 5895, Mapping Characters for Internationalized Domain Names in Applications (IDNA) 2008 (Informational)(https://tools.ietf.org/html/rfc5895)
* RFC 5936, DNS Zone Transfer Protocol (AXFR)(https://tools.ietf.org/html/rfc5936)
* RFC 5966, DNS Transport over TCP - Implementation Requirements(https://tools.ietf.org/html/rfc5966)
* RFC 6195, Domain Name System (DNS) IANA Considerations (BCP 42)(https://tools.ietf.org/html/rfc6495)
* RFC 6698, The DNS-Based Authentication of Named Entities (DANE) Transport Layer Security (TLS) Protocol: TLSA(https://tools.ietf.org/html/rfc6698)
* RFC 6742, DNS Resource Records for the Identifier-Locator Network Protocol (ILNP)(https://tools.ietf.org/html/rfc6742)
* RFC 6844, DNS Certification Authority Authorization (CAA) Resource Record(https://tools.ietf.org/html/rfc6844)
* RFC 6891, Extension Mechanisms for DNS (EDNS(0))(https://tools.ietf.org/html/rfc6891)
* RFC 6975, Signaling Cryptographic Algorithm Understanding in DNS Security Extensions (DNSSEC)(https://tools.ietf.org/html/rfc6975)
* RFC 7043, Resource Records for EUI-48 and EUI-64 Addresses in the DNS(https://tools.ietf.org/html/rfc7043)
* RFC 7314, Extension Mechanisms for DNS (EDNS) EXPIRE Option(https://tools.ietf.org/html/rfc7314)
* RFC 7766, DNS Transport over TCP - Implementation Requirements(https://tools.ietf.org/html/rfc7766)
* RFC 7871, Client Subnet in DNS Queries(https://tools.ietf.org/html/rfc7871)
* RFC 7873, Domain Name System (DNS) Cookies(https://tools.ietf.org/html/rfc7873)
*/

//About this list, please visit IANA Domain Name System (DNS) Parameters(https://www.iana.org/assignments/dns-parameters/dns-parameters.xhtml)
//Port and Flags definitions
#ifndef IPPORT_DNS
	#define IPPORT_DNS                    53U        //Standard DNS(TCP and UDP) Port
#endif
#ifndef IPPORT_MDNS
	#define IPPORT_MDNS                   5353U      //Multicast Domain Name System/mDNS Port
#endif
#ifndef IPPORT_LLMNR
	#define IPPORT_LLMNR                  5355U      //Link-Local Multicast Name Resolution/LLMNR Port
#endif
#define DNS_FLAG_REQUEST_STANDARD        0x0100       //Standard request
#define DNS_FLAG_SQR_NE                  0x8180       //Standard query response and No Error.
#define DNS_FLAG_SQR_NEA                 0x8580       //Standard query response, No Error and Authoritative.
#define DNS_FLAG_SQR_NETC                0x8380       //Standard query response and No Error, but Truncated.
#define DNS_FLAG_SQR_FE                  0x8181       //Standard query response, Format Error
#define DNS_FLAG_SQR_SF                  0x8182       //Standard query response, Server failure
#define DNS_FLAG_SQR_SNH                 0x8183       //Standard query response, No Such Name
#define DNS_FLAG_GET_BIT_RESPONSE        0x8000       //Get Response bit in DNS flags.
#define DNS_FLAG_GET_BIT_OPCODE          0x7800       //Get OPCode in DNS flags.
#define DNS_FLAG_GET_BIT_AA              0x0400       //Get Authoritative bit in DNS flags.
#define DNS_FLAG_GET_BIT_TC              0x0200       //Get Truncated bit in DNS flags.
#define DNS_FLAG_GET_BIT_RD              0x0100       //Get Recursion Desired bit in DNS flags.
#define DNS_FLAG_GET_BIT_RA              0x0080       //Get Recursion Available bit in DNS flags.
#define DNS_FLAG_GET_BIT_Z               0x0040       //Get Reserved bit in DNS flags.
#define DNS_FLAG_GET_BIT_AD              0x0020       //Get Authentic Data bit in DNS flags.
#define DNS_FLAG_GET_BIT_CD              0x0010       //Get Checking Disabled bit in DNS flags.
#define DNS_FLAG_GET_BIT_RCODE           0x000F       //Get RCode in DNS flags.
#define DNS_FLAG_GET_BIT_SERVER_FIXED    0xF8C0       //Get all bits without AA/Authoritative Answer, TC/Truncated, RD/Recursion Desired, AD/Authenticated Data, CD/Checking Disabled, and RCode/Return Code in DNS flags.
#define DNS_FLAG_SET_R                   0x8000       //Set Response bit in DNS flags.
#define DNS_FLAG_SET_R_TC                0x8200       //Set Response bit and Truncated bit in DNS flags.
#define DNS_FLAG_SET_R_A                 0x8580       //Set Response bit and Authoritative bit in DNS flags.
#define DNS_FLAG_SET_R_FE                0x8001       //Set Response bit and Format Error RCode in DNS flags.
#define DNS_FLAG_SET_R_SNH               0x8003       //Set Response bit and No Such Name RCode in DNS flags.
#define DNS_POINTER_8_BITS               0xC0         //DNS compression pointer(11000000)
#define DNS_POINTER_16_BITS              0xC000       //DNS compression pointer(1100000000000000)
#define DNS_POINTER_8_BIT_STRING         ('\xC0')     //DNS compression pointer string
#define DNS_POINTER_BIT_GET_LOCATE       0x3FFF       //Get location of DNS compression pointer(00111111111111111)
#define DNS_POINTER_QUERY                0xC00C       //Pointer of first query
#define DNS_RECORD_TTL_GET_BIT_HIGHEST   0x80000000   //Get highest 1 bit in TTL.

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
#ifndef DNS_CLASS_RESERVED
	#define DNS_CLASS_RESERVED      0                //DNS RESERVED Classes is 0.
#endif
#ifndef DNS_CLASS_INTERNET
	#define DNS_CLASS_INTERNET      0x0001           //DNS INTERNET Classes is 1.
#endif
#ifndef DNS_CLASS_CSNET
	#define DNS_CLASS_CSNET         0x0002           //DNS CSNET Classes is 2(Obsolete).
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
#ifndef DNS_RCODE_BADCOOKIE
	#define DNS_RCODE_BADCOOKIE     0x0017           //RCode Bad Cookie is 23.
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
	#define DNS_TYPE_TEXT         0x0010             //DNS Type TXT is 16.
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
	#define DNS_TYPE_NSAPPTR      0x0017             //DNS Type NSAPPTR is 23(Obsolete).
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
	#define DNS_TYPE_PRIVATE_A    0xFF00             //DNS Reserved Private use records is begin at 65280.
#endif
#ifndef DNS_TYPE_PRIVATE_B
	#define DNS_TYPE_PRIVATE_B    0xFFFE             //DNS Reserved Private use records is end at 65534.
#endif
#ifndef DNS_TYPE_RESERVED
	#define DNS_TYPE_RESERVED     0xFFFF             //DNS Reserved records is 65535.
#endif

/* Domain Name System/DNS header
//With User Datagram Protocol/UDP

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
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
#define DNS_PACKET_MAXSIZE_TRADITIONAL   512U   //Traditional DNS packet maximum size(512 bytes)
typedef struct _dns_hdr_
{
	uint16_t              ID;
//	union {
		uint16_t          Flags;
/* No need to define sub structure.
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
*/
//	};
	uint16_t              Question;
	uint16_t              Answer;
	uint16_t              Authority;
	uint16_t              Additional;
}dns_hdr;

/* Domain Name System/DNS header
//With Transmission Control Protocol/TCP

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Length             |        Identification         |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|Q|OPCode |A|T|R|R|Z|A|C| RCode |        Total Questions        |  QR/Query and Response, AA/Authoritative Answer, TC/Truncated, RD/Recursion Desired, RA/Recursion Available
|R|       |A|C|D|A| |D|D|       |                               |  Z/Zero, AD/Authenticated Data, CD/Checking Disabled, RCode/Return Code
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|       Total Answer RRs        |      Total Authority RRs      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Total Additional RRs      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_tcp_hdr_
{
	uint16_t              Length;
	uint16_t              ID;
//	union {
		uint16_t          Flags;
/* No need to define sub structure.
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       RD:1;
			uint8_t       TC:1;
			uint8_t       AA:1;
			uint8_t       OPCode_Second:1;
			uint8_t       OPCode_First:3;
			uint8_t       QR:1;
			uint8_t       RCode:4;
			uint8_t       CD:1;
			uint8_t       AD:1;
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
*/
//	};
	uint16_t              Question;
	uint16_t              Answer;
	uint16_t              Authority;
	uint16_t              Additional;
}dns_tcp_hdr;

/* Domain Name System/DNS Query

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Domain                             /
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

/* Domain Name System/DNS Standard Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
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
typedef struct _dns_record_standard_
{
//	uint8_t               *Name;
	uint16_t              Type;
	uint16_t              Classes;
	uint32_t              TTL;
	uint16_t              Length;
//	uint8_t               *Data;
}dns_record_standard;

/* Domain Name System/DNS A(IPv4) Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Domain                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Type              |            Classes            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         Time To Live                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Length             |            Address            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Address            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_record_a_
{
	uint16_t              Name;
	uint16_t              Type;
	uint16_t              Classes;
	uint32_t              TTL;
	uint16_t              Length;
	in_addr               Address;
}dns_record_a;

/* Domain Name System/DNS Canonical Name/CNAME Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Domain                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Type              |            Classes            |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|                         Time To Live                          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Length             |                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               /
/                                                               /
/                         Primary Name                          /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_record_cname_
{
	uint16_t              Name;
	uint16_t              Type;
	uint16_t              Classes;
	uint32_t              TTL;
	uint16_t              Length;
//	uint8_t               *PrimaryName;
}dns_record_cname;

/* Domain Name System/DNS Start Of a zone of Authority/SOA Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
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
typedef struct _dns_record_soa_
{
//	uint8_t               *PrimaryName;
//	uint8_t               *MailboxName;
	uint32_t              Serial;
	uint32_t              RefreshInterval;
	uint32_t              RetryInterval;
	uint32_t              ExpireLimit;
	uint32_t              MinimumTTL;
}dns_record_soa;

/* Domain Name System/DNS Pointer/PTR Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Pointer            |             Type              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Classes            |         Time To Live          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Time To Live          |            Length             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Domain                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_record_ptr_
{
	uint16_t              Pointer;
	uint16_t              Type;
	uint16_t              Classes;
	uint32_t              TTL;
	uint16_t              Length;
//	uint8_t               *Name;
}dns_record_ptr;

/* Domain Name System/DNS Mail eXchange/MX Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          Preference           |                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               /
/                                                               /
/                      Mail Exchange Name                       /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_record_mx_
{
	uint16_t              Preference;
//	uint8_t               MailExchangeName;
}dns_record_mx;

/* Domain Name System/DNS Test Strings/TXT Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Name              |             Type              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Classes            |         Time To Live          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Time To Live          |            Length             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|          TXT Length           |                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+                               /
/                                                               /
/                           TXT Data                            /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_record_txt_
{
	uint16_t              Name;
	uint16_t              Type;
	uint16_t              Classes;
	uint32_t              TTL;
	uint16_t              Length;
	uint8_t               TXT_Length;
//	uint8_t               *TXT;
}dns_record_txt;

/* Domain Name System/DNS AAAA(IPv6) Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Pointer            |             Type              |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Classes            |         Time To Live          |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|         Time To Live          |            Length             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Domain                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
typedef struct _dns_record_aaaa_
{
	uint16_t              Name;
	uint16_t              Type;
	uint16_t              Classes;
	uint32_t              TTL;
	uint16_t              Length;
	in6_addr              Address;
}dns_record_aaaa;

/* Domain Name System/DNS Server Selection/SRV Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
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
typedef struct _dns_record_srv_
{
	uint16_t             Priority;
	uint16_t             Weight;
	uint16_t             Port;
//	uint8_t              *Target;
}dns_record_srv;

/* Extension Mechanisms for Domain Name System/DNS, EDNS Label/OPT Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Name      |             Type              |UDP PayloadSize|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|UDP PayloadSize|Extended RCode | EDNS Version  |D|  Reserved   |   Extended RCode/Higher bits in extended Return Code, D/DO/DNSSEC OK bit
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|   Reserved    |            Length             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
//Code definitions
#define EDNS_VERSION_ZERO          0
#define EDNS_PACKET_MINSIZE        1220U
#define EDNS_PACKET_MAXSIZE        4096U
#define EDNS_FLAG_GET_BIT_DO       0x8000        //Get DO bit in Z field.

//EDNS Code definitions
#define EDNS_CODE_LLQ              0x0001        //Long-lived query
#define EDNS_CODE_UL               0x0002        //Update lease
#define EDNS_CODE_NSID             0x0003        //Name Server Identifier (RFC 5001)
#define EDNS_CODE_OWNER            0x0004        //Owner, reserved
#define EDNS_CODE_DAU              0x0005        //DNSSEC Algorithm Understood (RFC 6975)
#define EDNS_CODE_DHU              0x0006        //DS Hash Understood (RFC 6975)
#define EDNS_CODE_N3U              0x0007        //DSEC3 Hash Understood (RFC 6975)
#define EDNS_CODE_CSUBNET          0x0008        //Client subnet (RFC 7871)
#define EDNS_CODE_EDNS_EXPIRE      0x0009        //EDNS Expire (RFC 7314)
#define EDNS_CODE_COOKIES          0x000A        //DNS Cookies (RFC 7873)

//Address Family Numbers, please visit https://www.iana.org/assignments/address-family-numbers/address-family-numbers.xhtml.
#define EDNS_ADDRESS_FAMILY_IPV4   0x0001
#define EDNS_ADDRESS_FAMILY_IPV6   0x0002
typedef struct _dns_record_opt_
{
	uint8_t               Name;
	uint16_t              Type;
	uint16_t              UDP_PayloadSize;
	uint8_t               Extended_RCode;
	uint8_t               Version;
//	union {
		uint16_t          Z_Field;
/* No need to define sub structure.
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       Reserved_First:7;
			uint8_t       DO:1;
		#else //BIG_ENDIAN
			uint8_t       DO:1;
			uint8_t       Reserved_First:7;
		#endif
			uint8_t       Reserved_Second;
		}Z_Bits;
*/
//	};
	uint16_t              DataLength;
}dns_record_opt, edns_header;

/* Extension Mechanisms for Domain Name System/EDNS Option
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
typedef struct _dns_edns_option_
{
	uint16_t              Code;
	uint16_t              Length;
//	uint8_t               *Data;
}dns_edns_option;

/* Extension Mechanisms for Domain Name System/DNS, Client subnet in EDNS requests
* RFC 7871, Client Subnet in DNS Queries(https://tools.ietf.org/html/rfc7871)

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Code              |            Length             |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Family             |Source Netmask | Scope Netmask |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ 
/                                                               /
/                           Address                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
#define EDNS_CODE_LLQ                            0x0001   //Long-lived query
#define EDNS_CODE_UL                             0x0002   //Update lease
#define EDNS_CODE_NSID                           0x0003   //Name Server Identifier (RFC 5001)
#define EDNS_CODE_OWNER                          0x0004   //Owner, reserved
#define EDNS_CODE_DAU                            0x0005   //DNSSEC Algorithm Understood (RFC 6975)
#define EDNS_CODE_DHU                            0x0006   //DS Hash Understood (RFC 6975)
#define EDNS_CODE_N3U                            0x0007   //DSEC3 Hash Understood (RFC 6975)
#define EDNS_CODE_CSUBNET                        0x0008   //Client subnet as assigned by IANA
#define EDNS_CODE_EDNS_EXPIRE                    0x0009   //EDNS Expire (RFC 7314)

//About Address Family Numbers, visit https://www.iana.org/assignments/address-family-numbers/address-family-numbers.xhtml.
#define ADDRESS_FAMILY_IPV4                      0x0001
#define ADDRESS_FAMILY_IPV6                      0x0002

//Netmask Source bits
#define EDNS_CLIENT_SUBNET_NETMASK_SOURCE_IPV6   56U
#define EDNS_CLIENT_SUBNET_NETMASK_SOURCE_IPV4   24U
typedef struct _edns_client_subnet_
{
	uint16_t              Code;
	uint16_t              Length;
	uint16_t              Family;
	uint8_t               Netmask_Source;
	uint8_t               Netmask_Scope;
//	uint8_t               *Address;
}edns_client_subnet;

/* Domain Name System/DNS Delegation Signer/DS Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|            Key Tag            |   Algorithm   |     Type      |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Digest                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
#define DNSSEC_DS_DIGEST_RESERVED              0
#define DNSSEC_DS_DIGEST_SHA1                  1U       //RFC 3658, Delegation Signer (DS) Resource Record (RR)(https://tools.ietf.org/html/rfc3658)
#define DNSSEC_DS_DIGEST_SHA256                2U       //RFC 4509, Use of SHA-256 in DNSSEC Delegation Signer (DS) Resource Records (RRs)(https://tools.ietf.org/html/rfc4509)
#define DNSSEC_DS_DIGEST_GOST                  3U       //RFC 5933, Use of GOST Signature Algorithms in DNSKEY and RRSIG Resource Records for DNSSEC(https://tools.ietf.org/html/rfc5933)
#define DNSSEC_DS_DIGEST_SHA384                4U       //RFC 6605, Elliptic Curve Digital Signature Algorithm (DSA) for DNSSEC(https://tools.ietf.org/html/rfc6605)

//About this list, visit https://www.iana.org/assignments/ds-rr-types/ds-rr-types.xhtml
#define DNSSEC_DS_TYPE_RESERVED                0
#define DNSSEC_DS_TYPE_SHA1                    1U
#define DNSSEC_DS_TYPE_SHA256                  2U
#define DNSSEC_DS_TYPE_GOST                    3U
#define DNSSEC_DS_TYPE_SHA384                  4U
#define SHA1_LENGTH                            20       //SHA-1 output is 160 bits/20 bytes lentgh
#define SHA256_LENGTH                          32       //SHA-256 output is 256 bits/32 bytes lentgh
#define GOST_LENGTH                            32       //GOST R 34.11-94 output is 256 bits/32 bytes lentgh
#define SHA384_LENGTH                          48       //SHA-384 output is 384 bits/48 bytes lentgh
typedef struct _dns_record_ds_
{
	uint16_t              KeyTag;
	uint8_t               Algorithm;
	uint8_t               Type;
//	uint8_t               *Digest;
}dns_record_ds;

/* Domain Name System/DNS Resource Record Digital Signature/RRSIG Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
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
#define DNSSEC_AlGORITHM_RESERVED_0            0        //RFC 4034, Reserved(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define DNSSEC_AlGORITHM_RSA_MD5               1U       //RFC 3110, RSA-MD5(RSA/SHA-1 SIGs and RSA KEYs in the Domain Name System (DNS)(https://tools.ietf.org/html/rfc3110)
#define DNSSEC_AlGORITHM_DH                    2U       //RFC 2539, Diffie-Hellman(Storage of Diffie-Hellman Keys in the Domain Name System (DNS)(https://tools.ietf.org/html/rfc2539)
#define DNSSEC_AlGORITHM_DSA                   3U       //RFC 3755, DSA-SHA1(Legacy Resolver Compatibility for Delegation Signer (DS)(https://tools.ietf.org/html/rfc3755)
#define DNSSEC_AlGORITHM_RESERVED_4            4U       //RFC 6725, Reserved(DNS Security (DNSSEC) DNSKEY Algorithm IANA Registry Updates(https://tools.ietf.org/html/rfc6725)
#define DNSSEC_AlGORITHM_RSA_SHA1              5U       //RFC 4034, RSA-SHA-1(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define DNSSEC_AlGORITHM_DSA_NSEC3_SHA1        6U       //RFC 5155, DSA-NSEC3-SHA-1(DNS Security (DNSSEC) Hashed Authenticated Denial of Existence(https://tools.ietf.org/html/rfc5155)
#define DNSSEC_AlGORITHM_RSA_SHA1_NSEC3_SHA1   7U       //RFC 5155, RSA-SHA-1-NSEC3-SHA-1(DNS Security (DNSSEC) Hashed Authenticated Denial of Existence(https://tools.ietf.org/html/rfc5155)
#define DNSSEC_AlGORITHM_RSA_SHA256            8U       //RFC 5702, RSA-SHA-256(Use of SHA-2 Algorithms with RSA in DNSKEY and RRSIG Resource Records for DNSSEC(https://tools.ietf.org/html/rfc5702)
#define DNSSEC_AlGORITHM_RESERVED_9            9U       //RFC 6725, Reserved(DNS Security (DNSSEC) DNSKEY Algorithm IANA Registry Updates(https://tools.ietf.org/html/rfc6725)
#define DNSSEC_AlGORITHM_RSA_SHA512            10U      //RFC 5702, RSA-SHA-512(Use of SHA-2 Algorithms with RSA in DNSKEY and RRSIG Resource Records for DNSSEC(https://tools.ietf.org/html/rfc5702)
#define DNSSEC_AlGORITHM_RESERVED_11           11U      //RFC 6725, Reserved(DNS Security (DNSSEC) DNSKEY Algorithm IANA Registry Updates(https://tools.ietf.org/html/rfc6725)
#define DNSSEC_AlGORITHM_ECC_GOST              12U      //RFC 5933, GOST R 34.10-2001(Use of GOST Signature Algorithms in DNSKEY and RRSIG Resource Records for DNSSEC(https://tools.ietf.org/html/rfc5933)
#define DNSSEC_AlGORITHM_ECDSA_P256_SHA256     13U      //RFC 6605, ECDSA Curve P-256 with SHA-256(Elliptic Curve Digital Signature Algorithm (DSA) for DNSSEC(https://tools.ietf.org/html/rfc6605)
#define DNSSEC_AlGORITHM_ECDSA_P386_SHA386     14U      //RFC 6605, ECDSA Curve P-384 with SHA-384(Elliptic Curve Digital Signature Algorithm (DSA) for DNSSEC(https://tools.ietf.org/html/rfc6605)
#define DNSSEC_AlGORITHM_RESERVED_123          123U     //RFC 4034, Reserved area between 123 and 251(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define DNSSEC_AlGORITHM_RESERVED_251          251U     //RFC 4034, Reserved area between 123 and 251(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define DNSSEC_AlGORITHM_INDIRECT              252U     //RFC 4034, Reserved for Indirect Keys(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define DNSSEC_AlGORITHM_PRIVATE_DNS           253U     //RFC 4034, Private algorithm(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define DNSSEC_AlGORITHM_PRIVATE_OID           254U     //RFC 4034, Private algorithm(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define DNSSEC_AlGORITHM_RESERVED_255          255U     //RFC 4034, Reserved(Resource Records for the DNS Security Extensions(https://tools.ietf.org/html/rfc4034)
#define RSA_MIN_LENGTH                         64U
#define DH_MIN_LENGTH                          96U
#define DSA_MIN_LENGTH                         128U
#define ECC_MIN_LENGTH                         24U
typedef struct _dns_record_rrsig_
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
}dns_record_rrsig;

/* Domain Name System/DNS DNS Key/DNSKEY Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|             Flags             |    Protocol   |   Algorithm   |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                            Digest                             /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
#define DNSSEC_DNSKEY_FLAGS_ZK                 0x0100
#define DNSSEC_DNSKEY_FLAGS_KR                 0x0080
#define DNSSEC_DNSKEY_FLAGS_SEP                0x0001
#define DNSSEC_DNSKEY_FLAGS_RSV                0xFE7E
#define DNSSEC_DNSKEY_PROTOCOL                 3U
typedef struct _dns_record_dnskey_
{
//	union {
		uint16_t          Flags;
/* No need to define sub structure.
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       ZoneKey:1;
			uint8_t       Zero_A:7;
			uint8_t       KeySigningKey:1;
			uint8_t       Zero_B:6;
			uint8_t       KeyRevoked:1;
		#else //BIG_ENDIAN
			uint8_t       Zero_A:7;
			uint8_t       ZoneKey:1;
			uint8_t       KeyRevoked:1;
			uint8_t       Zero_B:6;
			uint8_t       KeySigningKey:1;
		#endif
		}FlagsBits;
*/
//	};
	uint8_t               Protocol;
	uint8_t               Algorithm;
//	uint8_t               *PublicKey;
}dns_record_dnskey;

/* Domain Name System/DNS Next-Secure/NSEC Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                       Next Domain Name                        /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                        Type Bit Maps                          /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

typedef struct _dns_record_nsec_
{
//	uint8_t                NextDomainName;
//	uint8_t                TypeBitMap;
}dns_record_nsec;
*/

/* Domain Name System/DNS NSEC version 3/NSEC3 Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Hash      |     Flags     |          Iterations           |
|   Algorithm   |               |                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Salt Length  |                     Salt                      /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Hash Length  |            Next Hashed Owner Name             /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
/                                                               /
/                         Type Bit Maps                         /
/                                                               /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
//About this list, visit IANA Domain Name System Security (DNSSEC) NextSECure3 (NSEC3) Parameters(https://www.iana.org/assignments/dnssec-nsec3-parameters/dnssec-nsec3-parameters.xhtml)
#define DNSSEC_NSEC3_ALGORITHM_SHA1            1U
typedef struct _dns_record_nsec3_
{
	uint8_t               Algorithm;
//	union {
		uint8_t           Flags;
/* No need to define sub structure.
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       OptOut:1;
			uint8_t       Zero:7;
		#else //BIG_ENDIAN
			uint8_t       Zero:7;
			uint8_t       OptOut:1;
		#endif
		}FlagsBits;
*/
//	};
	uint16_t              Iterations;
	uint8_t               SaltLength;
//	uint8_t               *Salt;
//	uint8_t               HashLength;
//	uint8_t               *NextHashedOwnerName;
//	uint8_t               *TypeBitMap;
}dns_record_nsec3;

/* Domain Name System/DNS NSEC version 3 Parameters/NSEC3PARAM Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     Hash      |     Flags     |          Iterations           |
|   Algorithm   |               |                               |
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|  Salt Length  |                     Salt                      /
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

*/
//About this list, visit IANA Domain Name System Security (DNSSEC) NextSECure3 (NSEC3) Parameters(https://www.iana.org/assignments/dnssec-nsec3-parameters/dnssec-nsec3-parameters.xhtml)
typedef struct _dns_record_nsec3param_
{
	uint8_t               Algorithm;
//	union {
		uint8_t           Flags;
/* No need to define sub structure.
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       Reserved:1;
			uint8_t       Zero:7;
		#else //BIG_ENDIAN
			uint8_t       Zero:7;
			uint8_t       Reserved:1;
		#endif
		}FlagsBits;
*/
//	};
	uint16_t              Iterations;
	uint8_t               SaltLength;
//	uint8_t               *Salt;
}dns_record_nsec3param;

/* Domain Name System/DNS Certification Authority Authorization/CAA Resource Records

                    1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 3
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
typedef struct _dns_record_caa_
{
//	union {
		uint8_t           Flags;
/* No need to define sub structure.
		struct {
		#if BYTE_ORDER == LITTLE_ENDIAN
			uint8_t       Zero:7;
			uint8_t       IssuerCritical:1;
		#else //BIG_ENDIAN
			uint8_t       IssuerCritical:1;
			uint8_t       Zero:7;
		#endif
		}FlagsBits;
*/
//	};
	uint8_t               Length;
//	uint8_t               *Tag;
//	uint8_t               *Value;
}dns_record_caa;

//Memory alignment settings(Part 2)
#pragma pack(pop) //Restore original alignment from stack.
#endif
