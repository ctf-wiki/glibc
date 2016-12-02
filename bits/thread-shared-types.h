/* C11 thread definition.
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

/* This file is required to have a shared types definition
   between POSIX threads and C11 threads. It allows to
   header files like pthread.h and thread.h to define their types
   with the same size and fields without corrupting namespaces.

   The interface is as follows:

   - bits/thread-shared-types.h. Defines structures for mutex and condvar,
     shared by threads.h and pthread.h. The macros defined are
     __PTHREAD_MUTEX_T_CONTENT and __PTHREAD_COND_T_CONTENT

   - bits/pthreadtypes-common.h. Works as a generic file that defines
     common pthread types like pthread_mutex_t and pthread_cond_t
     based on the macros included from thread-shared-types.h.

   - bits/pthreadtypes.h. Include pthreadtypes-common.h to have access
     to pthread types and expose them for the other types and functions
     that require it. */
