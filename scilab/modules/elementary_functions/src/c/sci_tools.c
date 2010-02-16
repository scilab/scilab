/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1997 - INRIA - FLEURY
 * Copyright (C) 2001 - INRIA - François DELEBECQUE
 * Copyright (C) 2001 - ENPC - Jean-Philippe CHANCELIER
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     sci_tools
   PURPOSE
     
   NOTES
     
   HISTORY
     fleury - Dec 17, 1997: Created.
     $Log: sci_tools.c,v $
     Revision 1.7  2005/10/22 18:53:10  cornet
     update memory management under Windows
     use HeapAlloc and VirtualAlloc (for scilab stack)
     Correction Bug 1576
     n=10000
     clf();
     plot2d([0,1],[0,n],0)
     xpols=[zeros(1,n); ones(2,n); zeros(1,n)];
     ypols=[2:n+1; 2:n+1; 1:n; 1:n];
     xfpolys(xpols,ypols,modulo((1:n),32))

     and for windows on PC with 256 mo
     stacksize(5000000*20)
     a=rand(9999,9999)

     Revision 1.6  2005/07/04 06:15:28  cornet
     correction compilation

     Revision 1.5  2005/07/03 18:16:10  cornet
     optimisation des MALLOC pour Windows ( A tester avec attention ) --> VirtualAlloc

     Revision 1.4  2005/07/01 07:08:08  cornet
     replace malloc, free, calloc & realloc by MALLOC,FREE,CALLOC & REALLOC defined in SCI/libs/MALLOC/includes/sci_mem_alloc.h

     Revision 1.3  2001/10/16 08:23:38  chanceli
     change in includes

     Revision 1.2  2001/06/11 17:53:43  delebecq
     f772sci with 2 pointers

     Revision 1.1.1.1  2001/04/26 07:47:34  scilab
     Imported sources

     Revision 1.5  1998/03/27 12:20:22  fleury
     Version pvm OK.
     TODO: faire des tests de compil sur plateforme separee (POPC0
     TODO: commenter source (-;
     TODO: faire un peu de netoyage

     Revision 1.4  1998/03/17 11:49:33  fleury
     Broadcast OK.
     TODO: mettre les listes.
     TODO: faire qcq tests

     Revision 1.3  1998/03/13 13:57:07  fleury
     Version send/recv avec pack. A tester.
     TODO: ajouter les listes + BROADCAST
     TODO: faire un clean du dir et des fichiers...

     Revision 1.2  1998/01/06 13:23:48  fleury
     Use memcopy instead of fori

     Revision 1.1  1997/12/18 18:35:57  fleury
     Premier commit

     Ajout de fct permettant de tester le type d une variable scilab
     conversion de complex format scilab to complex format f77
     TODO:use memcpy
     TODO:use imatrix

***/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "sci_tools.h"
#include "MALLOC.h" /* MALLOC */

#include "localization.h"
#include "Scierror.h"

void 
C2F(ccomplexf)(int *n, double **ip, double *op)
{
  memcpy(op, *ip, *n * sizeof(double));

  /* int i */
  /*   for (i = *n; --i >= 0; ) { */
  /*     op[i] = (*ip)[i];*/		/* TODO: replace by memcpy */ 
  /*   } */
  
  SET_TYPE_COMPLEX(op);		        /* type is complex */
  SET_NB_ROW(op,  NB_ROW(op) / 2);	/* nb  row is halfed */

  FREE((char*) (*ip));
} /* ccomplexf */

void SciToF77(double *ptr, int size, int lda)
{
  int i;
  double *tab;
  
  if ((tab = (double *) MALLOC(size * sizeof(double))) == NULL) {
    Scierror(999,_("%s: No more memory.\n"),"SciToF77");
    return;
  }

  /* for (i = size; --i >= 0; ) { */
  /*     tab[i] = ptr[i]; */
  /*   } */

  memcpy(tab, ptr, size * sizeof(double));

  for (i = 0; i < size; ++i) {
    ptr[2*i] = tab[i];
    ptr[2*i+1] = ptr[lda+i];
  }

  FREE(tab);
} /* SciToF77 */


void F77ToSci(double *ptr, int size, int lda)
{
  int i;
  double *tab;
  
  if ((tab = (double *) MALLOC(size * sizeof(double))) == NULL) {
    Scierror(999,_("%s: No more memory.\n"),"F77ToSci");
    return;
  }
  
  for (i = 0; i < size; ++i) {
    tab[i] = ptr[2*i+1];
    ptr[i] = ptr[2*i];
  }

  memcpy(ptr + lda, tab, size * sizeof(double));

  /*   for (i = size; --i >= 0; ) { */
  /*     ptr[lda+i] = tab[i]; */
  /*   } */

  FREE(tab);
} /* F77ToSci */


/* double2z and z2double : same as above with two pointers dest and src 
   double2z ptr = src, ptr77z = dest (z format)     
   z2double ptr = src (z format) , ptrsci = dest */  

void 
double2z(double *ptr, double *ptr77z, int size, int lda)
{
  int i;
  double *tab;
  
  if ((tab = (double *) MALLOC(size * sizeof(double))) == NULL) {
    Scierror(999,_("%s: No more memory.\n"),"double2z");
    return;
  }

  memcpy(tab, ptr, size * sizeof(double));

  for (i = 0; i < size; ++i) {
    ptr77z[2*i] = tab[i];
    ptr77z[2*i+1] = ptr[lda+i];
  }

  FREE(tab);
} 


//
// z2double --
//   Converts a memory space which stores a Fortran-like data
//   in alternate order into the Scilab internal representation
//   which is block-ordered.
//   
// Note:
//   Fortran representation is based on alternation of real
//   and imaginary parts, that is R1, I1, R2, I2, etc...
//   where R is the real part and I the imaginary part.
//   The Scilab internal representation is block-ordered, that is
//   R1, ..., Rn, I1, ..., In.
//
void 
z2double(double *ptrz, double *ptrsci, int size, int lda)
{
  int i;
  double *tab;
  
  if ((tab = (double *) MALLOC(size * sizeof(double))) == NULL) {
    Scierror(999,_("%s: No more memory.\n"),"z2double");
    return;
  }
  // Put the real parts in place and stores the imaginary parts
  // in the array tab.
  for (i = 0; i < size; ++i) {
    tab[i] = ptrz[2*i+1]; // imaginary part
    ptrsci[i] = ptrz[2*i]; // real part
  }
  // Puts the imaginary parts in the imaginary block.
  memcpy(ptrsci + lda, tab, size * sizeof(double));

  FREE(tab);
} 

