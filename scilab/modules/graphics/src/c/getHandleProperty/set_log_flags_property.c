/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_log_flags_property.c                                         */
/* desc : function to modify in Scilab the log_flags field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#ifndef _MSC_VER
#define _GNU_SOURCE
#endif
#include <string.h>
#include <math.h>
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "Sciwarning.h"

/*--------------------------------------------------------------------------*/
char ** ReBuildUserTicks(char old_logflag, char new_logflag, double* u_xgrads, int *u_nxgrads, char ** u_xlabels);
/*--------------------------------------------------------------------------*/
char ** CaseLogflagN2L(int * u_nxgrads, double *u_xgrads, char ** u_xlabels);
/*--------------------------------------------------------------------------*/
/* Remove negative graduations when switching from N (linear) to L (logarithmic) scale */
char ** CaseLogflagN2L(int * u_nxgrads, double *u_xgrads, char ** u_xlabels)
{
    int nbtics = *u_nxgrads;
    int i = 0;
    char ** ticklabel = (char **) NULL;
    int cmpteur = 0, cmpteur2 = 0, offset = 0;


    for (i = 0; i < nbtics; i++)
    {
        if (u_xgrads[i] <= 0)
        {
            Sciwarning("Warning: graduation number %d is ignored : when switching to logarithmic scale, we must have strictly positive graduations!\n", i);
        }
        else
        {
            u_xgrads[cmpteur] = log10(u_xgrads[i]);
            cmpteur++;
        }
    }

    if (cmpteur != nbtics)
    {
        if ((ticklabel = (char **)MALLOC(cmpteur * sizeof(char *))) == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "CaseLogflagN");
            return NULL;
        }

        cmpteur2 = 0;
        offset = nbtics - cmpteur;
        for (i = 0; i < cmpteur; i++)
        {
            if ((ticklabel[cmpteur2] = (char *)MALLOC((strlen(u_xlabels[i + offset]) + 1) * sizeof(char))) == NULL)
            {
                Scierror(999, _("%s: No more memory.\n"), "CaseLogflagN");
            }
            strcpy(ticklabel[cmpteur2], u_xlabels[i + offset]);
            cmpteur2++;
        }

        freeArrayOfString(u_xlabels, nbtics);
        u_xlabels = ticklabel;
    }

    *u_nxgrads = cmpteur;

    return u_xlabels;
}
/*--------------------------------------------------------------------------*/
/* Called by a.log_flags='nn','ln','nl', or 'll'*/
/* For the moment, z has no logflag F.Leray 05.10.04 */
char ** ReBuildUserTicks(char old_logflag, char new_logflag, double* u_xgrads, int *u_nxgrads, char ** u_xlabels)
{

    if (old_logflag == new_logflag)
    {
        return u_xlabels;    /* nothing to do l->l or n->n */
    }

    if (u_xgrads != NULL)
    {
        if (old_logflag == 'n' && new_logflag == 'l') /* n->l */ /* 10-> 1, 100->2 ...*/
        {

            u_xlabels = CaseLogflagN2L(u_nxgrads, u_xgrads, u_xlabels);

        }
        else if (old_logflag == 'l' && new_logflag == 'n')
        {
            int nbtics = *u_nxgrads;
            int i;

            for (i = 0; i < nbtics; i++)
            {
                u_xgrads[i] = exp10(u_xgrads[i]);
            }

        }
    }

    return  u_xlabels;
}
/*------------------------------------------------------------------------*/
int set_log_flags_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status[3];
    char * flags = NULL;
    char curLogFlags[4] = "nnn";
    int iLogFlag = 0;
    int* piLogFlag = &iLogFlag;
    int logFlags[3];
    int i = 0;
    double* dataBounds = NULL;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "log_flags");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 2 && nbRow * nbCol != 3)
    {
        Scierror(999, _("Wrong size for '%s' property: Must be %s or %s.\n"), "log_flags", "2", "3");
        return SET_PROPERTY_ERROR;
    }

    flags = (char*)_pvData;

    /* flags must be 'n' or 'l' */
    if (  (flags[0] != 'n' && flags[0] != 'l')
            || (flags[1] != 'n' && flags[1] != 'l'))
    {
        Scierror(999, _("%s: Wrong value for argument: '%s' or '%s' expected.\n"), "flags", "n", "l");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_X_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);

    if (piLogFlag == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "log_flags");
        return SET_PROPERTY_ERROR;
    }

    logFlags[0] = iLogFlag;

    getGraphicObjectProperty(iObjUID, __GO_Y_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    logFlags[1] = iLogFlag;

    getGraphicObjectProperty(iObjUID, __GO_Z_AXIS_LOG_FLAG__, jni_bool, (void **)&piLogFlag);
    logFlags[2] = iLogFlag;

    for (i = 0; i < 3; i++)
    {
        if (logFlags[i] == 1)
        {
            curLogFlags[i] = 'l';
        }
        else
        {
            curLogFlags[i] = 'n';
        }
    }

    getGraphicObjectProperty(iObjUID, __GO_DATA_BOUNDS__, jni_double_vector, (void **)&dataBounds);

    if (dataBounds == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "data_bounds");
        return SET_PROPERTY_ERROR;
    }

    /* X axes */
    if ((dataBounds[0] <= 0. || dataBounds[1] <= 0.) && flags[0] == 'l')
    {
        Scierror(999, _("Error: data_bounds on %s axis must be strictly positive to switch to logarithmic mode.\n"), "x");
        return SET_PROPERTY_ERROR;
    }

    /*
     * Commented out for now
     * To be implemented using the MVC framework
     */
