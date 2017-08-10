/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - 2000 - ENPC - Jean-Philipe Chancelier
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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

GRAPHICS_IMPEXP void compute_data_bounds2(int cflag, char dataflag, char * logflags, double *x, double  *y, int n1, int n2, double *drect);

GRAPHICS_IMPEXP BOOL update_specification_bounds(int iSubwinUID, double rect[6], int flag);


#endif  /* _SCI_ECH */






