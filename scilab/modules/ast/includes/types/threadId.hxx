/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
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

#ifndef __THREADID_HH__
#define __THREADID_HH__

#include "internal.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

namespace types
{
class EXTERN_AST ThreadId : public InternalType
{
public :
    ThreadId(__threadId _id, __threadKey _key);
    virtual                 ~ThreadId();

    bool                    isThreadId()
    {
        return true;
    }
    bool                    toString(std::wostringstream& ostr);

    /* return type as string ( double, int, cell, list, ... )*/
    virtual std::wstring    getTypeStr()
    {
        return L"ThreadId";
    }
    /* return type as short string ( s, i, ce, l, ... )*/
    virtual std::wstring    getShortTypeStr()
    {
        return L"tid";
    }
    ThreadId*               clone();

    __threadId              getThreadId();
    void                    setThreadId(__threadId _id);
    __threadKey             getKey();
    void                    setKey(__threadKey _key);

    /*
    ** Status
    */
    enum Status
    {
        Running,
        Paused,
        Aborted,
        Done
    };

    Status                  getStatus();
    void                    setStatus(Status _status);

    void                    suspend();
    void                    resume();
    void                    abort();

    void                    setInterrupt(bool _doInterrupt);
    bool                    getInterrupt();

    void                    setInterruptible(bool _isInterruptible);
    bool                    isInterruptible();

    void                    setCommandOrigin(int _iCmdOrigin);
    int                     getCommandOrigin();

private :
    std::wstring            StatusToString(Status _status);

protected :
    inline ScilabType       getType()
    {
        return ScilabThreadId;
    }
    inline ScilabId         getId()
    {
        return IdThreadId;
    }


private :
    __threadId              m_threadId;
    __threadKey             m_threadKey;
    __threadLock            m_threadLock;
    Status                  m_threadStatus;
    bool                    m_doInterrupt;
    bool                    m_isInterruptible;
    int                     m_iCmdOrigin;
};
}
#endif /* !__THREADID_HH__ */
