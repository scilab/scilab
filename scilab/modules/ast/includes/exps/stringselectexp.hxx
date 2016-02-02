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

#ifndef __STRING_SELECT_EXP_HXX__
#define __STRING_SELECT_EXP_HXX__

#include <string>
#include <unordered_map>
#include <vector>

#include "internal.hxx"

namespace ast
{
class StringSelectExp : public FastSelectExp
{

public:

    StringSelectExp(const Location& location,
                    Exp& select,
                    exps_t& cases,
                    Exp& defaultCase,
                    const std::vector<std::wstring> & _keys) : FastSelectExp(location, select, cases, defaultCase), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            map.emplace(key, *i);
            ++i;
        }
    }

    StringSelectExp(const Location& location,
                    Exp& select,
                    exps_t& cases,
                    const std::vector<std::wstring> & _keys) : FastSelectExp(location, select, cases), keys(_keys)
    {
        exps_t::iterator i = std::next(_exps.begin());
        for (const auto & key : keys)
        {
            map.emplace(key, *i);
            ++i;
        }
    }

    virtual StringSelectExp * clone()
    {
        exps_t * cases = cloneCases();
        StringSelectExp * cloned = nullptr;
        if (_hasDefault)
        {
            cloned = new StringSelectExp(getLocation(), *getSelect()->clone(), *cases, *getDefaultCase()->clone(), keys);
        }
        else
        {
            cloned = new StringSelectExp(getLocation(), *getSelect()->clone(), *cases, keys);
        }

        cloned->setVerbose(isVerbose());
        return cloned;
    }

    virtual ~StringSelectExp()
    {
    }

    inline Exp * getExp(const std::wstring & key) const
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

    inline bool isStringSelectExp() const
    {
        return true;
    }

    virtual ExpType getType() const
    {
        return STRINGSELECTEXP;
    }

    virtual void accept(Visitor& v)
    {
        v.visit(*this);
    }

    virtual void accept(ConstVisitor& v) const
    {
        v.visit(*this);
    }

private:

    std::unordered_map<std::wstring, ast::Exp *> map;
    std::vector<std::wstring> keys;
};

} // namespace ast

#endif // __STRING_SELECT_EXP_HXX__
