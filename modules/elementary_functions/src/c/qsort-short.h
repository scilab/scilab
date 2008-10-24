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
#ifndef __QSORT_SHORT_H__
#define __QSORT_SHORT_H__

void ColSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void RowSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortshort(short *a,int *ind,int flag,int n,int p,char dir);

void ColSortshort(short *a,int *ind,int flag,int n,int p,char dir);
void ColSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

void RowSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);
void GlobalSortushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

void LexiRowshort(short *a,int *ind,int flag,int n,int p,char dir);
void LexiRowushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

void LexiColshort(short *a,int *ind,int flag,int n,int p,char dir);
void LexiColushort(unsigned short *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_SHORT_H__ */
