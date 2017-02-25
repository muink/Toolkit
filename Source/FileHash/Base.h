// This code is part of Toolkit(FileHash)
// A useful and powerful toolkit(FileHash)
// Copyright (C) 2012-2017 Chengr28
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


#ifndef TOOLKIT_FILEHASH_BASE_H
#define TOOLKIT_FILEHASH_BASE_H

//////////////////////////////////////////////////
// Operating system selection
// 
/* This code is from Qt source, which in /src/corelib/global/qsystemdetection.h header file, please visit https://www.qt.io/developers.

   The operating system, must be one of: (PLATFOEM_x)

     DARWIN   - Any Darwin system (macOS, iOS, watchOS, tvOS)
     MACOS    - macOS
     IOS      - iOS
     WATCHOS  - watchOS
     TVOS     - tvOS
     MSDOS    - MS-DOS and Windows
     OS2      - OS/2
     OS2EMX   - XFree86 on OS/2 (not PM)
     WIN32    - Win32 (Windows 2000/XP/Vista/7 and Windows Server 2003/2008)
     WINRT    - WinRT (Windows 8 Runtime)
     CYGWIN   - Cygwin
     SOLARIS  - Sun Solaris
     HPUX     - HP-UX
     ULTRIX   - DEC Ultrix
     LINUX    - Linux [has variants]
     FREEBSD  - FreeBSD [has variants]
     NETBSD   - NetBSD
     OPENBSD  - OpenBSD
     BSDI     - BSD/OS
     INTERIX  - Interix
     IRIX     - SGI Irix
     OSF      - HP Tru64 UNIX
     SCO      - SCO OpenServer 5
     UNIXWARE - UnixWare 7, Open UNIX 8
     AIX      - AIX
     HURD     - GNU Hurd
     DGUX     - DG/UX
     RELIANT  - Reliant UNIX
     DYNIX    - DYNIX/ptx
     QNX      - QNX [has variants]
     QNX6     - QNX RTP 6.1
     LYNX     - LynxOS
     BSD4     - Any BSD 4.4 system
     UNIX     - Any UNIX BSD/SYSV system
     ANDROID  - Android platform
     HAIKU    - Haiku

   The following operating systems have variants:
     LINUX    - both PLATFORM_LINUX and PLATFORM_ANDROID are defined when building for Android
              - only PLATFORM_LINUX is defined if building for other Linux systems
     FREEBSD  - PLATFORM_FREEBSD is defined only when building for FreeBSD with a BSD userland
              - PLATFORM_FREEBSD_KERNEL is always defined on FreeBSD, even if the userland is from GNU
*/

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    define PLATFORM_DARWIN
#    define PLATFORM_BSD4
#    ifdef __LP64__
#      define PLATFORM_DARWIN64
#    else
#      define PLATFORM_DARWIN32
#    endif
#    if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#      if defined(TARGET_OS_WATCH) && TARGET_OS_WATCH
#        define PLATFORM_WATCHOS
#      else
#        define QT_PLATFORM_UIKIT
#        if defined(TARGET_OS_TV) && TARGET_OS_TV
#          define PLATFORM_TVOS
#        else
#          // TARGET_OS_IOS is only available in newer SDKs, 
#          // so assume any other iOS-based platform is iOS for now
#          define PLATFORM_IOS
#        endif
#      endif
#    else
#      // TARGET_OS_OSX is only available in newer SDKs, 
#      // so assume any non iOS-based platform is macOS for now
#      define PLATFORM_MACOS
#    endif
#  else
#    error "Qt has not been ported to this Apple platform - see https://www.qt.io/developers"
#  endif
#elif defined(__ANDROID__) || defined(ANDROID)
#  define PLATFORM_ANDROID
#  define PLATFORM_LINUX
#elif defined(__CYGWIN__)
#  define PLATFORM_CYGWIN
#elif !defined(SAG_COM) && (!defined(WINAPI_FAMILY) || WINAPI_FAMILY==WINAPI_FAMILY_DESKTOP_APP) && (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define PLATFORM_WIN32
#  define PLATFORM_WIN64
#elif !defined(SAG_COM) && (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  if defined(WINAPI_FAMILY)
#    ifndef WINAPI_FAMILY_PC_APP
#      define WINAPI_FAMILY_PC_APP WINAPI_FAMILY_APP
#    endif
#    if defined(WINAPI_FAMILY_PHONE_APP) && WINAPI_FAMILY==WINAPI_FAMILY_PHONE_APP
#      define PLATFORM_WINPHONE
#      define PLATFORM_WINRT
#    elif WINAPI_FAMILY==WINAPI_FAMILY_PC_APP
#      define PLATFORM_WINRT
#    else
#      define PLATFORM_WIN32
#    endif
#  else
#    define PLATFORM_WIN32
#  endif
#elif defined(__sun) || defined(sun)
#  define PLATFORM_SOLARIS
#elif defined(hpux) || defined(__hpux)
#  define PLATFORM_HPUX
#elif defined(__ultrix) || defined(ultrix)
#  define PLATFORM_ULTRIX
#elif defined(sinix)
#  define PLATFORM_RELIANT
#elif defined(__native_client__)
#  define PLATFORM_NACL
#elif defined(__linux__) || defined(__linux)
#  define PLATFORM_LINUX
#elif defined(__FreeBSD__) || defined(__DragonFly__) || defined(__FreeBSD_kernel__)
#  ifndef __FreeBSD_kernel__
#    define PLATFORM_FREEBSD
#  endif
#  define PLATFORM_FREEBSD_KERNEL
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
#elif defined(__INTERIX)
#  define PLATFORM_INTERIX
#  define PLATFORM_BSD4
#elif defined(__sgi)
#  define PLATFORM_IRIX
#elif defined(__osf__)
#  define PLATFORM_OSF
#elif defined(_AIX)
#  define PLATFORM_AIX
#elif defined(__Lynx__)
#  define PLATFORM_LYNX
#elif defined(__GNU__)
#  define PLATFORM_HURD
#elif defined(__DGUX__)
#  define PLATFORM_DGUX
#elif defined(__QNXNTO__)
#  define PLATFORM_QNX
#elif defined(_SEQUENT_)
#  define PLATFORM_DYNIX
#elif defined(_SCO_DS) /* SCO OpenServer 5 + GCC */
#  define PLATFORM_SCO
#elif defined(__USLC__) /* all SCO platforms + UDK or OUDK */
#  define PLATFORM_UNIXWARE
#elif defined(__svr4__) && defined(i386) /* Open UNIX 8 + GCC */
#  define PLATFORM_UNIXWARE
#elif defined(__INTEGRITY)
#  define PLATFORM_INTEGRITY
#elif defined(VXWORKS) /* there is no "real" VxWorks define - this has to be set in the mkspec! */
#  define PLATFORM_VXWORKS
#elif defined(__HAIKU__)
#  define PLATFORM_HAIKU
#elif defined(__MAKEDEPEND__)
#else
#  error "Qt has not been ported to this OS - see https://www.qt-project.org/"
#endif

