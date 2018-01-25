/* Mutex type.  Generic version.

   Copyright (C) 2000-2016
     Free Software Foundation, Inc.

   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the GNU C Library; see the file COPYING.LIB.  If not,
   write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#ifndef _BITS_MUTEX_H

#ifndef __need_pthread_mutex
# define _BITS_MUTEX_H	1
#endif

#ifndef __pthread_mutex_defined
# if defined __need_pthread_mutex || defined _BITS_MUTEX_H
#  undef __need_pthread_mutex
#  define __pthread_mutex_defined

#  include <bits/mutex-attr.h>

/* User visible part of a mutex.  */
struct __pthread_mutex
{
  unsigned int __lock;
  unsigned int __owner_id;
  unsigned int __cnt;
  int __shpid;
  int __type;
  int __flags;
  unsigned int __reserved1;
  unsigned int __reserved2;
};

/* Static mutex initializers. */
#define __PTHREAD_MUTEX_INITIALIZER   \
  { 0, 0, 0, 0, __PTHREAD_MUTEX_TIMED, 0, 0, 0 }

/* The +1 is to mantain binary compatibility with the old
 * libpthread implementation. */
#define __PTHREAD_ERRORCHECK_MUTEX_INITIALIZER   \
  { 0, 0, 0, 0, __PTHREAD_MUTEX_ERRORCHECK + 1, 0, 0, 0 }

#define __PTHREAD_RECURSIVE_MUTEX_INITIALIZER   \
  { 0, 0, 0, 0, __PTHREAD_MUTEX_RECURSIVE + 1, 0, 0, 0 }

# endif
#endif /* Not __pthread_mutex_defined.  */

#endif /* bits/mutex.h */
