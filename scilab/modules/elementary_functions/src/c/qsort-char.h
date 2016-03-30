/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
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
/*------------------------------------------------------------------------*/
#ifndef __QSORT_CHAR_H__
#define __QSORT_CHAR_H__

void ColSortchar(char *a, int *ind, int flag, int n, int p, char dir);
void RowSortchar(char *a, int *ind, int flag, int n, int p, char dir);

void GlobalSortchar(char *a, int *ind, int flag, int n, int p, char dir);

void ColSortuchar(unsigned char *a, int *ind, int flag, int n, int p, char dir);
void RowSortuchar(unsigned char *a, int *ind, int flag, int n, int p, char dir);

void GlobalSortuchar(unsigned char *a, int *ind, int flag, int n, int p, char dir);


void LexiColuchar(unsigned char *a, int *ind, int flag, int n, int p, char dir);
void LexiRowchar(char *a, int *ind, int flag, int n, int p, char dir);
void LexiRowuchar(unsigned char *a, int *ind, int flag, int n, int p, char dir);
void LexiColchar(char *a, int *ind, int flag, int n, int p, char dir);

#endif /* __QSORT_CHAR_H__ */
