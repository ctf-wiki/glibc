/* Copyright (C) 2005, 2007 Free Software Foundation, Inc.
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

#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H	1

#include <features.h>
#include <sys/types.h>
#ifdef __USE_XOPEN2K
# define __need_timespec
# include <time.h>
#endif

/* Get the definition for struct __semaphore.  */
#include <bits/semaphore.h>


__BEGIN_DECLS

#define SEM_FAILED ((void *) 0)

typedef struct __semaphore sem_t;

/* Initialize semaphore *SEM with value VALUE.  */
extern int sem_init (sem_t *__sem, int __pshared, unsigned int __value)
     __THROW;
/* Destroy semaphore *SEM created with sem_init.  */
extern int sem_destroy (sem_t *__sem) __THROW;

/* Open a named semaphore.  */
extern sem_t *sem_open (const char *__name, int __oflag, ...) __THROW;

/* Close a semaphore returned by sem_open.  */
extern int sem_close (sem_t *__sem) __THROW;

/* Unlink a named semaphore.  */
extern int sem_unlink (const char *__name) __THROW;

/* Perform a down operation on semaphore *SEM.

   This function is a cancellation point and therefore not marked with
   __THROW.  */
extern int sem_wait (sem_t *__sem);

#ifdef __USE_XOPEN2K
/* Perform a down operation on semaphore *SEM but don't wait longer
   than TIMEOUT.  */
extern int sem_timedwait (sem_t *__restrict __sem,
			  const struct timespec *__restrict __abstime);
#endif

/* Perform a down operation on semaphore *SEM if it can be done so
   without blocking.  */
extern int sem_trywait (sem_t *__sem) __THROWNL;

/* Perform an up operation on semaphore *SEM.  */
extern int sem_post (sem_t *__sem) __THROWNL;

/* Store the value of semaphore *SEM in *VALUE.  */
extern int sem_getvalue (sem_t *__restrict __sem, int *__restrict __sval)
    __THROW;


__END_DECLS

#endif /* semaphore.h */