#if defined(PLATFORM_WIN32) || defined(PLATFORM_WIN64) || defined(PLATFORM_WINRT)
#  define PLATFORM_WIN
#endif

#if defined(PLATFORM_WIN)
#  undef PLATFORM_UNIX
#elif !defined(PLATFORM_UNIX)
#  define PLATFORM_UNIX
#endif

//Compatibility synonyms
#ifdef PLATFORM_DARWIN
#define PLATFORM_MAC
#endif
#ifdef PLATFORM_DARWIN32
#define PLATFORM_MAC32
#endif
#ifdef PLATFORM_DARWIN64
#define PLATFORM_MAC64
#endif
#ifdef PLATFORM_MACOS
#define PLATFORM_MACOS
#define PLATFORM_OSX
#endif

#ifdef PLATFORM_DARWIN
#  include <Availability.h>
#  include <AvailabilityMacros.h>
#
#  ifdef PLATFORM_MACOS
#    if !defined(__MAC_OS_X_VERSION_MIN_REQUIRED) || __MAC_OS_X_VERSION_MIN_REQUIRED < __MAC_10_6
#       undef __MAC_OS_X_VERSION_MIN_REQUIRED
#       define __MAC_OS_X_VERSION_MIN_REQUIRED __MAC_10_6
#    endif
#    if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_6
#       undef MAC_OS_X_VERSION_MIN_REQUIRED
#       define MAC_OS_X_VERSION_MIN_REQUIRED MAC_OS_X_VERSION_10_6
#    endif
#  endif
#
#  // Numerical checks are preferred to named checks, but to be safe
#  // we define the missing version names in case Qt uses them.
#
#  if !defined(__MAC_10_7)
#       define __MAC_10_7 1070
#  endif
#  if !defined(__MAC_10_8)
#       define __MAC_10_8 1080
#  endif
#  if !defined(__MAC_10_9)
#       define __MAC_10_9 1090
#  endif
#  if !defined(__MAC_10_10)
#       define __MAC_10_10 101000
#  endif
#  if !defined(__MAC_10_11)
#       define __MAC_10_11 101100
#  endif
#  if !defined(__MAC_10_12)
#       define __MAC_10_12 101200
#  endif
#  if !defined(MAC_OS_X_VERSION_10_7)
#       define MAC_OS_X_VERSION_10_7 1070
#  endif
#  if !defined(MAC_OS_X_VERSION_10_8)
#       define MAC_OS_X_VERSION_10_8 1080
#  endif
#  if !defined(MAC_OS_X_VERSION_10_9)
#       define MAC_OS_X_VERSION_10_9 1090
#  endif
#  if !defined(MAC_OS_X_VERSION_10_10)
#       define MAC_OS_X_VERSION_10_10 101000
#  endif
#  if !defined(MAC_OS_X_VERSION_10_11)
#       define MAC_OS_X_VERSION_10_11 101100
#  endif
#  if !defined(MAC_OS_X_VERSION_10_12)
#       define MAC_OS_X_VERSION_10_12 101200
#  endif
#
#  if !defined(__IPHONE_4_3)
#       define __IPHONE_4_3 40300
#  endif
#  if !defined(__IPHONE_5_0)
#       define __IPHONE_5_0 50000
#  endif
#  if !defined(__IPHONE_5_1)
#       define __IPHONE_5_1 50100
#  endif
#  if !defined(__IPHONE_6_0)
#       define __IPHONE_6_0 60000
#  endif
#  if !defined(__IPHONE_6_1)
#       define __IPHONE_6_1 60100
#  endif
#  if !defined(__IPHONE_7_0)
#       define __IPHONE_7_0 70000
#  endif
#  if !defined(__IPHONE_7_1)
#       define __IPHONE_7_1 70100
#  endif
#  if !defined(__IPHONE_8_0)
#       define __IPHONE_8_0 80000
#  endif
#  if !defined(__IPHONE_8_1)
#       define __IPHONE_8_1 80100
#  endif
#  if !defined(__IPHONE_8_2)
#       define __IPHONE_8_2 80200
#  endif
#  if !defined(__IPHONE_8_3)
#       define __IPHONE_8_3 80300
#  endif
#  if !defined(__IPHONE_8_4)
#       define __IPHONE_8_4 80400
#  endif
#  if !defined(__IPHONE_9_0)
#       define __IPHONE_9_0 90000
#  endif
#  if !defined(__IPHONE_9_1)
#       define __IPHONE_9_1 90100
#  endif
#  if !defined(__IPHONE_9_2)
#       define __IPHONE_9_2 90200
#  endif
#  if !defined(__IPHONE_9_3)
#       define __IPHONE_9_3 90300
#  endif
#  if !defined(__IPHONE_10_0)
#       define __IPHONE_10_0 100000
#  endif
#endif

