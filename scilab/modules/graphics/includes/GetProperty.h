/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to GET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/



#ifndef __SCI_GET_PROPERTY__
#define __SCI_GET_PROPERTY__

#include "dynlib_graphics.h"
#include "StringMatrix.h"
#include "BOOL.h"

/* Graphic Context Functions */
GRAPHICS_IMPEXP int sciGetNumColors (int iObjUID); /* GET */

/* Text Functions */
GRAPHICS_IMPEXP BOOL sciisTextEmpty(int iIdentifier);

/* Window Functions */
GRAPHICS_IMPEXP double *sciGetPoint (int iObjUID, int *num, int *numco); /* GET */

/**
 * for a subwindow object, return if the its labels (x,y,z) are all
 *       empty or not.
 * @param pObjUID the subwindow's identifier.
 */
GRAPHICS_IMPEXP BOOL sciGetLegendDefined(int iObjUID);

GRAPHICS_IMPEXP void sciGetLogFlags(int iObjUID, char flags[3]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewCoordinate(int iObjUID, const double userCoords3D[3], double userCoords2D[2]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewCoordFromPixel(int iObjUID, const int pixelCoords[2], double userCoords2D[2]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewPixelCoordinates(int iObjUID, const double userCoords2D[2], int pixelCoords[2]); /* GET */

GRAPHICS_IMPEXP void sciGetViewingArea(int iObjUID, int * xPos, int * yPos, int * width, int * height); /* GET */

GRAPHICS_IMPEXP void printSetGetErrorMessage(const char * propertyName);

#endif /* __SCI_GET_PROPERTY__ */
