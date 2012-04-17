/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include <map>
#include <sstream>
#include "core_math.h"
#include "threadId.hxx"
#include "tostring_common.hxx"

extern "C"
{
#include "charEncoding.h"
#include "os_wcsdup.h"
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
    }

    __threadId ThreadId::getId()
    {
        return m_threadId;
    }

    void ThreadId::setId(__threadId _id)
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

    InternalType* ThreadId::clone()
    {
        return this;
    }

    InternalType::RealType ThreadId::getType()
    {
        return InternalType::RealThreadId;
    }

    std::wstring ThreadId::StatusToString(Status _status)
    {
        switch(_status)
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
}

