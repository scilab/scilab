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

#ifndef __TEMPORARY_HXX__
#define __TEMPORARY_HXX__

#include <iostream>
#include <set>
#include <stack>
#include <unordered_map>
#include <vector>

#include "TIType.hxx"
#include "Result.hxx"

namespace analysis
{

/* Class to count the number of required temporary variables
   We have two kinds of temporary: sized and non-sized (sized temp are just temp vars which we know the dims).
   The strategy to choose a sized temp is the following:
   i) we look for an existing temp (but not used) with enough storage
   ii) if such a one doesn't exist we take the last one and we increase its size or we create a new one.
   The idea is to allocate enough memory for all the temps before to execution.
 */
class Temporary
{
    struct __Temporary
    {
        struct __Sized_Temp
        {
            const int id;
            const unsigned int size;

            __Sized_Temp(const int _id, const unsigned int _size) : id(_id), size(_size) { }
            __Sized_Temp(const unsigned int _size) : id(0), size(_size) { }

            struct __Compare
            {
                inline std::size_t operator()(const __Sized_Temp & l, const __Sized_Temp & r) const
                {
                    return l.size < r.size;
                }
            };
        };

        typedef std::set<__Sized_Temp, __Sized_Temp::__Compare> SizedSet;
        SizedSet free_sized_ids;
        std::unordered_map<int, __Sized_Temp> used_sized_ids;
        std::stack<int> free_nonsized_ids;
        int max;
        int nonsized_max;

        __Temporary() : max(0), nonsized_max(-2) { }

        inline void remove(int id)
        {
            std::unordered_map<int, __Sized_Temp>::const_iterator i = used_sized_ids.find(id);
            if (i != used_sized_ids.end())
            {
                free_sized_ids.insert(i->second);
                used_sized_ids.erase(i);
            }
            else if (id <= -2)
            {
                free_nonsized_ids.push(id);
            }
        }

        inline int getId()
        {
            int id;
            if (free_nonsized_ids.empty())
            {
                id = max++;//nonsized_max--;
            }
            else
            {
                id = free_nonsized_ids.top();
                free_nonsized_ids.pop();
            }

            return id;
        }

        inline int getId(int size)
        {
            SizedSet::iterator i = free_sized_ids.lower_bound(__Sized_Temp(size));
            if (i != free_sized_ids.end())
            {
                const int id = i->id;
                used_sized_ids.emplace(std::pair<int, __Sized_Temp>(id, *i));
                free_sized_ids.erase(i);

                return id;
            }
            else
            {
                if (free_sized_ids.empty())
                {
                    const int id = max++;
                    used_sized_ids.emplace(std::pair<int, __Sized_Temp>(id, __Sized_Temp(id, size)));
                    return id;
                }
                else
                {
                    SizedSet::const_iterator last = --free_sized_ids.begin();
                    const int id = last->id;
                    used_sized_ids.emplace(std::pair<int, __Sized_Temp>(id, __Sized_Temp(id, size)));
                    free_sized_ids.erase(last);

                    return id;
                }
            }
        }

        inline std::vector<int64_t> getSizes(int64_t & totalSize) const
        {
            std::vector<int64_t> v(max, -1);
            unsigned int total = 0;
            for (const auto & st : free_sized_ids)
            {
                total += st.size;
                v[st.id] = st.size;
            }
            for (const auto & kv : used_sized_ids)
            {
                total += kv.second.size;
                v[kv.second.id] = kv.second.size;
            }

            totalSize = total;
            return v;
        }

        inline unsigned int totalSize() const
        {
            unsigned int total = 0;
            for (const auto & st : free_sized_ids)
            {
                total += st.size;
            }
            for (const auto & kv : used_sized_ids)
            {
                total += kv.second.size;
            }

            return total;
        }
    };

    __Temporary scalars[TIType::COUNT];
    __Temporary matrices[TIType::COUNT];

public:

    Temporary() { }

    inline int add(const TIType & t, const bool scalar = false)
    {
        if (t.isKnownDims() && t.isConstantDims())
        {
            return scalar ? scalars[t.type].getId(1) : matrices[t.type].getId((int) (t.rows.getConstant() * t.cols.getConstant()));
        }
        else
        {
            return scalar ? scalars[t.type].getId() : matrices[t.type].getId();
        }
    }

    inline void remove(const TIType & t, const int id, const bool scalar = false)
    {
        if (id >= 0)
        {
            if (scalar)
            {
                scalars[t.type].remove(id);
            }
            else
            {
                matrices[t.type].remove(id);
            }
        }
    }

    inline void remove(const Result & r)
    {
        remove(r.getType(), r.getTempId());
    }

    inline int get_scalar(const TIType::Type t) const
    {
        return scalars[t].max;
    }

    inline int get_matrix(const TIType::Type t) const
    {
        return matrices[t].max;
    }

    inline unsigned int totalSize(const TIType::Type t) const
    {
        return matrices[t].totalSize();
    }

    inline std::vector<int64_t> getSizes(const TIType::Type t, int64_t & totalSize) const
    {
        return matrices[t].getSizes(totalSize);
    }

    friend std::wostream & operator<<(std::wostream & out, const Temporary & temp)
    {
        bool hasTmpScal = false;
        bool hasTmpMat = false;
        for (unsigned int i = 0; i < TIType::COUNT; ++i)
        {
            if (temp.get_scalar(static_cast<TIType::Type>(i)))
            {
                hasTmpScal = true;
                break;
            }
        }

        for (unsigned int i = 0; i < TIType::COUNT; ++i)
        {
            if (temp.get_matrix(static_cast<TIType::Type>(i)))
            {
                hasTmpMat = true;
                break;
            }
        }

        if (hasTmpScal)
        {
            out << L"Temporary scalars:" << std::endl;
            for (unsigned int i = 0; i < TIType::COUNT; ++i)
            {
                const TIType::Type t = static_cast<TIType::Type>(i);
                if (temp.get_scalar(t))
                {
                    out << TIType(t) << ": " << temp.get_scalar(t) << std::endl;
                }
            }
        }

        if (hasTmpMat)
        {
            out << std::endl
                << L"Temporary arrays:" << std::endl;

            for (unsigned int i = 0; i < TIType::COUNT; ++i)
            {
                const TIType::Type t = static_cast<TIType::Type>(i);
                if (temp.get_matrix(t))
                {
                    out << TIType(t) << ": " << temp.get_matrix(t) << " (total size: " << temp.totalSize(t) << ")" << std::endl;
                }
            }
        }

        if (hasTmpScal || hasTmpMat)
        {
            out << std::endl;
        }
        return out;
    }
};

} // namespace analysis

#endif // __TEMPORARY_HXX__
