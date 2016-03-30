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

#ifndef __MAP_INT_SELECT_EXP_HXX__
#define __MAP_INT_SELECT_EXP_HXX__

#include <unordered_map>
#include <vector>

#include "internal.hxx"

namespace ast
{
class MapIntSelectExp : public IntSelectExp
{

public:

    MapIntSelectExp(const Location& location,
                    Exp& select,
                    exps_t& cases,
                    Exp& defaultCase,
                    const std::vector<int64_t> & _keys) : IntSelectExp(location, select, cases, defaultCase), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            map.emplace(key, *i);
            ++i;
        }
    }

    MapIntSelectExp(const Location& location,
                    Exp& select,
                    exps_t& cases,
                    const std::vector<int64_t> & _keys) : IntSelectExp(location, select, cases), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            map.emplace(key, *i);
            ++i;
        }
    }

    virtual ~MapIntSelectExp()
    {
    }

    virtual MapIntSelectExp * clone()
    {
        exps_t * cases = cloneCases();
        MapIntSelectExp * cloned = nullptr;
        if (_hasDefault)
        {
            cloned = new MapIntSelectExp(getLocation(), *getSelect()->clone(), *cases, *getDefaultCase()->clone(), keys);
        }
        else
        {
            cloned = new MapIntSelectExp(getLocation(), *getSelect()->clone(), *cases, keys);
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

    inline Exp * getExp(const int64_t key) const
    {
        auto i = map.find(key);
        if (i == map.end())
        {
            return getDefaultCase();
        }
        else
        {
            return i->second;
        }
    }

    inline bool isMapIntSelectExp() const
    {
        return true;
    }

    virtual ExpType getType() const
    {
        return MAPINTSELECTEXP;
    }

    inline const std::unordered_map<int64_t, Exp *> & getTable() const
    {
        return map;
    }

private:

    std::unordered_map<int64_t, Exp *> map;
    std::vector<int64_t> keys;

};

} // namespace ast

#endif // __MAP_INT_SELECT_EXP_HXX__
