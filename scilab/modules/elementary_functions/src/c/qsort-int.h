/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#ifndef __QSORT_INT__
#define __QSORT_INT__

void ColSortint(int *a,int *ind,int flag,int n,int p,char dir);
void ColSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void RowSortint(int *a,int *ind,int flag,int n,int p,char dir);
void RowSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortint(int *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void LexiRowint(int *a,int *ind,int flag,int n,int p,char dir);
void LexiRowuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);
void LexiColint(int *a,int *ind,int flag,int n,int p,char dir);
void LexiColuint(unsigned int *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_INT__ */
