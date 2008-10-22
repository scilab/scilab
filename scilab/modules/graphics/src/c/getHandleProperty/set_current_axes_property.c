/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: set_current_axes_property.c                                      */
/* desc : function to modify in Scilab the current_axes field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_current_axes_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  sciPointObj * curAxes   = NULL ;
  sciPointObj * parentFig = NULL ;
  int num = -1 ;
  int status1 = 1 ;
  int status2 = 1 ;
  
  if ( !isParameterHandle( valueType ) )
  {
    sciprint(_("Incompatible type for property %s.\n"),"current_axes") ;
    return SET_PROPERTY_ERROR ;
  }

  curAxes = sciGetPointerFromHandle( getHandleFromStack( stackPointer ) ) ;

  if ( curAxes == NULL)
  {
    sciprint("Object is not valid.\n") ;
    return -1 ;
  }
  if ( sciGetEntityType( curAxes ) != SCI_SUBWIN )
  {
    sciprint("Object is not an Axes Entity.\n") ;
    return -1 ;
  }

  status1 = sciSetSelectedSubWin( curAxes ) ;
  /* F.Leray 11.02.05 : if the new selected subwin is not inside the current figure, */
  /* we must also set the current figure to subwin->parent */
  parentFig = sciGetParentFigure( curAxes );

  num  = sciGetNum( parentFig ) ;

  status2 = sciSetUsedWindow( num ) ;

  /* End modif. on the 11.02.05 */
  return sciSetFinalStatus( (SetPropertyStatus)status1, (SetPropertyStatus)status2 ) ;

}
/*------------------------------------------------------------------------*/
