/* Copyright (C) 1991-2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <http://www.gnu.org/licenses/>.  */

#include <sysdep.h>

ENTRY (syscall)
	ABORT_TRANSACTION
	/* Creates a minimum stack frame so backtrace.  */
#ifdef __powerpc64__
	stdu r1, -FRAME_MIN_SIZE (r1)
	cfi_adjust_cfa_offset (FRAME_MIN_SIZE)
#else
	stwu r1,-16(1)
	cfi_def_cfa_offset (16)
#endif
	mr   r0,r3
	mr   r3,r4
	mr   r4,r5
	mr   r5,r6
	mr   r6,r7
	mr   r7,r8
	mr   r8,r9
#ifdef __powerpc64__
	addi r1, r1, FRAME_MIN_SIZE
#else
	addi r1,r1,16
#endif
        cfi_def_cfa_offset (0)
	sc
	PSEUDO_RET
PSEUDO_END (syscall)
