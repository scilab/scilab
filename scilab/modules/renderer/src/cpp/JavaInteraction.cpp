/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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
extern "C" {
#include "JavaInteraction.h"
#include "getScilabJavaVM.h"
}

#include "CallRenderer.hxx"

void startInteractiveZoom(int iObjUID)
{
    org_scilab_modules_renderer::CallRenderer::startInteractiveZoom(getScilabJavaVM(), iObjUID);
}

/*---------------------------------------------------------------------------------*/
double *javaClickRubberBox(int iObjUID, double *initialRect, int iRectSize)
{
    return org_scilab_modules_renderer::CallRenderer::clickRubberBox(getScilabJavaVM(), iObjUID, initialRect, iRectSize);
}

/*---------------------------------------------------------------------------------*/
double *javaDragRubberBox(int iObjUID)
{
    return org_scilab_modules_renderer::CallRenderer::dragRubberBox(getScilabJavaVM(), iObjUID);
}
/*---------------------------------------------------------------------------------*/
