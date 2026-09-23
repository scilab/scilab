/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systemes S.E. - Cedric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __RUNTIME_ACCESS_HXX__
#define __RUNTIME_ACCESS_HXX__

extern "C"
{
#include "dynlib_core.h"
}

typedef void (*RuntimeStableCallback)();

// Serializes interpreter execution with readers of the global Scilab runtime
// context (LSP, completion, diagnostics, ...).
class CORE_IMPEXP RuntimeAccess
{
public:
    static void beginExecution();
    static void endExecution();
    static void enterStableState();
    static void leaveStableState();
    static void runtimeInitialized();
    static void runtimeShuttingDown();
    static void lockRead();
    static bool tryLockRead();
    static void unlockRead();
    static void addStableCallback(RuntimeStableCallback callback);
    static void removeStableCallback(RuntimeStableCallback callback);
};

class CORE_IMPEXP RuntimeExecutionGuard
{
public:
    RuntimeExecutionGuard();
    ~RuntimeExecutionGuard();

    RuntimeExecutionGuard(const RuntimeExecutionGuard&) = delete;
    RuntimeExecutionGuard& operator=(const RuntimeExecutionGuard&) = delete;
};

class CORE_IMPEXP RuntimeUnlockGuard
{
public:
    RuntimeUnlockGuard();
    ~RuntimeUnlockGuard();

    RuntimeUnlockGuard(const RuntimeUnlockGuard&) = delete;
    RuntimeUnlockGuard& operator=(const RuntimeUnlockGuard&) = delete;
};

extern "C"
{
    int RuntimeAccess_tryLockRead(void);
    void RuntimeAccess_unlockRead(void);
    void RuntimeAccess_addStableCallback(RuntimeStableCallback callback);
    void RuntimeAccess_removeStableCallback(RuntimeStableCallback callback);
}

class CORE_IMPEXP RuntimeLockGuard
{
public:
    RuntimeLockGuard();
    ~RuntimeLockGuard();

    RuntimeLockGuard(const RuntimeLockGuard&) = delete;
    RuntimeLockGuard& operator=(const RuntimeLockGuard&) = delete;
};

class RuntimeTryLockGuard
{
public:
    RuntimeTryLockGuard() : m_locked(RuntimeAccess_tryLockRead() != 0)
    {
    }

    ~RuntimeTryLockGuard()
    {
        if (m_locked)
        {
            RuntimeAccess_unlockRead();
        }
    }

    bool ownsLock() const
    {
        return m_locked;
    }

    RuntimeTryLockGuard(const RuntimeTryLockGuard&) = delete;
    RuntimeTryLockGuard& operator=(const RuntimeTryLockGuard&) = delete;

private:
    bool m_locked;
};

#endif /* !__RUNTIME_ACCESS_HXX__ */
