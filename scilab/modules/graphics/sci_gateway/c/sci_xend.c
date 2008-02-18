/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/* file: sci_xend.c                                                       */
/* desc : interface for xend routine                                      */
/*------------------------------------------------------------------------*/

#include "sci_xend.h"
#include "DrawObjects.h"
#include "BuildObjects.h"
#include "gw_graphics.h"
#include "DestroyObjects.h"
#include "stack-c.h"
#include "ObjectStructure.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "graphicSession.h"
#include "CurrentObjectsManagement.h"
#include "DrawingBridge.h"

/*--------------------------------------------------------------------------*/
int sci_xend(char *fname,unsigned long fname_len)
{
  sciPointObj * figure = NULL;


  CheckRhs(-1,0);

  closeGraphicSession() ;   /* we DO draw now into the file/memory (depending on the driver type) */
  figure = sciGetCurrentFigure();
  sciDrawObj(figure);

  /* figure already defined above */
  destroyGraphicHierarchy(figure);

  LhsVar(1)=0;
  return 0;
}


/*--------------------------------------------------------------------------*/
