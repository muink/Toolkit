// This code is part of Toolkit(FileHash)
// FileHash, a useful and powerful toolkit
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


#include "RIPEMD.h"

//RIPEMD-128 init process
void MD_Init_128(
	dword * const MDbuf)
{
	MDbuf[0] = 0x67452301UL;
	MDbuf[1U] = 0xEFCDAB89UL;
	MDbuf[2U] = 0x98BADCFEUL;
	MDbuf[3U] = 0x10325476UL;

	return;
}

//RIPEMD-160 init process
void MD_Init_160(
	dword * const MDbuf)
{
	MDbuf[0] = 0x67452301UL;
	MDbuf[1U] = 0xEFCDAB89UL;
	MDbuf[2U] = 0x98BADCFEUL;
	MDbuf[3U] = 0x10325476UL;
	MDbuf[4U] = 0xC3D2E1F0UL;

	return;
}

//RIPEMD-256 init process
void MD_Init_256(
	dword * const MDbuf)
{
	MD_Init_128(MDbuf);
	MDbuf[4U] = 0x76543210UL;
	MDbuf[5U] = 0xFEDCBA98UL;
	MDbuf[6U] = 0x89ABCDEFUL;
	MDbuf[7U] = 0x01234567UL;

	return;
}

//RIPEMD-320 init process
void MD_Init_320(
	dword * const MDbuf)
{
	MD_Init_160(MDbuf);
	MDbuf[5U] = 0x76543210UL;
	MDbuf[6U] = 0xFEDCBA98UL;
	MDbuf[7U] = 0x89ABCDEFUL;
	MDbuf[8U] = 0x01234567UL;
	MDbuf[9U] = 0x3C2D1E0FUL;

	return;
}

//RIPEMD-128 compress process
void Compress_128(
	dword * const MDbuf, 
	dword * const X)
{
	dword aa = MDbuf[0], bb = MDbuf[1U], cc = MDbuf[2U], dd = MDbuf[3U];
	dword aaa = MDbuf[0], bbb = MDbuf[1U], ccc = MDbuf[2U], ddd = MDbuf[3U];

//Round 1
	FF_128(aa, bb, cc, dd, X[0], 11);
	FF_128(dd, aa, bb, cc, X[1U], 14);
	FF_128(cc, dd, aa, bb, X[2U], 15);
	FF_128(bb, cc, dd, aa, X[3U], 12);
	FF_128(aa, bb, cc, dd, X[4U], 5);
	FF_128(dd, aa, bb, cc, X[5U], 8);
	FF_128(cc, dd, aa, bb, X[6U], 7);
	FF_128(bb, cc, dd, aa, X[7U], 9);
	FF_128(aa, bb, cc, dd, X[8U], 11);
	FF_128(dd, aa, bb, cc, X[9U], 13);
	FF_128(cc, dd, aa, bb, X[10U], 14);
	FF_128(bb, cc, dd, aa, X[11U], 15);
	FF_128(aa, bb, cc, dd, X[12U], 6);
	FF_128(dd, aa, bb, cc, X[13U], 7);
	FF_128(cc, dd, aa, bb, X[14U], 9);
	FF_128(bb, cc, dd, aa, X[15U], 8);

//Round 2
	GG_128(aa, bb, cc, dd, X[7U], 7);
	GG_128(dd, aa, bb, cc, X[4U], 6);
	GG_128(cc, dd, aa, bb, X[13U], 8);
	GG_128(bb, cc, dd, aa, X[1U], 13);
	GG_128(aa, bb, cc, dd, X[10U], 11);
	GG_128(dd, aa, bb, cc, X[6U], 9);
	GG_128(cc, dd, aa, bb, X[15U], 7);
	GG_128(bb, cc, dd, aa, X[3U], 15);
	GG_128(aa, bb, cc, dd, X[12U], 7);
	GG_128(dd, aa, bb, cc, X[0], 12);
	GG_128(cc, dd, aa, bb, X[9U], 15);
	GG_128(bb, cc, dd, aa, X[5U], 9);
	GG_128(aa, bb, cc, dd, X[2U], 11);
	GG_128(dd, aa, bb, cc, X[14U], 7);
	GG_128(cc, dd, aa, bb, X[11U], 13);
	GG_128(bb, cc, dd, aa, X[8U], 12);

//Round 3
	HH_128(aa, bb, cc, dd, X[3U], 11);
	HH_128(dd, aa, bb, cc, X[10U], 13);
	HH_128(cc, dd, aa, bb, X[14U], 6);
	HH_128(bb, cc, dd, aa, X[4U], 7);
	HH_128(aa, bb, cc, dd, X[9U], 14);
	HH_128(dd, aa, bb, cc, X[15U], 9);
	HH_128(cc, dd, aa, bb, X[8U], 13);
	HH_128(bb, cc, dd, aa, X[1U], 15);
	HH_128(aa, bb, cc, dd, X[2U], 14);
	HH_128(dd, aa, bb, cc, X[7U], 8);
	HH_128(cc, dd, aa, bb, X[0], 13);
	HH_128(bb, cc, dd, aa, X[6U], 6);
	HH_128(aa, bb, cc, dd, X[13U], 5);
	HH_128(dd, aa, bb, cc, X[11U], 12);
	HH_128(cc, dd, aa, bb, X[5U], 7);
	HH_128(bb, cc, dd, aa, X[12U], 5);

//Round 4
	II_128(aa, bb, cc, dd, X[1U], 11);
	II_128(dd, aa, bb, cc, X[9U], 12);
	II_128(cc, dd, aa, bb, X[11U], 14);
	II_128(bb, cc, dd, aa, X[10U], 15);
	II_128(aa, bb, cc, dd, X[0], 14);
	II_128(dd, aa, bb, cc, X[8U], 15);
	II_128(cc, dd, aa, bb, X[12U], 9);
	II_128(bb, cc, dd, aa, X[4U], 8);
	II_128(aa, bb, cc, dd, X[13U], 9);
	II_128(dd, aa, bb, cc, X[3U], 14);
	II_128(cc, dd, aa, bb, X[7U], 5);
	II_128(bb, cc, dd, aa, X[15U], 6);
	II_128(aa, bb, cc, dd, X[14U], 8);
	II_128(dd, aa, bb, cc, X[5U], 6);
	II_128(cc, dd, aa, bb, X[6U], 5);
	II_128(bb, cc, dd, aa, X[2U], 12);

//Parallel round 1
	III_128(aaa, bbb, ccc, ddd, X[5U], 8);
	III_128(ddd, aaa, bbb, ccc, X[14U], 9);
	III_128(ccc, ddd, aaa, bbb, X[7U], 9);
	III_128(bbb, ccc, ddd, aaa, X[0], 11);
	III_128(aaa, bbb, ccc, ddd, X[9U], 13);
	III_128(ddd, aaa, bbb, ccc, X[2U], 15);
	III_128(ccc, ddd, aaa, bbb, X[11U], 15);
	III_128(bbb, ccc, ddd, aaa, X[4U], 5);
	III_128(aaa, bbb, ccc, ddd, X[13U], 7);
	III_128(ddd, aaa, bbb, ccc, X[6U], 7);
	III_128(ccc, ddd, aaa, bbb, X[15U], 8);
	III_128(bbb, ccc, ddd, aaa, X[8U], 11);
	III_128(aaa, bbb, ccc, ddd, X[1U], 14);
	III_128(ddd, aaa, bbb, ccc, X[10U], 14);
	III_128(ccc, ddd, aaa, bbb, X[3U], 12);
	III_128(bbb, ccc, ddd, aaa, X[12U], 6);

//Parallel round 2
	HHH_128(aaa, bbb, ccc, ddd, X[6U], 9);
	HHH_128(ddd, aaa, bbb, ccc, X[11U], 13);
	HHH_128(ccc, ddd, aaa, bbb, X[3U], 15);
	HHH_128(bbb, ccc, ddd, aaa, X[7U], 7);
	HHH_128(aaa, bbb, ccc, ddd, X[0], 12);
	HHH_128(ddd, aaa, bbb, ccc, X[13U], 8);
	HHH_128(ccc, ddd, aaa, bbb, X[5U], 9);
	HHH_128(bbb, ccc, ddd, aaa, X[10U], 11);
	HHH_128(aaa, bbb, ccc, ddd, X[14U], 7);
	HHH_128(ddd, aaa, bbb, ccc, X[15U], 7);
	HHH_128(ccc, ddd, aaa, bbb, X[8U], 12);
	HHH_128(bbb, ccc, ddd, aaa, X[12U], 7);
	HHH_128(aaa, bbb, ccc, ddd, X[4U], 6);
	HHH_128(ddd, aaa, bbb, ccc, X[9U], 15);
	HHH_128(ccc, ddd, aaa, bbb, X[1U], 13);
	HHH_128(bbb, ccc, ddd, aaa, X[2U], 11);

//Parallel round 3
	GGG_128(aaa, bbb, ccc, ddd, X[15U], 9);
	GGG_128(ddd, aaa, bbb, ccc, X[5U], 7);
	GGG_128(ccc, ddd, aaa, bbb, X[1U], 15);
	GGG_128(bbb, ccc, ddd, aaa, X[3U], 11);
	GGG_128(aaa, bbb, ccc, ddd, X[7U], 8);
	GGG_128(ddd, aaa, bbb, ccc, X[14U], 6);
	GGG_128(ccc, ddd, aaa, bbb, X[6U], 6);
	GGG_128(bbb, ccc, ddd, aaa, X[9U], 14);
	GGG_128(aaa, bbb, ccc, ddd, X[11U], 12);
	GGG_128(ddd, aaa, bbb, ccc, X[8U], 13);
	GGG_128(ccc, ddd, aaa, bbb, X[12U], 5);
	GGG_128(bbb, ccc, ddd, aaa, X[2U], 14);
	GGG_128(aaa, bbb, ccc, ddd, X[10U], 13);
	GGG_128(ddd, aaa, bbb, ccc, X[0], 13);
	GGG_128(ccc, ddd, aaa, bbb, X[4U], 7);
	GGG_128(bbb, ccc, ddd, aaa, X[13U], 5);

//Parallel round 4
	FFF_128(aaa, bbb, ccc, ddd, X[8U], 15);
	FFF_128(ddd, aaa, bbb, ccc, X[6U], 5);
	FFF_128(ccc, ddd, aaa, bbb, X[4U], 8);
	FFF_128(bbb, ccc, ddd, aaa, X[1U], 11);
	FFF_128(aaa, bbb, ccc, ddd, X[3U], 14);
	FFF_128(ddd, aaa, bbb, ccc, X[11U], 14);
	FFF_128(ccc, ddd, aaa, bbb, X[15U], 6);
	FFF_128(bbb, ccc, ddd, aaa, X[0], 14);
	FFF_128(aaa, bbb, ccc, ddd, X[5U], 6);
	FFF_128(ddd, aaa, bbb, ccc, X[12U], 9);
	FFF_128(ccc, ddd, aaa, bbb, X[2U], 12);
	FFF_128(bbb, ccc, ddd, aaa, X[13U], 9);
	FFF_128(aaa, bbb, ccc, ddd, X[9U], 12);
	FFF_128(ddd, aaa, bbb, ccc, X[7U], 5);
	FFF_128(ccc, ddd, aaa, bbb, X[10U], 15);
	FFF_128(bbb, ccc, ddd, aaa, X[14U], 8);

//Combine results
	ddd += cc + MDbuf[1U]; //Final result for MDbuf[0]
	MDbuf[1U] = MDbuf[2U] + dd + aaa;
	MDbuf[2U] = MDbuf[3U] + aa + bbb;
	MDbuf[3U] = MDbuf[0] + bb + ccc;
	MDbuf[0] = ddd;

	return;
}

