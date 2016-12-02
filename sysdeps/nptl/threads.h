/* ISO C11 Standard: 7.26 - Thread support library  <threads.h>.
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

#ifndef _THREADS_H
#define _THREADS_H	1

#include <features.h>
#include <time.h>

__BEGIN_DECLS

#include <bits/thread-shared-types.h>

#define ONCE_FLAG_INIT 0
#define thread_local _Thread_local
#define TSS_DTOR_ITERATIONS 4

typedef unsigned long int thrd_t;         /* Based on pthread_t.  */
typedef int __ONCE_FLAG_ALIGN once_flag;  /* Based on pthread_once_t.  */
typedef unsigned int tss_t;               /* Based on pthread_key_t.  */
typedef int (*thrd_start_t) (void*);
typedef void (*tss_dtor_t) (void*);

/* Exit and error codes.  */
enum
{
  thrd_success  = 0,
  thrd_busy     = 1,
  thrd_error    = 2,
  thrd_nomem    = 3,
  thrd_timedout = 4
};

/* Kinds of mutexes.  */
enum
{
  mtx_plain     = 0,
  mtx_recursive = 1,
  mtx_timed     = 2
};

/* Definition of mtx_t based on pthread_mutex_t.  */
typedef union
{
  __PTHREAD_MUTEX_T_CONTENT
} mtx_t;

/* Definition of cnd_t based on pthread_cond_t.  */
typedef union
{
  __PTHREAD_COND_T_CONTENT
} cnd_t;

/* Threads functions.  */

/* Create a new thread executing the function __func. Arguments to __func
   are passed through __arg. If succesful, __thr is set to new
   thread identifier.  */
extern int thrd_create (thrd_t *__thr, thrd_start_t __func, void *__arg);

/* Check if __lhs and __rhs point to the same thread.  */
extern int thrd_equal (thrd_t __lhs, thrd_t __rhs);

/* Return current thread identifier.  */
extern thrd_t thrd_current (void);

/* Block current thread execution for at least the time pointed by
   __time_point.  The current thread may resume if receives a signal.  In
   that case, if __remaining is not NULL, the remaining time is stored in
   the object pointed by __remaining.  */
extern int thrd_sleep (const struct timespec *__time_point,
		       struct timespec *__remaining);

/* Terminate current thread execution, cleaning up any thread local
   storage and freeing resources. Returns the value specified in __res.  */
extern void thrd_exit (int __res) __attribute__ ((__noreturn__));

/* Detache the thread identified by __thr from the current environment.
   That means we cannot join or wait for __thr to finish it's execution.  */
extern int thrd_detach (thrd_t __thr);

/* Block current thread until execution of __thr is complete.  In case that
   __res is not NULL, will store the return value of __thr when exiting.  */
extern int thrd_join (thrd_t __thr, int *__res);

/* Stop current thread execution and call the scheduler to decide which
   thread should execute next.  The current thread may be selected by the
   scheduler to keep running.  */
extern void thrd_yield (void);

/* Mutex functions.  */

/* Creates a new mutex object with type __type.  If successful the new
   object is pointed by __mutex.  */
extern int mtx_init (mtx_t *__mutex, int __type);

/* Block the current thread until the mutex pointed to
   by __mutex is unlocked. In case the mutex is free,
   the current thread will not be blocked.  */
extern int mtx_lock (mtx_t *__mutex);

/* Block the current thread until the mutex pointed by __mutex is unlocked
   or time pointed by __time_point is reached.  In case the mutex is free,
   the current thread will not be blocked.  */
extern int mtx_timedlock (mtx_t *__restrict __mutex,
			  const struct timespec *__restrict __time_point);

/* Try to lock the mutex pointed by __mutex without blocking.  If the mutex
   is free, current threads takes control of it, and if it's not, returns
   immediately.  */
extern int mtx_trylock (mtx_t *__mutex);

/* Unlock the mutex pointed to by __mutex, what can lead to
   awaking other threads waiting on this mutex.  */
extern int mtx_unlock (mtx_t *__mutex);

/* Destroy the mutex object pointed by __mutex.  */
extern void mtx_destroy (mtx_t *__mutex);

/* Call function __func exactly once, even if invoked from several threads.
   However, all calls must be made with the same __flag object.  */
extern void call_once (once_flag *__flag, void (*__func)(void));

/* Condition variable functions.  */

/* Initialize new condition variable pointed by __cond.  */
extern int cnd_init (cnd_t *__cond);

/* Unblock one thread that currently waits on condition variable pointed to
   by __cond.  */
extern int cnd_signal (cnd_t *__cond);

/* Unblock all threads currently waiting on condition variable pointed to by
   __cond.  */
extern int cnd_broadcast (cnd_t *__cond);

/* Block current thread on the condition variable
   pointed to by __cond.  */
extern int cnd_wait (cnd_t *__cond, mtx_t *__mutex);

/* Block current thread on the condition variable until condition variable
   pointed on by __cond is signalled or time pointed by __time_point is
   reached.  */
extern int cnd_timedwait (cnd_t *__restrict __cond,
			  mtx_t *__restrict __mutex,
			  const struct timespec *__restrict __time_point);

/* Destroy condition variable pointed to by __cond and free all of its
   resources.  */
extern void cnd_destroy (cnd_t *__cond);

/* Thread specific storage functions.  */

/* Create new thread-specific storage key and stores it in the object pointed
   by __tss_id.  If __destructor is not NULL, __destructor function is called
   when the thread terminates.  */
extern int tss_create (tss_t *__tss_id, tss_dtor_t __destructor);

/* Return the value held in thread-specific storage for the current thread
   identified by __tss_id.  */
extern void *tss_get (tss_t __tss_id);

/* Sets the value of the thread-specific storage identified by __tss_id for
   the current thread to __val.  */
extern int tss_set (tss_t __tss_id, void *__val);

/* Destroys the thread-specific storage identified by __tss_id.  However, the
   destructor is not called until thrd_exit is called.  */
extern void tss_delete (tss_t __tss_id);

#ifdef __USE_EXTERN_INLINES
/* Optimizations.  */
__extern_inline int
__NTH (thrd_equal (thrd_t __thread1, thrd_t __thread2))
{
  return __thread1 == __thread2;
}
#endif

__END_DECLS

#endif /* _THREADS_H */
