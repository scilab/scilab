/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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

protected:

    const std::wstring name;
    std::vector<TIType> args;

public:

    Call(const std::wstring & _name, const std::vector<TIType> & _args) : name(_name), args(_args) { }
    Call(const std::wstring & _name, const TIType & _arg) : name(_name), args(1, _arg) { }
    Call(const std::wstring & _name) : name(_name) { }
    Call(Call && call) : name(call.name), args(call.args) { }

    inline const std::wstring & getName() const
    {
        return name;
    }

    inline void setArgs(const std::vector<TIType> & _args)
    {
        args = _args;
    }

    inline const std::vector<TIType> & getArgs() const
    {
        return args;
    }

    friend std::wostream & operator<<(std::wostream & out, const Call & res)
    {
        out << L"Call " << res.name;
        return out;
    }
};

} // namespace analysis

#endif // __CALL_HXX__
