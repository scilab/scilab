/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Gustavo Barbosa Libotte <gustavolibotte@gmail.com>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_datatips_property.c                                          */
/* desc : function to modify in Scilab the datatips polyline's            */
/*        field of a handle                                               */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_datatips_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    Scierror(999, _("Datatips property can not be modified directly.\n"));
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/