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
#include "cell.hxx"
#include "threadId.hxx"
#include "tostring_common.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "charEncoding.h"
#include "os_wcsdup.h"
}

namespace types
{

	ThreadId::~ThreadId() { }

	ThreadId::ThreadId(__threadId _id)
	{
        m_iRows = 1;
        m_iCols = 1;
        m_iSize = 1;
        m_threadId = _id;
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

	wstring ThreadId::toString(int _iPrecision, int _iLineLen)
	{
		wostringstream ostr;

        ostr << L"ThreadId : " << this << std::endl;
        ostr << L"Status : " << StatusToString(this->getStatus());

        return ostr.str();
    }
}

