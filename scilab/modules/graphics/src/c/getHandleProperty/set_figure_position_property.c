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
/* file: set_figure_position_property.c                                   */
/* desc : function to modify in Scilab the figure_position field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"

/*------------------------------------------------------------------------*/
int set_figure_position_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
  return set_screen_position_property(_pvCtx, pobjUID, stackPointer, valueType, nbRow, nbCol ) ;
}
/*------------------------------------------------------------------------*/
