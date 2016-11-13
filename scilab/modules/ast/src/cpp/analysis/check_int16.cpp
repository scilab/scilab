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
TIType Checkers::check_int16(GVN & gvn, const TIType & in0)
{
    switch (in0.type)
    {
        case TIType::EMPTY :
        {
            return in0;
        }
        case TIType::DOUBLE :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        case TIType::INT16 :
        {
            return in0;
        }
        case TIType::INT32 :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        case TIType::INT64 :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        case TIType::INT8 :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        case TIType::UINT16 :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        case TIType::UINT32 :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        case TIType::UINT64 :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        case TIType::UINT8 :
        {
            return TIType(gvn, TIType::INT16, in0.rows, in0.cols);
        }
        default :
            return TIType(gvn);
    }
}

} // namespace analysis