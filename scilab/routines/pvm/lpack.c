/* Copyright (c) 1998 by Inria Lorraine.  All Rights Reserved */

/***
   NAME
     lpack
   PURPOSE
     
   NOTES
     
   HISTORY
     Serge Steer / Eric Fleury - Mar 13, 1998: Created.
     $Log: lpack.c,v $
     Revision 1.1  2001/04/26 07:49:01  scilab
     Initial revision

     Revision 1.1  1998/03/27 13:44:22  fleury
     Premier ajout de lpack.
     version OK

***/
#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"

#define IADR(l) ((l)+(l)-1)
#define SADR(l) (((l)/2)+1)
#define ODD(x) ((x)+((x)%2))

#ifdef __STDC__
extern void F2C(bpak)(int *il, int *pack, int *n, int *nmax);
extern void F2C(cpak)(int *il, int *pack, int *n, int *nmax);
extern void F2C(ppak)(int *il, int *pack, int *n, int *nmax);
extern void F2C(mpak)(int *il, int *pack, int *n, int *nmax);
extern void F2C(spak)(int *il, int *pack, int *n, int *nmax);
extern void F2C(sppak)(int *il, int *pack, int *n, int *nmax);
extern void F2C(bsppak)(int *il, int *pack, int *n, int *nmax);
#else
extern void F2C(bpak)();
extern void F2C(cpak)();
extern void F2C(ppak)();
extern void F2C(mpak)();
extern void F2C(spak)();
extern void F2C(sppak)();
extern void F2C(bsppak)();
#endif 



#ifdef __STDC__
void 
C2F(lpak)(int *il, int *pack, int *np, int *npmax)
#else
int C2F(lpak)(il, pack, np, npmax)
  int *il;
  int *pack;
  int *np;
  int *npmax;
#endif 
{
  int nmax;			/* size of the free space on the stack */
  int kp;			/* pack tab index */
  int ne, type, size;		/* nb elt */
  int nk;			/* size of the packing tab for a list elt. */
  int i;			/* loop index */
  int ilp, ill, li, l;		/* @ computation */
  int padding;			/* nb of padding double */
  int p_i, p_size;
  int *ptr;

  /* nb element of the list */
  ne = istk(*il)[1];
  /*  (void) fprintf(stderr, "nb elt: %d\n", ne);*/
  
  /* first pack the list header */
  *np = 2;
  if ((*np) > (*npmax)) {
    return;
  }
  pack[0] = ODD(ne + 3);
  pack[1] = 0;
  kp = 2;

  /* loop on objects */
  ilp = *il + 2;
  l = SADR(ilp + ne + 1);
  for (i = 1; i <= ne; ++i) {	
    li = istk(ilp)[i-1];
    ill = IADR(l + li -1);
    type = istk(ill)[0];
    size = istk(ilp)[i] - istk(ilp)[i-1];
    nmax = *npmax - kp;  
    /*    (void) fprintf(stderr, "elet=%d:type=%d:size=%d:li=%d\n", 
		   i, type, size, li);*/
    switch (type){
    case 1:			/* scalar matrix */
      F2C(spak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 2:			/* polynomial matrix */
      F2C(ppak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 4:			/* boolean matrix */
      F2C(bpak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 5:			/* sparse matrix */
      F2C(sppak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 6:			/* boolean sparse matrix */
      F2C(bsppak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 10:			/* character matrix */
      F2C(cpak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 11:			/* un-compiled function */
      F2C(mpak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 13:			/* compiled function */
      F2C(mpak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 15:			/* list */
      F2C(lpak)(&ill, &pack[kp], &nk, &nmax);
      break;
    case 16:			/* typed list (tlist) */
      C2F(lpak)(&ill, &pack[kp], &nk, &nmax);
      break;
    default:
      (void) fprintf(stderr, "ERROR: unknow type\n");
      return;
    }
    
    /* Complete the padding */
    ptr = &pack[kp];
    p_size = 0;
    for (p_i = 0; p_i < nk; p_i += 2) {
      p_size += ptr[p_i]/2 + ptr[p_i] % 2;
      p_size += ptr[p_i+1];
    }
    padding = size - p_size;
    if (padding) {	
      ptr[nk-1] += padding;
    }
    /* Update the pack index */
    kp += nk;
    if(nk > nmax) {
      *np = *npmax + 1;
      return;
    }
  }
  *np = kp;
  /*  (void) fprintf(stderr, "PACK[0..%d]=", *np);
  for (i = 0; i < *np; ++i) {
    (void) fprintf(stderr, "%d:", pack[i]);
  }
  (void) fprintf(stderr, "\n");*/
  return;
} /* lpak_ */


