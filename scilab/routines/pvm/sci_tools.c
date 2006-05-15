/* Copyright (c) 1997 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     sci_tools
   PURPOSE
     
   NOTES
     
   HISTORY
     fleury - Dec 17, 1997: Created.
     Revision 1.3  2002/07/25 08:42:45  chanceli
     updates

     Revision 1.2  2001/10/16 08:32:55  chanceli
     includes

     Revision 1.1.1.1  2001/04/26 07:49:01  scilab
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
#include "../machine.h"
#ifdef __STDC__
#include <stdlib.h>
#else
#include <malloc.h>
#endif
#include <stdio.h>
#include <string.h>

#include "sci_tools.h"
#include "sci_pvm.h"


void C2F(ccomplexf)(int *n, double **ip, double *op)
{
  memcpy(op, *ip, *n * sizeof(double));

  /* int i */
  /*   for (i = *n; --i >= 0; ) { */
  /*     op[i] = (*ip)[i];*/		/* TODO: replace by memcpy */ 
  /*   } */
  
  SET_TYPE_COMPLEX(op);		        /* type is complex */
  SET_NB_ROW(op,  NB_ROW(op) / 2);	/* nb  row is halfed */

  free((char*) (*ip));
}



void SciToF77(double *ptr, int size, int lda)
{
  int i;
  double *tab;
  
  if ((tab = (double *) malloc(size * sizeof(double))) == NULL) {
    (void) fprintf(stderr, "SciToF77: Error malloc\n");
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

  free(tab);
} 

void F77ToSci(double *ptr, int size, int lda)
{
  int i;
  double *tab;
  
  if ((tab = (double *) malloc(size * sizeof(double))) == NULL) {
    (void) fprintf(stderr, "F77ToSci: Error malloc\n");
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

  free(tab);
}

