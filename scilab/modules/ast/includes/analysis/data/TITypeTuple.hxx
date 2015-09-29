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

#ifndef __TITYPE_TUPLE_HXX__
#define __TITYPE_TUPLE_HXX__

#include <vector>

#include "tools.hxx"
#include "TIType.hxx"

namespace analysis
{
    struct TITypeTuple
    {
        std::vector<TIType> types;

        TITypeTuple() { }

        TITypeTuple(const std::vector<TIType> & _types) : types(_types)
            {
                /*types.reserve(_types.size());
                  for (const auto & type : _types)
                  {
                  types.emplace_back(type.type, type.isscalar());
                  }*/
            }

        inline std::size_t size() const
            {
                return types.size();
            }

        inline std::size_t hash() const
            {
                if (types.empty())
                {
                    return 0;
                }
                else
                {
                    std::size_t seed = types.front().hashPureType();
                    for (std::vector<TIType>::const_iterator i = ++types.begin(), e = types.end(); i != e; ++i)
                    {
                        seed = tools::hash_combine(seed, i->hashPureType());
                    }
                    return seed;
                }
            }

        inline bool operator==(const TITypeTuple & R) const
            {
                if (types.size() == R.types.size())
                {
                    for (std::vector<TIType>::const_iterator i = types.begin(), j = R.types.begin(), e = types.end(); i != e; ++i, ++j)
                    {
                        if (i->type != j->type || i->isscalar() != j->isscalar())
                        {
                            return false;
                        }
                    }
                    return true;
                }

                return false;
            }

        friend std::wostream & operator<<(std::wostream & out, const TITypeTuple & tuple)
            {
                if (tuple.types.empty())
                {
                    out << L"[]";
                }
                else
                {
                    out << L"[";
                    for (std::vector<TIType>::const_iterator i = tuple.types.begin(), e = std::prev(tuple.types.end()); i != e; ++i)
                    {
                        out << *i << L", ";
                    }
                    out << tuple.types.back() << L"]";
                }
		return out;
            }

        struct Hash
        {
            inline std::size_t operator()(const TITypeTuple & tuple) const
                {
                    return tuple.hash();
                }
        };

        struct Eq
        {
            inline bool operator()(const TITypeTuple & L, const TITypeTuple & R) const
                {
                    return L == R;
                }
        };
    };

} // namespace analysis

#endif // __TITYPE_TUPLE_HXX__
