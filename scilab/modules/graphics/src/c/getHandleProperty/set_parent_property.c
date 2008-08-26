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
/* file: set_parent_property.c                                            */
/* desc : function to modify in Scilab the parent field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "sciprint.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "InitUIMenu.h"
#include "SetUicontrolParent.h"

/*------------------------------------------------------------------------*/
int set_parent_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  if(sciGetEntityType( pobj ) == SCI_UIMENU)
    {
      return setMenuParent(pobj, stackPointer, valueType, nbRow, nbCol);
    }
  else if(sciGetEntityType(pobj) == SCI_UICONTROL)
    {
      return SetUicontrolParent(pobj, stackPointer, valueType, nbRow, nbCol);
    }
  else
    {
      sciprint("Parent property can not be modified directly.\n") ;
      return SET_PROPERTY_ERROR ;
    }
}
/*------------------------------------------------------------------------*/
