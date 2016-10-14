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


#include "MD4.h"

//Initialize the hash state
void MD4_Init(
	MD4_CTX * const c)
{
	memset(c, 0, sizeof(MD4_CTX));
	c->A = INIT_DATA_A;
	c->B = INIT_DATA_B;
	c->C = INIT_DATA_C;
	c->D = INIT_DATA_D;

	return;
}

//MD4 Block data order setting
void MD4_BlockDataOrder(
	MD4_CTX * const c, 
	const void * const data_, 
	size_t num)
{
	const uint8_t *data = (const uint8_t *)data_;
	uint32_t A = 0, B = 0, C = 0, D = 0, l = 0;
	uint32_t XX0 = 0, XX1 = 0, XX2 = 0, XX3 = 0, XX4 = 0, XX5 = 0, XX6 = 0, XX7 = 0, XX8 = 0, XX9 = 0, XX10 = 0, XX11 = 0, XX12 = 0, XX13 = 0, XX14 = 0, XX15 = 0;
#define X(i)   XX ## i

	A = c->A;
	B = c->B;
	C = c->C;
	D = c->D;

	for (;num--;)
	{
		(void)HOST_c2l(data, l); X(0) = l;
		(void)HOST_c2l(data, l); X(1) = l;

	//Round 0
		R0(A, B, C, D, X(0), 3, 0); (void)HOST_c2l(data, l); X(2) = l;
		R0(D, A, B, C, X(1), 7, 0); (void)HOST_c2l(data, l); X(3) = l;
		R0(C, D, A, B, X(2), 11, 0); (void)HOST_c2l(data, l); X(4) = l;
		R0(B, C, D, A, X(3), 19, 0); (void)HOST_c2l(data, l); X(5) = l;
		R0(A, B, C, D, X(4), 3, 0); (void)HOST_c2l(data, l); X(6) = l;
		R0(D, A, B, C, X(5), 7, 0); (void)HOST_c2l(data, l); X(7) = l;
		R0(C, D, A, B, X(6), 11, 0); (void)HOST_c2l(data, l); X(8) = l;
		R0(B, C, D, A, X(7), 19, 0); (void)HOST_c2l(data, l); X(9) = l;
		R0(A, B, C, D, X(8), 3, 0); (void)HOST_c2l(data, l); X(10) = l;
		R0(D, A, B, C, X(9), 7, 0); (void)HOST_c2l(data, l); X(11) = l;
		R0(C, D, A, B, X(10), 11, 0); (void)HOST_c2l(data, l); X(12) = l;
		R0(B, C, D, A, X(11), 19, 0); (void)HOST_c2l(data, l); X(13) = l;
		R0(A, B, C, D, X(12), 3, 0); (void)HOST_c2l(data, l); X(14) = l;
		R0(D, A, B, C, X(13), 7, 0); (void)HOST_c2l(data, l); X(15) = l;
		R0(C, D, A, B, X(14), 11, 0);
		R0(B, C, D, A, X(15), 19, 0);

	//Round 1
		R1(A, B, C, D, X(0), 3, 0x5A827999L);
		R1(D, A, B, C, X(4), 5, 0x5A827999L);
		R1(C, D, A, B, X(8), 9, 0x5A827999L);
		R1(B, C, D, A, X(12), 13, 0x5A827999L);
		R1(A, B, C, D, X(1), 3, 0x5A827999L);
		R1(D, A, B, C, X(5), 5, 0x5A827999L);
		R1(C, D, A, B, X(9), 9, 0x5A827999L);
		R1(B, C, D, A, X(13), 13, 0x5A827999L);
		R1(A, B, C, D, X(2), 3, 0x5A827999L);
		R1(D, A, B, C, X(6), 5, 0x5A827999L);
		R1(C, D, A, B, X(10), 9, 0x5A827999L);
		R1(B, C, D, A, X(14), 13, 0x5A827999L);
		R1(A, B, C, D, X(3), 3, 0x5A827999L);
		R1(D, A, B, C, X(7), 5, 0x5A827999L);
		R1(C, D, A, B, X(11), 9, 0x5A827999L);
		R1(B, C, D, A, X(15), 13, 0x5A827999L);

	//Round 2
		R2(A, B, C, D, X(0), 3, 0x6ED9EBA1L);
		R2(D, A, B, C, X(8), 9, 0x6ED9EBA1L);
		R2(C, D, A, B, X(4), 11, 0x6ED9EBA1L);
		R2(B, C, D, A, X(12), 15, 0x6ED9EBA1L);
		R2(A, B, C, D, X(2), 3, 0x6ED9EBA1L);
		R2(D, A, B, C, X(10), 9, 0x6ED9EBA1L);
		R2(C, D, A, B, X(6), 11, 0x6ED9EBA1L);
		R2(B, C, D, A, X(14), 15, 0x6ED9EBA1L);
		R2(A, B, C, D, X(1), 3, 0x6ED9EBA1L);
		R2(D, A, B, C, X(9), 9, 0x6ED9EBA1L);
		R2(C, D, A, B, X(5), 11, 0x6ED9EBA1L);
		R2(B, C, D, A, X(13), 15, 0x6ED9EBA1L);
		R2(A, B, C, D, X(3), 3, 0x6ED9EBA1L);
		R2(D, A, B, C, X(11), 9, 0x6ED9EBA1L);
		R2(C, D, A, B, X(7), 11, 0x6ED9EBA1L);
		R2(B, C, D, A, X(15), 15, 0x6ED9EBA1L);

		A = c->A += A;
		B = c->B += B;
		C = c->C += C;
		D = c->D += D;
	}

	return;
}

