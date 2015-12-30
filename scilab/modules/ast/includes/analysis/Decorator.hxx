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
    bool deleteData;
    bool safe;

    Decorator() : res(), opt(), deleteData(false), safe(false) { }

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

    inline Call & setCall(const std::string & name)
    {
        Call * call = new Call(name);
        opt.set(call);
        return *call;
    }

    inline Call & setCall(const std::string & name, const std::vector<TIType> & args)
    {
        Call * call = new Call(name, args);
        opt.set(call);
        return *call;
    }

    inline Call & setCall(const std::string & name, const TIType & arg)
    {
        Call * call = new Call(name, arg);
        opt.set(call);
        return *call;
    }

    inline LoopDecoration * getLoopDecoration() const
    {
        return opt.get<LoopDecoration>();
    }

    inline LoopDecoration & setLoopDecoration(LoopDecoration * _ld)
    {
        opt.set(_ld);
        return *_ld;
    }

    inline void addClone(const symbol::Symbol & sym)
    {
        LoopDecoration * ld = opt.get<LoopDecoration>();
        if (ld)
        {
            ld->addClone(sym);
        }
        else
        {
            ld = new LoopDecoration();
            ld->addClone(sym);
            opt.set(ld);
        }
    }

    inline void addPromotion(const symbol::Symbol & sym, const TIType & first, const TIType & second)
    {
        LoopDecoration * ld = opt.get<LoopDecoration>();
        if (ld)
        {
            ld->addPromotion(sym, first, second);
        }
        else
        {
            ld = new LoopDecoration();
            ld->addPromotion(sym, first, second);
            opt.set(ld);
        }
    }

    inline DollarInfo & setDollarInfo(const DollarInfo & di)
    {
        DollarInfo * _di = new DollarInfo(di);
        opt.set(_di);
        return *_di;
    }

    inline DollarInfo * getDollarInfo() const
    {
        return opt.get<DollarInfo>();
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

    friend std::ostream & operator<<(std::ostream & out, const Decorator & deco)
    {
        out << deco.res;
        if (!deco.opt.empty())
        {
            out << ", " << deco.opt;
        }
        if (deco.deleteData)
        {
            out << ", Del: T";
        }
        if (deco.safe)
        {
            out << ", Safe: T";
        }

        return out;
    }
};

} // namespace analysis

#endif // __DECORATOR_HXX__
