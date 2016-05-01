/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) ???? - INRIA - Jean-Baptiste SILVY
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

#include "dynlib_elementary_functions.h"

ELEMENTARY_FUNCTIONS_IMPEXP void C2F(gsorts)(char **data, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(gsortd)(double *xD, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(gsortint)(int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(gsortuint)(unsigned int *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(gsortshort)(short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(gsortushort)(unsigned short *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(gsortchar)(char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(gsortuchar)(unsigned char *xI, int *ind, int *iflag, int *m, int *n, char *type, char *iord);

#endif /* _GSORT_H_ */
