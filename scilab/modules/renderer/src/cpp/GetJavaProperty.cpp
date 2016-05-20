/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2012 - DIGITEO - Manuel Juliachs
 * desc : Interface functions between between GetProperty functions and the C++/Java part of module
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

extern "C"
{
#include "getScilabJavaVM.h"
#include "GetJavaProperty.h"
}

#include "CallRenderer.hxx"

using namespace org_scilab_modules_renderer;

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewPixelCoordinates(int iSubwinUID, const double userCoords[3], int pixCoords[2])
{
    double *tmp = NULL;
    double coords[3];

    coords[0] = userCoords[0];
    coords[1] = userCoords[1];
    coords[2] = userCoords[2];

    tmp = CallRenderer::getPixelFrom2dViewCoordinates(getScilabJavaVM(), iSubwinUID, coords, 3);

    pixCoords[0] = (int) tmp[0];
    pixCoords[1] = (int) tmp[1];
    delete[] tmp;
}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordinates(int iSubwinUID, const double userCoords3D[3], double userCoords2D[2])
{
    double *tmp = NULL;
    double coords[3];

    coords[0] = userCoords3D[0];
    coords[1] = userCoords3D[1];
    coords[2] = userCoords3D[2];

    tmp = CallRenderer::get2dViewCoordinates(getScilabJavaVM(), iSubwinUID, coords, 3);

    userCoords2D[0] = tmp[0];
    userCoords2D[1] = tmp[1];
    delete[] tmp;
}

/*---------------------------------------------------------------------------------*/
void sciGetJava2dViewCoordFromPixel(int iSubwinUID, const int pixCoords[2], double userCoords2D[2])
{
    double *tmp = NULL;
    double coords[2];

    coords[0] = (double) pixCoords[0];
    coords[1] = (double) pixCoords[1];

    tmp = CallRenderer::get2dViewFromPixelCoordinates(getScilabJavaVM(), iSubwinUID, coords, 2);

    userCoords2D[0] = tmp[0];
    userCoords2D[1] = tmp[1];
    delete[] tmp;
}

/*---------------------------------------------------------------------------------*/
void sciGetJavaViewingArea(int iSubwinUID, int *xPos, int *yPos, int *width, int *height)
{
    double *tmp = NULL;

    tmp = CallRenderer::getViewingArea(getScilabJavaVM(), iSubwinUID);

    *xPos = (int) tmp[0];
    *yPos = (int) tmp[1];
    *width = (int) tmp[2];
    *height = (int) tmp[3];
    delete[] tmp;
}

/*---------------------------------------------------------------------------------*/
