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

#ifndef _MSC_VER

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

#else
	#include <Windows.h>

	typedef HANDLE __threadId;
	typedef HANDLE __threadLock;
	typedef HANDLE __threadSignal;

	#define __Lock(lockName)			(WaitForSingleObject((*(lockName)),INFINITE))

	#define __UnLock(lockName)			ReleaseMutex(*(lockName))

	#define __InitSignal(signalName)		(*(signalName)) = CreateEvent(NULL,FALSE,FALSE,NULL)

	#define __Signal(signalName)			(SetEvent((*(signalName))))
	
	#define __Wait(signalName, lockName)		(ReleaseMutex(*(lockName)),(WaitForSingleObject((*(signalName)),INFINITE)))

	static DWORD _thread_tmp;

	#define __CreateThread(threadId, functionName)  (*(threadId))=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)functionName,NULL,0,&_thread_tmp)

	#define __WaitThreadDie(threadId)		WaitForSingleObject(threadId,INFINITE)

#endif

#endif /* !__THREAD_WRAPPER_H__ */

