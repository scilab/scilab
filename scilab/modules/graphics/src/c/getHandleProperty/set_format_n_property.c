/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_format_n_property.c                                          */
/* desc : function to modify in Scilab the format_n field of              */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "string.h"


/*------------------------------------------------------------------------*/
int set_format_n_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  char* format;
  int formatLength;
  if ( !isParameterStringMatrix( valueType ) )
  {
    Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "format_n");
    return SET_PROPERTY_ERROR ;
  }

  if ( sciGetEntityType(pobj) != SCI_AXES )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"format_n") ;
    return SET_PROPERTY_ERROR ;
  }

  if(pAXES_FEATURE(pobj)->format != NULL)
    FREE(pAXES_FEATURE(pobj)->format);

  format = getStringFromStack(stackPointer);
  formatLength = strlen(format);

  if(formatLength <= 1)
  {
    pAXES_FEATURE(pobj)->format = NULL;
    return SET_PROPERTY_SUCCEED;
  }

  pAXES_FEATURE(pobj)->format = MALLOC((formatLength + 1) * sizeof(char));
  if(pAXES_FEATURE(pobj)->format != NULL)
  {
    strcpy(pAXES_FEATURE(pobj)->format, format);
    return SET_PROPERTY_SUCCEED ;
  }
  else
  {
    Scierror(999, _("%s: No more memory.\n"),"set_format_n_property");
    return SET_PROPERTY_ERROR ;
  }
  return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
