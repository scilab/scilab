/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_showalluimenushandles.c                                      */
/* desc : interface for howalluimenushandles routine                      */
/*------------------------------------------------------------------------*/

#include "sci_showalluimenushandles.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "HandleManagement.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int sci_showalluimenushandles( char *fname, unsigned long fname_len )
{
  CheckRhs(1,1);
  CheckLhs(1,1);

  if (VarType(1) == sci_handles)
  {
    sciPointObj *pParent=NULL;
    int m1,n1,l1;

    GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1);
    pParent = sciGetPointerFromHandle((long)*hstk(l1));

    if (sciGetEntityType (pParent) != SCI_FIGURE ) 
    {
      Scierror(999,_("%s: Wrong type for input argument: '%s' handle expected.\n"),fname,"Figure");
      return 0;
    }
    else
    {
      SciShowAllUimenus(pParent);
    }
  }
  else
  {
	  Scierror(999,_("%s: Wrong type for input argument: '%s' handle expected.\n"),fname,"Figure");
  return 0;
  }

  LhsVar(1) = 0;
  C2F(putlhsvar)();

  return 0;
}
/*--------------------------------------------------------------------------*/
int SciShowAllUimenus( sciPointObj * pparent )
{
  sciSons * psonstmp = sciGetLastSons(pparent) ;

  while(psonstmp != (sciSons *) NULL)
  {   
    if( sciGetEntityType(psonstmp->pointobj) == SCI_UIMENU )
    {
      pUIMENU_FEATURE(psonstmp->pointobj)->handle_visible = TRUE;
      SciShowAllUimenus(psonstmp->pointobj);
    }

    psonstmp = psonstmp->pprev;
  }
  return 0;
}
/*--------------------------------------------------------------------------*/
