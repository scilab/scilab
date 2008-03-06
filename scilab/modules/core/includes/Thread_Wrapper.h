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

#ifndef _MSC_VER
	#include <pthread.h>
#else
	#ifndef _WIN32_WINNT
		#define _WIN32_WINNT 0x500
	#endif
	#include <Windows.h>
	#include <process.h>

	#define	pthread_t								HANDLE
	#define	pthread_create(t,u,f,d)					*(t)=CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)f,d,0,NULL)
	#define	pthread_join(thread, result)			((WaitForSingleObject((thread),INFINITE)!=WAIT_OBJECT_0) || !CloseHandle(thread))
	#define	pthread_cancel(thread)					TerminateThread(thread,0)

    /* returns to previous version with mutex */
    /* I don't want to manage a exception here on some windows and not some others */
	/* http://msdn2.microsoft.com/en-us/library/ms682411.aspx */
	/* http://msdn2.microsoft.com/en-us/library/ms683472(VS.85).aspx */
    /* Windows Server 2003 and Windows XP/2000:  In low memory situations,
	   InitializeCriticalSection can raise a STATUS_NO_MEMORY exception.
	   This exception was eliminated starting with Windows Vista.
	 */

	#define pthread_mutex_t							HANDLE

	#define pthread_mutex_init(pobject,pattr)       (*(pobject)=CreateMutex(NULL,FALSE,NULL))
	#define pthread_mutex_destroy(pobject)          CloseHandle(*pobject)
	#define pthread_mutex_lock(pobject)				WaitForSingleObject(*pobject,INFINITE)
	#define pthread_mutex_unlock(pobject)	        ReleaseMutex(*pobject)


	#define pthread_cond_t					        HANDLE
	#define pthread_cond_init(pobject,pattr)        (*pobject=CreateEvent(NULL,FALSE,FALSE,NULL))
	#define pthread_cond_destroy(pobject)           CloseHandle(*pobject)
	#define CV_TIMEOUT			INFINITE
	#define pthread_cond_wait(pobject,pmutex)		{ReleaseMutex(*pmutex);WaitForSingleObject(*pobject,CV_TIMEOUT);};
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

