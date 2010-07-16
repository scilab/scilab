/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_visible_property.c                                           */
/* desc : function to retrieve in Scilab the visible field of a handle    */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "GetUiobjectVisible.h"
#include "GetProperty.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/

int get_visible_property( sciPointObj * pobj )
{
  int* visible;

  if ( (sciGetEntityType(pobj) == SCI_UIMENU) || (sciGetEntityType(pobj) == SCI_UICONTROL) )
    {
      return GetUiobjectVisible(pobj);
    }

  visible = (int*)getGraphicObjectProperty(pobj->UID, __GO_VISIBLE__, jni_bool);

  if (*visible)
  {
    return sciReturnString( "on" ) ;
  }
  else
  {
    return sciReturnString( "off" ) ;
  }
}

int get_UID(sciPointObj * pobj)
{
    return sciReturnString(pobj->UID);
}
/*------------------------------------------------------------------------*/
