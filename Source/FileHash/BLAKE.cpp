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


#include "BLAKE.h"

//BLAKE-224 compress process
void BLAKE_224_Compress(
	state224 * const S, 
	const uint8_t * const block)
{
	uint32_t v[16U]{0}, m[16U]{0}, i = 0;
#define ROT(x, n) (((x) << (32 - n)) | ((x) >> (n)))
#define G(a, b, c, d, e)                                       \
	v[a] += (m[sigma[i][e]] ^ u256[sigma[i][e + 1]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 16);                               \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 12);                               \
	v[a] += (m[sigma[i][e + 1]] ^ u256[sigma[i][e]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 8);                                \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 7);

	for (i = 0;i < 16;++i)
		m[i] = U8TO32_BIG(block + i * 4);
	for (i = 0;i < 8;++i)
		v[i] = S->h[i];

	v[8U] = S->s[0] ^ u256[0];
	v[9U] = S->s[1U] ^ u256[1U];
	v[10U] = S->s[2U] ^ u256[2U];
	v[11U] = S->s[3U] ^ u256[3U];
	v[12U] = u256[4U];
	v[13U] = u256[5U];
	v[14U] = u256[6U];
	v[15U] = u256[7U];

//Don't xor t when the block is only padding
	if (!S->nullt)
	{
		v[12U] ^= S->t[0];
		v[13U] ^= S->t[0];
		v[14U] ^= S->t[1U];
		v[15U] ^= S->t[1U];
	}

	for (i = 0;i < 14;++i)
	{
	//Column step
		G(0, 4, 8, 12, 0);
		G(1, 5, 9, 13, 2);
		G(2, 6, 10, 14, 4);
		G(3, 7, 11, 15, 6);
	//Diagonal step
		G(0, 5, 10, 15, 8);
		G(1, 6, 11, 12, 10);
		G(2, 7, 8, 13, 12);
		G(3, 4, 9, 14, 14);
#undef G
#undef ROT
	}

	for (i = 0;i < 16;++i)
		S->h[i % 8] ^= v[i];
	for (i = 0;i < 8;++i)
		S->h[i] ^= S->s[i % 4];

	return;
}

//BLAKE init process
void BLAKE_224_Init(
	state224 * const S)
{
	S->h[0] = 0xC1059ED8;
	S->h[1U] = 0x367CD507;
	S->h[2U] = 0x3070DD17;
	S->h[3U] = 0xF70E5939;
	S->h[4U] = 0xFFC00B31;
	S->h[5U] = 0x68581511;
	S->h[6U] = 0x64F98FA7;
	S->h[7U] = 0xBEFA4FA4;
	S->t[0] = S->t[1U] = S->buflen = S->nullt = 0;
	S->s[0] = S->s[1U] = S->s[2U] = S->s[3U] = 0;

	return;
}

//BLAKE-224 update process
void BLAKE_224_Update(
	state224 * const S, 
	const uint8_t *in, 
	uint64_t inlen)
{
	int left = S->buflen, fill = 64 - left;

//Data left and data received fill a block
	if (left && (inlen >= fill))
	{
		memcpy((void *)(S->buf + left), (void *)in, fill);
		S->t[0] += 512;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_224_Compress(S, S->buf);
		in += fill;
		inlen -= fill;
		left = 0;
	}

//Compress blocks of data received.
	while (inlen >= 64)
	{
		S->t[0] += 512;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_224_Compress(S, in);
		in += 64;
		inlen -= 64;
	}

//Store any data left.
	if (inlen > 0)
	{
		memcpy((void *)(S->buf + left), (void *)in, (size_t)inlen);
		S->buflen = left + (int)inlen;
	}
	else {
		S->buflen = 0;
	}

	return;
}

