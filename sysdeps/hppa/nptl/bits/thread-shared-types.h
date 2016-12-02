/* Common thread definition for pthread and c11 threads. Linux/HPPA
   version.
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

/* This header contains macros definition required for
   the declaration of pthread_mutex_t and pthread_cond_t,
   both needed by C11 types mtx_t and cnd_t. */

#ifndef _BITS_THREAD_SHARED_TYPES_H
# define _BITS_THREAD_SHARED_TYPES_H	1

#define __SIZEOF_PTHREAD_MUTEX_T 48
#define __SIZEOF_PTHREAD_COND_T 48

typedef struct __pthread_internal_slist
{
  struct __pthread_internal_slist *__next;
} __pthread_slist_t;

/* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
#define __PTHREAD_SPINS 0

/* Data structure for mutex handling. */

struct __pthread_mutex_s
{
  int __lock __attribute__ ((__aligned__(16)));
  unsigned int __count;
  int __owner;
  /* KIND must stay at this position in the structure to maintain
     binary compatibility.  */
  int __kind;
  /* The old 4-word 16-byte aligned lock. This is initalized
     to all ones by the Linuxthreads PTHREAD_MUTEX_INITIALIZER.
     Unused in NPTL.  */
  int __compat_padding[4];
  /* In the old structure there are 4 words left due to alignment.
     In NPTL two words are used.  */
  unsigned int __nusers;
  __extension__ union
  {
    int __spins;
    __pthread_slist_t __list;
  };
  /* Two more words are left before the NPTL
     pthread_mutex_t is larger than Linuxthreads.  */
  int __reserved1;
  int __reserved2;
};

#define __PTHREAD_MUTEX_T_CONTENT         \
  struct __pthread_mutex_s __data;        \
  char __size[__SIZEOF_PTHREAD_MUTEX_T];  \
  long int __align;


/* Data structure for conditional variable handling */

#define __PTHREAD_COND_T_CONTENT          \
  struct                                  \
  {                                       \
    /* In the old Linuxthreads pthread_cond_t, this is the
       start of the 4-word lock structure, the next four words
       are set all to 1 by the Linuxthreads
       PTHREAD_COND_INITIALIZER.  */      \
    int __lock __attribute__ ((__aligned__(16)));  \
    /* Tracks the initialization of this structure:
       0  initialized with NPTL PTHREAD_COND_INITIALIZER.
       1  initialized with Linuxthreads PTHREAD_COND_INITIALIZER.
       2  initialization in progress.  */ \
    int __initializer;                    \
    unsigned int __futex;                 \
    void *__mutex;                        \
    /* In the old Linuxthreads this would have been the start
       of the pthread_fastlock status word.  */ \
    __extension__ unsigned long long int __total_seq; \
    __extension__ unsigned long long int __wakeup_seq; \
    __extension__ unsigned long long int __woken_seq; \
    unsigned int __nwaiters;              \
    unsigned int __broadcast_seq;         \
    /* The NPTL pthread_cond_t is exactly the same size as
       the Linuxthreads version, there are no words to spare.  */ \
  } __data;                               \
  char __size[__SIZEOF_PTHREAD_COND_T];   \
  __extension__ long long int __align;

#endif
