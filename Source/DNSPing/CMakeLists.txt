# This code is part of Toolkit(DNSPing)
# DNSPing, a useful and powerful toolkit
# Copyright (C) 2014-2018 Chengr28
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.


# CMake minimum version
cmake_minimum_required(VERSION 3.1)

# Project name, build type and list all source files.
project(DNSPing)
set(CMAKE_BUILD_TYPE "Release")
set(DNSPING_SRC_FILES
		"Console.h"
		"Definition.h"
		"DNSPing.h"
		"Include.h"
		"Initialization.h"
		"Platform.h"
		"Process.h"
		"Protocol.h"
		"ReadCommand.h"
		"Resolver.h"
		"Structure.h"
		"Terminal.h"
		"Type.h"
		"Console.cpp"
		"DNSPing.cpp"
		"Initialization.cpp"
		"Process.cpp"
		"Protocol.cpp"
		"ReadCommand.cpp"
		"Resolver.cpp"
		"Terminal.cpp")
add_executable(DNSPing ${DNSPING_SRC_FILES})

# Compiling and linking flags
## See https://developers.redhat.com/blog/2018/03/21/compiler-and-linker-flags-gcc.
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
IF(APPLE)
	add_definitions(-std=c++14)
ENDIF(APPLE)
IF(NOT PLATFORM_OPENWRT)
	IF(CMAKE_VERSION VERSION_LESS "3.9")
		add_definitions(-flto)
	ELSE(CMAKE_VERSION VERSION_LESS "3.9")
		set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
	ENDIF(CMAKE_VERSION VERSION_LESS "3.9")
	set(CMAKE_POSITION_INDEPENDENT_CODE TRUE)
	add_definitions(-D_FORTIFY_SOURCE=2)
	add_definitions(-D_GLIBCXX_ASSERTIONS)
	add_definitions(-fasynchronous-unwind-tables)
#	add_definitions(-fcf-protection=full)
	add_definitions(-fexceptions)
	IF(NOT APPLE)
		add_definitions(-fopenmp)
#		add_definitions(-fsplit-stack)
	ENDIF(NOT APPLE)
#	add_definitions(-fstack-clash-protection)
	add_definitions(-fstack-protector-strong)
	add_definitions(-g)
	add_definitions(-grecord-gcc-switches)
	add_definitions(-march=native)
#	add_definitions(-mcet)
	add_definitions(-pipe)
	add_definitions(-O3)
ENDIF(NOT PLATFORM_OPENWRT)

# Platform and library definitions
IF(PLATFORM_OPENWRT)
	add_definitions(-DPLATFORM_OPENWRT)
	install(TARGETS DNSPing RUNTIME DESTINATION sbin)
ENDIF(PLATFORM_OPENWRT)