//BLAKE-224 final process
void BLAKE_224_Final(
	state224 * const S, 
	uint8_t * const out)
{
	uint8_t msglen[8U], zz = 0x00, oz = 0x80;
	uint32_t lo = S->t[0] + (S->buflen << 3U), hi = S->t[1U];

//Support for hashing more than 2^32 bits
	if (lo < (uint32_t)(S->buflen << 3U))
		hi++;

	U32TO8_BIG(msglen + 0, hi);
	U32TO8_BIG(msglen + 4, lo);

//One padding byte
	if (S->buflen == 55)
	{
		S->t[0] -= 8;
		BLAKE_224_Update(S, &oz, 1);
	}
	else {
	//Enough space to fill the block
		if (S->buflen < 55)
		{
			if (!S->buflen)
				S->nullt = 1;

			S->t[0] -= 440 - (S->buflen << 3U);
			BLAKE_224_Update(S, padding, 55 - S->buflen);
		}
	//Need 2 compressions
		else {
			S->t[0] -= 512 - (S->buflen << 3U);
			BLAKE_224_Update(S, padding, 64 - S->buflen);
			S->t[0] -= 440;
			BLAKE_224_Update(S, padding + 1, 55);
			S->nullt = 1;
		}

		BLAKE_224_Update(S, &zz, 1);
		S->t[0] -= 8;
	}

	S->t[0] -= 64;
	BLAKE_224_Update(S, msglen, 8);
	U32TO8_BIG(out + 0, S->h[0]);
	U32TO8_BIG(out + 4, S->h[1U]);
	U32TO8_BIG(out + 8, S->h[2U]);
	U32TO8_BIG(out + 12, S->h[3U]);
	U32TO8_BIG(out + 16, S->h[4U]);
	U32TO8_BIG(out + 20, S->h[5U]);
	U32TO8_BIG(out + 24, S->h[6U]);

	return;
}

//BLAKE-256 compress process
void BLAKE_256_Compress(
	state256 * const S, 
	const uint8_t * const block)
{
	uint32_t v[16U]{0}, m[16U]{0}, i = 0;
#define ROT(x, n) (((x) << (32 - n)) | ((x) >> (n)))
#define G(a, b, c, d, e)                                       \
	v[a] += (m[sigma[i][e]] ^ u256[sigma[i][e + 1]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 16);                               \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 12);                               \
	v[a] += (m[sigma[i][e + 1]] ^ u256[sigma[i][e]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 8);                                \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 7);

	for (i = 0;i < 16;++i)
		m[i] = U8TO32_BIG(block + i * 4);
	for (i = 0;i < 8;++i)
		v[i] = S->h[i];

	v[8U] = S->s[0] ^ u256[0];
	v[9U] = S->s[1U] ^ u256[1U];
	v[10U] = S->s[2U] ^ u256[2U];
	v[11U] = S->s[3U] ^ u256[3U];
	v[12U] = u256[4U];
	v[13U] = u256[5U];
	v[14U] = u256[6U];
	v[15U] = u256[7U];

//Don't xor t when the block is only padding
	if (!S->nullt)
	{
		v[12U] ^= S->t[0];
		v[13U] ^= S->t[0];
		v[14U] ^= S->t[1U];
		v[15U] ^= S->t[1U];
	}

	for (i = 0;i < 14;++i)
	{
	//Column step
		G(0, 4, 8, 12, 0);
		G(1, 5, 9, 13, 2);
		G(2, 6, 10, 14, 4);
		G(3, 7, 11, 15, 6);
	//Diagonal step
		G(0, 5, 10, 15, 8);
		G(1, 6, 11, 12, 10);
		G(2, 7, 8, 13, 12);
		G(3, 4, 9, 14, 14);
#undef G
#undef ROT
	}

	for (i = 0;i < 16;++i)
		S->h[i % 8] ^= v[i];
	for (i = 0;i < 8;++i)
		S->h[i] ^= S->s[i % 4];

	return;
}

