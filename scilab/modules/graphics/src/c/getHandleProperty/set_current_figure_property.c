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
/* file: set_current_figure_property.c                                    */
/* desc : function to modify in Scilab the current_figure field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <stdio.h>

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "HandleManagement.h"

#include "BuildObjects.h"
#include "CurrentFigure.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "FigureList.h"
#include "CurrentSubwin.h"


/*------------------------------------------------------------------------*/
int set_current_figure_property(char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    int figNum = -1 ;
    int res = -1 ;
    char* pFigureUID = NULL;
    char* curFigUID = NULL;
    char* pstrAxesUID = NULL;

	if (pobjUID != NULL)
	{
		/* This property should not be called on an handle */
		Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_figure");
		return SET_PROPERTY_ERROR;
	}

    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("Wrong size for '%s' property: A scalar expected.\n"), "current_figure");
        return SET_PROPERTY_ERROR ;
    }

    if ( isParameterHandle( valueType ) )
    {

        curFigUID = getObjectFromHandle( getHandleFromStack( stackPointer ) ) ;

        if ( curFigUID == NULL )
        {
            Scierror(999, _("'%s' handle does not or no longer exists.\n"),"Figure");
            return SET_PROPERTY_ERROR ;
        }
        setCurrentFigure(curFigUID);

        return 0;
// FIXME
#if 0
        if ( sciGetEntityType( curFig ) != SCI_FIGURE )
        {
            Scierror(999, _("Wrong type for '%s' property: Real or '%s' handle expected.\n"), "current_figure","Figure") ;
            return SET_PROPERTY_ERROR;
        }
        startGraphicDataReading();
        figNum = sciGetNum( curFig ) ;
        endGraphicDataReading();
#endif
    }
    else if ( isParameterDoubleMatrix( valueType ) )
    {
        figNum = (int) getDoubleFromStack( stackPointer ) ;
    }
    else
    {
        Scierror(999, _("Wrong type for '%s' property: Real or '%s' handle expected.\n"), "current_figure","Figure") ;
        return SET_PROPERTY_ERROR ;
    }

    /* Retrieve figure with figNum */
    pFigureUID = getFigureFromIndex(figNum);

    if (pFigureUID == NULL)
    {
        // No Figure available with this index, should create it  !!
        pFigureUID = createNewFigureWithAxes();
        setGraphicObjectProperty(pFigureUID, __GO_ID__, &figNum, jni_int, 1);
    }
    setCurrentFigure(pFigureUID);

    getGraphicObjectProperty(pFigureUID, __GO_SELECTED_CHILD__, jni_string,  &pstrAxesUID);
    setCurrentSubWin(pstrAxesUID);

    return 0;
#if 0
    /* select the figure num */
    res = sciSetUsedWindow( figNum ) ;
    if ( res < 0 )
    {
        Scierror(999, _("Unable to create requested figure: No more memory.\n"));
    }
    return res ;
#endif
}
/*------------------------------------------------------------------------*/
