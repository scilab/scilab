/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2007-2008 - INRIA - Vincent COUVERT
 *  Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_TCLSCI__
#define __GW_TCLSCI__
/*--------------------------------------------------------------------------*/
#include "dynlib_tclsci.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
TCLSCI_IMPEXP int gw_tclsci(void);
/*--------------------------------------------------------------------------*/
TCLSCI_IMPEXP int sci_TCL_DoOneEvent (char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_EvalFile(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_EvalStr(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_GetVar(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_SetVar(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_opentk(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_GetVersion(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_UnsetVar(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_ExistVar(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_UpVar(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_DeleteInterp(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_CreateSlave(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_ExistInterp(char *fname,unsigned long fname_len);
TCLSCI_IMPEXP int sci_TCL_ExistArray(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_TCLSCI__ */
/*--------------------------------------------------------------------------*/

