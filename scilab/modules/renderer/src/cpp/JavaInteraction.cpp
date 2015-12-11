/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
