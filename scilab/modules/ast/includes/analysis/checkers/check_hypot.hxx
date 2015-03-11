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

// This file has been generated, so don't modify it by hand !!

#ifndef __CHECK_HYPOT_HXX__
#define __CHECK_HYPOT_HXX__

#include "TIType.hxx"

namespace analysis
{
    inline static TIType check_hypot(GVN & gvn, const TIType & in0, const TIType & in1)
    {
        switch (in0.type)
        {
        case TIType::EMPTY :
        {
            if (in1.type == TIType::EMPTY)
            {
                return in0;
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::COMPLEX :
        {
            switch (in1.type)
            {
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1 && in1.cols == in1.rows)
                {
                    return in1;
                }
                if (in0.cols == in0.rows && in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in0.isUnknownDims() || in1.isUnknownDims())
                {
                    return TIType(gvn, TIType::COMPLEX, /*scalar*/ false, /*unknown*/ true);
                }
                if (in0.cols == in0.rows && in1.rows == in0.rows && in1.cols == in0.rows)
                {
                    return in0;
                }
                return TIType(gvn);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1 && in1.cols == in1.rows)
                {
                    return TIType(gvn, TIType::COMPLEX, in1.rows, in1.rows);
                }
                if (in0.cols == in0.rows && in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in0.isUnknownDims() || in1.isUnknownDims())
                {
                    return TIType(gvn, TIType::COMPLEX, /*scalar*/ false, /*unknown*/ true);
                }
                if (in0.cols == in0.rows && in1.rows == in0.rows && in1.cols == in0.rows)
                {
                    return in0;
                }
                return TIType(gvn);
            }
            default :
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::DOUBLE :
        {
            switch (in1.type)
            {
            case TIType::COMPLEX :
            {
                if (in0.rows == 1 && in0.cols == 1 && in1.cols == in1.rows)
                {
                    return in1;
                }
                if (in0.cols == in0.rows && in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::COMPLEX, in0.rows, in0.rows);
                }
                if (in0.isUnknownDims() || in1.isUnknownDims())
                {
                    return TIType(gvn, TIType::COMPLEX, /*scalar*/ false, /*unknown*/ true);
                }
                if (in0.cols == in0.rows && in1.rows == in0.rows && in1.cols == in0.rows)
                {
                    return in1;
                }
                return TIType(gvn);
            }
            case TIType::DOUBLE :
            {
                if (in0.rows == 1 && in0.cols == 1 && in1.cols == in1.rows)
                {
                    return in1;
                }
                if (in0.cols == in0.rows && in1.rows == 1 && in1.cols == 1)
                {
                    return in0;
                }
                if (in0.isUnknownDims() || in1.isUnknownDims())
                {
                    return TIType(gvn, TIType::DOUBLE, /*scalar*/ false, /*unknown*/ true);
                }
                if (in0.cols == in0.rows && in1.rows == in0.rows && in1.cols == in0.rows)
                {
                    return in0;
                }
                return TIType(gvn);
            }
            default :
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        default :
            return TIType(gvn);
        }
    }

} // namespace analysis

#endif // __CHECK_hypot_HXX__