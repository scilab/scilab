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

#include "banner.hxx"
#include "yaspio.hxx"
#include "function.hxx"

extern "C" 
{
#include "version.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_banner(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    banner();

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
