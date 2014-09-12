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

#ifndef __RESULT_HXX__
#define __RESULT_HXX__

#include "TIType.hxx"

#include <iostream>

namespace analysis
{
class Result
{
    TIType type;
    bool temp;

public:

    Result() : type(), temp(false) { }
    Result(const TIType & _type, const bool _temp) : type(_type), temp(_temp) { }
    Result(TIType && _type, const bool _temp) : type(_type), temp(_temp) { }

    inline bool istemp() const
    {
        return temp;
    }

    inline const TIType & get_type() const
    {
        return type;
    }

    friend std::wostream & operator<<(std::wostream & out, const Result & res)
    {
        out << L"Result {" << res.type << L", temporary: " << (res.temp ? L"true" : L"false") << L"}";
        return out;
    }
};

} // namespace analysis

#endif // __RESULT_HXX__