//BLAKE-256 init process
void BLAKE_256_Init(
	state256 * const S)
{
	S->h[0] = 0x6A09E667;
	S->h[1U] = 0xBB67AE85;
	S->h[2U] = 0x3C6EF372;
	S->h[3U] = 0xA54FF53A;
	S->h[4U] = 0x510E527F;
	S->h[5U] = 0x9B05688C;
	S->h[6U] = 0x1F83D9AB;
	S->h[7U] = 0x5BE0CD19;
	S->t[0] = S->t[1U] = S->buflen = S->nullt = 0;
	S->s[0] = S->s[1U] = S->s[2U] = S->s[3U] = 0;

	return;
}

//BLAKE-256 update process
void BLAKE_256_Update(
	state256 * const S, 
	const uint8_t *in, 
	uint64_t inlen)
{
	int left = S->buflen, fill = 64 - left;

//Data left and data received fill a block
	if (left && (inlen >= fill))
	{
		memcpy((void *)(S->buf + left), (void *)in, fill);
		S->t[0] += 512;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_256_Compress(S, S->buf);
		in += fill;
		inlen -= fill;
		left = 0;
	}

//Compress blocks of data received.
	while (inlen >= 64)
	{
		S->t[0] += 512;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_256_Compress(S, in);
		in += 64;
		inlen -= 64;
	}

//Store any data left.
	if (inlen > 0)
	{
		memcpy((void *)(S->buf + left), (void *)in, (size_t)inlen);
		S->buflen = left + (int)inlen;
	}
	else {
		S->buflen = 0;
	}

	return;
}

//BLAKE-256 final process
void BLAKE_256_Final(
	state256 * const S, 
	uint8_t * const out)
{
	uint8_t msglen[8U], zo = 0x01, oo = 0x81;
	uint32_t lo = S->t[0] + (S->buflen << 3U), hi = S->t[1U];

//Support for hashing more than 2^32 bits
	if (lo < (uint32_t)(S->buflen << 3U))
		hi++;

	U32TO8_BIG(msglen + 0, hi);
	U32TO8_BIG(msglen + 4, lo);

//One padding byte
	if (S->buflen == 55)
	{
		S->t[0] -= 8;
		BLAKE_256_Update(S, &oo, 1);
	}
	else {
	//Enough space to fill the block
		if (S->buflen < 55)
		{
			if (!S->buflen)
				S->nullt = 1;

			S->t[0] -= 440 - (S->buflen << 3U);
			BLAKE_256_Update(S, padding, 55 - S->buflen);
		}
	//Need 2 compressions
		else {
			S->t[0] -= 512 - (S->buflen << 3U);
			BLAKE_256_Update(S, padding, 64 - S->buflen);
			S->t[0] -= 440;
			BLAKE_256_Update(S, padding + 1, 55);
			S->nullt = 1;
		}

		BLAKE_256_Update(S, &zo, 1);
		S->t[0] -= 8;
	}

	S->t[0] -= 64;
	BLAKE_256_Update(S, msglen, 8);
	U32TO8_BIG(out + 0, S->h[0]);
	U32TO8_BIG(out + 4, S->h[1U]);
	U32TO8_BIG(out + 8, S->h[2U]);
	U32TO8_BIG(out + 12, S->h[3U]);
	U32TO8_BIG(out + 16, S->h[4U]);
	U32TO8_BIG(out + 20, S->h[5U]);
	U32TO8_BIG(out + 24, S->h[6U]);
	U32TO8_BIG(out + 28, S->h[7U]);

	return;
}

