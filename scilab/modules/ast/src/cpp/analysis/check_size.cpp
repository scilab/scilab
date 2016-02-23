/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2015 - Scilab Enterprises - Calixte DENIZET
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

// This file has been generated, so don't modify it by hand !!

#include "checkers/Checkers.hxx"

namespace analysis
{
TIType Checkers::check_size(GVN & gvn, const TIType & in0)
{
    switch (in0.type)
    {
        case TIType::EMPTY :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::BOOLEAN :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::COMPLEX :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::DOUBLE :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::INT16 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::INT32 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::INT64 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::INT8 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::POLYNOMIAL :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::STRING :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::SPARSE :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::UINT16 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::UINT32 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::UINT64 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        case TIType::UINT8 :
        {
            return TIType(gvn, TIType::DOUBLE, 1, 1);
        }
        default :
            return TIType(gvn);
    }
}

TIType Checkers::check_size(GVN & gvn, const TIType & in0, const TIType & in1)
{
    switch (in0.type)
    {
        case TIType::EMPTY :
        {
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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
            if (in1.type == TIType::STRING)
            {
                if (in1.rows == 1 && in1.cols == 1)
                {
                    return TIType(gvn, TIType::DOUBLE, 1, 1);
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