//RIPEMD-160 compress process
void Compress_160(
	dword * const MDbuf, 
	dword * const X)
{
	dword aa = MDbuf[0], bb = MDbuf[1U], cc = MDbuf[2U], dd = MDbuf[3U], ee = MDbuf[4U];
	dword aaa = MDbuf[0], bbb = MDbuf[1U], ccc = MDbuf[2U], ddd = MDbuf[3U], eee = MDbuf[4U];

//Round 1
	FF_160(aa, bb, cc, dd, ee, X[0], 11);
	FF_160(ee, aa, bb, cc, dd, X[1U], 14);
	FF_160(dd, ee, aa, bb, cc, X[2U], 15);
	FF_160(cc, dd, ee, aa, bb, X[3U], 12);
	FF_160(bb, cc, dd, ee, aa, X[4U], 5);
	FF_160(aa, bb, cc, dd, ee, X[5U], 8);
	FF_160(ee, aa, bb, cc, dd, X[6U], 7);
	FF_160(dd, ee, aa, bb, cc, X[7U], 9);
	FF_160(cc, dd, ee, aa, bb, X[8U], 11);
	FF_160(bb, cc, dd, ee, aa, X[9U], 13);
	FF_160(aa, bb, cc, dd, ee, X[10U], 14);
	FF_160(ee, aa, bb, cc, dd, X[11U], 15);
	FF_160(dd, ee, aa, bb, cc, X[12U], 6);
	FF_160(cc, dd, ee, aa, bb, X[13U], 7);
	FF_160(bb, cc, dd, ee, aa, X[14U], 9);
	FF_160(aa, bb, cc, dd, ee, X[15U], 8);

//Round 2
	GG_160(ee, aa, bb, cc, dd, X[7U], 7);
	GG_160(dd, ee, aa, bb, cc, X[4U], 6);
	GG_160(cc, dd, ee, aa, bb, X[13U], 8);
	GG_160(bb, cc, dd, ee, aa, X[1U], 13);
	GG_160(aa, bb, cc, dd, ee, X[10U], 11);
	GG_160(ee, aa, bb, cc, dd, X[6U], 9);
	GG_160(dd, ee, aa, bb, cc, X[15U], 7);
	GG_160(cc, dd, ee, aa, bb, X[3U], 15);
	GG_160(bb, cc, dd, ee, aa, X[12U], 7);
	GG_160(aa, bb, cc, dd, ee, X[0], 12);
	GG_160(ee, aa, bb, cc, dd, X[9U], 15);
	GG_160(dd, ee, aa, bb, cc, X[5U], 9);
	GG_160(cc, dd, ee, aa, bb, X[2U], 11);
	GG_160(bb, cc, dd, ee, aa, X[14U], 7);
	GG_160(aa, bb, cc, dd, ee, X[11U], 13);
	GG_160(ee, aa, bb, cc, dd, X[8U], 12);

//Round 3
	HH_160(dd, ee, aa, bb, cc, X[3U], 11);
	HH_160(cc, dd, ee, aa, bb, X[10U], 13);
	HH_160(bb, cc, dd, ee, aa, X[14U], 6);
	HH_160(aa, bb, cc, dd, ee, X[4U], 7);
	HH_160(ee, aa, bb, cc, dd, X[9U], 14);
	HH_160(dd, ee, aa, bb, cc, X[15U], 9);
	HH_160(cc, dd, ee, aa, bb, X[8U], 13);
	HH_160(bb, cc, dd, ee, aa, X[1U], 15);
	HH_160(aa, bb, cc, dd, ee, X[2U], 14);
	HH_160(ee, aa, bb, cc, dd, X[7U], 8);
	HH_160(dd, ee, aa, bb, cc, X[0], 13);
	HH_160(cc, dd, ee, aa, bb, X[6U], 6);
	HH_160(bb, cc, dd, ee, aa, X[13U], 5);
	HH_160(aa, bb, cc, dd, ee, X[11U], 12);
	HH_160(ee, aa, bb, cc, dd, X[5U], 7);
	HH_160(dd, ee, aa, bb, cc, X[12U], 5);

//Round 4
	II_160(cc, dd, ee, aa, bb, X[1U], 11);
	II_160(bb, cc, dd, ee, aa, X[9U], 12);
	II_160(aa, bb, cc, dd, ee, X[11U], 14);
	II_160(ee, aa, bb, cc, dd, X[10U], 15);
	II_160(dd, ee, aa, bb, cc, X[0], 14);
	II_160(cc, dd, ee, aa, bb, X[8U], 15);
	II_160(bb, cc, dd, ee, aa, X[12U], 9);
	II_160(aa, bb, cc, dd, ee, X[4U], 8);
	II_160(ee, aa, bb, cc, dd, X[13U], 9);
	II_160(dd, ee, aa, bb, cc, X[3U], 14);
	II_160(cc, dd, ee, aa, bb, X[7U], 5);
	II_160(bb, cc, dd, ee, aa, X[15U], 6);
	II_160(aa, bb, cc, dd, ee, X[14U], 8);
	II_160(ee, aa, bb, cc, dd, X[5U], 6);
	II_160(dd, ee, aa, bb, cc, X[6U], 5);
	II_160(cc, dd, ee, aa, bb, X[2U], 12);

//Round 5
	JJ_160(bb, cc, dd, ee, aa, X[4U], 9);
	JJ_160(aa, bb, cc, dd, ee, X[0], 15);
	JJ_160(ee, aa, bb, cc, dd, X[5U], 5);
	JJ_160(dd, ee, aa, bb, cc, X[9U], 11);
	JJ_160(cc, dd, ee, aa, bb, X[7U], 6);
	JJ_160(bb, cc, dd, ee, aa, X[12U], 8);
	JJ_160(aa, bb, cc, dd, ee, X[2U], 13);
	JJ_160(ee, aa, bb, cc, dd, X[10U], 12);
	JJ_160(dd, ee, aa, bb, cc, X[14U], 5);
	JJ_160(cc, dd, ee, aa, bb, X[1U], 12);
	JJ_160(bb, cc, dd, ee, aa, X[3U], 13);
	JJ_160(aa, bb, cc, dd, ee, X[8U], 14);
	JJ_160(ee, aa, bb, cc, dd, X[11U], 11);
	JJ_160(dd, ee, aa, bb, cc, X[6U], 8);
	JJ_160(cc, dd, ee, aa, bb, X[15U], 5);
	JJ_160(bb, cc, dd, ee, aa, X[13U], 6);

//Parallel round 1
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[5U], 8);
	JJJ_160(eee, aaa, bbb, ccc, ddd, X[14U], 9);
	JJJ_160(ddd, eee, aaa, bbb, ccc, X[7U], 9);
	JJJ_160(ccc, ddd, eee, aaa, bbb, X[0], 11);
	JJJ_160(bbb, ccc, ddd, eee, aaa, X[9U], 13);
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[2U], 15);
	JJJ_160(eee, aaa, bbb, ccc, ddd, X[11U], 15);
	JJJ_160(ddd, eee, aaa, bbb, ccc, X[4U], 5);
	JJJ_160(ccc, ddd, eee, aaa, bbb, X[13U], 7);
	JJJ_160(bbb, ccc, ddd, eee, aaa, X[6U], 7);
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[15U], 8);
	JJJ_160(eee, aaa, bbb, ccc, ddd, X[8U], 11);
	JJJ_160(ddd, eee, aaa, bbb, ccc, X[1U], 14);
	JJJ_160(ccc, ddd, eee, aaa, bbb, X[10U], 14);
	JJJ_160(bbb, ccc, ddd, eee, aaa, X[3U], 12);
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[12U], 6);

