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

#ifndef __GVN_HXX__
#define __GVN_HXX__

#include <string>
#include <list>
#include <map>
#include <unordered_map>

#include "tools.hxx"
#include "MultivariatePolynomial.hxx"
#include "OpValue.hxx"
#include "symbol.hxx"

namespace analysis
{

/**
 * \class GVN
 * \brief GVN stands for Global Value Numbering
 *
 * This class implements a GVN but it is extended to track common expressions represented
 * by multivariate polynomials.
 * Symbolic calculus is allowed with integer values (such as matrix dimensions)
 *
 * All the pointers GVN::Value* returned by the GVN have the same lifetime than the GVN itself.
 * For information, the GVN::Value are stored in unordered_map (or multimap) or list.
 *
 */
class GVN
{

public:

    /**
     * \struct Value
     * \brief Represents a value and polynomial
     */
    struct Value
    {
        uint64_t value;
        const MultivariatePolynomial * poly;

        Value(const uint64_t _value) : value(_value), poly(nullptr) { }

        friend inline std::wostream & operator<<(std::wostream & out, const Value & v)
        {
            out << L"Value: " << v.value
                << L", Poly: ";
            if (v.poly)
            {
                out << *v.poly;
            }
            else
            {
                out << L"null";
            }
            return out;
        }

        inline bool operator==(const Value & R) const
        {
            return value == R.value;
        }

        inline bool operator!=(const Value & R) const
        {
            return value != R.value;
        }
    };

private:

    typedef std::unordered_map<OpValue, Value, OpValue::Hash, OpValue::Eq> MapValues;
    typedef std::unordered_map<int64_t, Value> MapInt64;
    typedef std::multimap<symbol::Symbol, Value> MapSymbols;
    typedef std::unordered_map<MultivariatePolynomial, Value *, MultivariatePolynomial::Hash, MultivariatePolynomial::Eq> MapPolys;
    typedef std::list<Value> ListValues;

    MapValues mapv;
    MapInt64 mapi64;
    MapSymbols maps;
    MapPolys mapp;
    ListValues list;

    uint64_t current;

public:

    // WARNING: current MUST be initialized with 0 and nothing else !
    // (because when mpoly are evaluated with a std::vector<MP> the index begins to 0)
    /**
     * \brief constructor
     */
    GVN() : current(0)
    {
    }

    /**
     * \brief Clear all the maps used in the GVN
     */
    inline void clear()
    {
        mapv.clear();
        mapi64.clear();
        maps.clear();
        mapp.clear();
        list.clear();
        current = 0;
    }

    inline uint64_t getCurrentValue() const
	{
	    return current;
	}

     /**
     * \brief Inserts a value associated with a polynomial
     * \param mp a polynomial
     * \param value a value
     */
    inline void insertValue(const MultivariatePolynomial & mp, Value & value)
    {
        MapPolys::iterator i = mapp.find(mp);
        if (i == mapp.end())
        {
            value.poly = &mapp.emplace(mp, &value).first->first;
        }
        else
        {
            value.value = i->second->value;
            value.poly = &i->first;
        }
    }

    /**
     * \brief Associated a symbol with a polynomial
     * \param sym a symbol
     * \param mp a polynomial
     */
    inline void setValue(const symbol::Symbol & sym, const MultivariatePolynomial & mp)
    {
        MapPolys::iterator i = mapp.find(mp);
        if (i != mapp.end())
        {
            maps.emplace(sym, i->second->value)->second.poly = i->second->poly;
        }
        else
        {
            Value & value = maps.emplace(sym, current++)->second;
            insertValue(mp, value);
            if (mp.isConstant() && mapi64.find(mp.constant) == mapi64.end())
            {
                mapi64.emplace(mp.constant, value);
            }
        }
    }

    /**
     * \brief Associated a symbol with a Value
     * \param sym a symbol
     * \param LV a value
     */
    inline void setValue(const symbol::Symbol & sym, const Value & LV)
    {
        maps.emplace(sym, LV.value)->second.poly = LV.poly;
    }

    /**
     * \brief Get a value
     * \return a Value
     */
    inline Value * getValue()
    {
        list.emplace_back(current);
        Value & value = list.back();
        insertValue(current++, value);

        return &value;
    }

    /**
     * \brief Get a value associated with a polynomial
     * \param mp a polynomial
     * \return a Value
     */
    inline Value * getValue(const MultivariatePolynomial & mp)
    {
        const auto i = mapp.find(mp);
        if (i != mapp.end())
        {
            return i->second;
        }
        else
        {
            Value * val = getValue();
            insertValue(mp, *val);

            return val;
        }
    }

    /**
     * \brief Get a value associated with a symbol
     * \param sym a symbol
     * \return a Value
     */
    inline Value * getValue(const symbol::Symbol & sym)
    {
        const auto i = maps.equal_range(sym);
        if (i.first == i.second)
        {
            Value & value = maps.emplace(sym, current)->second;
            insertValue(current++, value);

            return &value;
        }
        else
        {
            return &std::prev(i.second)->second;
        }
    }

    /**
     * \brief Get a value associated with a symbol
     * \param sym a symbol
     * \return a Value
     */
    inline Value * getExistingValue(const symbol::Symbol & sym)
    {
        const auto i = maps.equal_range(sym);
        if (i.first == i.second)
        {
            return nullptr;
        }
        else
        {
            return &std::prev(i.second)->second;
        }
    }

    /**
     * \brief Get a value associated with a double
     * \param x a double
     * \return a Value
     */
    inline Value * getValue(const double x)
    {
	int64_t _x;
	if (tools::asInteger(x, _x))
	{
	    const auto i = mapi64.find(_x);
	    if (i == mapi64.end())
	    {
		Value & value = mapi64.emplace(_x, current++).first->second;
		insertValue(_x, value);
		
		return &value;
	    }
	    else
	    {
		return &i->second;
	    }
	}

	return nullptr;
    }

