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


#include "Base.h"

//Functions
void PrintFlags(
	FILE *FileHandle, 
	const uint16_t Flags);
void PrintTypeClassesName(
	FILE *FileHandle, 
	const uint16_t Type, 
	const uint16_t Classes);
size_t PrintDomainName(
	FILE *FileHandle, 
	const uint8_t *Buffer, 
	const size_t Location);
void PrintResourseData(
	FILE *FileHandle, 
	const uint16_t Type, 
	const uint16_t Classes, 
	const uint8_t *Buffer, 
	const size_t Location, 
	const uint16_t Length);
