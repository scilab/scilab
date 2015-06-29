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
