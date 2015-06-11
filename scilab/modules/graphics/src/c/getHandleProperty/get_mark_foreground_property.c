/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_mark_foreground_property.c                                   */
/* desc : function to retrieve in Scilab the mark_foreground field of a   */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_mark_foreground_property(void* _pvCtx, int iObjUID)
{
    int iMarkForeground = 0;
    int* piMarkForeground = &iMarkForeground;

    int * markForegrounds = NULL;
    int numMarkForegrounds = 0;
    int * piNumMarkForegrounds = &numMarkForegrounds;

	getGraphicObjectProperty(iObjUID, __GO_NUM_MARK_FOREGROUNDS__, jni_int, (void**)&piNumMarkForegrounds);

	if (numMarkForegrounds == 0)
	{
		getGraphicObjectProperty(iObjUID, __GO_MARK_FOREGROUND__, jni_int, &piMarkForeground);
		return sciReturnDouble(_pvCtx, iMarkForeground);
	}
	else
	{
		getGraphicObjectProperty(iObjUID, __GO_MARK_FOREGROUNDS__, jni_int_vector, &markForegrounds);
		return sciReturnRowVectorFromInt(_pvCtx, markForegrounds, numMarkForegrounds);
	}
}
/*------------------------------------------------------------------------*/
