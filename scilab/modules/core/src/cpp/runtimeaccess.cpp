/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 *
 * Copyright (C) 2026 - Dassault Systemes S.E. - Cedric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include <algorithm>
#include <atomic>
#include <mutex>
#include <vector>

#include "runtimeaccess.hxx"

extern "C"
{
#include "storeCommand.h"
}

namespace
{
std::recursive_mutex& runtimeMutex()
{
    static std::recursive_mutex mutex;
    return mutex;
}

std::mutex& callbackMutex()
{
    static std::mutex mutex;
    return mutex;
}

std::vector<RuntimeStableCallback>& stableCallbacks()
{
    static std::vector<RuntimeStableCallback> callbacks;
    return callbacks;
}

thread_local bool executionOwnsRuntime = false;
std::atomic<bool> runtimeReadsEnabled{false};

void notifyRuntimeStable()
{
    std::lock_guard<std::mutex> lock(callbackMutex());
    for (RuntimeStableCallback callback : stableCallbacks())
    {
        callback();
    }
}
}

void RuntimeAccess::beginExecution()
{
    runtimeMutex().lock();
    executionOwnsRuntime = true;
}

void RuntimeAccess::endExecution()
{
    if (executionOwnsRuntime)
    {
        executionOwnsRuntime = false;
        runtimeMutex().unlock();
    }

    if (isEmptyCommandQueue())
    {
        notifyRuntimeStable();
    }
}

void RuntimeAccess::enterStableState()
{
    if (executionOwnsRuntime)
    {
        executionOwnsRuntime = false;
        runtimeMutex().unlock();
    }
    notifyRuntimeStable();
}

void RuntimeAccess::leaveStableState()
{
    runtimeMutex().lock();
    executionOwnsRuntime = true;
}

void RuntimeAccess::runtimeInitialized()
{
    {
        std::lock_guard<std::recursive_mutex> lock(runtimeMutex());
        runtimeReadsEnabled.store(true, std::memory_order_release);
    }
    notifyRuntimeStable();
}

void RuntimeAccess::runtimeShuttingDown()
{
    // Wait for readers that already own the runtime before teardown starts.
    // New non-blocking readers observe false before or immediately after
    // acquiring the mutex and cannot enter a context being destroyed.
    std::lock_guard<std::recursive_mutex> lock(runtimeMutex());
    runtimeReadsEnabled.store(false, std::memory_order_release);
}

void RuntimeAccess::lockRead()
{
    runtimeMutex().lock();
}

bool RuntimeAccess::tryLockRead()
{
    if (!runtimeReadsEnabled.load(std::memory_order_acquire)
            || !runtimeMutex().try_lock())
    {
        return false;
    }

    if (!runtimeReadsEnabled.load(std::memory_order_acquire))
    {
        runtimeMutex().unlock();
        return false;
    }

    return true;
}

void RuntimeAccess::unlockRead()
{
    runtimeMutex().unlock();
}

void RuntimeAccess::addStableCallback(RuntimeStableCallback callback)
{
    std::lock_guard<std::mutex> lock(callbackMutex());
    std::vector<RuntimeStableCallback>& callbacks = stableCallbacks();
    if (std::find(callbacks.begin(), callbacks.end(), callback) == callbacks.end())
    {
        callbacks.push_back(callback);
    }
}

void RuntimeAccess::removeStableCallback(RuntimeStableCallback callback)
{
    std::lock_guard<std::mutex> lock(callbackMutex());
    std::vector<RuntimeStableCallback>& callbacks = stableCallbacks();
    callbacks.erase(std::remove(callbacks.begin(), callbacks.end(), callback), callbacks.end());
}

RuntimeExecutionGuard::RuntimeExecutionGuard()
{
    RuntimeAccess::beginExecution();
}

RuntimeExecutionGuard::~RuntimeExecutionGuard()
{
    RuntimeAccess::endExecution();
}

RuntimeUnlockGuard::RuntimeUnlockGuard()
{
    RuntimeAccess::enterStableState();
}

RuntimeUnlockGuard::~RuntimeUnlockGuard()
{
    RuntimeAccess::leaveStableState();
}

RuntimeLockGuard::RuntimeLockGuard()
{
    RuntimeAccess::lockRead();
}

RuntimeLockGuard::~RuntimeLockGuard()
{
    RuntimeAccess::unlockRead();
}

int RuntimeAccess_tryLockRead(void)
{
    return RuntimeAccess::tryLockRead() ? 1 : 0;
}

void RuntimeAccess_unlockRead(void)
{
    RuntimeAccess::unlockRead();
}

void RuntimeAccess_addStableCallback(RuntimeStableCallback callback)
{
    RuntimeAccess::addStableCallback(callback);
}

void RuntimeAccess_removeStableCallback(RuntimeStableCallback callback)
{
    RuntimeAccess::removeStableCallback(callback);
}
