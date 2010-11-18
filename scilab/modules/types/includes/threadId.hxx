/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __THREADID_HH__
#define __THREADID_HH__

#include <map>

#include "types.hxx"

extern "C"
{
#include "Thread_Wrapper.h"
}

namespace types
{
    class ThreadId : public GenericType
    {
    public :
        virtual                 ~ThreadId();

        ThreadId*               getAsThreadId(void);
        wstring                 toString(int _iPrecision, int _iLineLen);

        /* return type as string ( double, int, cell, list, ... )*/
        virtual wstring         getTypeStr() {return L"ThreadId";}
        /* return type as short string ( s, i, ce, l, ... )*/
        virtual wstring         getShortTypeStr() {return L"tid";}
        ThreadId*                 clone();

        __threadId              getId();
        void                    setId(__threadId _id);

        static  ThreadId*       createThreadId(__threadId _id);

    private :
        /* Constructor is private to force use of createThreadId */
                                ThreadId(__threadId _id);

    protected :
        RealType                getType();//			{ return RealString; }

    private :
        __threadId                                 m_threadId;
        static std::map<__threadId, ThreadId *>    m_threadList;

    };
}
#endif /* !__THREADID_HH__ */
