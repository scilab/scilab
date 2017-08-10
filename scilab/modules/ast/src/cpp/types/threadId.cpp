/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include <map>
#include <sstream>
#include "core_math.h"
#include "threadId.hxx"
#include "tostring_common.hxx"
#include "scilabWrite.hxx"

extern "C"
{
#include "charEncoding.h"
#include "os_string.h"
}

namespace types
{

ThreadId::~ThreadId()
{
}

ThreadId::ThreadId(__threadId _id, __threadKey _key)
{
    m_threadId = _id;
    m_threadKey = _key;
    __InitLock(&m_threadLock);
    m_threadStatus = Running;
    m_isInterruptible = true;
    m_doInterrupt = false;
    m_iCmdOrigin = 0; // NONE
}

__threadId ThreadId::getThreadId()
{
    return m_threadId;
}

void ThreadId::setThreadId(__threadId _id)
{
    this->m_threadId = _id;
}

__threadKey ThreadId::getKey()
{
    return m_threadKey;
}

void ThreadId::setKey(__threadKey _key)
{
    this->m_threadKey = _key;
}

ThreadId* ThreadId::clone()
{
    return this;
}

std::wstring ThreadId::StatusToString(Status _status)
{
    switch (_status)
    {
        case Running :
            return L"Running";
        case Paused :
            return L"Paused";
        case Aborted :
            return L"Aborted";
        case Done :
            return L"Done";
    }
    return L"";
}

void ThreadId::setStatus(ThreadId::Status _status)
{
    m_threadStatus = _status;
}

ThreadId::Status ThreadId::getStatus(void)
{
    return m_threadStatus;
}

void ThreadId::suspend()
{
    setStatus(Paused);
    /*
    ** BJ: Under Linux / Mac having a lock twice will force thread to wait.
    ** this will not work under windows. So use native Thread suspend and resume functions.
    */

#ifndef _MSC_VER
    __Lock(&m_threadLock);
    // UnLock will come if resume is called for that thread.
    __Lock(&m_threadLock);
    __UnLock(&m_threadLock);
#else
    __SuspendThread(m_threadId);
#endif
}

void ThreadId::resume()
{
    setStatus(Running);
    /*
    ** BJ: Lock trick to force thread wait does not work under windows.
    ** This will release Thread::suspend function.
    ** Thread waiting in pause GW will then continue and check new thread status (Running)
    */
#ifndef _MSC_VER
    __UnLock(&m_threadLock);
#else
    __ResumeThread(m_threadId);
#endif
}

void ThreadId::abort()
{
    setStatus(Aborted);
    /*
    ** BJ: Lock trick to force thread wait does not work under windows.
    ** This will release Thread::suspend function.
    ** Thread waiting in pause GW will then continue and check new thread status (Aborted)
    */
#ifndef _MSC_VER
    __UnLock(&m_threadLock);
#else
    __ResumeThread(m_threadId);
#endif
}

bool ThreadId::toString(std::wostringstream& ostr)
{
    ostr << L"ThreadId : " << this << std::endl;
    ostr << L"Status : " << StatusToString(this->getStatus());
    return true;
}

void ThreadId::setInterrupt(bool _doInterrupt)
{
    m_doInterrupt = _doInterrupt;
}

bool ThreadId::getInterrupt()
{
    return m_doInterrupt;
}

void ThreadId::setInterruptible(bool _isInterruptible)
{
    m_isInterruptible = _isInterruptible;
}

bool ThreadId::isInterruptible()
{
    return m_isInterruptible;
}

void ThreadId::setCommandOrigin(int _iCmdOrigin)
{
    m_iCmdOrigin = _iCmdOrigin;
}

int ThreadId::getCommandOrigin()
{
    return m_iCmdOrigin;
}

}
