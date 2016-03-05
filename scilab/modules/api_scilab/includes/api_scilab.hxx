/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

