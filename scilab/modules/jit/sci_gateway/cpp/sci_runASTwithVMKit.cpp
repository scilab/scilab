/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "funcmanager.hxx"
#include "context.hxx"
#include "core_gw.hxx"
#include "string.hxx"

extern "C"
{
#include "sci_mode.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_mode.h"
#include "os_strdup.h"
}

// Defined at modules/core/src/cpp/scilab.cpp
extern bool ASTrunVMKit;

/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_runVMKit(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    Scierror(999, _("Comming soon...\n"));

    ASTrunVMKit = true;

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
