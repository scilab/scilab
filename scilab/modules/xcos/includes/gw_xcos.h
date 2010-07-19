/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
* Copyright (C) DIGITEO - 2010-2010 - Clément DAVID
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

XCOS_IMPEXP int sci_xcosPalLoad(char *fname, int* _piKey);
XCOS_IMPEXP int sci_xcosPalCategoryAdd(char *fname, int* _piKey);
XCOS_IMPEXP int sci_xcosPalDelete(char *fname, int* _piKey);
XCOS_IMPEXP int sci_xcosPalMove(char *fname, int* _piKey);
XCOS_IMPEXP int sci_xcosPalEnable(char *fname, int* _piKey);
XCOS_IMPEXP int sci_xcosPalDisable(char *fname, int* _piKey);
XCOS_IMPEXP int sci_xcosPalGenerateIcon(char *fname, int* _piKey);

/*--------------------------------------------------------------------------*/
#endif /* __GW_XCOS__ */
/*--------------------------------------------------------------------------*/
