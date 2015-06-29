/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DECORATOR_HXX__
#define __DECORATOR_HXX__

#include "call/Call.hxx"
#include "Result.hxx"

#include <iostream>

namespace analysis
{

struct Decorator
{
    Result res;
    Call * call;
    bool cloneData;
    bool deleteData;
    bool hasRefCount;
    bool safeIndex;
    bool safeInsertion;
    bool safe;

    Decorator() : res(), call(nullptr), cloneData(false), deleteData(false), hasRefCount(false), safeIndex(false), safeInsertion(false), safe(false) { }

    ~Decorator()
    {
        delete call;
    }

    inline Call * getCall() const
    {
        return call;
    }

    inline Call & setCall(Call * _call)
    {
	delete call;
        call = _call;
        return *call;
    }

    inline Call & setCall(const std::wstring & name)
    {
	delete call;
        call = new Call(name);
        return *call;
    }

    inline Call & setCall(const std::wstring & name, const std::vector<TIType> & args)
    {
	delete call;
        call = new Call(name, args);
        return *call;
    }
    
    inline Call & setCall(const std::wstring & name, const TIType & arg)
    {
	delete call;
        call = new Call(name, arg);
        return *call;
    }

    inline Result & setResult(Result && _res)
    {
        res = _res;
        return res;
    }

    inline const Result & getResult() const
    {
        return res;
    }

    inline Result & getResult()
    {
        return res;
    }

    friend std::wostream & operator<<(std::wostream & out, const Decorator & deco)
    {
        out << deco.res;
        if (deco.call)
        {
            out << L", " << (*deco.call);
        }
        out << L", Cl:" << (deco.cloneData ? L"T" : L"F")
            << L", Del:" << (deco.deleteData ? L"T" : L"F")
            << L", RefC:" << (deco.hasRefCount ? L"T" : L"F")
            << L", SafeIndex:" << (deco.safeIndex ? L"T" : L"F")
            << L", SafeInsertion:" << (deco.safeInsertion ? L"T" : L"F")
	    << L", Safe:" << (deco.safe ? L"T" : L"F");

        return out;
    }
};

} // namespace analysis

#endif // __DECORATOR_HXX__
