/* C11 threads thread sleep implementation.
   Copyright (C) 2016 Free Software Foundation, Inc.
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

#include "thrd_priv.h"
#include <time.h>

/* Block current thread execution for at least the time pointed by
   __time_point.  The current thread may resume if receives a signal.  In
   that case, if __remaining is not NULL, the remaining time is stored in
   the object pointed by __remaining.  */
int
thrd_sleep (const struct timespec* time_point, struct timespec* remaining)
{
  INTERNAL_SYSCALL_DECL (err);
  int ret = INTERNAL_SYSCALL_CALL (nanosleep, err, time_point, remaining);
  if (INTERNAL_SYSCALL_ERROR_P (ret, err))
    {
      /* C11 states thrd_sleep function returns -1 if it has been interrupted
         by a signal, or a negative value if it fails.  */
      ret = INTERNAL_SYSCALL_ERRNO (ret, err);
      if (ret == EINTR)
	return -1;
      return -2;
    }
  return 0;
}
