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

#include "Result.hxx"
#include "OptionalDecoration.hxx"

#include <iostream>

namespace analysis
{

struct Decorator
{
    Result res;
    OptionalDecoration opt;
    bool cloneData;
    bool deleteData;
    bool hasRefCount;
    bool safe;

    Decorator() : res(), opt(), cloneData(false), deleteData(false), hasRefCount(false), safe(false) { }

    ~Decorator()
    {
    }

    inline Call * getCall() const
    {
        return opt.get<Call>();
    }

    inline Call & setCall(Call * _call)
    {
        opt.set(_call);
        return *_call;
    }

    inline Call & setCall(const std::wstring & name)
    {
        Call * call = new Call(name);
        opt.set(call);
        return *call;
    }

    inline Call & setCall(const std::wstring & name, const std::vector<TIType> & args)
    {
        Call * call = new Call(name, args);
        opt.set(call);
        return *call;
    }

    inline Call & setCall(const std::wstring & name, const TIType & arg)
    {
        Call * call = new Call(name, arg);
        opt.set(call);
        return *call;
    }

    inline Clone * getClone() const
    {
        return opt.get<Clone>();
    }

    inline Clone & setClone(Clone * _clone)
    {
        opt.set(_clone);
        return *_clone;
    }

    inline Clone & addClone(const symbol::Symbol & sym)
    {
        Clone * clone = opt.get<Clone>();
        if (clone)
        {
            clone->add(sym);
        }
        else
        {
            clone = new Clone(sym);
            opt.set(clone);
        }
        return *clone;
    }

    inline Result & setResult(Result && _res)
    {
        res = _res;
        return res;
    }

    inline Result & setResult(Result & _res)
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
        if (!deco.opt.empty())
        {
            out << L", " << deco.opt;
        }
        out << L", Cl:" << (deco.cloneData ? L"T" : L"F")
            << L", Del:" << (deco.deleteData ? L"T" : L"F")
            << L", RefC:" << (deco.hasRefCount ? L"T" : L"F")
            << L", Safe:" << (deco.safe ? L"T" : L"F");

        return out;
    }
};

} // namespace analysis

#endif // __DECORATOR_HXX__
