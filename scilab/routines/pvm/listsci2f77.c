/* Copyright (c) 1998 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     listsci2f77
   PURPOSE
     
   NOTES
     
   HISTORY
     fleury - Jun 29, 1999: Created.
     $Log: listsci2f77.c,v $
     Revision 1.1  2001/04/26 07:49:01  scilab
     Initial revision

     Revision 1.2  1999/07/07 18:11:12  fleury
     Ajout des routines de conversion de complexes

     Revision 1.1  1999/07/05 15:58:07  fleury
     Ajout de la transformation de scilab vers f77

***/
#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"

#define IADR(l) ((l)+(l)-1)
#define SADR(l) (((l)/2)+1)
#define ODD(x) ((x)+((x)%2))

#ifdef __STDC__
void 
C2F(listsci2f77)(int *il)
#else
int C2F(listsci2f77)(il)
  int *il;
#endif 
{
  int ne, type, size;		/* nb elt */
  int nk;			/* size of the packing tab for a list elt. */
  int i;			/* loop index */
  int ilp, ill, li, l;		/* @ computation */
  int padding;			/* nb of padding double */
  int p_i, p_size;
  int *ptr;
  int mn, nel, m, n, id;
  
  /* nb element of the list */
  ne = istk(*il)[1];
  /*  (void) fprintf(stderr, "nb elt: %d\n", ne);*/
  
  /* loop on objects */
  ilp = *il + 2;
  l = SADR(ilp + ne + 1);
  for (i = 1; i <= ne; ++i) {	
    li = istk(ilp)[i-1];
    ill = IADR(l + li -1);
    type = istk(ill)[0];
    size = istk(ilp)[i] - istk(ilp)[i-1];
    switch (type){
    case 1:			/* scalar matrix */
      if (istk(ill)[3]==1){
	mn=istk(ill)[1]*istk(ill)[2];
	C2F(sci2f77)((stk(SADR(ill+4))), &mn);
      }
      break;
    case 2:			/* polynomial matrix */
      if (istk(ill)[3]==1){
	id=ill+8;
	mn=istk(ill)[1]*istk(ill)[2];
	nel = istk(id)[mn]-1;
	C2F(sci2f77)(stk(SADR(ill+9+mn)), &nel);
      }
      break;
    case 4:			/* boolean matrix */
      break;
    case 5:			/* sparse matrix */
      if (istk(ill)[3]==1){
	nel=istk(ill)[4];
	m=istk(ill)[1];
	n=istk(ill)[2];
	C2F(sci2f77)(stk(SADR(ill+5+m+nel)), &nel);
      }
      break;
    case 6:			/* boolean sparse matrix */
      break;
    case 10:			/* character matrix */
      break;
    case 11:			/* un-compiled function */
      break;
    case 13:			/* compiled function */
      break;
    case 15:			/* list */
      C2F(listsci2f77)(&ill);
      break;
    case 16:			/* typed list (tlist) */
      C2F(listsci2f77)(&ill);
      break;
    default:
      (void) fprintf(stderr, "ERROR: unknow type\n");
      return;
    }
  }
  return;
} /* listsci2f77 */


#ifdef __STDC__
void 
C2F(listf772sci)(int *il)
#else
int C2F(listf772sci)(il)
  int *il;
#endif 
{
  int ne, type, size;		/* nb elt */
  int nk;			/* size of the packing tab for a list elt. */
  int i;			/* loop index */
  int ilp, ill, li, l;		/* @ computation */
  int padding;			/* nb of padding double */
  int p_i, p_size;
  int *ptr;
  int mn, nel, m, n, id;
  
  /* nb element of the list */
  ne = istk(*il)[1];
  /*  (void) fprintf(stderr, "nb elt: %d\n", ne);*/
  
  /* loop on objects */
  ilp = *il + 2;
  l = SADR(ilp + ne + 1);
  for (i = 1; i <= ne; ++i) {	
    li = istk(ilp)[i-1];
    ill = IADR(l + li -1);
    type = istk(ill)[0];
    size = istk(ilp)[i] - istk(ilp)[i-1];
    switch (type){
    case 1:			/* scalar matrix */
      if (istk(ill)[3]==1){
	mn=istk(ill)[1]*istk(ill)[2];
	C2F(f772sci)((stk(SADR(ill+4))), &mn);
      }
      break;
    case 2:			/* polynomial matrix */
      if (istk(ill)[3]==1){
	id=ill+8;
	mn=istk(ill)[1]*istk(ill)[2];
	nel = istk(id)[mn]-1;
	C2F(f772sci)(stk(SADR(ill+9+mn)), &nel);
      }
      break;
    case 4:			/* boolean matrix */
      break;
    case 5:			/* sparse matrix */
      if (istk(ill)[3]==1){
	nel=istk(ill)[4];
	m=istk(ill)[1];
	n=istk(ill)[2];
	C2F(f772sci)(stk(SADR(ill+5+m+nel)), &nel);
      }
      break;
    case 6:			/* boolean sparse matrix */
      break;
    case 10:			/* character matrix */
      break;
    case 11:			/* un-compiled function */
      break;
    case 13:			/* compiled function */
      break;
    case 15:			/* list */
      C2F(listf772sci)(&ill);
      break;
    case 16:			/* typed list (tlist) */
      C2F(listf772sci)(&ill);
      break;
    default:
      (void) fprintf(stderr, "ERROR: unknow type\n");
      return;
    }
  }
  return;
} /* listf772sci */

