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
