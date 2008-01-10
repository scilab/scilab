/*
**  -*- C -*-
**
** Thread_Wrapper.h
** Made by  Bruno JOFRET <bruno.jofret@inria.fr>
**
** Started on  Thu Jan 10 15:00:38 2008 bruno
** Last update Thu Jan 10 15:39:20 2008 bruno
**
** Copyright INRIA 2008
*/

/*
** Thread wrapper to have an easy way to manage
** those features on each platform
*/

#ifndef __THREAD_WRAPPER_H__
#define __THREAD_WRAPPER_H__

#include <pthread.h>

typedef pthread_t __threadId;
typedef pthread_mutex_t __threadLock;
typedef pthread_cond_t __threadSignal;

#define __Lock(lockName)			pthread_mutex_lock(lockName)

#define __UnLock(lockName)			pthread_mutex_unlock(lockName)

#define __InitSignal(signalName)		pthread_cond_init(signalName, NULL)
#define __Signal(signalName)			pthread_cond_signal(signalName)

#define __Wait(signalName, lockName)		pthread_cond_wait(signalName, lockName)

#define __CreateThread(threadId, functionName)  pthread_create(threadId, NULL, functionName, NULL)

#define __WaitThreadDie(threadId)		pthread_join(threadId, NULL)

#endif /* !__THREAD_WRAPPER_H__ */

