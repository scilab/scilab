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
/* file: get_text_property.c                                              */
/* desc : function to retrieve in Scilab the text field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "StringMatrix.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------*/
int get_text_property( sciPointObj * pobj )
{
  int nbRow = 0;
  int nbCol = 0;

  char* text;
  char** textMatrix;

  /* Deactivated for now, as the text object stores a single string */
#if 0
  /* get the size of the text matrix */
  sciGetTextSize( pobj , &nbRow, &nbCol );
#endif

  /* Both values set to 1 since only one string is stored */
  nbRow = 1;
  nbCol = 1;

#if 0
  if ( nbRow < 0 || nbCol < 0 )
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"text");
    return -1;
  }
#endif

  text = (char*) getGraphicObjectProperty(pobj->UID, __GO_TEXT_STRING__, jni_string);

  if (text == NULL)
  {
    Scierror(999, _("'%s' property does not exist for this handle.\n"),"text");
    return -1;
  }

  textMatrix = &text;

  return sciReturnStringMatrix( textMatrix, nbRow, nbCol);

/* Deactivated for now since the text data is implemented as a single string. To be corrected. */
#if 0
  return sciReturnStringMatrix( getStrMatData( sciGetText( pobj ) ), nbRow, nbCol );
#endif
}
/*------------------------------------------------------------------------*/
