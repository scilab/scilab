/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __MACROINFO_HXX__
#define __MACROINFO_HXX__

#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

#include "FunctionBlock.hxx"

namespace analysis
{

struct TITypeTuple
{
    std::vector<TIType> types;
    TITypeTuple(const std::vector<Info> & _types) : types(makeTuple(_types)) { }

    inline static std::vector<TIType> makeTuple(const std::vector<Info> & tuple)
    {
        std::vector<TIType> v;
        v.reserve(tuple.size());
        for (const auto & i : tuple)
        {
            v.emplace_back(i.type);
        }
        return v;
    }

    struct MyHash
    {
        inline std::size_t operator()(const TITypeTuple & tuple) const
        {
            if (tuple.types.empty())
            {
                return 0;
            }
            else
            {
                std::size_t seed = std::hash<TIType>()(tuple.types.front());
                for (std::vector<TIType>::const_iterator i = ++tuple.types.begin(), e = tuple.types.end(); i != e; ++i)
                {
                    seed = combine(seed, *i);
                }
                return seed;
            }
        }

        inline static std::size_t combine(const std::size_t seed, const TIType & type)
        {
            // it is the way Boost has implemented hash_combine:
            // http://www.boost.org/doc/libs/1_35_0/doc/html/boost/hash_combine_id241013.html
            return seed ^ (std::hash<TIType>()(type) + 0x9e3779b9 + (seed << 6) + (seed >> 2));
        }
    };
};

struct MacroInfo
{
    std::map < symbol::Symbol, Info > ;
    std::vector<symbol::Symbol> globals;

    inline bool isClosure() const
    {
        return !globals.empty();
    }
};

class PolymorphicMacroCache
{
    typedef std::unordered_map<TITypeTuple, MacroInfo, TITypeTuple::MyHash> FunBlockMap;
    FunctionBlock * fblock;
    FunBlockMap map;

public:

    PolymorphicMacroCache(FunctionBlock * _fblock) : fblock(_fblock) { }

    const MacroInfo & getOutTypes(const std::vector<Info> & info)
    {
        TITypeTuple tuple(info);
        FunBlockMap::const_iterator i = map.find(tuple);
        if (i != map.end())
        {
            return i->second;
        }
        else
        {

        }
    }
};

} // namespace analysis

#endif // __MACROINFO_HXX__
