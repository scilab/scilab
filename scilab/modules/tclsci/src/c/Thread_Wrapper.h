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
#else
	#define _WIN32_WINNT 0x500
	#include <Windows.h>
	#include <process.h>

	#define	pthread_t								HANDLE
	#define	pthread_create(t,u,f,d)					*(t)=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)f,d,0,NULL)
	#define	pthread_join(thread, result)			((WaitForSingleObject((thread),INFINITE)!=WAIT_OBJECT_0) || !CloseHandle(thread))
	
	/* http://msdn2.microsoft.com/en-us/library/ms683472(VS.85).aspx */
	#define pthread_mutex_t							CRITICAL_SECTION
		
	#define pthread_mutex_init(pobject,pattr)       InitializeCriticalSection(pobject)
	#define pthread_mutex_destroy(pobject)          
	#define pthread_mutex_lock(pobject)				EnterCriticalSection(pobject)
	#define pthread_mutex_unlock(pobject)	        LeaveCriticalSection(pobject)

	#define pthread_cond_t					        HANDLE
	#define pthread_cond_init(pobject,pattr)        (*pobject=CreateEvent(NULL,FALSE,FALSE,NULL))
	#define pthread_cond_destroy(pobject)           CloseHandle(*pobject)
	#define CV_TIMEOUT			INFINITE
	#define pthread_cond_wait(pobject,pmutex)		{LeaveCriticalSection(pmutex);WaitForSingleObject(*pobject,CV_TIMEOUT);};
	#define pthread_cond_signal(pobject)			SetEvent(*pobject)
#endif

typedef pthread_t __threadId;
typedef pthread_mutex_t __threadLock;
typedef pthread_cond_t __threadSignal;
#define __InitLock(lockName)		pthread_mutex_init(lockName, NULL)

#define __Lock(lockName)			pthread_mutex_lock(lockName)

#define __UnLock(lockName)			pthread_mutex_unlock(lockName)

#define __InitSignal(signalName)		pthread_cond_init(signalName, NULL)
#define __Signal(signalName)			pthread_cond_signal(signalName)

#define __Wait(signalName, lockName)		pthread_cond_wait(signalName, lockName)

#define __CreateThread(threadId, functionName)  pthread_create(threadId, NULL, functionName, NULL)

#define __WaitThreadDie(threadId)		pthread_join(threadId, NULL)


#endif /* !__THREAD_WRAPPER_H__ */

