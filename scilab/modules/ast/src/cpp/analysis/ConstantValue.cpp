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

namespace analysis
{

void ConstantValue::merge(const ConstantValue & cv)
{
    if (kind == cv.kind)
    {
        switch (kind)
        {
            case DOUBLE:
                merge<double>(cv);
                break;
            case GVNVAL:
                merge<GVN::Value *>(cv);
                break;
            case STRING:
                merge<const std::wstring *>(cv);
                break;
            case BOOLEAN:
                merge<bool>(cv);
                break;
            case COMPLEX:
                //merge<const std::complex<double> &>(cv);
                break;
            default:
                break;
        }
    }
    else if (kind == GVNVAL && cv.kind == DOUBLE)
    {
        if (!get<GVN::Value *>()->poly->isConstant(cv.get<double>()))
        {
            kind = UNKNOWN;
        }
    }
    else if (kind == DOUBLE && cv.kind == GVNVAL)
    {
        if (cv.get<GVN::Value *>()->poly->isConstant(get<double>()))
        {
            set(cv.get<GVN::Value *>());
        }
        else
        {
            kind = UNKNOWN;
        }
    }
    else
    {
        kind = UNKNOWN;
    }
}
}
