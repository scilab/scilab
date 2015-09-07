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
    TIType Checkers::check_matrix(GVN & gvn, const TIType & in0, const TIType & in1)
    {
        switch (in0.type)
        {
        case TIType::EMPTY :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
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
        case TIType::BOOLEAN :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::BOOLEAN, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::COMPLEX :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::COMPLEX, -2, -2);
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
                    return TIType(gvn, TIType::DOUBLE, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT16 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::INT16, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT32 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::INT32, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT64 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::INT64, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT8 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::INT8, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::POLYNOMIAL :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::POLYNOMIAL, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::STRING :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::STRING, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::SPARSE :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::SPARSE, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT16 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::UINT16, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT32 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::UINT32, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT64 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::UINT64, -2, -2);
                }
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT8 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::UINT8, -2, -2);
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

    TIType Checkers::check_matrix(GVN & gvn, const TIType & in0, const TIType & in1, const TIType & in2)
    {
        switch (in0.type)
        {
        case TIType::EMPTY :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
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
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::BOOLEAN :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::BOOLEAN, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::COMPLEX :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::COMPLEX, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
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
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::DOUBLE, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT16 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::INT16, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT32 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::INT32, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT64 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::INT64, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::INT8 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::INT8, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::POLYNOMIAL :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::POLYNOMIAL, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::STRING :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::STRING, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::SPARSE :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::SPARSE, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT16 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT16, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT32 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT32, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT64 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT64, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
            }
            else
            {
                return TIType(gvn);
            }
            return TIType(gvn);
        }
        case TIType::UINT8 :
        {
            if (in1.type == TIType::DOUBLE)
            {
                if (in2.type == TIType::DOUBLE)
                {
                    if (in1.rows == 1 && in1.cols == 1 && in2.rows == 1 && in2.cols == 1)
                    {
                        return TIType(gvn, TIType::UINT8, -2, -2);
                    }
                }
                else
                {
                    return TIType(gvn);
                }
                return TIType(gvn);
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