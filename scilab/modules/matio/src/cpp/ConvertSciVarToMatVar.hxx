/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Sylvain GENIN
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
