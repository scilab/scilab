/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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

#include "machine.h"

#ifndef __OPERATION_F_H__
#define __OPERATION_F_H__

int C2F(dpmul)(double *_pdblP1, int *_piD1, double *_pdblP2, int *_piD2, double *_pdblP3, int *_piD3);
int C2F(dmpmu)(double *mp1, int *d1, int *nl1, double *mp2, int *d2, int *nl2, double *mp3, int *d3, int *l, int *m, int *n);
#endif /* __OPERATION_F_H__ */