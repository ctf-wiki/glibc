/* Cancellable syscall wrapper.  Linux/IA64 version.
   Copyright (C) 2017 Free Software Foundation, Inc.
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
#undef ret

/* long int __syscall_cancel_arch (int *cancelhandling, long int nr,
				   long int arg1, long int arg2, long int arg3,
				   long int arg4, long int arg5, long int arg6)
*/

ENTRY (__syscall_cancel_arch)
	.prologue 14, 40
	.mmi
	.save ar.pfs, r41
	alloc r41 = ar.pfs, 8, 4, 8, 0
	.vframe r42
	mov r42 = r12
	.save rp, r40
	mov r40 = b0
	.body
	;;

	.global __syscall_cancel_arch_start	
	.type __syscall_cancel_arch_start,@function	
__syscall_cancel_arch_start:

	;;
	.mmi
	nop 0
	ld4.acq r14 = [r32]
	nop 0
	;;
	.mib
	nop 0
	tbit.z p6, p7 = r14, 2
	.pred.safe_across_calls p1-p63
	(p7) br.call.dpnt.many b0 = __syscall_do_cancel#
	.pred.safe_across_calls p1-p5,p16-p63
	;;
	.mmi
	mov r15 = r33
	mov r49 = r39
	mov r48 = r38
	.mmi
	mov r47 = r37
	mov r46 = r36
	mov r45 = r35
	;;
	.mmi
	nop 0
	mov r44 = r34
	nop 0
	;;
	break 0x100000
	;;

	.global __syscall_cancel_arch_end	
	.type __syscall_cancel_arch_end,@function	
__syscall_cancel_arch_end:

	;;
	.mmi
	cmp.ne p6, p7 = -1, r10
	nop 0
	mov ar.pfs = r41
	;;
	.mmi
	nop 0
	(p7) sub r8 = r0, r8
	mov b0 = r40
	.mmb
	nop 0
	.restore sp
	mov r12 = r42
	br.ret.sptk.many b0

END (__syscall_cancel_arch)
libc_hidden_def (__syscall_cancel_arch)
