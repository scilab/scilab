/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __CREATEMATLABTREEVARIABLE_HXX__
#define __CREATEMATLABTREEVARIABLE_HXX__

#include "gatewaystruct.hxx"
#include "double.hxx"
#include "int.hxx"
#include "cell.hxx"
#include "struct.hxx"
#include "string.hxx"
#include "sparse.hxx"


extern "C"
{
#include "CreateMatlabVariable.h"
#include "api_scilab.h"
#include "Scierror.h"
}

matvar_t *ConvertSciVarToMatVar(types::InternalType* pIT, const char *name, int matfile_version);

#endif /* !__CREATEMATLABTREEVARIABLE_HXX__ */