#ifdef __LSB_VERSION__
#  if __LSB_VERSION__ < 40
#    error "This version of the Linux Standard Base is unsupported"
#  endif
#ifndef QT_LINUXBASE
#  define QT_LINUXBASE
#endif
#endif


//////////////////////////////////////////////////
// Platform check
//Toolkit now support Windows, Linux and macOS.
#if !(defined(PLATFORM_WIN) || defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#error "This platform is unsupported."
#endif


//////////////////////////////////////////////////
// Base headers
// 
//C Standard Library and C++ Standard Template Library/STL headers
#include <cerrno>                  //Error report support
#include <climits>                 //Limits support
#include <cstdio>                  //File Input and Output support
#include <cstdlib>                 //Several general purpose functions support
#include <cstring>                 //C-Style String support
#include <cwchar>                  //Wide-Character support
#include <memory>                  //Manage dynamic memory support
#include <string>                  //STL String support
#include <vector>                  //Vector support

#if defined(PLATFORM_WIN)
	#include <windows.h>                 //Master include file in Windows
	#pragma comment(lib, "ws2_32.lib")   //Windows WinSock 2.0+ support

//Endian definitions
	#define __LITTLE_ENDIAN            1234                      //Little Endian
	#define __BIG_ENDIAN               4321                      //Big Endian
	#define __BYTE_ORDER               __LITTLE_ENDIAN           //x86 and x86-64/x64 is Little Endian in Windows.
	#define LITTLE_ENDIAN              __LITTLE_ENDIAN
	#define BIG_ENDIAN                 __BIG_ENDIAN
	#define BYTE_ORDER                 __BYTE_ORDER