//Update MD4 status
void MD4_Update(
	MD4_CTX * const c, 
	const void * const data_, 
	size_t len)
{
	const uint8_t *data = (const uint8_t *)data_;
	uint8_t *p = nullptr;
	MD4_LONG l = 0;
	size_t n = 0;

	if (len == 0)
		return;
	l = (c->Nl + (((MD4_LONG)len) << 3U)) & 0xFFFFFFFFUL;
	if (l < c->Nl)
		c->Nh++;
	c->Nh += (MD4_LONG)(len >> 29U);
	c->Nl = l;
	n = c->Num;
	if (n != 0)
	{
		p = (uint8_t *)c->Data;
		if (len >= MD4_BLOCK_SIZE || len + n >= MD4_BLOCK_SIZE)
		{
			memcpy(p + n, data, MD4_BLOCK_SIZE - n);
			MD4_BlockDataOrder(c, p, 1);
			n = MD4_BLOCK_SIZE - n;
			data += n;
			len -= n;
			c->Num = 0;
			memset(p, 0, MD4_BLOCK_SIZE); //Keep it zeroed.
		}
		else {
			memcpy(p + n, data, len);
			c->Num += (unsigned int)len;
			return;
		}
	}
	n = len / MD4_BLOCK_SIZE;
	if (n > 0)
	{
		MD4_BlockDataOrder(c, data, n);
		n *= MD4_BLOCK_SIZE;
		data += n;
		len -= n;
	}
	if (len != 0)
	{
		p = (uint8_t *)c->Data;
		c->Num = (unsigned int)len;
		memcpy(p, data, len);
	}

	return;
}

