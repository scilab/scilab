/* Copyright (c) 1999 by LORIA, INRIA.  All Rights Reserved */

/***
   NAME
     sci2f77
   PURPOSE
     Convert old fashion complex representation 
     [r_0, r_1,..., r_n, i_0, i_1,..., i_n] of scilab/matlab into a
     DOUBLE COMPLEX one [r_0, i_0, r_1, i_1, ..., r_n, i_n]

     Complexity O(nlogn) for this version. One can easly have
     O(nloglogn) by pruning the recursion. Next version wil take care 
     of the cache size.
   NOTES
     This is only a test program. 
   HISTORY
     fleury - May 7, 1999: Created.
     $Id: sci2f77.c,v 1.1 2001/04/26 07:49:01 scilab Exp $
***/
#include <stdio.h>
#include "../machine.h"

static void print __PARAMS((double*, int));
static void swap __PARAMS((double*, double*, int));
void C2F(sci2f77) __PARAMS((double*, int*));
void C2F(f772sci) __PARAMS((double*, int*));

static void print(tab, n)
     double* tab;
     int n;
{
  int i;
  for (i = 0; i < n; ++i){
    (void) printf("%f:", tab[i]);
  }
  (void) printf("\n");
} /* print */

static void swap(ptr1, ptr2, size)
     double* ptr1;
     double* ptr2;
     int size;
{
  double tmp;
  int i;
  for (i = 0; i < size; ++i) {
    tmp = ptr1[i];
    ptr1[i] = ptr2[i];
    ptr2[i] = tmp;
  }
} /* swap */


void C2F(sci2f77)(tab, size)
     double *tab;
     int *size;
{
  int nb;
  
  if (*size == 1) {
    return;
  }
  nb = *size / 2;
  if (*size % 2) {		/* si le nbr est impaire on "coupe" un
				   complexe en deux et donc il faut
				   reparer ce crime... */
    swap(&(tab[nb]), &(tab[*size + nb]), 1);
    swap(&tab[*size - nb - 1], &tab[*size], nb + 1); 
    C2F(sci2f77)(&tab[0], &nb);
    C2F(sci2f77)(&tab[*size + 1], &nb);
  }
  else {
    swap(&tab[*size - nb], &tab[*size], nb); 
    C2F(sci2f77)(&tab[0], &nb);
    C2F(sci2f77)(&tab[*size], &nb);
  }
} /* sci2f77 */


void C2F(f772sci)(tab, size)
     double *tab;
     int *size;
{
  int nb;
  
  if (*size == 1) {
    return;
  }
  nb = *size / 2;
  if (*size % 2) {		/* si le nbr est impaire on "coupe" un
				   complexe en deux et donc il faut
				   reparer ce crime... */
    C2F(f772sci)(&tab[0], &nb);
    C2F(f772sci)(&tab[*size + 1], &nb);
    swap(&(tab[*size - 1]), &(tab[*size]), 1);
    swap(&tab[*size - nb - 1], &tab[*size], nb + 1); 
  }
  else {
    C2F(f772sci)(&tab[0], &nb);
    C2F(f772sci)(&tab[*size], &nb);
    swap(&tab[*size - nb], &tab[*size], nb); 
  }
} /* f772sci */
