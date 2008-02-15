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
#ifndef __QSORT_STRING_H__
#define __QSORT_STRING_H__

void ColSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
void RowSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortstring(char * *a,int *ind,int flag,int n,int p,char dir);
void LexiRowstring(char * *a,int *ind,int flag,int n,int p,char dir);
void LexiColstring(char * *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_STRING_H__ */