#if 0
    ppSubWin->axes.u_xlabels = ReBuildUserTicks(curLogFlags[0], flags[0],
                               ppSubWin->axes.u_xgrads,
                               &ppSubWin->axes.u_nxgrads,
                               ppSubWin->axes.u_xlabels);
#endif

    curLogFlags[0] = flags[0];

    /* Y axes */
    if ((dataBounds[2] <= 0. || dataBounds[3] <= 0.) && flags[1] == 'l')
    {
        Scierror(999, _("Error: data_bounds on %s axis must be strictly positive to switch to logarithmic mode.\n"), "y");
        return SET_PROPERTY_ERROR;
    }

    /*
     * Commented out for now
     * To be implemented using the MVC framework
     */
#if 0
    ppSubWin->axes.u_ylabels = ReBuildUserTicks(curLogFlags[1], flags[1],
                               ppSubWin->axes.u_ygrads,
                               &ppSubWin->axes.u_nygrads,
                               ppSubWin->axes.u_ylabels);
#endif

    curLogFlags[1] = flags[1];


    if (nbRow * nbCol == 3)
    {
        if (flags[2] != 'n' && flags[2] != 'l')
        {
            Scierror(999, "flags must be 'n' or 'l'.\n");
            return SET_PROPERTY_ERROR;
        }

        if ((dataBounds[4] <= 0. || dataBounds[5] <= 0.) && flags[2] == 'l')
        {
            Scierror(999, _("Error: data_bounds on %s axis must be strictly positive to switch to logarithmic mode.\n"), "z");
            return SET_PROPERTY_ERROR;
        }

        /*
         * Commented out for now
         * To be implemented using the MVC framework
         */
#if 0
        ppSubWin->axes.u_zlabels = ReBuildUserTicks(curLogFlags[2], flags[2],
                                   ppSubWin->axes.u_zgrads,
                                   &ppSubWin->axes.u_nzgrads,
                                   ppSubWin->axes.u_zlabels);
#endif

        curLogFlags[2] = flags[2];
    }

    for (i = 0; i < 3; i++)
    {
        if (curLogFlags[i] == 'l')
        {
            logFlags[i] = 1;
        }
        else
        {
            logFlags[i] = 0;
        }
    }

    status[0] = setGraphicObjectProperty(iObjUID, __GO_X_AXIS_LOG_FLAG__, &logFlags[0], jni_bool, 1);
    status[1] = setGraphicObjectProperty(iObjUID, __GO_Y_AXIS_LOG_FLAG__, &logFlags[1], jni_bool, 1);
    status[2] = setGraphicObjectProperty(iObjUID, __GO_Z_AXIS_LOG_FLAG__, &logFlags[2], jni_bool, 1);

    if (status[0] == TRUE && status[1] == TRUE && status[2] == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "log_flags");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