//Windows compatible definitions
	typedef SSIZE_T                    ssize_t;
#elif defined(PLATFORM_LINUX)
	#include <arpa/inet.h>             //Internet operations
	#include <endian.h>                //Endian
#elif defined(PLATFORM_MACOS)
//Endian definitions
	#define __LITTLE_ENDIAN            1234                      //Little Endian
	#define __BIG_ENDIAN               4321                      //Big Endian
	#define __BYTE_ORDER               __LITTLE_ENDIAN           //x86 and x86-64/x64 is Little Endian in macOS.
#endif


//////////////////////////////////////////////////
// Main definitions
// 
//Code definitions
#define BYTES_TO_BITS                           8U
#define FILE_BUFFER_SIZE                        4096U
#if defined(PLATFORM_WIN)
	#define MBSTOWCS_NULL_TERMINATE                 (-1)                       //MultiByteToWideChar function find null-terminate.
#endif
#define PADDING_RESERVED_BYTES                  2U

//Character value definitions
#define ASCII_SPACE                             32                          //" "
#define ASCII_PERIOD                            46                          //"."

//Version definitions
#define FULL_VERSION                            L"0.3.6.0"
#define COPYRIGHT_MESSAGE                       L"Copyright (C) 2012-2017 Chengr28"

//Command definitions
#define COMMAND_MIN_COUNT                       2
#define COMMAND_MAX_COUNT                       6
#if defined(PLATFORM_WIN)
	#define COMMAND_LONG_PRINT_VERSION              L"--VERSION"
	#define COMMAND_SHORT_PRINT_VERSION             L"-V"
	#define COMMAND_LONG_HELP                       L"--HELP"
	#define COMMAND_SHORT_HELP                      L"-H"
	#define COMMAND_SIGN_HELP                       L"-?"
	#define COMMAND_LOWERCASE                       L"--LOWERCASE"
	#define COMMAND_OUTPUT_FILE                     L"--OUTPUT"
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define COMMAND_LONG_PRINT_VERSION              ("--VERSION")
	#define COMMAND_SHORT_PRINT_VERSION             ("-V")
	#define COMMAND_LONG_HELP                       ("--HELP")
	#define COMMAND_SHORT_HELP                      ("-H")
	#define COMMAND_SIGN_HELP                       ("-?")
	#define COMMAND_LOWERCASE                       ("--LOWERCASE")
	#define COMMAND_OUTPUT_FILE                     ("--OUTPUT")