//BLAKE-384 compress process
void BLAKE_384_Compress(
	state384 * const S, 
	const uint8_t * const block)
{
	uint64_t v[16U]{0}, m[16U]{0}, i = 0;
#define ROT(x, n) (((x) << (64 - n)) | ((x) >> (n)))
#define G(a, b, c, d, e)                                       \
	v[a] += (m[sigma[i][e]] ^ u512[sigma[i][e + 1]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 32);                               \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 25);                               \
	v[a] += (m[sigma[i][e + 1]] ^ u512[sigma[i][e]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 16);                               \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 11);

	for (i = 0;i < 16;++i)
		m[i] = U8TO64_BIG(block + i * 8);
	for (i = 0;i < 8;++i)
		v[i] = S->h[i];

	v[8U] = S->s[0] ^ u512[0];
	v[9U] = S->s[1U] ^ u512[1U];
	v[10U] = S->s[2U] ^ u512[2U];
	v[11U] = S->s[3U] ^ u512[3U];
	v[12U] = u512[4U];
	v[13U] = u512[5U];
	v[14U] = u512[6U];
	v[15U] = u512[7U];

//Don't xor t when the block is only padding
	if (!S->nullt)
	{
		v[12U] ^= S->t[0];
		v[13U] ^= S->t[0];
		v[14U] ^= S->t[1U];
		v[15U] ^= S->t[1U];
	}

	for (i = 0;i < 16;++i)
	{
	//Column step
		G(0, 4, 8, 12, 0);
		G(1, 5, 9, 13, 2);
		G(2, 6, 10, 14, 4);
		G(3, 7, 11, 15, 6);
	//Diagonal step
		G(0, 5, 10, 15, 8);
		G(1, 6, 11, 12, 10);
		G(2, 7, 8, 13, 12);
		G(3, 4, 9, 14, 14);
#undef G
#undef ROT
	}

	for (i = 0;i < 16;++i)
		S->h[i % 8] ^= v[i];
	for (i = 0;i < 8;++i)
		S->h[i] ^= S->s[i % 4];

	return;
}

//BLAKE-384 init process
void BLAKE_384_Init(
	state384 * const S)
{
	S->h[0] = 0xCBBB9D5DC1059ED8ULL;
	S->h[1U] = 0x629A292A367CD507ULL;
	S->h[2U] = 0x9159015A3070DD17ULL;
	S->h[3U] = 0x152FECD8F70E5939ULL;
	S->h[4U] = 0x67332667FFC00B31ULL;
	S->h[5U] = 0x8EB44A8768581511ULL;
	S->h[6U] = 0xDB0C2E0D64F98FA7ULL;
	S->h[7U] = 0x47B5481DBEFA4FA4ULL;
	S->t[0] = S->t[1U] = S->buflen = S->nullt = 0;
	S->s[0] = S->s[1U] = S->s[2U] = S->s[3U] = 0;

	return;
}

//BLAKE-384 update process
void BLAKE_384_Update(
	state384 * const S, 
	const uint8_t *in, 
	uint64_t inlen)
{
	int left = S->buflen, fill = 128 - left;

//Data left and data received fill a block
	if (left && (inlen >= fill))
	{
		memcpy((void *)(S->buf + left), (void *)in, fill);
		S->t[0] += 1024;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_384_Compress(S, S->buf);
		in += fill;
		inlen -= fill;
		left = 0;
	}

//Compress blocks of data received.
	while (inlen >= 128)
	{
		S->t[0] += 1024;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_384_Compress(S, in);
		in += 128;
		inlen -= 128;
	}

//Store any data left.
	if (inlen > 0)
	{
		memcpy((void *)(S->buf + left), (void *)in, (size_t)inlen);
		S->buflen = left + (int)inlen;
	}
	else {
		S->buflen = 0;
	}

	return;
}

