/* Default pthread common types definitions.
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


/* Declaration of common pthread types for all architectures.
   Macros *_CONTENT are architecture dependent and defined in
   bits/thread-shared-types.h */

#ifndef _BITS_PTHREADTYPES_COMMON_H
# define _BITS_PTHREADTYPES_COMMON_H	1

#include <bits/thread-shared-types.h>

/* Common definition of pthread_mutex_t. */

typedef union
{
  __PTHREAD_MUTEX_T_CONTENT
} pthread_mutex_t;


/* Common definition of pthread_cond_t. */

typedef union
{
  __PTHREAD_COND_T_CONTENT
} pthread_cond_t;

#endif