//Finish MD4 process
void MD4_Final(
	uint8_t *md, 
	MD4_CTX * const c)
{
	uint8_t *p = (uint8_t *)c->Data;
	size_t n = c->Num;

	p[n] = 0x80; //There is always room for one.
	n++;
	if (n > (MD4_BLOCK_SIZE - 8U))
	{
		memset(p + n, 0, MD4_BLOCK_SIZE - n);
		n = 0;
		MD4_BlockDataOrder(c, p, 1);
	}
	memset(p + n, 0, MD4_BLOCK_SIZE - 8U - n);
	p += MD4_BLOCK_SIZE - 8U;
#if BYTE_ORDER == LITTLE_ENDIAN
	(void)HOST_l2c(c->Nl, p);
	(void)HOST_l2c(c->Nh, p);
#else
	(void)HOST_l2c(c->Nh, p);
	(void)HOST_l2c(c->Nl, p);
#endif
	p -= MD4_BLOCK_SIZE;
	MD4_BlockDataOrder(c, p, 1U);
	c->Num = 0;
	memset(p, 0, MD4_BLOCK_SIZE);
	HASH_MAKE_STRING(c, md);

	return;
}

//////////////////////////////////////////////////
// Hash function
// 
//MD4 hash function
bool MD4_Hash(
	FILE * const FileHandle)
{
//Parameters check
	if ((HashFamilyID != HASH_ID_MD4 && HashFamilyID != HASH_ID_ED2K) || FileHandle == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Parameters error.\n");
		return false;
	}

//Initialization
	size_t ReadBlockSize = FILE_BUFFER_SIZE, ReadLength = 0, RoundCount = 0;
	if (HashFamilyID == HASH_ID_ED2K)
		ReadBlockSize = ED2K_SIZE_BLOCK;
	std::shared_ptr<uint8_t> Buffer(new uint8_t[ReadBlockSize]()), StringBuffer(new uint8_t[FILE_BUFFER_SIZE]()), BufferED2K(new uint8_t[MD4_DIGEST_SIZE]());
	memset(Buffer.get(), 0, ReadBlockSize);
	memset(StringBuffer.get(), 0, FILE_BUFFER_SIZE);
	memset(BufferED2K.get(), 0, MD4_DIGEST_SIZE);

//MD4 initialization
	MD4_CTX HashInstance, HashInstanceED2K;
	memset(&HashInstance, 0, sizeof(HashInstance));
	memset(&HashInstanceED2K, 0, sizeof(HashInstanceED2K));
	MD4_Init(&HashInstance);
	if (HashFamilyID == HASH_ID_ED2K)
		MD4_Init(&HashInstanceED2K);

//Hash process
	while (!feof(FileHandle))
	{
		memset(Buffer.get(), 0, ReadBlockSize);
		_set_errno(0);
		ReadLength = fread_s(Buffer.get(), ReadBlockSize, sizeof(uint8_t), ReadBlockSize, FileHandle);
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
			MD4_Update(&HashInstance, Buffer.get(), ReadLength);
			if (HashFamilyID == HASH_ID_ED2K)
			{
				MD4_Final(Buffer.get(), &HashInstance);
				memcpy_s(BufferED2K.get(), MD4_DIGEST_SIZE, Buffer.get(), MD4_DIGEST_SIZE);
				MD4_Update(&HashInstanceED2K, Buffer.get(), MD4_DIGEST_SIZE);
				MD4_Init(&HashInstance);
			}

			++RoundCount;
		}
	}

//Finish hash process.
	memset(Buffer.get(), 0, ReadBlockSize);
	if (HashFamilyID == HASH_ID_MD4)
	{
		MD4_Final(Buffer.get(), &HashInstance);
	}
	else if (HashFamilyID == HASH_ID_ED2K)
	{
		if (RoundCount > 1U)
			MD4_Final(Buffer.get(), &HashInstanceED2K);
		else 
			memcpy_s(Buffer.get(), MD4_DIGEST_SIZE, BufferED2K.get(), MD4_DIGEST_SIZE);
	}
	else {
		fwprintf_s(stderr, L"[Error] Parameters error.\n");
		return false;
	}

//Binary to hex
	if (sodium_bin2hex(StringBuffer.get(), FILE_BUFFER_SIZE, (const uint8_t *)Buffer.get(), MD4_DIGEST_SIZE) == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Convert binary to hex error.\n");
		return false;
	}
	else {
		PrintToScreen(StringBuffer.get());
	}

	return true;
}
