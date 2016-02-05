/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#include "internal.hxx"
#include "double.hxx"
#include "core_gw.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "string.hxx"
#include "context.hxx"

extern "C" {
#include "Scierror.h"
#include "localization.h"
#include "sci_types.h"
}

types::Function::ReturnValue sci_type(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d expected.\n"), "type", 1);
        return types::Function::Error;
    }

    int type;
    switch (in[0]->getType())
    {
        case types::InternalType::ScilabDouble :
            type = sci_matrix;
            break;
        case types::InternalType::ScilabPolynom :
            type = sci_poly;
            break;
        case types::InternalType::ScilabBool :
            type = sci_boolean;
            break;
        case types::InternalType::ScilabSparse :
            type = sci_sparse;
            break;
        case types::InternalType::ScilabSparseBool :
            type = sci_boolean_sparse;
            break;
        //case types::InternalType::RealMatlabSparse :
        //    *_piType = sci_matlab_sparse;
        //    break;
        case types::InternalType::ScilabInt8 :
        case types::InternalType::ScilabUInt8 :
        case types::InternalType::ScilabInt16 :
        case types::InternalType::ScilabUInt16 :
        case types::InternalType::ScilabInt32 :
        case types::InternalType::ScilabUInt32 :
        case types::InternalType::ScilabInt64 :
        case types::InternalType::ScilabUInt64 :
            type = sci_ints;
            break;
        case types::InternalType::ScilabHandle :
            type = sci_handles;
            break;
        case types::InternalType::ScilabString :
            type = sci_strings;
            break;
        case types::InternalType::ScilabMacroFile :
            type = sci_u_function;
            break;
        case types::InternalType::ScilabMacro :
            type = sci_c_function;
            break;
        case types::InternalType::ScilabList :
            type = sci_list;
            break;
        case types::InternalType::ScilabCell :
            type = sci_mlist;
            break;
        case types::InternalType::ScilabTList :
            type = sci_tlist;
            break;
        case types::InternalType::ScilabMList :
            type = sci_mlist;
            break;
        case types::InternalType::ScilabStruct :
            // Scilab < 6 compatibility... Struct have type 17;
            type = sci_mlist;
            break;
        case types::InternalType::ScilabUserType :
            type = sci_pointer;
            break;
        case types::InternalType::ScilabColon :
        case types::InternalType::ScilabImplicitList :
            type = sci_implicit_poly;
            break;
        case types::InternalType::ScilabFunction:
            type = sci_intrinsic_function;
            break;
        case types::InternalType::ScilabLibrary:
            type = sci_lib;
            break;
        default:
            type = 0;
    }

    out.push_back(new types::Double((double)type));
    return types::Function::OK;
}
