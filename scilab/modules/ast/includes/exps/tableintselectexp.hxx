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

#ifndef __TABLE_INT_SELECT_EXP_HXX__
#define __TABLE_INT_SELECT_EXP_HXX__

#include <vector>

#include "internal.hxx"

namespace ast
{
class TableIntSelectExp : public IntSelectExp
{

public:

    TableIntSelectExp(const Location& location,
                      Exp& select,
                      exps_t& cases,
                      Exp& defaultCase,
                      const std::vector<int64_t> & _keys, const int64_t _min, const int64_t _max) : IntSelectExp(location, select, cases, defaultCase), min(_min), max(_max), table(_max - _min + 1, &defaultCase), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            table[key - min] = *i;
            ++i;
        }
    }

    TableIntSelectExp(const Location& location,
                      Exp& select,
                      exps_t& cases,
                      const std::vector<int64_t> & _keys, const int64_t _min, const int64_t _max) : IntSelectExp(location, select, cases), min(_min), max(_max), table(_max - _min + 1, nullptr), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            table[key - min] = *i;
            ++i;
        }
    }

    virtual ~TableIntSelectExp()
    {
    }

    virtual TableIntSelectExp * clone()
    {
        exps_t * cases = cloneCases();
        TableIntSelectExp * cloned = nullptr;
        if (_hasDefault)
        {
            cloned = new TableIntSelectExp(getLocation(), *getSelect()->clone(), *cases, *getDefaultCase()->clone(), keys, min, max);
        }
        else
        {
            cloned = new TableIntSelectExp(getLocation(), *getSelect()->clone(), *cases, keys, min, max);
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

    inline Exp * getExp(const int64_t key) const
    {
        if (key >= min && key <= max)
        {
            return table[key - min];
        }

        return getDefaultCase();
    }

    inline bool isTableIntSelectExp() const
    {
        return true;
    }

    virtual ExpType getType() const
    {
        return TABLEINTSELECTEXP;
    }

    inline int64_t getMin() const
    {
        return min;
    }

    inline int64_t getMax() const
    {
        return max;
    }

    inline const std::vector<Exp *> & getTable() const
    {
        return table;
    }

private:

    int64_t min, max;
    std::vector<Exp *> table;
    std::vector<int64_t> keys;

};

} // namespace ast

#endif // __TABLE_INT_SELECT_EXP_HXX__
