/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __GW_XCOS__
#define __GW_XCOS__
/*--------------------------------------------------------------------------*/
#include "dynlib_xcos.h"
/*--------------------------------------------------------------------------*/
XCOS_IMPEXP int gw_xcos(void);

//YaSp
XCOS_IMPEXP int sci_Xcos(char* fname, int* _piKey);
XCOS_IMPEXP int sci_xcosNotify(char* fname, int* _piKey);
XCOS_IMPEXP int sci_warnBlockByUID(char* fname, int* _piKey);
XCOS_IMPEXP int sci_closeXcosFromScilab(char* fname, int* _piKey);
XCOS_IMPEXP int sci_xcosDiagramToHDF5(char* fname, int* _piKey);
XCOS_IMPEXP int sci_xcosDiagramOpen(char* fname, int* _piKey);
XCOS_IMPEXP int sci_xcosDiagramClose(char* fname, int* _piKey);
/*--------------------------------------------------------------------------*/
#endif /* __GW_XCOS__ */
/*--------------------------------------------------------------------------*/
