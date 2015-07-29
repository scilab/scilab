/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This file has been generated, so don't modify it by hand !!

#include "checkers/Checkers.hxx"

namespace analysis
{
    TIType Checkers::check_argn(GVN & gvn)
    {
        return TIType(gvn, TIType::DOUBLE, 1, 1);
    }

    TIType Checkers::check_argn(GVN & gvn, const TIType & in0)
    {
        if (in0.type == TIType::DOUBLE)
        {
            if (in0.rows == 1 && in0.cols == 1)
            {
                return in0;
            }
        }
        else
        {
            return TIType(gvn);
        }
        return TIType(gvn);
    }

} // namespace analysis