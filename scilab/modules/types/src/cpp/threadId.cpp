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
#include "string.hxx"
#include "threadId.hxx"
#include "tostring_common.hxx"

extern "C"
{
#include "charEncoding.h"
#include "os_wcsdup.h"
}

namespace types
{

    std::map<__threadId, ThreadId *>    ThreadId::m_threadList;

    ThreadId*       ThreadId::createThreadId(__threadId _id)
    {
        ThreadId* ptidThread = m_threadList[_id];

        if(ptidThread == NULL)
        {
            ptidThread = new ThreadId(_id);
            m_threadList[_id] = ptidThread;
        }


        return ptidThread;
    }

	ThreadId::~ThreadId()
    {
        this->DecreaseRef();

        if (this->isDeletable())
        {
            m_threadList.erase(this->getId());
        }
    }

	ThreadId::ThreadId(__threadId _id)
	{
        m_threadId = _id;
	}

    __threadId ThreadId::getId()
    {
        return m_threadId;
    }

    void ThreadId::setId(__threadId _id)
    {
        m_threadId = _id;
    }

	ThreadId *ThreadId::clone()
	{
        return this;
	}

	ThreadId* ThreadId::getAsThreadId(void)
	{
		return this;
	}

	GenericType::RealType ThreadId::getType()
	{
		return GenericType::RealThreadId;
	}

	wstring ThreadId::toString(int _iPrecision, int _iLineLen)
	{
		wostringstream ostr;

        ostr << L"ThreadId : " << this;

        return ostr.str();
    }
}

