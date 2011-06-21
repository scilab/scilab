/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_size_property.c                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : return the dimension (width, height) in pixels of a graphical   */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

/*------------------------------------------------------------------------*/
int get_dimension_property(char *pobjUID)
{
    int size[2] ;

    //
    // FIXME
    //

#if 0
    if (sciGetEntityType(pobj) != SCI_FIGURE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"dimension");
        return -1;
    }
#endif

  size[0] = sciGetWidth(pobjUID) ;
  size[1] = sciGetHeight(pobjUID) ;
  return sciReturnRowVectorFromInt( size, 2 ) ;
}
/*------------------------------------------------------------------------*/
