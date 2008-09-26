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

#include "ObjectStructure.h"



int setscale2d( double WRect[4],
                 double ARect[4],
                 double FRect[4],
                 char   logscale[2]);

int getscale2d( double WRect[4], double FRect[4], char logscale[2], double ARect[4] ) ;


void unzoom( void ) ;


void convertUserCoordToPixelCoords(const double xCoords[], const double yCoords[],
                                   int xPixCoords[], int yPixCoords[], int nbCoords,
                                   int rect[4]);

void convertPixelCoordsToUserCoords(const int xPixCoords[], const int yPixCoords[],
                                    double xUserCoords[], double yUserCoords[], int nbCoords,
                                    int rect[4]);

void compute_data_bounds2(int cflag,char dataflag, char * logflags, double *x,double  *y, int n1,int n2, double *drect);

BOOL update_specification_bounds(sciPointObj  *psubwin,double rect[6],int flag);


#endif  /* _SCI_ECH */






