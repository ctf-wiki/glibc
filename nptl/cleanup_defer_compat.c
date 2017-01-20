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

#include "pthreadP.h"


void
_pthread_cleanup_push_defer (struct _pthread_cleanup_buffer *buffer,
			     void (*routine) (void *), void *arg)
{
  struct pthread *self = THREAD_SELF;

  buffer->__routine = routine;
  buffer->__arg = arg;
  buffer->__prev = THREAD_GETMEM (self, cleanup);

  volatile struct pthread *pd = (volatile struct pthread *) self;
  pd->canceltype = PTHREAD_CANCEL_DEFERRED;
  buffer->__canceltype = pd->canceltype;

  THREAD_SETMEM (self, cleanup, buffer);
}
strong_alias (_pthread_cleanup_push_defer, __pthread_cleanup_push_defer)


void
_pthread_cleanup_pop_restore (struct _pthread_cleanup_buffer *buffer,
			      int execute)
{
  struct pthread *self = THREAD_SELF;

  THREAD_SETMEM (self, cleanup, buffer->__prev);

  volatile struct pthread *pd = (volatile struct pthread *) self;
  if (buffer->__canceltype != PTHREAD_CANCEL_DEFERRED
      && pd->canceltype == PTHREAD_CANCEL_DEFERRED)
    {
      pd->canceltype = PTHREAD_CANCEL_ASYNCHRONOUS;
      __pthread_testcancel ();
    }

  /* If necessary call the cleanup routine after we removed the
     current cleanup block from the list.  */
  if (execute)
    buffer->__routine (buffer->__arg);
}
strong_alias (_pthread_cleanup_pop_restore, __pthread_cleanup_pop_restore)