//Parallel round 2
	III_160(eee, aaa, bbb, ccc, ddd, X[6U], 9);
	III_160(ddd, eee, aaa, bbb, ccc, X[11U], 13);
	III_160(ccc, ddd, eee, aaa, bbb, X[3U], 15);
	III_160(bbb, ccc, ddd, eee, aaa, X[7U], 7);
	III_160(aaa, bbb, ccc, ddd, eee, X[0], 12);
	III_160(eee, aaa, bbb, ccc, ddd, X[13U], 8);
	III_160(ddd, eee, aaa, bbb, ccc, X[5U], 9);
	III_160(ccc, ddd, eee, aaa, bbb, X[10U], 11);
	III_160(bbb, ccc, ddd, eee, aaa, X[14U], 7);
	III_160(aaa, bbb, ccc, ddd, eee, X[15U], 7);
	III_160(eee, aaa, bbb, ccc, ddd, X[8U], 12);
	III_160(ddd, eee, aaa, bbb, ccc, X[12U], 7);
	III_160(ccc, ddd, eee, aaa, bbb, X[4U], 6);
	III_160(bbb, ccc, ddd, eee, aaa, X[9U], 15);
	III_160(aaa, bbb, ccc, ddd, eee, X[1U], 13);
	III_160(eee, aaa, bbb, ccc, ddd, X[2U], 11);

