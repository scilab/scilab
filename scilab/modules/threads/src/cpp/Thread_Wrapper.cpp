/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *  Copyright (C) 2008-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2011-2011 - DIGITEO - Bruno JOFRET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

extern "C"
{
#include "Thread_Wrapper.h"
}

void __InitLock(__threadLock *lockName)
{
#ifdef _MSC_VER
    *(lockName) = CreateMutex(NULL, FALSE, NULL);
#else
    pthread_mutex_init(lockName, NULL);
#endif
}

void __Lock(__threadLock *lockName)
{
#ifdef _MSC_VER
    WaitForSingleObject(*lockName, INFINITE);
#else
    pthread_mutex_lock(lockName);
#endif
}

void __UnLock(__threadLock *lockName)
{
#ifdef _MSC_VER
    ReleaseMutex(*lockName);
#else
    pthread_mutex_unlock(lockName);
#endif
}

void __InitSignalLock(__threadSignalLock *lockName)
{
#ifdef _MSC_VER
    InitializeCriticalSection(lockName);
#else
    /* PTHREAD_MUTEX_ERRORCHECK needed for a safe release atexit when we try to release without knowing if we own the lock
       PTHREAD_PROCESS_SHARED needed for interprocess synch (plus alloc in shared mem thread_mutexattr_settype
       Linux uses PTHREAD_MUTEX_ERRORCHECK_NP other Posix use PTHREAD_MUTEX_ERRORCHECK
    */
#ifndef PTHREAD_MUTEX_ERRORCHECK
#define PTHREAD_MUTEX_ERRORCHECK PTHREAD_MUTEX_ERRORCHECK_NP
#endif
    pthread_mutexattr_t attr;
    pthread_mutexattr_init (&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutexattr_setpshared (&attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(lockName, NULL);
    pthread_mutexattr_destroy(&attr);
#endif
}


void __LockSignal(__threadSignalLock *lockName)
{
#ifdef _MSC_VER
    EnterCriticalSection(lockName);
#else
    pthread_mutex_lock(lockName);
#endif
}

void __UnLockSignal(__threadSignalLock *lockName)
{
#ifdef _MSC_VER
    LeaveCriticalSection(lockName);
#else
    pthread_mutex_unlock(lockName);
#endif
}

void __InitSignal(__threadSignal *signalName)
{
#ifdef _MSC_VER
    *signalName = CreateEvent(NULL, FALSE, FALSE, NULL);
#else
    pthread_cond_init(signalName, NULL);
#endif
}

void __Signal(__threadSignal *signalName)
{
#ifdef _MSC_VER
    SetEvent(*signalName);
#else
    pthread_cond_signal(signalName);
#endif
}

void __Wait(__threadSignal *signalName, __threadSignalLock *lockName)
{
#ifdef _MSC_VER
    ResetEvent(*signalName);
    __UnLockSignal(lockName);
    WaitForSingleObject(*signalName, INFINITE);
    __LockSignal(lockName);
#else
    pthread_cond_wait(signalName, lockName);
#endif
}

void __CreateThread(__threadId *threadId, __threadKey *threadKey, void * (*functionName) (void *))
{
    __CreateThreadWithParams(threadId, threadKey, functionName, NULL);
}

void __CreateThreadWithParams(__threadId *threadId, __threadKey *threadKey, void * (*functionName) (void *), void *params)
{
#ifdef _MSC_VER
    size_t size = 128 * 1024 * 1024;
    *(threadId) = CreateThread(NULL, size, (LPTHREAD_START_ROUTINE)functionName, params, 0, threadKey);
#else

    /*
    ** We need to increase call stack under MacOSX && LINUX.
    ** The default one is too small...
    */
    pthread_attr_t threadAttr;
#ifdef __APPLE__
    size_t size = 128 * 1024 * 1024;
    //void *stackbase = (void *) malloc(size);
    pthread_attr_init(&threadAttr);

    pthread_attr_setstacksize(&threadAttr, size);
    //pthread_attr_setstackaddr(&threadAttr, stackbase);
    pthread_create(threadId, &threadAttr, functionName, params);
#else //Linux
    size_t size = 128 * 1024 * 1024;
    //void *stackbase = (void *) malloc(size);
    pthread_attr_init(&threadAttr);

    //pthread_attr_setstack(&threadAttr, stackbase, size);
    pthread_attr_setstacksize(&threadAttr, size);
    pthread_create(threadId, &threadAttr, functionName, params);
#endif
    *threadKey = *threadId;
#endif
}

void __WaitThreadDie(__threadId threadId)
{
#ifdef _MSC_VER
    ((WaitForSingleObject((threadId), INFINITE) != WAIT_OBJECT_0) || !CloseHandle(threadId));
#else
    pthread_join(threadId, NULL);
#endif
}

void __Terminate(__threadId threadId)
{
#ifdef _MSC_VER
    TerminateThread(threadId, 0);
#else
    pthread_cancel(threadId);
#endif
}

__threadId __GetCurrentThreadId()
{
#ifdef _MSC_VER
    return GetCurrentThread();
#else
    return pthread_self();
#endif
}

__threadKey __GetCurrentThreadKey()
{
#ifdef _MSC_VER
    return GetCurrentThreadId();
#else
    return pthread_self();
#endif
}

void __SuspendThread(__threadId ThreadId)
{
#ifdef _MSC_VER
    SuspendThread(ThreadId);
#else
    pthread_kill(ThreadId, SIGUSR1);
#endif
}

void __ResumeThread(__threadId ThreadId)
{
#ifdef _MSC_VER
    ResumeThread(ThreadId);
#else
    pthread_kill(ThreadId, SIGUSR2);
#endif
}

