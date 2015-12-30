/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __DOLLAR_INFO_HXX__
#define __DOLLAR_INFO_HXX__

#include <iostream>
#include <cstdint>

namespace ast
{
class SimpleVar;
}

namespace analysis
{

class DollarInfo
{

    const ast::SimpleVar & var;
    const uint32_t argsCount;
    uint32_t index;

public:

    DollarInfo(const ast::SimpleVar & _var, const uint32_t _argsCount, const uint32_t _index) : var(_var), argsCount(_argsCount), index(_index) { }

    inline const ast::SimpleVar & getVar() const
    {
        return var;
    }

    inline uint32_t getArgsCount() const
    {
        return argsCount;
    }

    inline uint32_t getIndex() const
    {
        return index;
    }

    inline uint32_t & getIndex()
    {
        return index;
    }

    friend std::ostream & operator<<(std::ostream & out, const DollarInfo & di);

};

} // namespace analysis

#endif // __DOLLAR_INFO_HXX__
