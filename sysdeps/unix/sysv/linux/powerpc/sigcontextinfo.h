/* Copyright (C) 1998-2017 Free Software Foundation, Inc.
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

#ifndef _SIGCONTEXTINFO_H
#define _SIGCONTEXTINFO_H

#include <signal.h>
#include <stdint.h>

#define SIGCONTEXT struct sigcontext *
#define SIGCONTEXT_EXTRA_ARGS
#define GET_PC(ctx)	((void *)((ctx)->regs->nip))
#define GET_FRAME(ctx)	(*(void **)((ctx)->regs->gpr[1]))
#define GET_STACK(ctx)	((void *)((ctx)->regs->gpr[1]))
#define CALL_SIGHANDLER(handler, signo, ctx) \
  (handler)((signo), SIGCONTEXT_EXTRA_ARGS (ctx))

static inline uintptr_t
ucontext_get_pc (const ucontext_t *uc)
{
#ifdef __powerpc64__
  return uc->uc_mcontext.gp_regs[PT_NIP];
#else
  return uc->uc_mcontext.uc_regs->gregs[PT_NIP];
#endif
}

#endif /* _SIGCONTEXTINFO_H  */
