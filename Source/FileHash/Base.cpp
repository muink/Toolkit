// This code is part of Toolkit(FileHash)
// A useful and powerful toolkit(FileHash)
// Copyright (C) 2012-2016 Chengr28
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

//Check empty buffer
bool CheckEmptyBuffer(
	const void *Buffer, 
	const size_t Length)
{
//Null pointer
	if (Buffer == nullptr)
	{
		return false;
	}
	else {
	//Scan all data.
		for (size_t Index = 0;Index < Length;++Index)
		{
			if (*(((uint8_t *)Buffer) + Index) != 0)
				return false;
		}
	}

	return true;
}

//Convert multiple bytes to wide char string
bool MBSToWCSString(
	const uint8_t *Buffer, 
	const size_t MaxLen, 
	std::wstring &Target)
{
//Check buffer.
	Target.clear();
	if (Buffer == nullptr || MaxLen == 0)
		return false;
	size_t Length = strnlen_s((const char *)Buffer, MaxLen);
	if (Length == 0 || CheckEmptyBuffer(Buffer, Length))
		return false;

//Convert string.
	std::shared_ptr<wchar_t> TargetPTR(new wchar_t[Length + PADDING_RESERVED_BYTES]());
	wmemset(TargetPTR.get(), 0, Length + PADDING_RESERVED_BYTES);
#if defined(PLATFORM_WIN)
	if (MultiByteToWideChar(
			CP_ACP, 
			0, 
			(LPCCH)Buffer, 
			MBSTOWCS_NULLTERMINATE, 
			TargetPTR.get(), 
			(int)(Length + PADDING_RESERVED_BYTES)) == 0)
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	if (mbstowcs(TargetPTR.get(), (const char *)Buffer, Length + PADDING_RESERVED_BYTES) == (size_t)RETURN_ERROR)
#endif
	{
		return false;
	}
	else {
		if (wcsnlen_s(TargetPTR.get(), Length + PADDING_RESERVED_BYTES) == 0)
			return false;
		else 
			Target = TargetPTR.get();
	}

	return true;
}

//Convert lowercase/uppercase words to uppercase/lowercase words(C++ string version)
void CaseConvert(
	std::string &Buffer, 
	const bool IsLowerToUpper)
{
	for (auto &StringIter:Buffer)
	{
	//Lowercase to uppercase
		if (IsLowerToUpper)
			StringIter = (char)toupper(StringIter);
	//Uppercase to lowercase
		else 
			StringIter = (char)tolower(StringIter);
	}

	return;
}

//Convert lowercase/uppercase words to uppercase/lowercase words(C++ wstring version)
void CaseConvert(
	std::wstring &Buffer, 
	const bool IsLowerToUpper)
{
	for (auto &StringIter:Buffer)
	{
	//Lowercase to uppercase
		if (IsLowerToUpper)
			StringIter = (wchar_t)toupper(StringIter);
	//Uppercase to lowercase
		else 
			StringIter = (wchar_t)tolower(StringIter);
	}

	return;
}

