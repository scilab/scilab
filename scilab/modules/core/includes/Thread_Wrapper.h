/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2008-2008 - INRIA - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*
** Thread wrapper to have an easy way to manage
** those features on each platform
*/

#ifndef __THREAD_WRAPPER_H__
#define __THREAD_WRAPPER_H__

#ifdef _MSC_VER
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x500
	#endif
	#include <Windows.h>
	#include <process.h>

	typedef HANDLE				__threadId;
	typedef HANDLE				__threadLock;
	typedef CRITICAL_SECTION	__threadSignalLock;
	typedef HANDLE				__threadSignal;

	#define __InitSignalLock(lockName)				InitializeCriticalSection(lockName)

	#define __LockSignal(lockName)					EnterCriticalSection(lockName)

	#define __UnLockSignal(lockName)				LeaveCriticalSection(lockName)

	#define __InitLock(lockName)					(*(lockName)=CreateMutex(NULL,FALSE,NULL))

	#define __Lock(lockName)						WaitForSingleObject(*lockName,INFINITE)

	#define __UnLock(lockName)						ReleaseMutex(*lockName)

	#define __InitSignal(signalName)				(*signalName=CreateEvent(NULL,FALSE,FALSE,NULL))

	#define __Signal(signalName)					SetEvent(*signalName)

	#define __Wait(signalName, lockName)			{ResetEvent(*signalName);__UnLockSignal(lockName);WaitForSingleObject(*signalName, INFINITE);__LockSignal(lockName);};

	#define __CreateThread(threadId, functionName)  *(threadId)=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)functionName, NULL,0,NULL)

	#define __WaitThreadDie(threadId)				((WaitForSingleObject((threadId),INFINITE)!=WAIT_OBJECT_0) || !CloseHandle(threadId))

	#define __Terminate(threadId)					TerminateThread(threadId, 0)

        #define __StaticInitLock                                        NULL

#else
	#include <pthread.h>

typedef pthread_t __threadId;
typedef pthread_mutex_t __threadLock;
typedef pthread_mutex_t __threadSignalLock;
typedef pthread_cond_t __threadSignal;

#define __InitLock(lockName)			pthread_mutex_init(lockName, NULL)

#define __Lock(lockName)			pthread_mutex_lock(lockName)

#define __UnLock(lockName)			pthread_mutex_unlock(lockName)

#define __InitSignalLock(lockName)			pthread_mutex_init(lockName, NULL)

#define __LockSignal(lockName)			pthread_mutex_lock(lockName)

#define __UnLockSignal(lockName)		pthread_mutex_unlock(lockName)

#define __InitSignal(signalName)		pthread_cond_init(signalName, NULL)

#define __Signal(signalName)			pthread_cond_signal(signalName)

#define __Wait(signalName, lockName)		pthread_cond_wait(signalName, lockName)

#define __CreateThread(threadId, functionName)  pthread_create(threadId, NULL, functionName, NULL)

#define __WaitThreadDie(threadId)		pthread_join(threadId, NULL)

#define __Terminate(threadId)			pthread_cancel(threadId)

#define __StaticInitLock                        PTHREAD_MUTEX_INITIALIZER

#endif //_MSC_VER

#endif /* !__THREAD_WRAPPER_H__ */