#endif

//Hash definitions
#define HASH_ID_BLAKE                           1U
#define HASH_ID_BLAKE2                          2U
#define HASH_ID_CRC                             3U
#define HASH_ID_CHECKSUM                        4U
#define HASH_ID_MD2                             5U
#define HASH_ID_MD4                             6U
#define HASH_ID_ED2K                            7U
#define HASH_ID_MD5                             8U
#define HASH_ID_MD                              HASH_ID_MD5
#define HASH_ID_RIPEMD                          9U
#define HASH_ID_SHA1                            10U
#define HASH_ID_SHA2                            11U
#define HASH_ID_SHA3                            12U
#define HASH_ID_SHA                             HASH_ID_SHA3
#define DEFAULT_HASH_ID                         HASH_ID_SHA3
#if defined(PLATFORM_WIN)
	#define HASH_COMMAND_BLAKE                      L"-BLAKE"
	#define HASH_COMMAND_BLAKE2                     L"-BLAKE2"
	#define HASH_COMMAND_CRC                        L"-CRC"
	#define HASH_COMMAND_CHECKSUM                   L"-CHECKSUM"
	#define HASH_COMMAND_MD                         L"-MD"
	#define HASH_COMMAND_MD2                        L"-MD2"
	#define HASH_COMMAND_MD4                        L"-MD4"
	#define HASH_COMMAND_ED2K                       L"-ED2K"
	#define HASH_COMMAND_MD5                        L"-MD5"
	#define HASH_COMMAND_RIPEMD                     L"-RIPEMD"
	#define HASH_COMMAND_SHA                        L"-SHA"
	#define HASH_COMMAND_SHA1                       L"-SHA1"
	#define HASH_COMMAND_SHA1_UNDERLINE             L"-SHA_1"
	#define HASH_COMMAND_SHA2                       L"-SHA2"
	#define HASH_COMMAND_SHA2_UNDERLINE             L"-SHA_2"
	#define HASH_COMMAND_SHA2_256                   L"-SHA256"
	#define HASH_COMMAND_SHA2_384                   L"-SHA384"
	#define HASH_COMMAND_SHA2_512                   L"-SHA512"
	#define HASH_COMMAND_SHA3                       L"-SHA3"
	#define HASH_COMMAND_SHA3_UNDERLINE             L"-SHA_3"
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	#define HASH_COMMAND_BLAKE                      ("-BLAKE")
	#define HASH_COMMAND_BLAKE2                     ("-BLAKE2")
	#define HASH_COMMAND_CRC                        ("-CRC")
	#define HASH_COMMAND_CHECKSUM                   ("-CHECKSUM")
	#define HASH_COMMAND_MD                         ("-MD")
	#define HASH_COMMAND_MD2                        ("-MD2")
	#define HASH_COMMAND_MD4                        ("-MD4")
	#define HASH_COMMAND_ED2K                       ("-ED2K")
	#define HASH_COMMAND_MD5                        ("-MD5")
	#define HASH_COMMAND_RIPEMD                     ("-RIPEMD")
	#define HASH_COMMAND_SHA                        ("-SHA")
	#define HASH_COMMAND_SHA1                       ("-SHA1")
	#define HASH_COMMAND_SHA1_UNDERLINE             ("-SHA_1")
	#define HASH_COMMAND_SHA2                       ("-SHA2")
	#define HASH_COMMAND_SHA2_UNDERLINE             ("-SHA_2")
	#define HASH_COMMAND_SHA2_256                   ("-SHA256")
	#define HASH_COMMAND_SHA2_384                   ("-SHA384")
	#define HASH_COMMAND_SHA2_512                   ("-SHA512")
	#define HASH_COMMAND_SHA3                       ("-SHA3")
	#define HASH_COMMAND_SHA3_UNDERLINE             ("-SHA_3")
