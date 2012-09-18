/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: get_type_property.c                                              */
/* desc : function to retrieve in Scilab the type field of a              */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

static char *getTypeNameFromInt(int _iType)
{
    switch (_iType)
    {
    case __GO_ARC__ :                       return "Arc";
    case __GO_AXES__ :                      return "Axes";
    case __GO_AXESMODEL__ :                 return "AxesModel";
    case __GO_AXIS__ :                      return "Axis";
    case __GO_CHAMP__ :                     return "Champ";
    case __GO_COMPOUND__ :                  return "Compound";
    case __GO_FAC3D__ :                     return "Fac3d";
    case __GO_FEC__ :                       return "Fec";
    case __GO_FIGURE__ :                    return "Figure";
    case __GO_FIGUREMODEL__ :               return "FigureModel";
    case __GO_GRAYPLOT__ :                  return "Grayplot";
    case __GO_LABEL__ :                     return "Label";
    case __GO_LEGEND__ :                    return "Legend";
    case __GO_MATPLOT__ :                   return "Matplot";
    case __GO_PLOT3D__ :                    return "Plot3d";
    case __GO_POLYLINE__ :                  return "Polyline";
    case __GO_RECTANGLE__ :                 return "Rectangle";
    case __GO_SEGS__ :                      return "Segs";
    case __GO_TEXT__ :                      return "Text";
    case __GO_UICONTROL__ :                 return "uicontrol";
    case __GO_UIMENU__ :                    return "uimenu";
    case __GO_UICONTEXTMENU__ :             return "uicontextmenu";
    case __GO_CONSOLE__ :                   return "Console";
    case __GO_SHOWHIDDENHANDLES__ :         return "ShowHiddenHandles";
    case __GO_WAITBAR__ :                   return "Waitbar";
    default :                               return "????";
    }
}

/*------------------------------------------------------------------------*/
int get_type_property(void* _pvCtx, char* pobjUID)
{
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piType);
    return sciReturnString(_pvCtx, getTypeNameFromInt(iType));
    //return sciReturnString( sciGetCharEntityType( pobj ) ) ;
}
/*------------------------------------------------------------------------*/
