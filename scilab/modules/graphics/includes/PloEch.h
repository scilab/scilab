/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2000 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 * --------------------------------------------------------------------------*/
#ifndef _SCI_ECH
#define _SCI_ECH

#include "dynlib_graphics.h"

GRAPHICS_IMPEXP void convertUserCoordToPixelCoords(const double xCoords[], const double yCoords[],
                                   int xPixCoords[], int yPixCoords[], int nbCoords,
                                   int rect[4]);

GRAPHICS_IMPEXP void convertPixelCoordsToUserCoords(const int xPixCoords[], const int yPixCoords[],
                                    double xUserCoords[], double yUserCoords[], int nbCoords,
                                    int rect[4]);

GRAPHICS_IMPEXP void compute_data_bounds2(int cflag,char dataflag, char * logflags, double *x,double  *y, int n1,int n2, double *drect);

GRAPHICS_IMPEXP BOOL update_specification_bounds(char *psubwinUID,double rect[6],int flag);


#endif  /* _SCI_ECH */