//BLAKE-384 final process
void BLAKE_384_Final(
	state384 * const S, 
	uint8_t * const out)
{
	uint8_t msglen[16U], zz = 0x00, oz = 0x80;
	uint64_t lo = S->t[0] + (((uint64_t)S->buflen) << 3U), hi = S->t[1U];

//Support for hashing more than 2^32 bits
	if (lo < (((uint64_t)S->buflen) << 3U))
		hi++;

	U64TO8_BIG(msglen + 0, hi);
	U64TO8_BIG(msglen + 8, lo);

//One padding byte
	if (S->buflen == 111)
	{
		S->t[0] -= 8;
		BLAKE_384_Update(S, &oz, 1);
	}
	else {
	//Enough space to fill the block
		if (S->buflen < 111)
		{
			if (!S->buflen) S->nullt = 1;

			S->t[0] -= 888 - (S->buflen << 3U);
			BLAKE_384_Update(S, padding, 111 - S->buflen);
		}
	//Need 2 compressions
		else {
			S->t[0] -= 1024 - (S->buflen << 3U);
			BLAKE_384_Update(S, padding, 128 - S->buflen);
			S->t[0] -= 888;
			BLAKE_384_Update(S, padding + 1, 111);
			S->nullt = 1;
		}

		BLAKE_384_Update(S, &zz, 1);
		S->t[0] -= 8;
	}

	S->t[0] -= 128;
	BLAKE_384_Update(S, msglen, 16);
	U64TO8_BIG(out + 0, S->h[0]);
	U64TO8_BIG(out + 8, S->h[1U]);
	U64TO8_BIG(out + 16, S->h[2U]);
	U64TO8_BIG(out + 24, S->h[3U]);
	U64TO8_BIG(out + 32, S->h[4U]);
	U64TO8_BIG(out + 40, S->h[5U]);

	return;
}

//BLAKE-512 compress process
void BLAKE_512_Compress(
	state512 * const S, 
	const uint8_t * const block)
{
	uint64_t v[16U]{0}, m[16U]{0}, i = 0;
#define ROT(x, n) (((x) << (64 - n)) | ((x) >> (n)))
#define G(a, b, c, d, e)                                       \
	v[a] += (m[sigma[i][e]] ^ u512[sigma[i][e + 1]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 32);                               \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 25);                               \
	v[a] += (m[sigma[i][e + 1]] ^ u512[sigma[i][e]]) + v[b];   \
	v[d] = ROT(v[d] ^ v[a], 16);                               \
	v[c] += v[d];                                              \
	v[b] = ROT(v[b] ^ v[c], 11);

	for (i = 0;i < 16;++i)
		m[i] = U8TO64_BIG(block + i * 8);
	for (i = 0;i < 8;++i)
		v[i] = S->h[i];

	v[8U] = S->s[0] ^ u512[0];
	v[9U] = S->s[1U] ^ u512[1U];
	v[10U] = S->s[2U] ^ u512[2U];
	v[11U] = S->s[3U] ^ u512[3U];
	v[12U] = u512[4U];
	v[13U] = u512[5U];
	v[14U] = u512[6U];
	v[15U] = u512[7U];

//Don't xor t when the block is only padding
	if (!S->nullt)
	{
		v[12U] ^= S->t[0];
		v[13U] ^= S->t[0];
		v[14U] ^= S->t[1U];
		v[15U] ^= S->t[1U];
	}

	for (i = 0;i < 16;++i)
	{
	//Column step
		G(0, 4, 8, 12, 0);
		G(1, 5, 9, 13, 2);
		G(2, 6, 10, 14, 4);
		G(3, 7, 11, 15, 6);
	//Diagonal step
		G(0, 5, 10, 15, 8);
		G(1, 6, 11, 12, 10);
		G(2, 7, 8, 13, 12);
		G(3, 4, 9, 14, 14);
#undef G
#undef ROT
	}

	for (i = 0;i < 16;++i)
		S->h[i % 8] ^= v[i];
	for (i = 0;i < 8;++i)
		S->h[i] ^= S->s[i % 4];

	return;
}

//BLAKE-512 init process
void BLAKE_512_Init(
	state512 * const S)
{
	S->h[0] = 0x6A09E667F3BCC908ULL;
	S->h[1U] = 0xBB67AE8584CAA73BULL;
	S->h[2U] = 0x3C6EF372FE94F82BULL;
	S->h[3U] = 0xA54FF53A5F1D36F1ULL;
	S->h[4U] = 0x510E527FADE682D1ULL;
	S->h[5U] = 0x9B05688C2B3E6C1FULL;
	S->h[6U] = 0x1F83D9ABFB41BD6BULL;
	S->h[7U] = 0x5BE0CD19137E2179ULL;
	S->t[0] = S->t[1U] = S->buflen = S->nullt = 0;
	S->s[0] = S->s[1U] = S->s[2U] = S->s[3U] = 0;

	return;
}

