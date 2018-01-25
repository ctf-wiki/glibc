#ifndef _PTHREAD_H
#include <pthread/pthread.h>

/* These represent the interface used by glibc itself.  */

extern int __pthread_mutex_lock (pthread_mutex_t *__mutex);
extern int __pthread_mutex_unlock (pthread_mutex_t *__mutex);

typedef struct __cthread *__cthread_t;
typedef int __cthread_key_t;
typedef void *	(*__cthread_fn_t)(void *__arg);

__cthread_t __cthread_fork (__cthread_fn_t, void *);
void __cthread_detach (__cthread_t);
int __cthread_keycreate (__cthread_key_t *);
int __cthread_getspecific (__cthread_key_t, void **);
int __cthread_setspecific (__cthread_key_t, void *);

int __pthread_getattr_np (pthread_t, pthread_attr_t *);
int __pthread_attr_getstack (const pthread_attr_t *, void **, size_t *);

#endif
