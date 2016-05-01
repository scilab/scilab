/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
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

#ifndef __FUNCTIONS_GW_HXX__
#define __FUNCTIONS_GW_HXX__

#include "cpp_gateway_prototype.hxx"

#include "dynlib_functions_gw.h"

CPP_GATEWAY_PROTOTYPE_EXPORT(sci_exec, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_execstr, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_funclist, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_whereis, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_getCurrentThreadId, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_getThreads, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_macrovar, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_libraryinfo, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_librarieslist, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_testGVN, FUNCTIONS_GW_IMPEXP);
CPP_GATEWAY_PROTOTYPE_EXPORT(sci_testAnalysis, FUNCTIONS_GW_IMPEXP);

#endif /* __FUNCTIONS_GW_HXX__ */