//BLAKE-512 update process
void BLAKE_512_Update(
	state512 * const S, 
	const uint8_t *in, 
	uint64_t inlen)
{
	int left = S->buflen, fill = 128 - left;

//Data left and data received fill a block
	if (left && (inlen >= fill))
	{
		memcpy((void *)(S->buf + left), (void *)in, fill);
		S->t[0] += 1024;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_512_Compress(S, S->buf);
		in += fill;
		inlen -= fill;
		left = 0;
	}

//Compress blocks of data received.
	while (inlen >= 128)
	{
		S->t[0] += 1024;

		if (S->t[0] == 0)
			S->t[1U]++;

		BLAKE_512_Compress(S, in);
		in += 128;
		inlen -= 128;
	}

//Store any data left.
	if (inlen > 0)
	{
		memcpy((void *)(S->buf + left), (void *)in, (size_t)inlen);
		S->buflen = left + (int)inlen;
	}
	else {
		S->buflen = 0;
	}

	return;
}

//BLAKE-512 final process
void BLAKE_512_Final(
	state512 * const S, 
	uint8_t * const out)
{
	uint8_t msglen[16U], zo = 0x01, oo = 0x81;
	uint64_t lo = S->t[0] + (((uint64_t)S->buflen) << 3U), hi = S->t[1U];

//Support for hashing more than 2^32 bits
	if (lo < (((uint64_t)S->buflen) << 3U))
		hi++;

	U64TO8_BIG(msglen + 0, hi);
	U64TO8_BIG(msglen + 8, lo);

//One padding byte
	if (S->buflen == 111)
	{
		S->t[0] -= 8;
		BLAKE_512_Update(S, &oo, 1);
	}
	else {
	//Enough space to fill the block
		if (S->buflen < 111)
		{
			if (!S->buflen) S->nullt = 1;

			S->t[0] -= 888 - (S->buflen << 3U);
			BLAKE_512_Update(S, padding, 111 - S->buflen);
		}
	//Need 2 compressions
		else {
			S->t[0] -= 1024 - (S->buflen << 3U);
			BLAKE_512_Update(S, padding, 128 - S->buflen);
			S->t[0] -= 888;
			BLAKE_512_Update(S, padding + 1, 111);
			S->nullt = 1;
		}

		BLAKE_512_Update(S, &zo, 1);
		S->t[0] -= 8;
	}

	S->t[0] -= 128;
	BLAKE_512_Update(S, msglen, 16);
	U64TO8_BIG(out + 0, S->h[0]);
	U64TO8_BIG(out + 8, S->h[1U]);
	U64TO8_BIG(out + 16, S->h[2U]);
	U64TO8_BIG(out + 24, S->h[3U]);
	U64TO8_BIG(out + 32, S->h[4U]);
	U64TO8_BIG(out + 40, S->h[5U]);
	U64TO8_BIG(out + 48, S->h[6U]);
	U64TO8_BIG(out + 56, S->h[7U]);

	return;
}

//////////////////////////////////////////////////
// Hash function
// 
//Read commands(BLAKE)
bool ReadCommand_BLAKE(
#if defined(PLATFORM_WIN)
	std::wstring &Command)
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command)
#endif
{
//Hash function check
	if (Command == COMMAND_BLAKE_224) //BLAKE 224 bits
	{
		BLAKE_HashFunctionID = HASH_ID_BLAKE_224;
	}
	else if (Command == HASH_COMMAND_BLAKE || Command == COMMAND_BLAKE_256) //BLAKE 256 bits
	{
		BLAKE_HashFunctionID = HASH_ID_BLAKE_256;
	}
	else if (Command == COMMAND_BLAKE_384) //BLAKE 384 bits
	{
		BLAKE_HashFunctionID = HASH_ID_BLAKE_384;
	}
	else if (Command == COMMAND_BLAKE_512) //BLAKE 512 bits
	{
		BLAKE_HashFunctionID = HASH_ID_BLAKE_512;
	}
	else { //Commands error
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return false;
	}

	return true;
}