#endif

#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
//Linux and macOS compatible
	#define RETURN_ERROR                                      (-1)
	#define fwprintf_s                                        fwprintf
	#define strnlen_s                                         strnlen
	#define wcsnlen_s                                         wcsnlen
	#define _set_errno(Value)                                 errno = (Value)
	#define fread_s(Dst, DstSize, ElementSize, Count, File)   fread(Dst, ElementSize, Count, File)
	#define memcpy_s(Dst, DstSize, Src, Size)                 memcpy(Dst, Src, Size)
#endif

//Function definitions
#if BYTE_ORDER == LITTLE_ENDIAN
	#define hton64(Value)         (static_cast<uint64_t>(((static_cast<uint64_t>(htonl(static_cast<uint32_t>                             \
									(((Value) << (sizeof(uint32_t) * BYTES_TO_BITS)) >>                                                  \
									(sizeof(uint32_t) * BYTES_TO_BITS))))) <<                                                            \
									(sizeof(uint32_t) * BYTES_TO_BITS)) | static_cast<uint32_t>(htonl(static_cast<uint32_t>((Value) >>   \
									(sizeof(uint32_t) * BYTES_TO_BITS))))))
#else //BIG_ENDIAN
	#define hton64(Value)         (Value)
#endif
#define ntoh64                hton64


//////////////////////////////////////////////////
// Main functions
// 
//Base.cpp
bool CheckEmptyBuffer(
	const void * const Buffer, 
	const size_t Length);
bool MBS_To_WCS_String(
	const uint8_t * const Buffer, 
	const size_t MaxLen, 
	std::wstring &Target);
void CaseConvert(
	std::string &Buffer, 
	const bool IsLowerToUpper);
void CaseConvert(
	std::wstring &Buffer, 
	const bool IsLowerToUpper);
uint8_t *sodium_bin2hex(
	uint8_t * const hex, 
	const size_t hex_maxlen, 
	const uint8_t * const bin, 
	const size_t bin_len);
void ErrorCodeToMessage(
	const ssize_t ErrorCode, 
	std::wstring &Message);
void WriteMessage_ScreenFile(
	const FILE * const FileHandle, 
	const uint8_t * const Message);
void PrintDescription(
	void);

//BLAKE.cpp
bool ReadCommand_BLAKE(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool BLAKE_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//BLAKE2.cpp
bool ReadCommand_BLAKE2(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool BLAKE2_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//Checksum.cpp
bool Checksum_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//CRC.cpp
bool ReadCommand_CRC(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool CRC_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//FileHash.cpp
bool ReadCommand(
#if defined(PLATFORM_WIN)
	std::vector<std::wstring> CommandList, 
	std::wstring &FileName, 
	std::wstring &OutputFile);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::vector<std::string> CommandList, 
	std::string &FileName, 
	std::string &OutputFile);
#endif
bool MainHashProcess(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//MD2.cpp
bool MD2_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//MD4.cpp
bool MD4_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//MD5.cpp
bool MD5_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//RIPEMD.cpp
bool ReadCommand_RIPEMD(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
bool RIPEMD_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//SHA-1.cpp
bool SHA1_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);

//SHA-2.cpp
bool SHA2_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);
bool ReadCommand_SHA2(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif

//SHA-3.cpp
bool SHA3_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile);
bool ReadCommand_SHA3(
#if defined(PLATFORM_WIN)
	std::wstring &Command);
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command);
#endif
#endif
