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

#ifndef __SMALL_MAP_INT_SELECT_EXP_HXX__
#define __SMALL_MAP_INT_SELECT_EXP_HXX__

#include <vector>

#include "internal.hxx"

namespace ast
{
class SmallIntSelectExp : public IntSelectExp
{

public:

    SmallIntSelectExp(const Location& location,
                      Exp& select,
                      exps_t& cases,
                      Exp& defaultCase,
                      const std::vector<int64_t> & keys) : IntSelectExp(location, select, cases, defaultCase), table(keys)
    {
    }

    SmallIntSelectExp(const Location& location,
                      Exp& select,
                      exps_t& cases,
                      const std::vector<int64_t> & keys) : IntSelectExp(location, select, cases), table(keys)
    {
    }

    virtual ~SmallIntSelectExp()
    {
    }

    virtual SmallIntSelectExp * clone()
    {
        exps_t * cases = cloneCases();
        SmallIntSelectExp * cloned = nullptr;
        if (_hasDefault)
        {
            cloned = new SmallIntSelectExp(getLocation(), *getSelect()->clone(), *cases, *getDefaultCase()->clone(), table);
        }
        else
        {
            cloned = new SmallIntSelectExp(getLocation(), *getSelect()->clone(), *cases, table);
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

    inline Exp * getExp(const int64_t key) const
    {
        const int size = static_cast<int>(table.size());
        for (int i = 0; i < size; ++i)
        {
            if (table[i] == key)
            {
                return _exps[i];
            }
        }
        return getDefaultCase();
    }

    inline bool isSmallIntSelectExp() const
    {
        return true;
    }

    virtual ExpType getType() const
    {
        return SMALLINTSELECTEXP;
    }

private:

    std::vector<int64_t> table;

};

} // namespace ast

#endif // __MAP_INT_SELECT_EXP_HXX__
