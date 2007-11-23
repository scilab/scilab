/*--------------------------------------------------------------------------*/
/* Written by  Jean-Philippe Chancelier Copyright (C) ENPC 1998-2001                 */
/* Modified (restructuration and bug fix) by Allan Cornet  INRIA 2006                */
/* Jean-Baptiste Silvy                                                               */
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
