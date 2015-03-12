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

#ifndef __INFO_HXX__
#define __INFO_HXX__

#include <iostream>

#include "TIType.hxx"
#include "Data.hxx"
#include "gvn/GVN.hxx"
#include "ConstantValue.hxx"

namespace analysis
{

struct Info
{
    enum Local
    {
        INFO_TRUE, INFO_FALSE, INFO_UNKNOWN
    };

    bool R;
    bool W;
    bool O;
    bool isint;
    Local local;
    bool cleared;
    bool exists;
    TIType type;
    Data * data;
    ast::Exp * exp;
    ConstantValue constant;

    Info(Data * _data = nullptr) : R(false), W(false), O(false), isint(false), local(Local::INFO_TRUE), cleared(false), exists(true), data(_data), exp(nullptr) { }
    Info(const Info & i) : R(i.R), W(i.W), O(i.O), isint(i.isint), local(i.local), cleared(i.cleared), exists(i.exists), constant(i.constant), type(i.type), data(i.data ? new Data(*i.data) : nullptr), exp(i.exp) { }

    inline void merge(Info & info)
    {
        R = R || info.R;
        W = W || info.W;
        O = O || info.O;
        isint = isint && info.isint;
        if (local != info.local)
        {
            local = Local::INFO_UNKNOWN;
        }
        cleared = cleared && info.cleared;
        exists = exists || info.exists;
        constant.merge(info.constant);
        //knownValue = knownValue && info.knownValue && value == info.value;
        type.merge(info.type);
        data->valid = data->same(info.data);
    }

    inline void addData(const bool known, const symbol::Symbol & sym)
    {
        data = new Data(known, sym);
    }
    inline void addData(Data * _data, const symbol::Symbol & sym)
    {
        data = _data;
        data->add(sym);
    }

    inline ConstantValue & getConstant()
    {
        return constant;
    }

    inline const ConstantValue & getConstant() const
    {
        return constant;
    }

    inline ConstantValue & setConstant(ConstantValue & val)
    {
        constant = val;
        return constant;
    }

    inline bool isknown() const
    {
        return local == Local::INFO_TRUE;
    }

    inline static const symbol::Symbol & getRightSym(ast::Exp * exp)
    {
        return static_cast<const ast::SimpleVar &>(static_cast<const ast::AssignExp *>(exp)->getRightExp()).getSymbol();
    }

    inline const TIType & getType() const
    {
        return type;
    }

    friend std::wostream & operator<<(std::wostream & out, const Info & info)
    {
        out << L"Type: " << info.type << L" - RWO:"
            << (info.R ? L"T" : L"F")
            << (info.W ? L"T" : L"F")
            << (info.O ? L"T" : L"F")
            << L" - int:" << (info.isint ? L"T" : L"F")
            << L" - local:" << (info.local == Local::INFO_TRUE ? L"T" : (info.local == Local::INFO_FALSE ? L"F" : L"U"))
            << L" - cleared:" << (info.cleared ? L"T" : L"F")
            << L" - exists:" << (info.exists ? L"T" : L"F")
            << L" - constant:" << info.constant;

        out << L" - data:";
        if (info.data)
        {
            out << *info.data;
        }
        else
        {
            out << L"null";
        }

        return out;
    }
};

} // namespace analysis

#endif // __INFO_HXX__