    /**
     * \brief Get a value associated with an unary operation applyed to a value
     * \param kind the kind of the operation
     * \param LV a Value
     * \return a Value
     */
    inline Value * getValue(const OpValue::Kind kind, const Value & LV)
    {
        OpValue ov(kind, LV.value);

        switch (kind)
        {
            case OpValue::UNARYMINUS:
                return getValue([](const MultivariatePolynomial & mp)
                {
                    return -mp;
                }, LV, ov);
            default:
                return getValue([](const MultivariatePolynomial & mp)
                {
                    return MultivariatePolynomial::getInvalid();
                }, LV, ov);
        }
    }

    /**
     * \brief Get a value associated with a binary operation applyed to two values
     * \param kind the kind of the operation
     * \param LV a Value (Left)
     * \param RV a Value (Right)
     * \return a Value
     */
    inline Value * getValue(const OpValue::Kind kind, const Value & LV, const Value & RV)
    {
        OpValue ov(kind, LV.value, RV.value);

        switch (kind)
        {
            case OpValue::PLUS:
                return getValue([](const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP)
                {
                    return LMP + RMP;
                }, LV, RV, ov);
            case OpValue::MINUS:
                return getValue([](const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP)
                {
                    return LMP - RMP;
                }, LV, RV, ov);
            case OpValue::TIMES:
            case OpValue::DOTTIMES:
                return getValue([](const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP)
                {
                    return LMP * RMP;
                }, LV, RV, ov);
            case OpValue::RDIV:
            case OpValue::DOTRDIV:
                return getValue([](const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP)
                {
                    return LMP / RMP;
                }, LV, RV, ov);
            case OpValue::POWER:
            case OpValue::DOTPOWER:
                return getValue([](const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP)
                {
                    return LMP ^ RMP;
                }, LV, RV, ov);
            default:
                return getValue([](const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP)
                {
                    return MultivariatePolynomial::getInvalid();
                }, LV, RV, ov);
        }
    }

    /**
     * \brief Get a map containing association between symbol names and value (as ULL)
     * \return a map
     */
    inline std::map<std::wstring, uint64_t> getSymMap() const
    {
        std::map<std::wstring, uint64_t> map;
        for (const auto & p : maps)
        {
            map.emplace(p.first.getName(), p.second.value);
        }

        return map;
    }

    /**
     * \brief Overload of the operator << for GVN
     */
    friend inline std::wostream & operator<<(std::wostream & out, const GVN & gvn)
    {
        out << L"Constants:" << std::endl;
        for (const auto & p : gvn.mapi64)
        {
            out << L"  " << p.first << L" -> " << p.second.value << std::endl;
        }

        out << L"Symbols:" << std::endl;
        for (const auto & p : gvn.maps)
        {
            out << L"  " << p.first << L" -> " << p.second.value << std::endl;
        }

        std::map<uint64_t, std::wstring> map;
        for (const auto & p : gvn.maps)
        {
            map.emplace(p.second.value, p.first.getName());
        }

        out << L"OpValues:" << std::endl;
        for (const auto & p : gvn.mapv)
        {
            out << L"  " << p.first << L" -> " << p.second.value << L", P: " << p.second.poly->print(map) << std::endl;
        }

        // Don't remove: useful to debug

        /*const bool show_collisions = true;
        out << std::endl << L"Map polynomials stats:" << std::endl;
        tools::printMapInfo(out, gvn.mapp, show_collisions);

        out << std::endl << L"Map constants stats:" << std::endl;
        tools::printMapInfo(out, gvn.mapi64, show_collisions);

        out << std::endl << L"Map values stats:" << std::endl;
        tools::printMapInfo(out, gvn.mapv, show_collisions);*/

        return out;
    }

private:

    /**
     * \brief Get a value associated with a polynomial which is the result of an operation
     * \param mp a polynomial
     * \param ov an operation
     * \return a Value
     */
    inline Value * getValue(const MultivariatePolynomial & mp, const OpValue & ov)
    {
        if (mp.isConstant())
        {
            return getValue(mp.constant);
        }
        else
        {
            const auto j = mapp.find(mp);
            if (j == mapp.end())
            {
                Value * value = &mapv.emplace(ov, current++).first->second;
                value->poly = &mapp.emplace(mp, value).first->first;

                return value;
            }
            else
            {
                return j->second;
            }
        }
    }

    /**
     * \brief Helper function to get the result of an unary operation applyed to the polynomial associated with a value
     * \param OPER the operation
     * \param LV a value
     * \param ov the operation kind
     * \return a Value
     */
    inline Value * getValue(MultivariatePolynomial(OPER)(const MultivariatePolynomial & mp), const Value & LV, const OpValue & ov)
    {
        const auto i = mapv.find(ov);
        if (i == mapv.end())
        {
            return getValue(OPER(*LV.poly), ov);
        }
        else
        {
            return &i->second;
        }
    }

    /**
     * \brief Helper function to get the result of a binary operation applyed to the polynomials associated with two values
     * \param OPER the operation
     * \param LV a value (Left)
     * \param RV a value (Right)
     * \param ov the operation kind
     * \return a Value
     */
    inline Value * getValue(MultivariatePolynomial(OPER)(const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP), const Value & LV, const Value & RV, const OpValue & ov)
    {
        const auto i = mapv.find(ov);
        if (i == mapv.end())
        {
            return getValue(OPER(*LV.poly, *RV.poly), ov);
        }
        else
        {
            return &i->second;
        }
    }
};

} // namespace analysis

#endif // __GVN_HXX__
