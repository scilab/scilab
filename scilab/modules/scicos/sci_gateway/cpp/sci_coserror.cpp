/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2015 - Scilab Enterprises - Paul Bignier
 *  Copyright (C) INRIA - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <string>
#include <cstdio>
#include <cwchar>

#include "gw_scicos.hxx"

#include "internal.hxx"
#include "types.hxx"
#include "string.hxx"
#include "function.hxx"

extern "C"
{
#include "machine.h" /* C2F */
#include "scicos.h" /* set_block_error() */
#include "scicos-def.h"
#include "localization.h"
#include "Scierror.h"

    /*--------------------------------------------------------------------------*/
    // Variable defined in scicos.c
    extern COSERR_struct coserr;
    /*--------------------------------------------------------------------------*/
    // Variable defined in sci_scicosim.cpp
    extern COSIM_struct C2F(cosim);
    /*--------------------------------------------------------------------------*/
}
/*--------------------------------------------------------------------------*/
/* Coserror : break the simulation and return a message
*            in the scicos editor
*
* []=Coserror('errmsg')
*
* Input : first rhs is a string
*
*/
/*--------------------------------------------------------------------------*/
static const std::string funname = "coserror";

/* Renvoie un message d'erreur */
types::Function::ReturnValue sci_coserror(types::typed_list &in, int _iRetCount, types::typed_list &/*out*/)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }

    const int isrun = C2F(cosim).isrun;
    if (!isrun)
    {
        Scierror(999, _("%s: scicosim is not running.\n"), funname.data());
        return types::Function::Error;
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), funname.data(), 1);
        return types::Function::Error;
    }
    types::String* inputMsg = in[0]->getAs<types::String>();

    sprintf(coserr.buf, " ");
    // Write the argument message elements into 'coserr.buf', line by line
    for (int i = 0; i < inputMsg->getSize(); ++i)
    {
        char* c_str = wide_string_to_UTF8(inputMsg->get(i));
        sprintf(coserr.buf, "%s\n%s", coserr.buf, c_str);
        FREE(c_str);
    }
    set_block_error(-5);

    return types::Function::OK;
}