//Parallel round 3
	HHH_160(ddd, eee, aaa, bbb, ccc, X[15U], 9);
	HHH_160(ccc, ddd, eee, aaa, bbb, X[5U], 7);
	HHH_160(bbb, ccc, ddd, eee, aaa, X[1U], 15);
	HHH_160(aaa, bbb, ccc, ddd, eee, X[3U], 11);
	HHH_160(eee, aaa, bbb, ccc, ddd, X[7U], 8);
	HHH_160(ddd, eee, aaa, bbb, ccc, X[14U], 6);
	HHH_160(ccc, ddd, eee, aaa, bbb, X[6U], 6);
	HHH_160(bbb, ccc, ddd, eee, aaa, X[9U], 14);
	HHH_160(aaa, bbb, ccc, ddd, eee, X[11U], 12);
	HHH_160(eee, aaa, bbb, ccc, ddd, X[8U], 13);
	HHH_160(ddd, eee, aaa, bbb, ccc, X[12U], 5);
	HHH_160(ccc, ddd, eee, aaa, bbb, X[2U], 14);
	HHH_160(bbb, ccc, ddd, eee, aaa, X[10U], 13);
	HHH_160(aaa, bbb, ccc, ddd, eee, X[0], 13);
	HHH_160(eee, aaa, bbb, ccc, ddd, X[4U], 7);
	HHH_160(ddd, eee, aaa, bbb, ccc, X[13U], 5);

//Parallel round 4
	GGG_160(ccc, ddd, eee, aaa, bbb, X[8U], 15);
	GGG_160(bbb, ccc, ddd, eee, aaa, X[6U], 5);
	GGG_160(aaa, bbb, ccc, ddd, eee, X[4U], 8);
	GGG_160(eee, aaa, bbb, ccc, ddd, X[1U], 11);
	GGG_160(ddd, eee, aaa, bbb, ccc, X[3U], 14);
	GGG_160(ccc, ddd, eee, aaa, bbb, X[11U], 14);
	GGG_160(bbb, ccc, ddd, eee, aaa, X[15U], 6);
	GGG_160(aaa, bbb, ccc, ddd, eee, X[0], 14);
	GGG_160(eee, aaa, bbb, ccc, ddd, X[5U], 6);
	GGG_160(ddd, eee, aaa, bbb, ccc, X[12U], 9);
	GGG_160(ccc, ddd, eee, aaa, bbb, X[2U], 12);
	GGG_160(bbb, ccc, ddd, eee, aaa, X[13U], 9);
	GGG_160(aaa, bbb, ccc, ddd, eee, X[9U], 12);
	GGG_160(eee, aaa, bbb, ccc, ddd, X[7U], 5);
	GGG_160(ddd, eee, aaa, bbb, ccc, X[10U], 15);
	GGG_160(ccc, ddd, eee, aaa, bbb, X[14U], 8);

//Parallel round 5
	FFF_160(bbb, ccc, ddd, eee, aaa, X[12U], 8);
	FFF_160(aaa, bbb, ccc, ddd, eee, X[15U], 5);
	FFF_160(eee, aaa, bbb, ccc, ddd, X[10U], 12);
	FFF_160(ddd, eee, aaa, bbb, ccc, X[4U], 9);
	FFF_160(ccc, ddd, eee, aaa, bbb, X[1U], 12);
	FFF_160(bbb, ccc, ddd, eee, aaa, X[5U], 5);
	FFF_160(aaa, bbb, ccc, ddd, eee, X[8U], 14);
	FFF_160(eee, aaa, bbb, ccc, ddd, X[7U], 6);
	FFF_160(ddd, eee, aaa, bbb, ccc, X[6U], 8);
	FFF_160(ccc, ddd, eee, aaa, bbb, X[2U], 13);
	FFF_160(bbb, ccc, ddd, eee, aaa, X[13U], 6);
	FFF_160(aaa, bbb, ccc, ddd, eee, X[14U], 5);
	FFF_160(eee, aaa, bbb, ccc, ddd, X[0], 15);
	FFF_160(ddd, eee, aaa, bbb, ccc, X[3U], 13);
	FFF_160(ccc, ddd, eee, aaa, bbb, X[9U], 11);
	FFF_160(bbb, ccc, ddd, eee, aaa, X[11U], 11);

//Combine results
	ddd += cc + MDbuf[1U]; //Final result for MDbuf[0]
	MDbuf[1U] = MDbuf[2U] + dd + eee;
	MDbuf[2U] = MDbuf[3U] + ee + aaa;
	MDbuf[3U] = MDbuf[4U] + aa + bbb;
	MDbuf[4U] = MDbuf[0] + bb + ccc;
	MDbuf[0] = ddd;

	return;
}

