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

#ifndef __CONSTANT_VALUE_HXX__
#define __CONSTANT_VALUE_HXX__

#include <complex>
#include <iostream>
#include <string>
#include <type_traits>

#include "gvn/GVN.hxx"

namespace analysis
{
class ConstantValue
{

public:

    enum Kind : unsigned char { DOUBLE, GVNVAL, STRING, BOOLEAN, COMPLEX, UNKNOWN };

private:
    union Value
    {
        double dbl;
        GVN::Value * gvnVal;
        const std::wstring * str = NULL;
        bool boolean;
        //std::complex<double> cplx;

        Value() { }
        Value(const double val) : dbl(val) { }
        Value(GVN::Value * val) : gvnVal(val) { }
        Value(const std::wstring * val) : str(val) { }
        Value(const bool val) : boolean(val) { }
        //Value(const std::complex<double> & val) : cplx(val) { }
    };

    Value val;
    Kind kind;

public:

    ConstantValue() : kind(UNKNOWN) { }
    ConstantValue(double _val) : val(_val), kind(DOUBLE) { }
    ConstantValue(GVN::Value * _val) : val(_val), kind(GVNVAL) { }
    ConstantValue(const std::wstring * _val) : val(_val), kind(STRING) { }
    ConstantValue(bool _val) : val(_val), kind(BOOLEAN) { }
    //ConstantValue(const std::complex<double> & _val) : val(_val), kind(COMPLEX) { }

    inline Kind getKind() const
    {
        return kind;
    }

    inline bool isKnown() const
    {
        return kind != UNKNOWN;
    }

    inline bool getDblValue(double & _val) const
    {
        if (kind == DOUBLE)
        {
            _val = val.dbl;
            return true;
        }
        else if (kind == GVNVAL)
        {
            if (GVN::Value * gvnValue = val.gvnVal)
            {
                if (gvnValue->poly->isConstant())
                {
                    _val = gvnValue->poly->constant;
                    return true;
                }
            }
        }
        return false;
    }

    inline GVN::Value * getGVNValue() const
    {
        if (kind == ConstantValue::GVNVAL)
        {
            return val.gvnVal;
        }

        return nullptr;
    }

    template<typename T> inline void set(T _val);
    template<typename T> inline T get() const;

    void merge(const ConstantValue & cv);

    friend std::wostream & operator<<(std::wostream & out, const ConstantValue & cv)
    {
        switch (cv.kind)
        {
            case ConstantValue::DOUBLE:
                out << cv.val.dbl;
                break;
            case ConstantValue::GVNVAL:
                out << *cv.val.gvnVal;
                break;
            case ConstantValue::STRING:
                out << L"\"" << *cv.val.str << L"\"";
                break;
            case ConstantValue::BOOLEAN:
                out << (cv.val.boolean ? L"T" : L"F");
                break;
            //case ConstantValue::COMPLEX:
            //    out << cv.val.cplx.real() << L"+i*" << cv.val.cplx.imag();
            //    break;
            default:
                break;
        }

        return out;
    }

private:

    template<typename T, bool Ptr>
    struct __merge
    {
        inline void operator()(ConstantValue & cv1, const ConstantValue & cv2);
    };

    template<typename T>
    struct __merge<T, true>
    {
        inline void operator()(ConstantValue & cv1, const ConstantValue & cv2)
        {
            T t1 = cv1.get<T>();
            T t2 = cv2.get<T>();
            if (t1 != t2 && *t1 != *t2)
            {
                cv1.kind = UNKNOWN;
            }
        }
    };

    template<typename T>
    struct __merge<T, false>
    {
        inline void operator()(ConstantValue & cv1, const ConstantValue & cv2)
        {
            if (cv1.get<T>() != cv2.get<T>())
            {
                cv1.kind = UNKNOWN;
            }
        }
    };

    template<typename T>
    inline void merge(const ConstantValue & cv)
    {
        __merge<T, std::is_pointer<T>::value>()(*this, cv);
    }

};

template<> inline void ConstantValue::set<>(int _val)
{
    val = (double)_val;
    kind = DOUBLE;
}
template<> inline void ConstantValue::set<>(unsigned int _val)
{
    val = (double)_val;
    kind = DOUBLE;
}
template<> inline void ConstantValue::set<>(double _val)
{
    val = _val;
    kind = DOUBLE;
}
template<> inline void ConstantValue::set<>(GVN::Value * _val)
{
    val = _val;
    kind = GVNVAL;
}
template<> inline void ConstantValue::set<>(const std::wstring * _val)
{
    val = _val;
    kind = STRING;
}
template<> inline void ConstantValue::set<>(bool _val)
{
    val = _val;
    kind = BOOLEAN;
}
//template<> inline void ConstantValue::set<>(std::complex<double> && _val)
//{
//    val = _val;
//    kind = COMPLEX;
//}

template<> inline double ConstantValue::get<>() const
{
    return val.dbl;
}
template<> inline GVN::Value * ConstantValue::get<>() const
{
    return val.gvnVal;
}
template<> inline const std::wstring * ConstantValue::get<>() const
{
    return val.str;
}
template<> inline bool ConstantValue::get<>() const
{
    return val.boolean;
}
//template<> inline const std::complex<double> & ConstantValue::get<>() const
//{
//    return val.cplx;
//}

} // namespace analysis

#endif // __CONSTANT_VALUE_HXX__
