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
#ifndef __QSORT_CHAR_H__
#define __QSORT_CHAR_H__

void ColSortchar(char *a,int *ind,int flag,int n,int p,char dir);
void RowSortchar(char *a,int *ind,int flag,int n,int p,char dir);

void GlobalSortchar(char *a,int *ind,int flag,int n,int p,char dir);

void ColSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
void RowSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);

void GlobalSortuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);


void LexiColuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
void LexiRowchar(char *a,int *ind,int flag,int n,int p,char dir);
void LexiRowuchar(unsigned char *a,int *ind,int flag,int n,int p,char dir);
void LexiColchar(char *a,int *ind,int flag,int n,int p,char dir);

#endif /* __QSORT_CHAR_H__ */
