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
    TIType Checkers::check_isreal(GVN & gvn, const TIType & in0)
    {
        switch (in0.type)
        {
        case TIType::COMPLEX :
        {
            return TIType(gvn, TIType::BOOLEAN, 1, 1);
        }
        case TIType::DOUBLE :
        {
            return TIType(gvn, TIType::BOOLEAN, 1, 1);
        }
        default :
            return TIType(gvn);
        }
    }

    TIType Checkers::check_isreal(GVN & gvn, const TIType & in0, const TIType & in1)
    {
        switch (in0.type)
        {
        case TIType::COMPLEX :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::DOUBLE :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, 1, 1);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        default :
            return TIType(gvn);
        }
    }

} // namespace analysis