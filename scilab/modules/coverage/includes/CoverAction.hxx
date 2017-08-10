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

#ifndef __COVER_ACTION_HXX__
#define __COVER_ACTION_HXX__

#include <functional>
#include <unordered_set>

#include "macro.hxx"
#include "tools.hxx"

namespace coverage
{

class CoverAction
{
    struct __Caller
    {
        const types::Macro * caller;
        const Location & loc;
        mutable uint64_t count;

        __Caller(const types::Macro * _caller, const Location & _loc) : caller(_caller), loc(_loc), count(0) { }

        inline void call() const
        {
            ++count;
        }

        struct Hash
        {
            inline std::size_t operator()(const __Caller & c) const
            {
                return analysis::tools::hash_combine(std::hash<const types::Macro *>()(c.caller), c.loc.first_line, c.loc.first_column, c.loc.last_line, c.loc.last_column);
            }
        };

        struct Eq
        {
            inline bool operator()(const __Caller & L, const __Caller & R) const
            {
                return L.caller == R.caller && L.loc == R.loc;
            }
        };
    };

    typedef std::unordered_set<__Caller, __Caller::Hash, __Caller::Eq> SiteSet;
    SiteSet sites;

public:

    CoverAction() { }

    inline void call(const types::Macro * caller, const Location & loc)
    {
        __Caller c(caller, loc);
        SiteSet::iterator i = sites.find(c);
        if (i == sites.end())
        {
            i = sites.insert(c).first;
        }
        i->call();
    }

};

} // namespace coverage

#endif // __COVER_ACTION_HXX__
