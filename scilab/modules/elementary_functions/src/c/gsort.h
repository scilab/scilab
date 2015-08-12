/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) ???? - INRIA - Jean-Baptiste SILVY
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* Written by  Jean-Philippe Chancelier                                     */
/* Modified (restructuration and bug fix) by Allan Cornet                   */
/* Jean-Baptiste Silvy                                                      */
/*--------------------------------------------------------------------------*/

#ifndef _GSORT_H_
#define _GSORT_H_

#include "machine.h"
#define INCREASE_COMMAND 'i'
#define DECREASE_COMMAND 'd'
#define ROW_SORT 'r'
#define COLUMN_SORT 'c'
#define LIST_SORT 'l'
#define GLOBAL_SORT 'g'


void C2F(gsorts)(char **data, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
int C2F(gsortd)(double *xD, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
int C2F(gsortint)(int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
int C2F(gsortuint)(unsigned int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
int C2F(gsortshort)(short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
int C2F(gsortushort)(unsigned short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
int C2F(gsortchar)(char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
int C2F(gsortuchar)(unsigned char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);

#endif /* _GSORT_H_ */
