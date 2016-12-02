/* Common thread definition for pthread and c11 threads. Linux/alpha
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

/* This header contains macros definition required for the declaration of
   pthread_mutex_t and pthread_cond_t, both needed by C11 types mtx_t and
   cnd_t. */

#ifndef _BITS_PTHREAD_SHARED_TYPES_H
# define _BITS_PTHREAD_SHARED_TYPES_H	1

#define __SIZEOF_PTHREAD_MUTEX_T 40
#define __SIZEOF_PTHREAD_COND_T 48


typedef struct __pthread_internal_list
{
  struct __pthread_internal_list *__prev;
  struct __pthread_internal_list *__next;
} __pthread_list_t;


struct __pthread_mutex_s
{
  int __lock;
  unsigned int __count;
  int __owner;
  unsigned int __nusers;
  /* KIND must stay at this position in the structure to maintain
     binary compatibility with static initializers.  */
  int __kind;
  int __spins;
  __pthread_list_t __list;
#define __PTHREAD_MUTEX_HAVE_PREV	1
};

/* Mutex __spins initializer used by PTHREAD_MUTEX_INITIALIZER.  */
#define __PTHREAD_SPINS 0

#define __PTHREAD_MUTEX_T_CONTENT		\
  struct __pthread_mutex_s  __data;		\
  char __size[__SIZEOF_PTHREAD_MUTEX_T];	\
  long int __align;


/* Data structure for conditional variable handling */
#define __PTHREAD_COND_T_CONTENT			\
  struct						\
  {							\
    __extension__ union					\
    { 							\
      __extension__ unsigned long long int __wseq;	\
      struct {						\
        unsigned int __low;				\
        unsigned int __high;				\
      } __wseq32;					\
    };							\
    __extension__ union					\
    {							\
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
