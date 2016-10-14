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

//Global variables
extern size_t HashFamilyID;

//Checksum update process
uint32_t Checksum_Update(
	uint32_t Checksum, 
	const uint16_t *Buffer, 
	const size_t Length)
{
	size_t InnerLength = Length;
	while (InnerLength > 1U)
	{ 
		Checksum += *Buffer++;
		InnerLength -= sizeof(uint16_t);
	}

	return Checksum;
}

//Finish checksum hash process
uint16_t Checksum_Final(
	uint32_t Checksum, 
	const uint16_t * const Buffer, 
	const size_t Length)
{
	if (Length)
		Checksum += *(uint8_t *)Buffer;
	Checksum = (Checksum >> (sizeof(uint16_t) * BYTES_TO_BITS)) + (Checksum & UINT16_MAX);
	Checksum += (Checksum >> (sizeof(uint16_t) * BYTES_TO_BITS));

	return (uint16_t)(~Checksum);
}

//////////////////////////////////////////////////
// Hash function
// 
//Internet protocol checksum hash function
bool Checksum_Hash(
	FILE * const FileHandle)
{
//Parameters check
	if (HashFamilyID != HASH_ID_CHECKSUM || FileHandle == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Parameters error.\n");
		return false;
	}

//Initialization
	std::shared_ptr<uint8_t> Buffer(new uint8_t[FILE_BUFFER_SIZE]());
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
	size_t ReadLength = 0;

//Checksum initialization
	uint16_t Checksum16 = 0;
	uint32_t Checksum32 = 0;

//Hash process
	while (!feof(FileHandle))
	{
		memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
		_set_errno(0);
		ReadLength = fread_s(Buffer.get(), FILE_BUFFER_SIZE, sizeof(uint8_t), FILE_BUFFER_SIZE, FileHandle);
		if (ReadLength == 0 && errno != 0)
		{
			std::wstring Message(L"[Error] Read file error");
			ErrorCodeToMessage(errno, Message);
			if (errno == 0)
				fwprintf_s(stderr, Message.c_str());
			else 
				fwprintf_s(stderr, Message.c_str(), errno);

			return false;
		}
		else {
			Checksum32 = Checksum_Update(Checksum32, (uint16_t *)Buffer.get(), ReadLength);
		}
	}

//Finish hash process and binary to hex.
	Checksum16 = Checksum_Final(Checksum32, (uint16_t *)(Buffer.get() + ReadLength - ReadLength % sizeof(uint16_t)), ReadLength % sizeof(uint16_t));
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
	if (sodium_bin2hex(Buffer.get(), FILE_BUFFER_SIZE, (const uint8_t *)&Checksum16, sizeof(uint16_t)) == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Convert binary to hex error.\n");
		return false;
	}
	else {
		PrintToScreen(Buffer.get());
	}

	return true;
}
