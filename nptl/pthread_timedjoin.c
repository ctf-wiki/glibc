/* Copyright (C) 2002-2017 Free Software Foundation, Inc.
   This file is part of the GNU C Library.
   Contributed by Ulrich Drepper <drepper@redhat.com>, 2002.

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

#include <errno.h>
#include <stdlib.h>
#include <atomic.h>
#include "pthreadP.h"

#include <stap-probe.h>


static void
cleanup (void *arg)
{
  /* If we already changed the waiter ID, reset it.  The call cannot
     fail for any reason but the thread not having done that yet so
     there is no reason for a loop.  */
  struct pthread *self = THREAD_SELF;
  atomic_compare_exchange_weak_acquire (&arg, &self, NULL);
}


int
__pthread_timedjoin_np (pthread_t threadid, void **thread_return,
			const struct timespec *abstime)
{
  struct pthread *pd = (struct pthread *) threadid;
  int result = 0, ct;

  /* Make sure the descriptor is valid.  */
  if (INVALID_NOT_TERMINATED_TD_P (pd))
    /* Not a valid thread handle.  */
    return ESRCH;

  /* Is the thread joinable?.  */
  if (IS_DETACHED (pd))
    /* We cannot wait for the thread.  */
    return EINVAL;

  LIBC_PROBE (pthread_join, 1, threadid);

  struct pthread *self = THREAD_SELF;
  unsigned int ch = atomic_load_relaxed (&pd->cancelhandling);
  if ((pd == self || (self->joinid == pd && ch == 0))
      && !(self->cancelstate == PTHREAD_CANCEL_ENABLE
           && ch & THREAD_CANCELED))
    /* This is a deadlock situation.  The threads are waiting for each
       other to finish.  Note that this is a "may" error.  To be 100%
       sure we catch this error we would have to lock the data
       structures but it is not necessary.  In the unlikely case that
       two threads are really caught in this situation they will
       deadlock.  It is the programmer's problem to figure this
       out.  */
    return EDEADLK;

  /* Wait for the thread to finish.  If it is already locked something
     is wrong.  There can only be one waiter.  */
  if (__glibc_unlikely (atomic_compare_exchange_weak_acquire (&pd->joinid,
							      &self, NULL)))
    return EINVAL;


  /* During the wait we change to asynchronous cancellation.  If we
     are cancelled the thread we are waiting for must be marked as
     un-wait-ed for again.  */
  pthread_cleanup_push (cleanup, &pd->joinid);

  /* Switch to asynchronous cancellation.  */
  __pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, &ct);

  /* Wait for the child.  */
  if (abstime != NULL)
    result = lll_timedwait_tid (pd->tid, abstime);
  else
    lll_wait_tid (pd->tid);

  __pthread_setcanceltype (ct, NULL);

  /* Remove the handler.  */
  pthread_cleanup_pop (0);


  /* We might have timed out.  */
  if (__glibc_likely (result == 0))
    {
      /* We mark the thread as terminated and as joined.  */
      pd->tid = -1;

      /* Store the return value if the caller is interested.  */
      if (thread_return != NULL)
	*thread_return = pd->result;

      /* Free the TCB.  */
      __free_tcb (pd);
    }
  else
    pd->joinid = NULL;

  LIBC_PROBE (pthread_join_ret, 3, threadid, result, pd->result);

  return result;
}
weak_alias (__pthread_timedjoin_np, pthread_timedjoin_np)
hidden_def (__pthread_timedjoin_np)
