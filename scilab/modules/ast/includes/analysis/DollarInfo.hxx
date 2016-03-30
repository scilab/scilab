/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Calixte DENIZET
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

    friend std::wostream & operator<<(std::wostream & out, const DollarInfo & di);

};

} // namespace analysis

#endif // __DOLLAR_INFO_HXX__
