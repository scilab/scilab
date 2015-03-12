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

#ifndef __CALL_HXX__
#define __CALL_HXX__

#include <iostream>
#include <string>
#include <vector>

#include "TIType.hxx"

namespace analysis
{
class Call
{

public:

    enum Kind {UNDEFINED, IDENTITY, HYPOT, DOTHYPOT, __COUNT__};

private:

    Kind kind;
    TIType type;
    std::wstring name;
    std::vector<TIType> args;

public:

    Call(Kind _kind, const TIType & _type, const std::wstring & _name, const std::vector<TIType> & _args) : kind(_kind),
        type(_type),
        name(_name),
        args(_args) { }

    Call(const TIType & _type, const std::wstring & _name, const std::vector<TIType> & _args) : Call(UNDEFINED, _type, _name, _args) { }
    Call(Kind _kind, const TIType & _type, const std::wstring & _name) : kind(_kind),
        type(_type),
        name(_name) { }
    Call(const TIType & _type, const std::wstring & _name) : Call(UNDEFINED, _type, _name) { }

    Call(const std::wstring & _name) : kind(UNDEFINED), type(), name(_name) { }
    Call(Call && call) : kind(call.kind), type(call.type), name(call.name), args(call.args) { }

    inline void setName(const std::wstring & _name)
    {
        name = _name;
    }

    inline const std::wstring & getName() const
    {
        return name;
    }

    inline const TIType & getType() const
    {
        return type;
    }

    inline void setKind(Kind _kind)
    {
        kind = _kind;
    }

    inline Kind getKind() const
    {
        return kind;
    }

    inline void setArgs(const std::vector<TIType> & _args)
    {
        args = _args;
    }

    inline std::vector<TIType> & getArgs()
    {
        return args;
    }

    friend std::wostream & operator<<(std::wostream & out, const Call & res)
    {
        out << L"Call " << res.name << L"{" << res.type << L"}, Identity:" << (res.kind == IDENTITY ? L"T" : L"F");
        return out;
    }
};

} // namespace analysis

#endif // __CALL_HXX__
