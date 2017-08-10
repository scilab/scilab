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

#ifndef __CONSTANT_VALUE_HXX__
#define __CONSTANT_VALUE_HXX__

#include <complex>
#include <iostream>
#include <string>
#include <type_traits>

#include "gvn/GVN.hxx"

namespace types
{
class InternalType;
}

namespace analysis
{
class EXTERN_AST ConstantValue
{

public:

    enum Kind : unsigned char { GVNVAL, ITVAL, UNKNOWN };

private:

    union Value
    {
        GVN::Value * gvnVal;
        types::InternalType * pIT;

        Value() { }
        Value(GVN::Value * val) : gvnVal(val) { }
        Value(types::InternalType * val) : pIT(val) { }
    };

    Value val;
    Kind kind;

public:

    ConstantValue() : kind(UNKNOWN) { }
    ConstantValue(GVN::Value * _val) : val(_val), kind(GVNVAL) { }
    ConstantValue(types::InternalType * _val);
    ConstantValue(const ConstantValue & cv);
    ConstantValue(ConstantValue && cv) : val(cv.val), kind(cv.kind)
    {
        cv.kind = UNKNOWN;
    }

    ConstantValue & operator=(const ConstantValue & R);
    ConstantValue & operator=(types::InternalType * const pIT);
    ConstantValue & operator=(GVN::Value * const _val);
    ConstantValue & operator=(ConstantValue && R);

    ~ConstantValue();

    inline Kind getKind() const
    {
        return kind;
    }

    inline bool isKnown() const
    {
        return kind != UNKNOWN;
    }

    types::InternalType * getIT() const;
    GVN::Value * getGVNValue() const;
    bool getGVNValue(GVN & gvn, GVN::Value *& _val) const;
    bool getDblValue(double & _val) const;
    bool getBoolValue(bool & _val) const;
    bool getCplxValue(std::complex<double> & _val) const;
    bool getStrValue(std::wstring & val) const;
    void merge(const ConstantValue & cv);

    friend std::wostream & operator<<(std::wostream & out, const ConstantValue & cv);
};

} // namespace analysis

#endif // __CONSTANT_VALUE_HXX__
