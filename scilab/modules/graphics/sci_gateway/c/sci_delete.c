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
/* file: sci_delete.c                                                     */
/* desc : interface for delete routine                                    */
/*------------------------------------------------------------------------*/

#include "gw_graphics.h"
#include "stack-c.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "Interaction.h"
#include "CurrentObjectsManagement.h"
#include "ObjectSelection.h"
#include "DrawingBridge.h"
#include "localization.h"
#include "GraphicSynchronizerInterface.h"
#include "DestroyUimenu.h"
#include "DestroyUicontrol.h"
#include "DestroyWaitBar.h"
#include "Scierror.h"
#include "HandleManagement.h"
/*--------------------------------------------------------------------------*/
int sci_delete(char *fname,unsigned long fname_len)
{
  int m1,n1,l1,m2,n2,l2,num, lw;
  unsigned long hdl;
  sciPointObj * pobj;
  sciPointObj * parentFigure;

  CheckRhs(0,1);
  CheckLhs(0,1);

  switch(VarType(1))
  {
  case sci_handles: /* delete Entity given by a handle */
    GetRhsVar(1,GRAPHICAL_HANDLE_DATATYPE,&m1,&n1,&l1); /* Gets the Handle passed as argument */
    if (m1!=1||n1!=1) {
      lw = 1 + Top - Rhs;
      C2F(overload)(&lw,"delete",6);return 0;
    }
    if (Rhs == 2)
    {
      GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2); /* Gets the command name */
    }
    hdl = (unsigned long)*hstk(l1); /* Puts the value of the Handle to hdl */
    break;
  case sci_strings: /* delete("all") */
    CheckRhs(1,1);
    GetRhsVar(1,STRING_DATATYPE,&m2,&n2,&l2);
    if (strcmp(cstk(l2),"all") == 0)
    {
			startGraphicDataWriting();
			sciClearFigure(sciGetCurrentFigure());
			endGraphicDataWriting();
			sciDrawObj(sciGetCurrentFigure()); /* redraw the figure to see the change */
			LhsVar(1) = 0;
			C2F(putlhsvar)();
      return 0;
    }
    else
    {
      Scierror(999,_("%s: Wrong value for input argument #%d: '%s' expected.\n"),fname,1,"all");
      return 0;
    }
    break;
  default:
    if (Rhs==0) /* Delete current object */
      {
        hdl = (unsigned long) sciGetHandle(sciGetCurrentObj());
      }
    else
      {
        Scierror(999,_("%s: Wrong type for input argument #%d: '%s' or handle expected.\n"),fname,1,"all");
        return 0;
      }
    break;
  }

  pobj = sciGetPointerFromHandle(hdl);

  if (pobj == NULL)
  {
    Scierror(999,_("%s: The handle is not valid.\n"),fname);
    return 0;
  }

  parentFigure = sciGetParentFigure(pobj);
      
  num = sciGetNumFigure( pobj ) ;

  if ((Rhs == 2) && (strcmp(cstk(l2),"callback") == 0))
  {
    startFigureDataWriting(parentFigure);
    sciDelCallback((sciPointObj *)pobj);
    endFigureDataWriting(parentFigure);
  }
  else
  {
    
    sciEntityType objType = sciGetEntityType( pobj ) ;

    if (objType == SCI_UIMENU)
      {
        DestroyUimenu(pobj);
      }
    else if(objType == SCI_UICONTROL)
      {
        DestroyUicontrol(pobj);
      }
    else if (objType == SCI_WAITBAR || objType == SCI_PROGRESSIONBAR)
      {
        DestroyWaitBar(pobj);
      }
    else if ( sciGetParentFigure(pobj) != NULL && objType != SCI_FIGURE)
      {
        BOOL selected = sciGetIsSelected( pobj ) ;
        sciPointObj * parentObj = sciGetParent(pobj);
        startFigureDataWriting(parentFigure);
				if (sciIsCurrentObject(pobj))
				{
					/* If the object is the current one, modify the current object pointer */
					sciSetCurrentObj(parentObj) ; /* A LAISSER F.Leray 25.03.04*/
				}
        sciDelGraphicObj( pobj ) ; /* don't use pobj after this point */
        pobj = NULL ;
        
        /* test here: we could have deleted the selected subwindow, we must choose an other */
        /* We must always have one selected subwindow (if at least one subwindow exists) */
        if ( objType == SCI_SUBWIN && selected )
        {
          /* we have to select antoher subwindow if one exists at least */
          sciSelectFirstSubwin( parentFigure ) ;
        }
        
        endFigureDataWriting(parentFigure);
        
        /* redraw the window */
        sciDrawObj( parentObj ) ;
      }
    else if( sciGetEntityType(pobj) == SCI_FIGURE ) /* F.Leray 13.04.04: We delete the special object Figure !!*/
      {
        sciDeleteWindow( num );
      }
    
  }
  
  LhsVar(1) = 0;
  C2F(putlhsvar)();
  return 0;
}
