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

#include "tools.hxx"
#include "gvn/GVN.hxx"

namespace analysis
{

void GVN::clear()
{
    mapv.clear();
    mapi64.clear();
    maps.clear();
    mapp.clear();
    list.clear();
    current = 0;
}

uint64_t GVN::getCurrentValue() const
{
    return current;
}

void GVN::insertValue(const MultivariatePolynomial & mp, Value & value)
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

void GVN::setValue(const symbol::Symbol & sym, const MultivariatePolynomial & mp)
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

void GVN::setValue(const symbol::Symbol & sym, const Value & LV)
{
    maps.emplace(sym, LV.value)->second.poly = LV.poly;
}

GVN::Value * GVN::getValue()
{
    list.emplace_back(current);
    Value & value = list.back();
    insertValue(current++, value);

    return &value;
}

GVN::Value * GVN::getValue(const MultivariatePolynomial & mp)
{
    const auto i = mapp.find(mp);
    if (i != mapp.end())
    {
        return i->second;
    }
    else
    {
        list.emplace_back(current++);
        Value & val = list.back();
        insertValue(mp, val);

        return &val;
    }
}

GVN::Value * GVN::getValue(const symbol::Symbol & sym)
{
    const auto i = maps.equal_range(sym);
    if (i.first == i.second)
    {
        GVN::Value & value = maps.emplace(sym, current)->second;
        insertValue(current++, value);

        return &value;
    }
    else
    {
        return &std::prev(i.second)->second;
    }
}

GVN::Value * GVN::getExistingValue(const symbol::Symbol & sym)
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

GVN::Value * GVN::getValue(const int64_t x)
{
    const auto i = mapi64.find(x);
    if (i == mapi64.end())
    {
        GVN::Value & value = mapi64.emplace(x, current++).first->second;
        insertValue(x, value);

        return &value;
    }
    else
    {
        return &i->second;
    }
}

GVN::Value * GVN::getValue(const double x)
{
    int64_t _x;
    if (tools::asInteger(x, _x))
    {
        return getValue(_x);
    }

    return nullptr;
}

GVN::Value * GVN::getValue(const OpValue::Kind kind, const GVN::Value & LV)
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

GVN::Value * GVN::getValue(const OpValue::Kind kind, const GVN::Value & LV, const GVN::Value & RV)
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

std::map<std::wstring, uint64_t> GVN::getSymMap() const
{
    std::map<std::wstring, uint64_t> map;
    for (const auto & p : maps)
    {
        map.emplace(p.first.getName(), p.second.value);
    }

    return map;
}

std::wostream & operator<<(std::wostream & out, const GVN & gvn)
{
    out << L"Constants:" << std::endl;
    for (const auto & p : gvn.mapi64)
    {
        out << L"  " << p.first << L" -> " << p.second.value << std::endl;
    }

    out << L"Symbols:" << std::endl;
    for (const auto & p : gvn.maps)
    {
        out << L"  " << p.first.getName().data() << L" -> " << p.second.value << std::endl;
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

GVN::Value * GVN::getValue(const MultivariatePolynomial & mp, const OpValue & ov)
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
            GVN::Value * value = &mapv.emplace(ov, current++).first->second;
            value->poly = &mapp.emplace(mp, value).first->first;

            return value;
        }
        else
        {
            return j->second;
        }
    }
}

GVN::Value * GVN::getValue(MultivariatePolynomial(OPER)(const MultivariatePolynomial & mp), const GVN::Value & LV, const OpValue & ov)
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

GVN::Value * GVN::getValue(MultivariatePolynomial(OPER)(const MultivariatePolynomial & LMP, const MultivariatePolynomial & RMP), const GVN::Value & LV, const GVN::Value & RV, const OpValue & ov)
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

} // namespace analysis
