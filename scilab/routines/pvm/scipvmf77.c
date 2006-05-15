/*------------------------------------------------------------------------
 *    PVM functions 
 *    Copyright (c) 1997-2002 by Inria Lorraine.  All Rights Reserved 
 *------------------------------------------------------------------------
 *   NAME 
 *     scipvmf77tosci and  scipvmscitof77 
 *     static functions: 
 *   PURPOSE 
 *     convert F77complex to scilab ones in a Scilab variable.
 *   NOTES 
 *   HISTORY 
 *     fleury - Jun 29, 1999: Created. 
 *     Revision 1.3  2005/01/19 14:40:36  cornet
 *     remove some functions not used or only for debug (not recquired by scilab)
 *     Cleaning Sources ...
 *
 *     Revision 1.2  2004/09/16 13:11:40  steer
 *     name changes in C version of fortran stack commons
 *
 *     Revision 1.1  2002/07/25 08:08:07  chanceli
 *     C translation and simplifications
 * 
 * 
 *     Highly modified : Chancelier 2002/07/19
 * 
 *     Revision 1.1.1.1  2001/04/26 07:49:01  scilab 
 *     Imported sources 
 *
 *     Revision 1.1  1999/07/07 18:11:13  fleury 
 *     Ajout des routines de conversion de complexes 
 *------------------------------------------------------------------------*/

#include <stdio.h>
#include "../stack-c.h"
#include "../machine.h"
#include "sci_pvm.h"


static void swap (double*, double*, int);
static void sci_to_f77 (double*, int*);
static void f77_to_sci (double*, int*);

typedef void (*Fm)(double *,int *); 
typedef void (*Fl)(int *); 
static void sci_object_walk(int il,Fm fm,int stk_pos);


/*------------------------------------------------------------------------
 *  Given a scilab variable, stored in the stack at the position k (in 
 *  lstk), this function converts complex matrices contained in the 
 *  object from f77 representation to scilab representation. 
 *------------------------------------------------------------------------*/

void C2F(scipvmf77tosci)(int *k)
{
  /* call sci_object_walk 
   * object is given by its lstk position 
   */
  sci_object_walk(*k,f77_to_sci,1);
} 


/*------------------------------------------------------------------------
 *  Given a scilab variable, stored in the stack at the position k (in 
 *  lstk), this function converts complex matrices contained in the 
 *  object form Scilab representation to f77 representation. 
 *------------------------------------------------------------------------*/

void C2F(scipvmscitof77)(int *k)
{
  /* call sci_object_walk 
   * object is given by its lstk position 
   */
  sci_object_walk(*k,sci_to_f77,1);
} 

/*--------------------------------------------------------
 * sci2f77
 *    Converts Scilab complex representation 
 *     [r_0, r_1,..., r_n, i_0, i_1,..., i_n]
 *    into f77 representation 
       DOUBLE COMPLEX one [r_0, i_0, r_1, i_1, ..., r_n, i_n]
 *
 *     Complexity O(nlogn) for this version. One can easly have
 *     O(nloglogn) by pruning the recursion. Next version wil take care 
 *     of the cache size.
 *     fleury - May 7, 1999: Created.
 *--------------------------------------------------------*/

/* utility */ 

static void swap(double* ptr1, double* ptr2, int size)
{
  double tmp;
  int i;
  for (i = 0; i < size; ++i) {
    tmp = ptr1[i];
    ptr1[i] = ptr2[i];
    ptr2[i] = tmp;
  }
}

static void sci_to_f77( double *tab, int *size)
{
  int nb;
  
  if (*size == 1) {
    return;
  }
  nb = *size / 2;
  if (*size % 2) {		
    /* si le nbr est impaire on "coupe" un
     *   complexe en deux et donc il faut
     * reparer ce crime... 
     */
    swap(&(tab[nb]), &(tab[*size + nb]), 1);
    swap(&tab[*size - nb - 1], &tab[*size], nb + 1); 
    sci_to_f77(&tab[0], &nb);
    sci_to_f77(&tab[*size + 1], &nb);
  }
  else {
    swap(&tab[*size - nb], &tab[*size], nb); 
    sci_to_f77(&tab[0], &nb);
    sci_to_f77(&tab[*size], &nb);
  }
}


