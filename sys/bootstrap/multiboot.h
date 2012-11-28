//
//  multiboot.h
//  Firedrake
//
//  Created by Sidney Just
//  Copyright (c) 2012 by Sidney Just
//  Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//  documentation files (the "Software"), to deal in the Software without restriction, including without limitation 
//  the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, 
//  and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//  The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR 
//  PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE 
//  FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, 
//  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#ifndef _MULTIBOOT_H_
#define _MULTIBOOT_H_

#include <types.h>

struct multiboot_s
{
	uint32_t    flags;
	uint32_t    mem_lower;
	uint32_t    mem_upper;
	
	uint32_t    bootdevice;
	uint32_t    cmdline;
	
	uint32_t    mods_count;
	void		*mods_addr;
	
	uint32_t    syms[4];
	
	uint32_t    mmap_length;
	void		*mmap_addr;
} __attribute__((packed));

struct multiboot_mmap_s
{
	uint32_t    entry_size;
	uint64_t    base;
	uint64_t    length;
	uint32_t    type;
} __attribute__((packed));

struct multiboot_module_s
{
	void		*start;
	void		*end;
	void		*name;
	uint32_t	reserved;
} __attribute__((packed));

extern struct multiboot_s *bootinfo;

#endif