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

#include "ConstantValue.hxx"
#include "alltypes.hxx"
#include "tools.hxx"

namespace analysis
{

ConstantValue::ConstantValue(types::InternalType * _val) : val(_val), kind(ITVAL)
{
    _val->IncreaseRef();
}

ConstantValue::ConstantValue(const ConstantValue & cv) : val(cv.val), kind(cv.kind)
{
    if (kind == ITVAL)
    {
        val.pIT->IncreaseRef();
    }
}

ConstantValue::~ConstantValue()
{
    if (kind == ITVAL)
    {
        val.pIT->DecreaseRef();
        val.pIT->killMe();
    }
}

ConstantValue & ConstantValue::operator=(const ConstantValue & R)
{
    if (kind == ITVAL)
    {
        val.pIT->DecreaseRef();
        val.pIT->killMe();
    }
    val = R.val;
    kind = R.kind;
    if (kind == ITVAL)
    {
        val.pIT->IncreaseRef();
    }

    return *this;
}

ConstantValue & ConstantValue::operator=(types::InternalType * const pIT)
{
    if (kind == ITVAL)
    {
        val.pIT->DecreaseRef();
        val.pIT->killMe();
    }
    val = pIT;
    kind = ITVAL;
    pIT->IncreaseRef();

    return *this;
}

ConstantValue & ConstantValue::operator=(GVN::Value * const _val)
{
    if (kind == ITVAL)
    {
        val.pIT->DecreaseRef();
        val.pIT->killMe();
    }
    val = _val;
    kind = GVNVAL;

    return *this;
}

ConstantValue & ConstantValue::operator=(ConstantValue && R)
{
    if (kind == ITVAL)
    {
        val.pIT->DecreaseRef();
        val.pIT->killMe();
    }
    val = R.val;
    kind = R.kind;
    R.kind = UNKNOWN;

    return *this;
}

types::InternalType * ConstantValue::getIT() const
{
    if (kind == ITVAL)
    {
        return val.pIT;
    }

    return nullptr;
}

GVN::Value * ConstantValue::getGVNValue() const
{
    if (kind == GVNVAL)
    {
        return val.gvnVal;
    }

    return nullptr;
}

void ConstantValue::merge(const ConstantValue & cv)
{
    if (kind != UNKNOWN && cv.kind != UNKNOWN)
    {
        if (kind == cv.kind)
        {
            switch (kind)
            {
                case GVNVAL:
                    if (val.gvnVal != cv.val.gvnVal)
                    {
                        kind = UNKNOWN;
                    }
                    break;
                case ITVAL:
                    if (val.pIT != cv.val.pIT && *val.pIT != *cv.val.pIT)
                    {
                        val.pIT->DecreaseRef();
                        val.pIT->killMe();
                        kind = UNKNOWN;
                    }
                    break;
                default:
                    break;
            }
        }
        else if (kind == GVNVAL)
        {
            // cv.kind == ITVAL
            double x;
            if (cv.getDblValue(x) && val.gvnVal->poly->isConstant(x))
            {
                kind = ITVAL;
                val.pIT = cv.val.pIT;
                val.pIT->IncreaseRef();
            }
            else
            {
                kind = UNKNOWN;
            }
        }
        else
        {
            // kind == ITVAL
            double x;
            if (!getDblValue(x) || !cv.val.gvnVal->poly->isConstant(x))
            {
                val.pIT->DecreaseRef();
                val.pIT->killMe();
                kind = UNKNOWN;
            }
        }
    }
    else
    {
        if (kind == ITVAL)
        {
            val.pIT->DecreaseRef();
            val.pIT->killMe();
        }
        kind = UNKNOWN;
    }
}

bool ConstantValue::getGVNValue(GVN & gvn, GVN::Value *& _val) const
{
    if (kind == GVNVAL)
    {
        _val = val.gvnVal;
        return true;
    }
    else if (kind == ITVAL)
    {
        if (val.pIT->isDouble() && static_cast<types::Double *>(val.pIT)->getSize() == 1)
        {
            types::Double * pDbl = static_cast<types::Double *>(val.pIT);
            if (!pDbl->isComplex() || pDbl->getImg(0) == 0)
            {
                const double x = pDbl->get()[0];
                int64_t i;
                if (tools::asInteger(x, i))
                {
                    _val = gvn.getValue(i);
                    return true;
                }
            }
        }
    }

    return false;
}

bool ConstantValue::getDblValue(double & _val) const
{
    if (kind == ITVAL)
    {
        if (val.pIT->isDouble() && static_cast<types::Double *>(val.pIT)->getSize() == 1)
        {
            types::Double * pDbl = static_cast<types::Double *>(val.pIT);
            if (!pDbl->isComplex() || pDbl->getImg(0) == 0)
            {
                _val = pDbl->get()[0];
                return true;
            }
        }
    }
    else if (kind == GVNVAL)
    {
        if (GVN::Value * gvnValue = val.gvnVal)
        {
            if (gvnValue->poly->isConstant())
            {
                _val = (double)gvnValue->poly->constant;
                return true;
            }
        }
    }
    return false;
}

bool ConstantValue::getBoolValue(bool & _val) const
{
    if (kind == ITVAL)
    {
        if (val.pIT->isBool() && static_cast<types::Bool *>(val.pIT)->getSize() == 1)
        {
            _val = (bool)static_cast<types::Bool *>(val.pIT)->get(0);
        }
        else
        {
            _val = val.pIT->isTrue();
        }
        return true;
    }
    else if (kind == GVNVAL)
    {
        if (GVN::Value * gvnValue = val.gvnVal)
        {
            if (gvnValue->poly->isConstant())
            {
                _val = gvnValue->poly->constant != 0;
                return true;
            }
        }
    }
    return false;
}

bool ConstantValue::getCplxValue(std::complex<double> & _val) const
{
    if (kind == ITVAL)
    {
        if (val.pIT->isDouble() && static_cast<types::Double *>(val.pIT)->getSize() == 1)
        {
            types::Double * pDbl = static_cast<types::Double *>(val.pIT);
            if (pDbl->isComplex())
            {
                _val.real(pDbl->get(0));
                _val.imag(pDbl->getImg(0));
            }
            else
            {
                _val.real(pDbl->get(0));
                _val.imag(0);
            }
            return true;
        }
    }
    else if (kind == GVNVAL)
    {
        if (GVN::Value * gvnValue = val.gvnVal)
        {
            if (gvnValue->poly->isConstant())
            {
                _val.real((double)gvnValue->poly->constant);
                _val.imag(0.);
                return true;
            }
        }
    }
    return false;
}

bool ConstantValue::getStrValue(std::string & _val) const
{
    if (kind == ITVAL)
    {
        if (val.pIT->isString() && static_cast<types::String *>(val.pIT)->getSize() == 1)
        {
            _val = std::string(static_cast<types::String *>(val.pIT)->get(0));
            return true;
        }
    }

    return false;
}


std::ostream & operator<<(std::ostream & out, const ConstantValue & cv)
{
    switch (cv.kind)
    {
        case ConstantValue::GVNVAL:
            out << *cv.val.gvnVal;
            break;
        case ConstantValue::ITVAL:
        {
            if (cv.val.pIT->isDouble() && static_cast<types::Double *>(cv.val.pIT)->getSize() == 1)
            {
                out << static_cast<types::Double *>(cv.val.pIT)->get(0, 0);
            }
            else
            {
                out << "\"" << cv.val.pIT->getTypeStr() << "\"";
            }
            break;
        }
        default:
            break;
    }

    return out;
}
}
