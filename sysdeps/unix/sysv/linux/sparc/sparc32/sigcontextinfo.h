/* Copyright (C) 1999-2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Jakub Jelinek <jakub@redhat.com>, 1999.

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

#ifndef _SIGCONTEXTINFO_H
#define _SIGCONTEXTINFO_H

#include <stdint.h>

#define SIGCONTEXT struct sigcontext *
#define SIGCONTEXT_EXTRA_ARGS
#define GET_PC(__ctx)	((void *) ((__ctx)->si_regs.pc))
#define FIRST_FRAME_POINTER \
  ({ void *ret;							\
     asm volatile ("ta 3; add %%fp, 56, %0" : "=r" (ret)); ret; })
#define ADVANCE_STACK_FRAME(__next) \
	((void *) (((unsigned *)(__next))+14))

#define GET_STACK(__ctx)	((void *) (__ctx)->si_regs.u_regs[14])
#define GET_FRAME(__ctx)	ADVANCE_STACK_FRAME (GET_STACK(__ctx))
#define CALL_SIGHANDLER(handler, signo, ctx) \
  (handler)((signo), SIGCONTEXT_EXTRA_ARGS (ctx))

/* Different that other architectures, SPARC32 pass a pt_regs (or pt_regs32
   in 32 bits compat mode) struct pointer as third argument for sa_sigaction
   handler with SA_SIGINFO.

   Also current sparc32 rt signal frame layout is:

   field                                  | size
   ---------------------------------------| ----
   struct rt_signal_frame {               |
     struct sparc_stackf     ss;          |  96
     siginfo_t               info;        | 128
     struct pt_regs          regs;        |  80
     sigset_t                mask;        | 128
     __siginfo_fpu_t __user  *fpu_save;   |   4
     unsigned int            insns[2];    |   8
     stack_t                 stack;       |  12
     unsigned int            extra_size;  |   4
     __siginfo_rwin_t __user *rwin_save;  |   4
   };

   So to obtain a pointer to signal mask based on address of pt_regs
   we need to add 208.  */

struct pt_regs32
{
   unsigned int psr;
   unsigned int pc;
   unsigned int npc;
   unsigned int y;
   unsigned int u_regs[16];
};

static inline uintptr_t
ucontext_get_pc (struct pt_regs32 *regs)
{
  return regs->pc;
}

static inline sigset_t *
ucontext_get_mask (const void *ctx)
{
  return (sigset_t *)((uintptr_t)ctx + 208);
}
#define UCONTEXT_SIGMASK(ctx) ucontext_get_mask (ctx)

#endif /* _SIGCONTEXTINFO_H  */