//RIPEMD-256 compress process
void Compress_256(
	dword * const MDbuf, 
	dword * const X)
{
	dword aa = MDbuf[0], bb = MDbuf[1U], cc = MDbuf[2U], dd = MDbuf[3U];
	dword aaa = MDbuf[4U], bbb = MDbuf[5U], ccc = MDbuf[6U], ddd = MDbuf[7U];
	dword T = 0;

//Round 1
	FF_128(aa, bb, cc, dd, X[0], 11);
	FF_128(dd, aa, bb, cc, X[1U], 14);
	FF_128(cc, dd, aa, bb, X[2U], 15);
	FF_128(bb, cc, dd, aa, X[3U], 12);
	FF_128(aa, bb, cc, dd, X[4U], 5);
	FF_128(dd, aa, bb, cc, X[5U], 8);
	FF_128(cc, dd, aa, bb, X[6U], 7);
	FF_128(bb, cc, dd, aa, X[7U], 9);
	FF_128(aa, bb, cc, dd, X[8U], 11);
	FF_128(dd, aa, bb, cc, X[9U], 13);
	FF_128(cc, dd, aa, bb, X[10U], 14);
	FF_128(bb, cc, dd, aa, X[11U], 15);
	FF_128(aa, bb, cc, dd, X[12U], 6);
	FF_128(dd, aa, bb, cc, X[13U], 7);
	FF_128(cc, dd, aa, bb, X[14U], 9);
	FF_128(bb, cc, dd, aa, X[15U], 8);

//Parallel round 1
	III_128(aaa, bbb, ccc, ddd, X[5U], 8);
	III_128(ddd, aaa, bbb, ccc, X[14U], 9);
	III_128(ccc, ddd, aaa, bbb, X[7U], 9);
	III_128(bbb, ccc, ddd, aaa, X[0], 11);
	III_128(aaa, bbb, ccc, ddd, X[9U], 13);
	III_128(ddd, aaa, bbb, ccc, X[2U], 15);
	III_128(ccc, ddd, aaa, bbb, X[11U], 15);
	III_128(bbb, ccc, ddd, aaa, X[4U], 5);
	III_128(aaa, bbb, ccc, ddd, X[13U], 7);
	III_128(ddd, aaa, bbb, ccc, X[6U], 7);
	III_128(ccc, ddd, aaa, bbb, X[15U], 8);
	III_128(bbb, ccc, ddd, aaa, X[8U], 11);
	III_128(aaa, bbb, ccc, ddd, X[1U], 14);
	III_128(ddd, aaa, bbb, ccc, X[10U], 14);
	III_128(ccc, ddd, aaa, bbb, X[3U], 12);
	III_128(bbb, ccc, ddd, aaa, X[12U], 6);

//Round 1 exchange
	T = aa;
	aa = aaa;
	aaa = T;

//Round 2
	GG_128(aa, bb, cc, dd, X[7U], 7);
	GG_128(dd, aa, bb, cc, X[4U], 6);
	GG_128(cc, dd, aa, bb, X[13U], 8);
	GG_128(bb, cc, dd, aa, X[1U], 13);
	GG_128(aa, bb, cc, dd, X[10U], 11);
	GG_128(dd, aa, bb, cc, X[6U], 9);
	GG_128(cc, dd, aa, bb, X[15U], 7);
	GG_128(bb, cc, dd, aa, X[3U], 15);
	GG_128(aa, bb, cc, dd, X[12U], 7);
	GG_128(dd, aa, bb, cc, X[0], 12);
	GG_128(cc, dd, aa, bb, X[9U], 15);
	GG_128(bb, cc, dd, aa, X[5U], 9);
	GG_128(aa, bb, cc, dd, X[2U], 11);
	GG_128(dd, aa, bb, cc, X[14U], 7);
	GG_128(cc, dd, aa, bb, X[11U], 13);
	GG_128(bb, cc, dd, aa, X[8U], 12);

//Parallel round 2
	HHH_128(aaa, bbb, ccc, ddd, X[6U], 9);
	HHH_128(ddd, aaa, bbb, ccc, X[11U], 13);
	HHH_128(ccc, ddd, aaa, bbb, X[3U], 15);
	HHH_128(bbb, ccc, ddd, aaa, X[7U], 7);
	HHH_128(aaa, bbb, ccc, ddd, X[0], 12);
	HHH_128(ddd, aaa, bbb, ccc, X[13U], 8);
	HHH_128(ccc, ddd, aaa, bbb, X[5U], 9);
	HHH_128(bbb, ccc, ddd, aaa, X[10U], 11);
	HHH_128(aaa, bbb, ccc, ddd, X[14U], 7);
	HHH_128(ddd, aaa, bbb, ccc, X[15U], 7);
	HHH_128(ccc, ddd, aaa, bbb, X[8U], 12);
	HHH_128(bbb, ccc, ddd, aaa, X[12U], 7);
	HHH_128(aaa, bbb, ccc, ddd, X[4U], 6);
	HHH_128(ddd, aaa, bbb, ccc, X[9U], 15);
	HHH_128(ccc, ddd, aaa, bbb, X[1U], 13);
	HHH_128(bbb, ccc, ddd, aaa, X[2U], 11);

//Round 2 exchange
	T = bb;
	bb = bbb;
	bbb = T;

//Round 3
	HH_128(aa, bb, cc, dd, X[3U], 11);
	HH_128(dd, aa, bb, cc, X[10U], 13);
	HH_128(cc, dd, aa, bb, X[14U], 6);
	HH_128(bb, cc, dd, aa, X[4U], 7);
	HH_128(aa, bb, cc, dd, X[9U], 14);
	HH_128(dd, aa, bb, cc, X[15U], 9);
	HH_128(cc, dd, aa, bb, X[8U], 13);
	HH_128(bb, cc, dd, aa, X[1U], 15);
	HH_128(aa, bb, cc, dd, X[2U], 14);
	HH_128(dd, aa, bb, cc, X[7U], 8);
	HH_128(cc, dd, aa, bb, X[0], 13);
	HH_128(bb, cc, dd, aa, X[6U], 6);
	HH_128(aa, bb, cc, dd, X[13U], 5);
	HH_128(dd, aa, bb, cc, X[11U], 12);
	HH_128(cc, dd, aa, bb, X[5U], 7);
	HH_128(bb, cc, dd, aa, X[12U], 5);

//Parallel round 3
	GGG_128(aaa, bbb, ccc, ddd, X[15U], 9);
	GGG_128(ddd, aaa, bbb, ccc, X[5U], 7);
	GGG_128(ccc, ddd, aaa, bbb, X[1U], 15);
	GGG_128(bbb, ccc, ddd, aaa, X[3U], 11);
	GGG_128(aaa, bbb, ccc, ddd, X[7U], 8);
	GGG_128(ddd, aaa, bbb, ccc, X[14U], 6);
	GGG_128(ccc, ddd, aaa, bbb, X[6U], 6);
	GGG_128(bbb, ccc, ddd, aaa, X[9U], 14);
	GGG_128(aaa, bbb, ccc, ddd, X[11U], 12);
	GGG_128(ddd, aaa, bbb, ccc, X[8U], 13);
	GGG_128(ccc, ddd, aaa, bbb, X[12U], 5);
	GGG_128(bbb, ccc, ddd, aaa, X[2U], 14);
	GGG_128(aaa, bbb, ccc, ddd, X[10U], 13);
	GGG_128(ddd, aaa, bbb, ccc, X[0], 13);
	GGG_128(ccc, ddd, aaa, bbb, X[4U], 7);
	GGG_128(bbb, ccc, ddd, aaa, X[13U], 5);

//Round 3 exchange
	T = cc;
	cc = ccc;
	ccc = T;

//Round 4
	II_128(aa, bb, cc, dd, X[1U], 11);
	II_128(dd, aa, bb, cc, X[9U], 12);
	II_128(cc, dd, aa, bb, X[11U], 14);
	II_128(bb, cc, dd, aa, X[10U], 15);
	II_128(aa, bb, cc, dd, X[0], 14);
	II_128(dd, aa, bb, cc, X[8U], 15);
	II_128(cc, dd, aa, bb, X[12U], 9);
	II_128(bb, cc, dd, aa, X[4U], 8);
	II_128(aa, bb, cc, dd, X[13U], 9);
	II_128(dd, aa, bb, cc, X[3U], 14);
	II_128(cc, dd, aa, bb, X[7U], 5);
	II_128(bb, cc, dd, aa, X[15U], 6);
	II_128(aa, bb, cc, dd, X[14U], 8);
	II_128(dd, aa, bb, cc, X[5U], 6);
	II_128(cc, dd, aa, bb, X[6U], 5);
	II_128(bb, cc, dd, aa, X[2U], 12);

//Parallel round 4
	FFF_128(aaa, bbb, ccc, ddd, X[8U], 15);
	FFF_128(ddd, aaa, bbb, ccc, X[6U], 5);
	FFF_128(ccc, ddd, aaa, bbb, X[4U], 8);
	FFF_128(bbb, ccc, ddd, aaa, X[1U], 11);
	FFF_128(aaa, bbb, ccc, ddd, X[3U], 14);
	FFF_128(ddd, aaa, bbb, ccc, X[11U], 14);
	FFF_128(ccc, ddd, aaa, bbb, X[15U], 6);
	FFF_128(bbb, ccc, ddd, aaa, X[0], 14);
	FFF_128(aaa, bbb, ccc, ddd, X[5U], 6);
	FFF_128(ddd, aaa, bbb, ccc, X[12U], 9);
	FFF_128(ccc, ddd, aaa, bbb, X[2U], 12);
	FFF_128(bbb, ccc, ddd, aaa, X[13U], 9);
	FFF_128(aaa, bbb, ccc, ddd, X[9U], 12);
	FFF_128(ddd, aaa, bbb, ccc, X[7U], 5);
	FFF_128(ccc, ddd, aaa, bbb, X[10U], 15);
	FFF_128(bbb, ccc, ddd, aaa, X[14U], 8);

//Round 4 exchange
	T = dd;
	dd = ddd;
	ddd = T;

//Combine results
	MDbuf[0] += aa;
	MDbuf[1U] += bb;
	MDbuf[2U] += cc;
	MDbuf[3U] += dd;
	MDbuf[4U] += aaa;
	MDbuf[5U] += bbb;
	MDbuf[6U] += ccc;
	MDbuf[7U] += ddd;

	return;
}

