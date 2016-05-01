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
#ifndef __QSORT_STRING_H__
#define __QSORT_STRING_H__

void ColSortstring(char * *a, int *ind, int flag, int n, int p, char dir);
void RowSortstring(char * *a, int *ind, int flag, int n, int p, char dir);
void GlobalSortstring(char * *a, int *ind, int flag, int n, int p, char dir);
void LexiRowstring(char * *a, int *ind, int flag, int n, int p, char dir);
void LexiColstring(char * *a, int *ind, int flag, int n, int p, char dir);

#endif /* __QSORT_STRING_H__ */
