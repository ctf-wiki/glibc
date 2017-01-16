/* Copyright (C) 2002-2017 Free Software Foundation, Inc.
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

#include <stdint.h>

#define SIGCONTEXT siginfo_t *_si, struct sigcontext *
#define SIGCONTEXT_EXTRA_ARGS _si,
#define GET_PC(ctx)	((ctx)->sc_ip)
#define GET_FRAME(ctx)	((void *) 0)
#define GET_STACK(ctx)	((void *) 0)

#define CALL_SIGHANDLER(handler, signo, ctx) \
  (handler)((signo), SIGCONTEXT_EXTRA_ARGS (ctx))

/* Different that other architectures, SPARC pass a sigcontext_t struct
   in third argument for sa_sigaction handler with SA_SIGINFO.  */
static inline uintptr_t
ucontext_get_pc (const struct sigcontext *sigctx)
{
  return sigctx->sc_ip;
}

static inline sigset_t *
ucontext_get_mask (const struct sigcontext *sigctx)
{
  /* IA64 sigcontext::sa_mask is a sigset_t since Linux 2.6.12 (initial
     git repository build).  */
  return (sigset_t *) &sigctx->sc_mask;
}
#define UCONTEXT_SIGMASK(ctx) ucontext_get_mask (ctx)

#endif /* _SIGCONTEXTINFO_H  */
