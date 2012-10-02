/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: sci_delete.c                                                     */
/* desc : interface for delete routine                                    */
/*------------------------------------------------------------------------*/

#include "MALLOC.h"
#include "gw_graphics.h"
#include "stack-c.h"
#include "DestroyObjects.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "DrawObjects.h"
#include "Interaction.h"
#include "localization.h"
#include "Scierror.h"
#include "HandleManagement.h"
#include "FigureList.h"
#include "deleteGraphicObject.h"
#include "CurrentObject.h"
#include "CurrentFigure.h"
#include "BuildObjects.h"

#include "AxesModel.h"
#include "FigureModel.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "getConsoleIdentifier.h"
#include "CurrentSubwin.h"
#include "sciprint.h"

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_delete(char *fname, unsigned long fname_len)
{
    int m1 = 0, n1 = 0, l1 = 0, m2 = 0, n2 = 0, l2 = 0, lw = 0;
    unsigned long hdl = 0;
    int nb_handles = 0, i = 0, dont_overload = 0;
    char *pobjUID = NULL;
    char* pFigureUID = NULL;
    char** childrenUID = NULL;
    int iChildrenCount = 0;
    int* childrencount = &iChildrenCount;
    int iHidden = 0;
    int *piHidden = &iHidden;

    char *pstParentUID = NULL;
    int iParentType = -1;
    int *piParentType = &iParentType;
    int iObjType = -1;
    int *piObjType = &iObjType;

    CheckRhs(0, 1);
    CheckLhs(0, 1);

    if (Rhs == 0)               /* Delete current object */
    {
        pobjUID = (char*)getCurrentObject();
        if (pobjUID == NULL)
        {
            //No current object, we can leave
            LhsVar(1) = 0;
            PutLhsVar();
            return 0;
        }

        hdl = (unsigned long)getHandle(pobjUID);
        dont_overload = 1;
        nb_handles = 1;
    }
    else
    {
        switch (VarType(1))
        {
            case sci_handles:      /* delete Entity given by a handle */
                GetRhsVar(1, GRAPHICAL_HANDLE_DATATYPE, &m1, &n1, &l1); /* Gets the Handle passed as argument */
                nb_handles = m1 * n1;

                if (Rhs == 2)
                {
                    GetRhsVar(2, STRING_DATATYPE, &m2, &n2, &l2);   /* Gets the command name */
                }
                hdl = (unsigned long) * hstk(l1); /* Puts the value of the Handle to hdl */
                break;
            case sci_strings:      /* delete("all") */
                CheckRhs(1, 1);
                GetRhsVar(1, STRING_DATATYPE, &m2, &n2, &l2);
                if (strcmp(cstk(l2), "all") == 0)
                {
                    int i = 0;
                    int iFigureNumber = sciGetNbFigure();

                    if (iFigureNumber == 0)
                    {
                        //no graphic windows, we can leave
                        LhsVar(1) = 0;
                        PutLhsVar();
                        return 0;
                    }

                    pFigureUID = (char*)getCurrentFigure();

                    getGraphicObjectProperty(pFigureUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&childrencount);

                    getGraphicObjectProperty(pFigureUID, __GO_CHILDREN__, jni_string_vector, (void **)&childrenUID);

                    for (i = 0; i < childrencount[0]; ++i)
                    {
                        getGraphicObjectProperty(childrenUID[i], __GO_HIDDEN__, jni_bool, (void **)&piHidden);
                        if (iHidden == 0)
                        {
                            deleteGraphicObject(childrenUID[i]);
                        }
                    }

                    LhsVar(1) = 0;
                    PutLhsVar();

                    return 0;
                }
                else
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: '%s' expected.\n"), fname, 1, "all");
                    return 0;
                }
                break;
            default:
                // Overload
                lw = 1 + Top - Rhs;
                C2F(overload) (&lw, "delete", 6);
                return 0;
        }
    }
    for (i = 0; i < nb_handles; i++)
    {
        char* pstTemp = NULL;
        if (Rhs != 0)
        {
            hdl = (unsigned long) * hstk(l1 + i); /* Puts the value of the Handle to hdl */
        }

        pobjUID = (char*)getObjectFromHandle(hdl);

        if (pobjUID == NULL)
        {
            Scierror(999, _("%s: The handle is not valid.\n"), fname);
            return 0;
        }

        if (isFigureModel(pobjUID) || isAxesModel(pobjUID))
        {
            Scierror(999, _("This object cannot be deleted.\n"));
            return 0;
        }

        /* Object type */
        getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **)&piObjType);
        if (iObjType == __GO_AXES__)
        {
            /* Parent object */
            getGraphicObjectProperty(pobjUID, __GO_PARENT__, jni_string, (void **)&pstParentUID);
            /* Parent type */
            getGraphicObjectProperty(pstParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);
        }

        if (iObjType == __GO_LABEL__)
        {
            Scierror(999, _("A Label object cannot be deleted.\n"));
            return 0;
        }

        //bug #11485 : duplicate pobjUID before delete it.
        pstTemp = strdup(pobjUID);
        deleteGraphicObject(pobjUID);

        /*
         ** All figure must have at least one axe child.
         ** If the last one is removed, add a new default one.
         */
        if (iObjType == __GO_AXES__ && iParentType == __GO_FIGURE__)
        {
            int iChild = 0;
            int iChildCount = 0;
            int *piChildCount = &iChildCount;
            char **pstChildren = NULL;
            int iChildType = -1;
            int *piChildType = &iChildType;
            int iAxesFound = 0;

            getGraphicObjectProperty(pstParentUID, __GO_CHILDREN_COUNT__, jni_int, (void **)&piChildCount);
            getGraphicObjectProperty(pstParentUID, __GO_CHILDREN__, jni_string_vector, (void **)&pstChildren);
            for (iChild = 0; iChild < iChildCount; iChild++)
            {
                getGraphicObjectProperty(pstChildren[iChild], __GO_TYPE__, jni_int, (void **)&piChildType);
                if (iChildType == __GO_AXES__)
                {
                    if (strcmp(getCurrentSubWin(), pstTemp) == 0) // Current axes has been deleted
                    {
                        setCurrentSubWin(pstChildren[iChild]);
                    }
                    iAxesFound = 1;
                    break;
                }
            }
            if (!iAxesFound)
            {
                /*
                 * Clone a new Axes object using the Axes model which is then
                 * attached to the newly created Figure.
                 */
                cloneAxesModel(pstParentUID);
            }
        }

        FREE(pstTemp);
    }

    if (!dont_overload)
    {
        // Overload
        lw = 1 + Top - Rhs;
        C2F(overload) (&lw, "delete", 6);
    }
    else
    {
        LhsVar(1) = 0;
        PutLhsVar();
    }

    return 0;
}
