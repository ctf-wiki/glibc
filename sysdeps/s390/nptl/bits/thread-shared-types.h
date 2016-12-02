/* Copyright (C) 2016 Free Software Foundation, Inc.
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
#define _BITS_THREAD_SHARED_TYPES_H	1

#include <bits/wordsize.h>

#if __WORDSIZE == 64
# define __SIZEOF_PTHREAD_MUTEX_T 40
# define __SIZEOF_PTHREAD_COND_T 48
#else
# define __SIZEOF_PTHREAD_MUTEX_T 24
# define __SIZEOF_PTHREAD_COND_T 48
#endif

#if __WORDSIZE == 64
typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;
#else
typedef struct __pthread_internal_slist
{
  struct __pthread_internal_slist *__next;
} __pthread_slist_t;
#endif


/* Data structure for mutex handling. */

struct __pthread_mutex_s
{
  int __lock;
  unsigned int __count;
  int __owner;
#if __WORDSIZE == 64
  unsigned int __nusers;
#endif
  /* KIND must stay at this position in the structure to maintain
     binary compatibility.  */
  int __kind;
#if __WORDSIZE == 64
# ifdef ENABLE_LOCK_ELISION
  short __spins;
  short __elision;
  /* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
#  define __PTHREAD_SPINS               0, 0
# else
  int __spins;
  /* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
#  define __PTHREAD_SPINS               0
# endif
  __pthread_list_t __list;
# define __PTHREAD_MUTEX_HAVE_PREV	1
#else
  unsigned int __nusers;
  __extension__ union
  {
# ifdef ENABLE_LOCK_ELISION
    struct
    {
      short __espins;
      short __elision;
    } _d;
#  define __spins _d.__espins
#  define __elision _d.__elision
  /* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
#  define __PTHREAD_SPINS               { 0, 0 }
# else
    int __spins;
  /* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
#  define __PTHREAD_SPINS               0
# endif
    __pthread_slist_t __list;
  };
#endif
};

#define __PTHREAD_MUTEX_T_CONTENT         \
  struct __pthread_mutex_s __data;        \
  char __size[__SIZEOF_PTHREAD_MUTEX_T];  \
  long int __align;


/* Data structure for conditional variable handling */

#define __PTHREAD_COND_T_CONTENT			\
  struct						\
  {							\
    __extension__ union					\
    {							\
      __extension__ unsigned long long int __wseq;	\
      struct {						\
        unsigned int __low;				\
        unsigned int __high;				\
      } __wseq32;					\
    };							\
    __extension__ union					\
    { 							\
      __extension__ unsigned long long int __g1_start;	\
      struct {						\
        unsigned int __low;				\
        unsigned int __high;				\
      } __g1_start32;					\
    };							\
    unsigned int __g_refs[2];				\
    unsigned int __g_size[2];				\
    unsigned int __g1_orig_size;			\
    unsigned int __wrefs;				\
    unsigned int __g_signals[2];			\
  } __data;						\
  char __size[__SIZEOF_PTHREAD_COND_T];			\
  __extension__ long long int __align;

/* Used to force once_flag alignment.  */
#define __ONCE_FLAG_ALIGN

#endif
