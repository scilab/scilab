/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*/

#ifdef _MSC_VER
#pragma warning( disable : 4275 )
#endif

#ifndef __API_SCILAB_HXX__
#define __API_SCILAB_HXX__

#include "dynlib_api_scilab.h"
#include "api_variable.hxx"
#include "api_double.hxx"
#include "api_string.hxx"
#include "api_boolean.hxx"
#include "api_int.hxx"
#include "api_sparse.hxx"
#include "api_handle.hxx"
#include "api_pointer.hxx"
#include "api_list.hxx"
#include "api_common.hxx"
#include "function.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}
#endif /* ! __API_SCILAB_HXX__ */

