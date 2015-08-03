/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include <string.h>
#include "function.hxx"
#include "io_gw.hxx"

extern "C"
{
#include "systemc.h"
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_unix(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return sci_host(in, _iRetCount, out);
}