//RIPEMD-320 compress process
void Compress_320(
	dword * const MDbuf, 
	dword * const X)
{
	dword aa = MDbuf[0], bb = MDbuf[1U], cc = MDbuf[2U], dd = MDbuf[3U], ee = MDbuf[4U];
	dword aaa = MDbuf[5U], bbb = MDbuf[6U], ccc = MDbuf[7U], ddd = MDbuf[8U], eee = MDbuf[9U];
	dword T = 0;

//Round 1
	FF_160(aa, bb, cc, dd, ee, X[0], 11);
	FF_160(ee, aa, bb, cc, dd, X[1U], 14);
	FF_160(dd, ee, aa, bb, cc, X[2U], 15);
	FF_160(cc, dd, ee, aa, bb, X[3U], 12);
	FF_160(bb, cc, dd, ee, aa, X[4U], 5);
	FF_160(aa, bb, cc, dd, ee, X[5U], 8);
	FF_160(ee, aa, bb, cc, dd, X[6U], 7);
	FF_160(dd, ee, aa, bb, cc, X[7U], 9);
	FF_160(cc, dd, ee, aa, bb, X[8U], 11);
	FF_160(bb, cc, dd, ee, aa, X[9U], 13);
	FF_160(aa, bb, cc, dd, ee, X[10U], 14);
	FF_160(ee, aa, bb, cc, dd, X[11U], 15);
	FF_160(dd, ee, aa, bb, cc, X[12U], 6);
	FF_160(cc, dd, ee, aa, bb, X[13U], 7);
	FF_160(bb, cc, dd, ee, aa, X[14U], 9);
	FF_160(aa, bb, cc, dd, ee, X[15U], 8);

//Parallel round 1
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[5U], 8);
	JJJ_160(eee, aaa, bbb, ccc, ddd, X[14U], 9);
	JJJ_160(ddd, eee, aaa, bbb, ccc, X[7U], 9);
	JJJ_160(ccc, ddd, eee, aaa, bbb, X[0], 11);
	JJJ_160(bbb, ccc, ddd, eee, aaa, X[9U], 13);
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[2U], 15);
	JJJ_160(eee, aaa, bbb, ccc, ddd, X[11U], 15);
	JJJ_160(ddd, eee, aaa, bbb, ccc, X[4U], 5);
	JJJ_160(ccc, ddd, eee, aaa, bbb, X[13U], 7);
	JJJ_160(bbb, ccc, ddd, eee, aaa, X[6U], 7);
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[15U], 8);
	JJJ_160(eee, aaa, bbb, ccc, ddd, X[8U], 11);
	JJJ_160(ddd, eee, aaa, bbb, ccc, X[1U], 14);
	JJJ_160(ccc, ddd, eee, aaa, bbb, X[10U], 14);
	JJJ_160(bbb, ccc, ddd, eee, aaa, X[3U], 12);
	JJJ_160(aaa, bbb, ccc, ddd, eee, X[12U], 6);

//Round 1 exchange
	T = aa;
	aa = aaa;
	aaa = T;

//Round 2
	GG_160(ee, aa, bb, cc, dd, X[7U], 7);
	GG_160(dd, ee, aa, bb, cc, X[4U], 6);
	GG_160(cc, dd, ee, aa, bb, X[13U], 8);
	GG_160(bb, cc, dd, ee, aa, X[1U], 13);
	GG_160(aa, bb, cc, dd, ee, X[10U], 11);
	GG_160(ee, aa, bb, cc, dd, X[6U], 9);
	GG_160(dd, ee, aa, bb, cc, X[15U], 7);
	GG_160(cc, dd, ee, aa, bb, X[3U], 15);
	GG_160(bb, cc, dd, ee, aa, X[12U], 7);
	GG_160(aa, bb, cc, dd, ee, X[0], 12);
	GG_160(ee, aa, bb, cc, dd, X[9U], 15);
	GG_160(dd, ee, aa, bb, cc, X[5U], 9);
	GG_160(cc, dd, ee, aa, bb, X[2U], 11);
	GG_160(bb, cc, dd, ee, aa, X[14U], 7);
	GG_160(aa, bb, cc, dd, ee, X[11U], 13);
	GG_160(ee, aa, bb, cc, dd, X[8U], 12);

//Parallel round 2
	III_160(eee, aaa, bbb, ccc, ddd, X[6U], 9);
	III_160(ddd, eee, aaa, bbb, ccc, X[11U], 13);
	III_160(ccc, ddd, eee, aaa, bbb, X[3U], 15);
	III_160(bbb, ccc, ddd, eee, aaa, X[7U], 7);
	III_160(aaa, bbb, ccc, ddd, eee, X[0], 12);
	III_160(eee, aaa, bbb, ccc, ddd, X[13U], 8);
	III_160(ddd, eee, aaa, bbb, ccc, X[5U], 9);
	III_160(ccc, ddd, eee, aaa, bbb, X[10U], 11);
	III_160(bbb, ccc, ddd, eee, aaa, X[14U], 7);
	III_160(aaa, bbb, ccc, ddd, eee, X[15U], 7);
	III_160(eee, aaa, bbb, ccc, ddd, X[8U], 12);
	III_160(ddd, eee, aaa, bbb, ccc, X[12U], 7);
	III_160(ccc, ddd, eee, aaa, bbb, X[4U], 6);
	III_160(bbb, ccc, ddd, eee, aaa, X[9U], 15);
	III_160(aaa, bbb, ccc, ddd, eee, X[1U], 13);
	III_160(eee, aaa, bbb, ccc, ddd, X[2U], 11);

//Round 2 exchange
	T = bb;
	bb = bbb;
	bbb = T;

//Round 3
	HH_160(dd, ee, aa, bb, cc, X[3U], 11);
	HH_160(cc, dd, ee, aa, bb, X[10U], 13);
	HH_160(bb, cc, dd, ee, aa, X[14U], 6);
	HH_160(aa, bb, cc, dd, ee, X[4U], 7);
	HH_160(ee, aa, bb, cc, dd, X[9U], 14);
	HH_160(dd, ee, aa, bb, cc, X[15U], 9);
	HH_160(cc, dd, ee, aa, bb, X[8U], 13);
	HH_160(bb, cc, dd, ee, aa, X[1U], 15);
	HH_160(aa, bb, cc, dd, ee, X[2U], 14);
	HH_160(ee, aa, bb, cc, dd, X[7U], 8);
	HH_160(dd, ee, aa, bb, cc, X[0], 13);
	HH_160(cc, dd, ee, aa, bb, X[6U], 6);
	HH_160(bb, cc, dd, ee, aa, X[13U], 5);
	HH_160(aa, bb, cc, dd, ee, X[11U], 12);
	HH_160(ee, aa, bb, cc, dd, X[5U], 7);
	HH_160(dd, ee, aa, bb, cc, X[12U], 5);

