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
/* file: set_y_ticks_property.c                                           */
/* desc : function to modify in Scilab the y_ticks field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <math.h>
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CheckTicksProperty.h"
#include "MALLOC.h"
#include "BasicAlgos.h"
#include "DrawObjects.h"
#include "freeArrayOfString.h"
#include "loadTextRenderingAPI.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
/* @TODO: remove stackPointer, nbRow, nbCol which are used */
int set_y_ticks_property( sciPointObj * pobj, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL autoTicks;
    BOOL status;
    AssignedList * tlist     = NULL ;
    sciSubWindow * ppSubWin  = NULL ;
    int            nbTicsRow = 0    ;
    int            nbTicsCol = 0    ;
    char        ** labels    = NULL ;

    double* userGrads = NULL;
    char** userLabels = NULL;
    int iLogFlag = 0;
    int* piLogFlag = &iLogFlag;

    if ( !isParameterTlist( valueType ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Typed list expected.\n"), "y_ticks");
        return SET_PROPERTY_ERROR ;
    }

#if 0
    if ( sciGetEntityType(pobj) != SCI_SUBWIN )
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"y_ticks");
        return SET_PROPERTY_ERROR;
    }
#endif

    /* To be removed */
#if 0
    ppSubWin = pSUBWIN_FEATURE(pobj);
#endif

    tlist = createTlistForTicks();

    if ( tlist == NULL )
    {
        return SET_PROPERTY_ERROR;
    }

    /* locations */
    /* To be implemented */
#if 0
    ppSubWin->axes.u_nygrads = 0;
#endif

    userGrads = createCopyDoubleMatrixFromList( tlist, &nbTicsRow, &nbTicsCol );

    if ( userGrads == NULL && nbTicsRow == -1 )
    {
        Scierror(999, _("%s: No more memory.\n"),"set_y_ticks_property");
        return SET_PROPERTY_ERROR ;
    }

    getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, &piLogFlag);

    if (iLogFlag)
    {
        int  i ;
        for ( i = 0 ; i < nbTicsRow * nbTicsCol ; i++ )
        {
            userGrads[i] = log10(userGrads[i]);
        }
    }
    else
    {
        int nbSubticks = 0;
        int *piNbSubTicks = &nbSubticks;

        getGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, jni_int, &piNbSubTicks);

        /* Nb of subtics computation and storage */ /* F.Leray 07.10.04 */
        nbSubticks = ComputeNbSubTics(pobj, userGrads, 'n', NULL, nbSubticks);

        setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_SUBTICKS__, &nbSubticks, jni_int, 1);
    }

    /* Automatic ticks must be first deactivated in order to set user ticks */
    autoTicks = FALSE;

    setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);

    status = setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LOCATIONS__, userGrads, jni_double_vector, nbTicsRow*nbTicsCol);

    if (status == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"y_ticks");
        FREE(userGrads);
        return SET_PROPERTY_ERROR;
    }

    /*  labels */
    // Here we check the size of "locations" instead of "labels", but they have the same size.
    // We need to check the size to not be 0 because an empty matrix is a matrix of double
    // and 'getCurrentStringMatrixFromList' expect a matrix of string (see bug 5148).
    // P.Lando
    if( nbTicsCol * nbTicsRow )
    {
        userLabels = getCurrentStringMatrixFromList( tlist, &nbTicsRow, &nbTicsCol );
        /* Check if we should load LaTex / MathML Java libraries */
        loadTextRenderingAPI(userLabels, nbTicsCol, nbTicsRow);

        setGraphicObjectProperty(pobj->UID, __GO_Y_AXIS_TICKS_LABELS__, userLabels, jni_string_vector, nbTicsRow*nbTicsCol);
    }
    else
    {
        /* To be implemented */
#if 0
        ppSubWin->axes.u_ylabels = NULL;
#endif
    }

    /* To be implemented */
#if 0
    ppSubWin->axes.u_nygrads = nbTicsRow * nbTicsCol;
#endif

    if (userGrads != NULL)
    {
        FREE(userGrads);
    }

    destroyAssignedList( tlist );

    return SET_PROPERTY_SUCCEED;

}
/*------------------------------------------------------------------------*/
