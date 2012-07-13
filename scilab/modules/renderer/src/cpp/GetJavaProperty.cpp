/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 * desc : Interface functions between between GetProperty functions and the C++/Java part of module
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "GetJavaProperty.h"
#include "BasicAlgos.hxx"

extern "C"
{
#include "BasicAlgos.h"
#include "getScilabJavaVM.h"
}

#include "CallRenderer.hxx"

using namespace org_scilab_modules_renderer;

/*---------------------------------------------------------------------------------*/
void sciGetJavaPixelCoordinates(sciPointObj * pSubwin, const double userCoord[3], int pixCoord[2])
{

}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewPixelCoordinates(char * pSubwinUID, const double userCoords[3], int pixCoords[2])
{
    double *tmp = NULL;
    double coords[3];

    coords[0] = userCoords[0];
    coords[1] = userCoords[1];
    coords[2] = userCoords[2];

    tmp = CallRenderer::getPixelFrom2dViewCoordinates(getScilabJavaVM(), pSubwinUID, coords, 3);

    pixCoords[0] = (int) tmp[0];
    pixCoords[1] = (int) tmp[1];
}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordinates(char *pSubwinUID, const double userCoords3D[3], double userCoords2D[2])
{
    double *tmp = NULL;
    double coords[3];

    coords[0] = userCoords3D[0];
    coords[1] = userCoords3D[1];
    coords[2] = userCoords3D[2];

    tmp = CallRenderer::get2dViewCoordinates(getScilabJavaVM(), pSubwinUID, coords, 3);

    userCoords2D[0] = tmp[0];
    userCoords2D[1] = tmp[1];
}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordFromPixel(char * pSubwinUID, const int pixCoords[2], double userCoords2D[2])
{
    double *tmp = NULL;
    double coords[2];

    coords[0] = (double) pixCoords[0];
    coords[1] = (double) pixCoords[1];

    tmp = CallRenderer::get2dViewFromPixelCoordinates(getScilabJavaVM(), pSubwinUID, coords, 2);

    userCoords2D[0] = tmp[0];
    userCoords2D[1] = tmp[1];
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaBoundingBox(sciPointObj * pText, double corner1[3], double corner2[3], double corner3[3], double corner4[3])
{
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaViewingArea(char * pSubwinUID, int *xPos, int *yPos, int *width, int *height)
{
    double *tmp = NULL;

    tmp = CallRenderer::getViewingArea(getScilabJavaVM(), pSubwinUID);

    *xPos = (int) tmp[0];
    *yPos = (int) tmp[1];
    *width = (int) tmp[2];
    *height = (int) tmp[3];
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaSegsBoundingBox(sciPointObj * pSegs, double bounds[6])
{

}

/*---------------------------------------------------------------------------------*/
BOOL sciGetJavaAutoResizeMode(sciPointObj * pFigure)
{
    //if (getFigureDrawer(pFigure)->getAutoResizeMode())
    {
        return TRUE;
    }
    //else
    {
        return FALSE;
    }
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaViewport(sciPointObj * pFigure, int viewport[4])
{
    //getFigureDrawer(pFigure)->getViewport(viewport);
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaNbSubticks(sciPointObj * pSubwin, double nbsubtics[3])
{

}

/*---------------------------------------------------------------------------------*/
int sciGetJavaNbXTicks(sciPointObj * pSubwin)
{
    return 0;
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaXTicksPos(sciPointObj * pSubwin, double ticksPos[], char **ticksLabels)
{
    // ticksLabels comes from graphics so is allocated with MALLOC
    // however getTicksPos will use new to allocate
    int nbTicks = sciGetJavaNbXTicks(pSubwin);
    char **javaLabels = BasicAlgos::createStringArray(nbTicks);


    // copy it into ticksLabels
    stringArrayCopy(ticksLabels, javaLabels, nbTicks);

    BasicAlgos::destroyStringArray(javaLabels, nbTicks);
}

/*---------------------------------------------------------------------------------*/
int sciGetJavaNbYTicks(sciPointObj * pSubwin)
{
    abort();
    return 0;
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaYTicksPos(sciPointObj * pSubwin, double ticksPos[], char **ticksLabels)
{
    // ticksLabels comes from graphics so is allocated with MALLOC
    // however getTicksPos will use new to allocate
    int nbTicks = sciGetJavaNbYTicks(pSubwin);
    char **javaLabels = BasicAlgos::createStringArray(nbTicks);


    // copy it into ticksLabels
    stringArrayCopy(ticksLabels, javaLabels, nbTicks);

    BasicAlgos::destroyStringArray(javaLabels, nbTicks);
}

/*---------------------------------------------------------------------------------*/
int sciGetJavaNbZTicks(sciPointObj * pSubwin)
{
    abort();
    return 0;
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaZTicksPos(sciPointObj * pSubwin, double ticksPos[], char **ticksLabels)
{
    // ticksLabels comes from graphics so is allocated with MALLOC
    // however getTicksPos will use new to allocate
    int nbTicks = sciGetJavaNbZTicks(pSubwin);
    char **javaLabels = BasicAlgos::createStringArray(nbTicks);

    // copy it into ticksLabels
    stringArrayCopy(ticksLabels, javaLabels, nbTicks);

    BasicAlgos::destroyStringArray(javaLabels, nbTicks);
}

/*---------------------------------------------------------------------------------*/
BOOL sciGetJavaIsAbleToCreateWindow(void)
{
    //if (DrawableFigure::isAbleToCreateFigure())
    {
        return booltoBOOL(true);
    }
    //else
    {
        return booltoBOOL(false);
    }
}

/*---------------------------------------------------------------------------------*/