//Parallel round 3
	HHH_160(ddd, eee, aaa, bbb, ccc, X[15U], 9);
	HHH_160(ccc, ddd, eee, aaa, bbb, X[5U], 7);
	HHH_160(bbb, ccc, ddd, eee, aaa, X[1U], 15);
	HHH_160(aaa, bbb, ccc, ddd, eee, X[3U], 11);
	HHH_160(eee, aaa, bbb, ccc, ddd, X[7U], 8);
	HHH_160(ddd, eee, aaa, bbb, ccc, X[14U], 6);
	HHH_160(ccc, ddd, eee, aaa, bbb, X[6U], 6);
	HHH_160(bbb, ccc, ddd, eee, aaa, X[9U], 14);
	HHH_160(aaa, bbb, ccc, ddd, eee, X[11U], 12);
	HHH_160(eee, aaa, bbb, ccc, ddd, X[8U], 13);
	HHH_160(ddd, eee, aaa, bbb, ccc, X[12U], 5);
	HHH_160(ccc, ddd, eee, aaa, bbb, X[2U], 14);
	HHH_160(bbb, ccc, ddd, eee, aaa, X[10U], 13);
	HHH_160(aaa, bbb, ccc, ddd, eee, X[0], 13);
	HHH_160(eee, aaa, bbb, ccc, ddd, X[4U], 7);
	HHH_160(ddd, eee, aaa, bbb, ccc, X[13U], 5);

//Round 3 exchange
	T = cc;
	cc = ccc;
	ccc = T;

//Round 4
	II_160(cc, dd, ee, aa, bb, X[1U], 11);
	II_160(bb, cc, dd, ee, aa, X[9U], 12);
	II_160(aa, bb, cc, dd, ee, X[11U], 14);
	II_160(ee, aa, bb, cc, dd, X[10U], 15);
	II_160(dd, ee, aa, bb, cc, X[0], 14);
	II_160(cc, dd, ee, aa, bb, X[8U], 15);
	II_160(bb, cc, dd, ee, aa, X[12U], 9);
	II_160(aa, bb, cc, dd, ee, X[4U], 8);
	II_160(ee, aa, bb, cc, dd, X[13U], 9);
	II_160(dd, ee, aa, bb, cc, X[3U], 14);
	II_160(cc, dd, ee, aa, bb, X[7U], 5);
	II_160(bb, cc, dd, ee, aa, X[15U], 6);
	II_160(aa, bb, cc, dd, ee, X[14U], 8);
	II_160(ee, aa, bb, cc, dd, X[5U], 6);
	II_160(dd, ee, aa, bb, cc, X[6U], 5);
	II_160(cc, dd, ee, aa, bb, X[2U], 12);

//Parallel round 4
	GGG_160(ccc, ddd, eee, aaa, bbb, X[8U], 15);
	GGG_160(bbb, ccc, ddd, eee, aaa, X[6U], 5);
	GGG_160(aaa, bbb, ccc, ddd, eee, X[4U], 8);
	GGG_160(eee, aaa, bbb, ccc, ddd, X[1U], 11);
	GGG_160(ddd, eee, aaa, bbb, ccc, X[3U], 14);
	GGG_160(ccc, ddd, eee, aaa, bbb, X[11U], 14);
	GGG_160(bbb, ccc, ddd, eee, aaa, X[15U], 6);
	GGG_160(aaa, bbb, ccc, ddd, eee, X[0], 14);
	GGG_160(eee, aaa, bbb, ccc, ddd, X[5U], 6);
	GGG_160(ddd, eee, aaa, bbb, ccc, X[12U], 9);
	GGG_160(ccc, ddd, eee, aaa, bbb, X[2U], 12);
	GGG_160(bbb, ccc, ddd, eee, aaa, X[13U], 9);
	GGG_160(aaa, bbb, ccc, ddd, eee, X[9U], 12);
	GGG_160(eee, aaa, bbb, ccc, ddd, X[7U], 5);
	GGG_160(ddd, eee, aaa, bbb, ccc, X[10U], 15);
	GGG_160(ccc, ddd, eee, aaa, bbb, X[14U], 8);

//Round 4 exchange
	T = dd;
	dd = ddd;
	ddd = T;

//Round 5
	JJ_160(bb, cc, dd, ee, aa, X[4U], 9);
	JJ_160(aa, bb, cc, dd, ee, X[0], 15);
	JJ_160(ee, aa, bb, cc, dd, X[5U], 5);
	JJ_160(dd, ee, aa, bb, cc, X[9U], 11);
	JJ_160(cc, dd, ee, aa, bb, X[7U], 6);
	JJ_160(bb, cc, dd, ee, aa, X[12U], 8);
	JJ_160(aa, bb, cc, dd, ee, X[2U], 13);
	JJ_160(ee, aa, bb, cc, dd, X[10U], 12);
	JJ_160(dd, ee, aa, bb, cc, X[14U], 5);
	JJ_160(cc, dd, ee, aa, bb, X[1U], 12);
	JJ_160(bb, cc, dd, ee, aa, X[3U], 13);
	JJ_160(aa, bb, cc, dd, ee, X[8U], 14);
	JJ_160(ee, aa, bb, cc, dd, X[11U], 11);
	JJ_160(dd, ee, aa, bb, cc, X[6U], 8);
	JJ_160(cc, dd, ee, aa, bb, X[15U], 5);
	JJ_160(bb, cc, dd, ee, aa, X[13U], 6);

//Parallel round 5
	FFF_160(bbb, ccc, ddd, eee, aaa, X[12U], 8);
	FFF_160(aaa, bbb, ccc, ddd, eee, X[15U], 5);
	FFF_160(eee, aaa, bbb, ccc, ddd, X[10U], 12);
	FFF_160(ddd, eee, aaa, bbb, ccc, X[4U], 9);
	FFF_160(ccc, ddd, eee, aaa, bbb, X[1U], 12);
	FFF_160(bbb, ccc, ddd, eee, aaa, X[5U], 5);
	FFF_160(aaa, bbb, ccc, ddd, eee, X[8U], 14);
	FFF_160(eee, aaa, bbb, ccc, ddd, X[7U], 6);
	FFF_160(ddd, eee, aaa, bbb, ccc, X[6U], 8);
	FFF_160(ccc, ddd, eee, aaa, bbb, X[2U], 13);
	FFF_160(bbb, ccc, ddd, eee, aaa, X[13U], 6);
	FFF_160(aaa, bbb, ccc, ddd, eee, X[14U], 5);
	FFF_160(eee, aaa, bbb, ccc, ddd, X[0], 15);
	FFF_160(ddd, eee, aaa, bbb, ccc, X[3U], 13);
	FFF_160(ccc, ddd, eee, aaa, bbb, X[9U], 11);
	FFF_160(bbb, ccc, ddd, eee, aaa, X[11U], 11);

//Round 5 exchange
	T = ee;
	ee = eee;
	eee = T;

//Combine results
	MDbuf[0] += aa;
	MDbuf[1U] += bb;
	MDbuf[2U] += cc;
	MDbuf[3U] += dd;
	MDbuf[4U] += ee;
	MDbuf[5U] += aaa;
	MDbuf[6U] += bbb;
	MDbuf[7U] += ccc;
	MDbuf[8U] += ddd;
	MDbuf[9U] += eee;

	return;
}