//Derived from original code by CodesInChaos(LibSodium)
uint8_t *sodium_bin2hex(
	uint8_t *const hex, 
	const size_t hex_maxlen, 
	const uint8_t *const bin, 
	const size_t bin_len)
{
	size_t       i = (size_t)0U;
	unsigned int x = 0;
	int          b = 0;
	int          c = 0;

	if (bin_len >= SIZE_MAX / 2 || hex_maxlen <= bin_len * 2U)
		abort(); /* LCOV_EXCL_LINE */
	while (i < bin_len)
	{
		c = bin[i] & 0xf;
		b = bin[i] >> 4;
		x = (uint8_t)(87U + c + (((c - 10U) >> 8) & ~38U)) << 8 | (uint8_t) (87U + b + (((b - 10U) >> 8) & ~38U));
		hex[i * 2U] = (char)x;
		x >>= 8;
		hex[i * 2U + 1U] = (char)x;
		i++;
	}
	hex[i * 2U] = 0U;

	return hex;
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

//Print to screen
void PrintToScreen(
	const uint8_t *Message)
{
	std::string InnerMessage((const char *)Message);
	CaseConvert(InnerMessage, true);
	for (size_t Index = 0;Index < InnerMessage.length();++Index)
		fwprintf_s(stderr, L"%c", InnerMessage.c_str()[Index]);
	fwprintf_s(stderr, L"\n");

	return;
}

//Print description to screen
void PrintDescription(
	void)
{
//Description
	fwprintf_s(stderr, L"\n--------------------------------------------------\n");
	fwprintf_s(stderr, L"FileHash ");
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
	fwprintf_s(stderr, L"A useful and powerful toolkit(FileHash)\n");
	fwprintf_s(stderr, COPYRIGHT_MESSAGE);
	fwprintf_s(stderr, L"\n--------------------------------------------------\n");

//Usage
	fwprintf_s(stderr, L"       FileHash -options/-algorithm [Filename]\n");
	fwprintf_s(stderr, L"  e.g. FileHash -SHA3 Filename\n");

//Supported options
	fwprintf_s(stderr, L"\nSupported options:\n");
	fwprintf_s(stderr, L"   -v/--version:     Print current version on screen.\n");
	fwprintf_s(stderr, L"   -?/-h/--help      Print description.\n");

//Supported hash algorithm list
	fwprintf_s(stderr, L"\nSupported hash algorithms:\n");
	fwprintf_s(stderr, L"   * CRC family:     -CRC                        = -CRC32\n");
	fwprintf_s(stderr, L"                     -CRC8                       CRC 8 bits\n");
	fwprintf_s(stderr, L"                     -CRC8_ITU                   CRC 8 bits ITU\n");
	fwprintf_s(stderr, L"                     -CRC8_ATM                   CRC 8 bits ATM\n");
	fwprintf_s(stderr, L"                     -CRC8_CCITT                 CRC 8 bits CCITT\n");
	fwprintf_s(stderr, L"                     -CRC8_MAXIM                 CRC 8 bits Maxim\n");
	fwprintf_s(stderr, L"                     -CRC8_ICODE                 CRC 8 bits Icode\n");
	fwprintf_s(stderr, L"                     -CRC8_J1850                 CRC 8 bits J1850\n");
	fwprintf_s(stderr, L"                     -CRC8_WCDMA                 CRC 8 bits WCDMA\n");
	fwprintf_s(stderr, L"                     -CRC8_ROHC                  CRC 8 bits Rohc\n");
	fwprintf_s(stderr, L"                     -CRC8_DARC                  CRC 8 bits Darc\n");
	fwprintf_s(stderr, L"                     -CRC16                      CRC 16 bits\n");
	fwprintf_s(stderr, L"                     -CRC16_BUYPASS              CRC 16 bits Buypass\n");
	fwprintf_s(stderr, L"                     -CRC16_DDS_110              CRC 16 bits DDS 110\n");
	fwprintf_s(stderr, L"                     -CRC16_EN_13757             CRC 16 bits EN 13757\n");
	fwprintf_s(stderr, L"                     -CRC16_TELEDISK             CRC 16 bits Teledisk\n");
	fwprintf_s(stderr, L"                     -CRC16_MODBUS               CRC 16 bits Modbus\n");
	fwprintf_s(stderr, L"                     -CRC16_MAXIM                CRC 16 bits Maxim\n");
	fwprintf_s(stderr, L"                     -CRC16_USB                  CRC 16 bits USB\n");
	fwprintf_s(stderr, L"                     -CRC16_T10_DIF              CRC 16 bits T10 DIF\n");
	fwprintf_s(stderr, L"                     -CRC16_DECT_X               CRC 16 bits DECT X\n");
	fwprintf_s(stderr, L"                     -CRC16_DECT_R               CRC 16 bits DECT R\n");
	fwprintf_s(stderr, L"                     -CRC16_SICK                 CRC 16 bits Sick\n");
	fwprintf_s(stderr, L"                     -CRC16_DNP                  CRC 16 bits DNP\n");
	fwprintf_s(stderr, L"                     -CRC16_CCITT_XMODEM         CRC 16 bits CCITT Xmodem\n");
	fwprintf_s(stderr, L"                     -CRC16_CCITT_FFFF           CRC 16 bits CCITT FFFF\n");
	fwprintf_s(stderr, L"                     -CRC16_CCITT_1D0F           CRC 16 bits CCITT 1D0F\n");
	fwprintf_s(stderr, L"                     -CRC16_GENIBUS              CRC 16 bits Genibus\n");
	fwprintf_s(stderr, L"                     -CRC16_KERMIT               CRC 16 bits Kermit\n");
	fwprintf_s(stderr, L"                     -CRC16_X25                  CRC 16 bits X25\n");
	fwprintf_s(stderr, L"                     -CRC16_MCRF4XX              CRC 16 bits MCRF4XX\n");
	fwprintf_s(stderr, L"                     -CRC16_RIELLO               CRC 16 bits Riello\n");
	fwprintf_s(stderr, L"                     -CRC16_FLETCHER             CRC 16 bits Fletcher\n");
	fwprintf_s(stderr, L"                     -CRC24                      = -CRC24_R64\n");
	fwprintf_s(stderr, L"                     -CRC24_FLEXRAY_A            CRC 24 bits Flexray A\n");
	fwprintf_s(stderr, L"                     -CRC24_FLEXRAY_B            CRC 24 bits Flexray B\n");
	fwprintf_s(stderr, L"                     -CRC24_R64                  CRC 24 bits R64\n");
	fwprintf_s(stderr, L"                     -CRC32                      CRC 32 bits\n");
	fwprintf_s(stderr, L"                     -CRC32_JAM                  CRC 32 bits JamCRC\n");
	fwprintf_s(stderr, L"                     -CRC32C                     CRC 32 bits C\n");
	fwprintf_s(stderr, L"                     -CRC32D                     CRC 32 bits D\n");
	fwprintf_s(stderr, L"                     -CRC32_BZIP2                CRC 32 bits BZIP2\n");
	fwprintf_s(stderr, L"                     -CRC32_MPEG2                CRC 32 bits MPEG2\n");
	fwprintf_s(stderr, L"                     -CRC32_POSIX                CRC 32 bits POSIX\n");
	fwprintf_s(stderr, L"                     -CRC32K                     CRC 32 bits K\n");
	fwprintf_s(stderr, L"                     -CRC32Q                     CRC 32 bits Q\n");
	fwprintf_s(stderr, L"                     -CRC40                      CRC 40 bits\n");
	fwprintf_s(stderr, L"                     -CRC64                      CRC 64 bits\n");
	fwprintf_s(stderr, L"                     -CRC64_1B                   CRC 64 bits 1B\n");
	fwprintf_s(stderr, L"                     -CRC64_WE                   CRC 64 bits WE\n");
	fwprintf_s(stderr, L"                     -CRC64_JONES                CRC 64 bits JONES\n");
	fwprintf_s(stderr, L"   * Checksum:       -CHECKSUM                   Internet protocol checksum\n");
	fwprintf_s(stderr, L"   * MD2:            -MD2\n");
	fwprintf_s(stderr, L"   * MD4 family:     -MD4\n");
	fwprintf_s(stderr, L"                     -ED2K                       eDonkey/eMule hash algorithm\n");
	fwprintf_s(stderr, L"   * MD5:            -MD5\n");
	fwprintf_s(stderr, L"   * SHA-1:          -SHA1\n");
	fwprintf_s(stderr, L"   * SHA-2 family:   -SHA2                       = -SHA2_256\n");
	fwprintf_s(stderr, L"                     -SHA224/-SHA2_224           SHA-2 224 bits\n");
	fwprintf_s(stderr, L"                     -SHA256/-SHA2_256           SHA-2 256 bits\n");
	fwprintf_s(stderr, L"                     -SHA384/-SHA2_384           SHA-2 384 bits\n");
	fwprintf_s(stderr, L"                     -SHA512/-SHA2_512           SHA-2 512 bits\n");
	fwprintf_s(stderr, L"                     -SHA512_224/-SHA2_512_224   SHA-2 512/224 bits truncated\n");
	fwprintf_s(stderr, L"                     -SHA512_256/-SHA2_512_256   SHA-2 512/256 bits truncated\n");
	fwprintf_s(stderr, L"   * SHA-3 family:   -SHA3                       = -SHA3_256\n");
	fwprintf_s(stderr, L"                     -SHA3_224                   SHA-3 224 bits\n");
	fwprintf_s(stderr, L"                     -SHA3_256                   SHA-3 256 bits\n");
	fwprintf_s(stderr, L"                     -SHA3_384                   SHA-3 384 bits\n");
	fwprintf_s(stderr, L"                     -SHA3_512                   SHA-3 512 bits\n");
	fwprintf_s(stderr, L"                     -SHA3_SHAKE                 = -SHA3_SHAKE_128\n");
	fwprintf_s(stderr, L"                     -SHA3_SHAKE=Size            = -SHA3_SHAKE_128=Size\n");
	fwprintf_s(stderr, L"                     -SHA3_SHAKE_128=Size        SHA-3 SHAKE 128 bits\n");
	fwprintf_s(stderr, L"                                                 Size = Digest output length\n");
	fwprintf_s(stderr, L"                     -SHA3_SHAKE_256=Size        SHA-3 SHAKE 256 bits\n");
	fwprintf_s(stderr, L"                                                 Size = Digest output length\n");

#if (defined(PLATFORM_LINUX) || defined(PLATFORM_MACX))
	fwprintf_s(stderr, L"\n");
#endif
	return;
}
