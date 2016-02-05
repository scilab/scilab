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
                      const std::vector<int64_t> & _keys, const int64_t min, const int64_t max) : IntSelectExp(location, select, cases, defaultCase), _min(min), _max(max), table(_max - _min + 1, &defaultCase), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            table[key - _min] = *i;
            ++i;
        }
    }

    TableIntSelectExp(const Location& location,
                      Exp& select,
                      exps_t& cases,
                      const std::vector<int64_t> & _keys, const int64_t min, const int64_t max) : IntSelectExp(location, select, cases), _min(min), _max(max), table(_max - _min + 1, nullptr), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            table[key - _min] = *i;
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
            cloned = new TableIntSelectExp(getLocation(), *getSelect()->clone(), *cases, *getDefaultCase()->clone(), keys, _min, _max);
        }
        else
        {
            cloned = new TableIntSelectExp(getLocation(), *getSelect()->clone(), *cases, keys, _min, _max);
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

    inline Exp * getExp(const int64_t key) const
    {
        if (key >= _min && key <= _max)
        {
            return table[key - _min];
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
        return _min;
    }

    inline int64_t getMax() const
    {
        return _max;
    }

    inline const std::vector<Exp *> & getTable() const
    {
        return table;
    }

private:

    int64_t _min, _max;
    std::vector<Exp *> table;
    std::vector<int64_t> keys;

};

} // namespace ast

#endif // __TABLE_INT_SELECT_EXP_HXX__