//RIPEMD finish process
void MD_Finish(
	dword * const MDbuf, 
	uint8_t *strptr, 
	dword lswlen, 
	dword mswlen)
{
	unsigned int i = 0;   //Counter
	dword X[16U]{0};   //Message words

//Put bytes from strptr into X.
	for (i = 0;i < (lswlen & 63);++i)
	//byte i goes into word X[i div 4] at pos. 8 * (i mod 4)
		X[i >> 2U] ^= (dword)*strptr++ << (8 * (i & 3));

//Append the bit m_n == 1.
	X[(lswlen >> 2U) & 15] ^= (dword)1 << (8 * (lswlen & 3) + 7);
	if ((lswlen & 63) > 55)
	{
	//Length goes to next block.
		if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_128)
			Compress_128(MDbuf, X);
		else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_160)
			Compress_160(MDbuf, X);
		else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_256)
			Compress_256(MDbuf, X);
		else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_320)
			Compress_320(MDbuf, X);
		else //Commands error
			return;

		memset(X, 0, sizeof(dword) * 16U);
	}

//Append length in bits.
	X[14U] = lswlen << 3U;
	X[15U] = (lswlen >> 29U) | (mswlen << 3U);
	if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_128)
		Compress_128(MDbuf, X);
	else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_160)
		Compress_160(MDbuf, X);
	else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_256)
		Compress_256(MDbuf, X);
	else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_320)
		Compress_320(MDbuf, X);

	return;
}

//////////////////////////////////////////////////
// Hash function
// 
//Read commands(RIPEMD)
bool ReadCommand_RIPEMD(
#if defined(PLATFORM_WIN)
	std::wstring &Command
#elif (defined(PLATFORM_LINUX) || defined(PLATFORM_MACOS))
	std::string &Command
#endif
)
{
//Hash function check
	if (Command == COMMAND_RIPEMD_128) //RIPEMD 128 bits
	{
		RIPEMD_HashFunctionID = HASH_ID_RIPEMD_128;
	}
	else if (Command == HASH_COMMAND_RIPEMD || Command == COMMAND_RIPEMD_160) //RIPEMD 160 bits
	{
		RIPEMD_HashFunctionID = HASH_ID_RIPEMD_160;
	}
	else if (Command == COMMAND_RIPEMD_256) //RIPEMD 256 bits
	{
		RIPEMD_HashFunctionID = HASH_ID_RIPEMD_256;
	}
	else if (Command == COMMAND_RIPEMD_320) //RIPEMD 320 bits
	{
		RIPEMD_HashFunctionID = HASH_ID_RIPEMD_320;
	}
	else { //Commands error
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return false;
	}

	return true;
}

//RIPEMD hash function
bool RIPEMD_Hash(
	FILE * const FileHandle, 
	FILE * const OutputFile)
{
//Parameters check
	if (HashFamilyID != HASH_ID_RIPEMD || FileHandle == nullptr)
	{
		fwprintf_s(stderr, L"[Error] Parameters error.\n");
		return false;
	}

//Initialization
	std::unique_ptr<uint8_t[]> Buffer(new uint8_t[FILE_BUFFER_SIZE + PADDING_RESERVED_BYTES]());
	std::unique_ptr<uint8_t[]> StringBuffer(new uint8_t[FILE_BUFFER_SIZE + PADDING_RESERVED_BYTES]());
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE + PADDING_RESERVED_BYTES);
	memset(StringBuffer.get(), 0, FILE_BUFFER_SIZE + PADDING_RESERVED_BYTES);
	size_t ReadLength = 0, DigestSize = RIPEMD_DIGEST_MAXSIZE;

//RIPEMD initialization
	std::unique_ptr<dword[]> MDbuf(new dword[DigestSize / (sizeof(uint32_t) * BYTES_TO_BITS) + PADDING_RESERVED_BYTES]());
	memset(MDbuf.get(), 0, DigestSize / (sizeof(uint32_t) * BYTES_TO_BITS) + PADDING_RESERVED_BYTES);
	if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_128)
	{
		DigestSize = RIPEMD_DIGEST_SIZE_128;
		MD_Init_128(MDbuf.get());
	}
	else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_160)
	{
		DigestSize = RIPEMD_DIGEST_SIZE_160;
		MD_Init_160(MDbuf.get());
	}
	else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_256)
	{
		DigestSize = RIPEMD_DIGEST_SIZE_256;
		MD_Init_256(MDbuf.get());
	}
	else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_320)
	{
		DigestSize = RIPEMD_DIGEST_SIZE_320;
		MD_Init_320(MDbuf.get());
	}
	else { //Commands error
		fwprintf_s(stderr, L"[Error] Commands error.\n");
		return false;
	}

//Hash process
	size_t Index_A = 0, Index_B = 0;
	dword Length[2U]{0}, X[16U]{0};
	while (!feof(FileHandle))
	{
		memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
		_set_errno(0);
		ReadLength = fread_s(Buffer.get(), 1024U, sizeof(uint8_t), 1024U, FileHandle);
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
		//Process all complete blocks.
			for (Index_A = 0;Index_A < (ReadLength >> 6U);++Index_A)
			{
				for (Index_B = 0;Index_B < 16U;++Index_B)
					X[Index_B] = BYTES_TO_DWORD(Buffer.get() + 64 * Index_A + 4 * Index_B);

			//Compress process
				if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_128)
				{
					Compress_128(MDbuf.get(), X);
				}
				else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_160)
				{
					Compress_160(MDbuf.get(), X);
				}
				else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_256)
				{
					Compress_256(MDbuf.get(), X);
				}
				else if (RIPEMD_HashFunctionID == HASH_ID_RIPEMD_320)
				{
					Compress_320(MDbuf.get(), X);
				}
				else { //Commands error
					fwprintf_s(stderr, L"[Error] Commands error.\n");
					return false;
				}
			}

		//Update length values.
			if (Length[0] + ReadLength < Length[0])
				Length[1U]++; //Overflow to msb of length
			Length[0] += static_cast<dword>(ReadLength);
		}
	}

//Finish hash process.
	MD_Finish(MDbuf.get(), Buffer.get() + (Length[0] & 0x03C0), Length[0], Length[1U]); //Extract bytes 6 to 10 inclusive
	memset(Buffer.get(), 0, FILE_BUFFER_SIZE);
	for (Index_A = 0;Index_A < DigestSize / (sizeof(uint8_t) * BYTES_TO_BITS);Index_A += 4U)
	{
		Buffer.get()[Index_A] = static_cast<uint8_t>(MDbuf.get()[Index_A >> 2U]);
		Buffer.get()[Index_A + 1U] = static_cast<uint8_t>(MDbuf.get()[Index_A >> 2U] >> 8U);
		Buffer.get()[Index_A + 2U] = static_cast<uint8_t>(MDbuf.get()[Index_A >> 2U] >> 16U);
		Buffer.get()[Index_A + 3U] = static_cast<uint8_t>(MDbuf.get()[Index_A >> 2U] >> 24U);
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