/*--------------------------------------------------------
 * f772sci 
 *    Converts f77 complex representation 
 *    into scilab  representation 
 *     Complexity O(nlogn) for this version. One can easly have
 *     O(nloglogn) by pruning the recursion. Next version wil take care 
 *     of the cache size.
 *     fleury - May 7, 1999: Created.
 *--------------------------------------------------------*/

static void f77_to_sci(double *tab, int *size)
{
  int nb;
  
  if (*size == 1) {
    return;
  }
  nb = *size / 2;
  if (*size % 2) {		
    /* si le nbr est impaire on "coupe" un
     * complexe en deux et donc il faut
     * reparer ce crime... 
     */
    f77_to_sci(&tab[0], &nb);
    f77_to_sci(&tab[*size + 1], &nb);
    swap(&(tab[*size - 1]), &(tab[*size]), 1);
    swap(&tab[*size - nb - 1], &tab[*size], nb + 1); 
  }
  else {
    f77_to_sci(&tab[0], &nb);
    f77_to_sci(&tab[*size], &nb);
    swap(&tab[*size - nb], &tab[*size], nb); 
  }
} 



/*--------------------------------------------------------
 * Utility function 
 * Chancelier 2002 
 * recursively walk on scilab object 
 *   if stk_pos==0 the object is given by 
 *        its il position 
 *        I.e the object is at position istk(il)+.... 
 *   if stk_pos==1 the object is given by its k 
 *        position in Lstk(k) 
 * 
 * During the walk fm is applied on some data structures 
 * 
 *--------------------------------------------------------*/

static void sci_object_walk(int ilk,Fm fm,int stk_pos)
{
  int ix1, ix2,type, m, n,id, mn, nel,ne,il,ilp,i,li,ill,l;

  if ( stk_pos == 1 ) 
    {
      /* object given by its stk position */ 
      il = iadr(*Lstk(ilk));
      if (*istk(il ) < 0) {
	il = iadr(*istk(il +1));
      }
    }
  else 
    {
      il = ilk;
    }

  type = *istk(il); 

  switch ( type ) {
  case sci_matrix : 
    if ( *istk(il + 3) == 1) {
      /* this is a complex scalar matrix */ 
      mn = *istk(il +1) * *istk(il + 2);
      ix1 = il + 4;
      fm(stk(sadr(ix1) ), &mn);
    } 
    break; 
  case sci_poly : 
    if ( *istk(il + 3) == 1) {
      /* this is a complex polynomial  matrix */ 
      id = il + 8;
      mn = *istk(il +1) * *istk(il+2);
      ix1 = il + 9 + mn;
      ix2 = *istk(id + mn ) - 1;
      fm(stk(sadr(ix1) ), &ix2);
    }
    break; 
  case sci_sparse : 
    if ( *istk(il + 3) == 1) {
      /* this is a complex sparse matrix */ 
      nel = *istk(il + 3 +1);
      m = *istk(il +1);
      n = *istk(il + 1 +1);
      ix1 = il + 5 + m + nel;
      fm(stk(sadr(ix1) ), &nel);
    }
    break; 
  case sci_list : 
  case sci_tlist : 
  case sci_mlist : 
    /* nb element of the list */
    ne = istk(il)[1];
    /* loop on objects */
    ilp = il + 2;
    l = sadr(ilp + ne + 1);
    for (i = 1; i <= ne; ++i) {	
      li  = istk(ilp)[i-1];
      ill = iadr(l + li -1);
      /* recursive call but now with an istk position 
       * i.e stk_pos == 0 
       */ 
      sci_object_walk(ill,fm,0);
    }
    break ; 
  default : 
    break;
  }
}


