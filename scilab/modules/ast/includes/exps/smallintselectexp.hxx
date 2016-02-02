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

    inline const std::vector<int64_t> & getTable() const
    {
        return table;
    }

private:

    std::vector<int64_t> table;

};

} // namespace ast

#endif // __MAP_INT_SELECT_EXP_HXX__
