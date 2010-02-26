/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
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
/* file: sci_unzoom.h                                                     */
/* desc : interface for unzoom routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "GetProperty.h"
#include "MALLOC.h"
#include "axesScale.h"
#include "localization.h"
#include "Scierror.h"
#include "getPropertyAssignedValue.h"
#include "HandleManagement.h"

/*--------------------------------------------------------------------------*/
int sci_unzoom(char *fname,unsigned long fname_len)
{
  CheckRhs(0,1) ;
  CheckLhs(0,1) ;
  if ( Rhs == 0 )
  {
    sciUnzoomAll();
  }
  else
  {
    int nbUnzoomedObjects = 0;
    int m,n,i;
    size_t stackPointer = 0;
    sciPointObj ** zoomedObjects = NULL; /* array of object to unzoom */
    GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m,&n,&stackPointer);
    
    nbUnzoomedObjects = m * n;
    zoomedObjects = MALLOC(nbUnzoomedObjects * sizeof(sciPointObj *));
    if (zoomedObjects == NULL)
    {
      Scierror(999, _("%s: No more memory.\n"),fname);
      return -1;
    }

    /* first pass, check that all the handles are subwindows or figures */
    /* and copy them into an array of objects */
    for ( i = 0 ; i < nbUnzoomedObjects ; i++ )
    {
      zoomedObjects[i] = sciGetPointerFromHandle(getHandleFromStack(stackPointer + i));
      if (sciGetEntityType(zoomedObjects[i]) != SCI_SUBWIN && sciGetEntityType(zoomedObjects[i]) != SCI_FIGURE)
      {
        FREE(zoomedObjects);
        Scierror(999, _("%s: Wrong type for input argument: Vector of Axes and Figure handles expected.\n"),fname);
        return -1;
      }
    }

    /* second pass draw the objects */
    sciUnzoomArray(zoomedObjects, nbUnzoomedObjects);

    FREE(zoomedObjects);

  }
  

  LhsVar(1)=0; 
	C2F(putlhsvar)();
  return 0;
}
/*--------------------------------------------------------------------------*/
