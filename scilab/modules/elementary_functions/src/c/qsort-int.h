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
#ifndef __QSORT_INT__
#define __QSORT_INT__

void ColSortint(int *a, int *ind, int flag, int n, int p, char dir);
void ColSortuint(unsigned int *a, int *ind, int flag, int n, int p, char dir);
void RowSortint(int *a, int *ind, int flag, int n, int p, char dir);
void RowSortuint(unsigned int *a, int *ind, int flag, int n, int p, char dir);
void GlobalSortint(int *a, int *ind, int flag, int n, int p, char dir);
void GlobalSortuint(unsigned int *a, int *ind, int flag, int n, int p, char dir);
void LexiRowint(int *a, int *ind, int flag, int n, int p, char dir);
void LexiRowuint(unsigned int *a, int *ind, int flag, int n, int p, char dir);
void LexiColint(int *a, int *ind, int flag, int n, int p, char dir);
void LexiColuint(unsigned int *a, int *ind, int flag, int n, int p, char dir);

#endif /* __QSORT_INT__ */