//BLAKE hash function
bool BLAKE_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile)
{
//Parameters check
	if (HashFamilyID != HASH_ID_BLAKE || FileHandle == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Parameters error.\n");
		return false;
	}

//Initialization
	std::unique_ptr<uint8_t[]> Buffer(new uint8_t[FILE_BUFFER_SIZE]());
	std::unique_ptr<uint8_t[]> StringBuffer(new uint8_t[FILE_BUFFER_SIZE]());
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
	memset(StringBuffer.get(), 0, FILE_BUFFER_SIZE);
	size_t ReadLength = 0, DigestSize = 0;

//BLAKE initialization
	state224 State224;
	state256 State256;
	state384 State384;
	state512 State512;
	memset(&State224, 0, sizeof(State224));
	memset(&State256, 0, sizeof(State256));
	memset(&State384, 0, sizeof(State384));
	memset(&State512, 0, sizeof(State512));
	if (BLAKE_HashFunctionID == HASH_ID_BLAKE_224) //BLAKE 224 bits
	{
		BLAKE_224_Init(&State224);
		DigestSize = BLAKE_DIGEST_SIZE_224;
	}
	else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_256) //BLAKE 256 bits
	{
		BLAKE_256_Init(&State256);
		DigestSize = BLAKE_DIGEST_SIZE_256;
	}
	else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_384) //BLAKE 384 bits
	{
		BLAKE_384_Init(&State384);
		DigestSize = BLAKE_DIGEST_SIZE_384;
	}
	else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_512) //BLAKE 512 bits
	{
		BLAKE_512_Init(&State512);
		DigestSize = BLAKE_DIGEST_SIZE_512;
	}
	else { //Commands error
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return false;
	}

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
			if (BLAKE_HashFunctionID == HASH_ID_BLAKE_224) //BLAKE 224 bits
			{
				BLAKE_224_Update(&State224, Buffer.get(), ReadLength);
			}
			else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_256) //BLAKE 256 bits
			{
				BLAKE_256_Update(&State256, Buffer.get(), ReadLength);
			}
			else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_384) //BLAKE 384 bits
			{
				BLAKE_384_Update(&State384, Buffer.get(), ReadLength);
			}
			else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_512) //BLAKE 512 bits
			{
				BLAKE_512_Update(&State512, Buffer.get(), ReadLength);
			}
			else { //Commands error
				fwprintf_s(stderr, L"[Error] Commands error.\n");
				return false;
			}
		}
	}

//Finish hash process.
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
	if (BLAKE_HashFunctionID == HASH_ID_BLAKE_224) //BLAKE 224 bits
	{
		BLAKE_224_Final(&State224, Buffer.get());
	}
	else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_256) //BLAKE 256 bits
	{
		BLAKE_256_Final(&State256, Buffer.get());
	}
	else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_384) //BLAKE 384 bits
	{
		BLAKE_384_Final(&State384, Buffer.get());
	}
	else if (BLAKE_HashFunctionID == HASH_ID_BLAKE_512) //BLAKE 512 bits
	{
		BLAKE_512_Final(&State512, Buffer.get());
	}
	else { //Commands error
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return false;
	}

//Binary to hex
	if (sodium_bin2hex(StringBuffer.get(), FILE_BUFFER_SIZE, Buffer.get(), DigestSize / BYTES_TO_BITS) == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Convert binary to hex error.\n");
		return false;
	}
	else {
	//Lowercase convert.
		std::string Hex(reinterpret_cast<const char *>(StringBuffer.get()));
		if (!IsLowerCase)
			CaseConvert(Hex, true);

	//Print to screen and file.
		WriteMessage_ScreenFile(stderr, reinterpret_cast<const uint8_t *>(Hex.c_str()));
		if (OutputFile != nullptr)
			WriteMessage_ScreenFile(OutputFile, reinterpret_cast<const uint8_t *>(Hex.c_str()));
	}

	return true;
}
