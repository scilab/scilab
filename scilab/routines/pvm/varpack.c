/*---------------------------------------------------------------
 * Copyright (c) 1998-2002 by Inria Lorraine.  All Rights Reserved 
 *    Serge Steer / Eric Fleury - Mar 13, 1998: Created.
 *
 * July 2002 : modified by merging lpak and varpak to 
 *             avoid code duplication 
 *             Chancelier Enpc 
 * 
 * A set of functions for packing Scilab data 
 *---------------------------------------------------------------*/ 

#include <stdio.h>
#include "../machine.h"
#include "../stack-c.h"
#include "sci_pvm.h"

/* Table of constant values */


static int cx4 = 4;
static int cx2 = 2;

#define ODD(x) ((x)+((x)%2))

static void C2F(ipak)  (int *il, int *pack, int *n, int *nMax);
static void F2C(bpak)  (int *il, int *pack, int *n, int *nmax);
static void F2C(cpak)  (int *il, int *pack, int *n, int *nmax);
static void F2C(ppak)  (int *il, int *pack, int *n, int *nmax);
static void F2C(mpak)  (int *il, int *pack, int *n, int *nmax);
static void F2C(spak)  (int *il, int *pack, int *n, int *nmax);
static void F2C(sppak) (int *il, int *pack, int *n, int *nmax);
static void F2C(bsppak)(int *il, int *pack, int *n, int *nmax);
static void C2F(libpak)(int *il, int *pack, int *np, int *npMax);
static int pak_object_info (int ilk,int stk_pos, int *pack, int *np, int *npmax);

/*-----------------------------------------------------------------
 *
 *     Given a scilab variable, stored in the stack at the position k (in 
 *     lstk) this function returns a "packing" vector pack for pvm. 
 *     A scilab variable is a consecutive memory region formed by a 
 *     succession of int and double precision vectors. 
 *     pack contains a sequence of pairs of ints. 
 *     varpak returns a vector which contains the size of this int 
 *     and double precision vectors. pack contains a sequence of pairs of 
 *     ints:  pack=[i1,r1,i2,r2,...,in,rn] 
 *     il is a number of int words 
 *     rl is a number of double precision word 
 *     n    : is the returned length of pack 
 *     nMax : is the Maximum size allowed for pack  
 *-----------------------------------------------------------------*/ 

int C2F(varpak)(int *k, int *pack, int *n, int *nMax, int *ierr) {
  *n = 0; /* n must be initialized before calling pak_object_info */
  /* ierr = 0 OK 
   * ierr = 1 nMax is not enough 
   * ierr = 2 unknown data type 
   */
  *ierr= pak_object_info(*k,1,pack,n,nMax);
  return 0;
}


static int  pak_object_info(int ilk,int stk_pos, int *pack, int *np, int *nMax)
{
  int type,ne,il,ilp,i,li,ill,l,kp;

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
  case sci_matrix:         C2F(spak)(&il, pack, np, nMax); break; 
  case sci_poly  :         C2F(ppak)(&il, pack, np, nMax); break;
  case sci_boolean:        C2F(bpak)(&il, pack, np, nMax); break;
  case sci_sparse:         C2F(sppak)(&il, pack, np, nMax); break;
  case sci_boolean_sparse: C2F(bsppak)(&il, pack, np, nMax);break;
  case sci_ints:           C2F(ipak)(&il, pack, np, nMax); break;
  case sci_strings:        C2F(cpak)(&il, pack, np, nMax);break;
  case sci_u_function:
  case sci_c_function:     C2F(mpak)(&il, pack, np, nMax); break;
  case sci_lib :           C2F(libpak)(&il, pack, np, nMax);break;
  case sci_list : 
  case sci_tlist : 
  case sci_mlist :  
    /* nb element of the list */
    ne = istk(il)[1];
    /* first pack the list header */
    kp = 2;
    *np += kp;
    if ((*np ) > (*nMax)) {
      return 1;
    }
    pack[0] = ODD(ne + 3);
    pack[1] = 0;
    /* loop on objects */
    ilp = il + 2;
    l = sadr(ilp + ne + 1);
    for (i = 1; i <= ne; ++i) {	
      int np1,nk,*ptr,p_size,p_i,padding,size,ierr;
      li  = istk(ilp)[i-1];
      ill = iadr(l + li -1);
      size = istk(ilp)[i] - istk(ilp)[i-1];
      /* recursive call but now with an istk position 
       * i.e stk_pos == 0 
       */ 
      np1 = *np;
      ierr= pak_object_info(ill,0,pack+kp,&np1,nMax);
      if ( ierr != 0) return ierr;
      /* Complete the padding */
      nk = np1 - *np;
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
      *np += nk;
      if( *np > *nMax) return 1;
    }
    break ; 
  default : return 2;
    break;
  }
  if ( *np > *nMax) return 1; 
  return 0;
}


/* utilities */ 

int C2F(allignf)(int *n, int *m)
{
  int rest= *n % *m ; 
  if ( rest == 0 ) 
    return *n ; 
  else 
    return *n - rest + *m ; 
} 

/* scalar matrix case */ 

static void C2F(spak)(int *il, int *pack, int *n, int *nMax)
{
  *n += 2;
  if (*n > *nMax) return ;
  pack[0] = 4;
  pack[1] = *istk(*il +1) * *istk(*il + 1 +1) * (*istk(*il + 2 +1) + 1);
} 

/*     matrix of interger */

static void C2F(ipak)(int *il, int *pack, int *n, int *nMax)
{
  int ix1, ix2;
  *n += 2;
  if (*n > *nMax)  return ; 

  pack[0] = 4;
  pack[1] = 0;
  /*     char */
  if (*istk(*il + 2 +1) == 1) {
    ix2 = *istk(*il +1) * *istk(*il + 1 +1);
    ix1 = pack[0] + C2F(allignf)(&ix2, &cx4) / 4;
    pack[0] = ODD(ix1);
  }
  /*     short */
  if (*istk(*il + 2 +1) == 2) {
    ix2 = *istk(*il +1) * *istk(*il + 1 +1);
    ix1 = pack[0] + C2F(allignf)(&ix2, &cx2) / 2;
    pack[0] = ODD(ix1);
  }
  /*     int */
  if (*istk(*il + 2 +1) == 4) {
    ix1 = pack[0] + *istk(*il +1) * *istk(*il + 1 +1);
    pack[0] = ODD(ix1);
  }
  /*     unsigned char */
  if (*istk(*il + 2 +1) == 11) {
    ix2 = *istk(*il +1) * *istk(*il + 1 +1);
    ix1 = pack[0] + C2F(allignf)(&ix2, &cx4) / 4;
    pack[0] = ODD(ix1);
  }
  /*     unsigned short */
  if (*istk(*il + 2 +1) == 12) {
    ix2 = *istk(*il +1) * *istk(*il + 1 +1);
    ix1 = pack[0] + C2F(allignf)(&ix2, &cx2) / 2;
    pack[0] = ODD(ix1);
    
  }
  /*     unsigned int */
  if (*istk(*il + 2 +1) == 14) {
    ix1 = pack[0] + *istk(*il +1) * *istk(*il + 1 +1);
    pack[0] = ODD(ix1);
  }
  return ;
} 

/*     matrix of polynomial case */

static void C2F(ppak)(int *il, int *pack, int *np, int *npMax)
{
  int ix1;
  static int id, mn;

  *np += 2;
  if (*np > *npMax) {
    return ;
  }
  id = *il + 8;
  mn = *istk(*il +1) * *istk(*il + 1 +1);
  ix1 = mn + 9;
  pack[0] = ODD(ix1);
  pack[1] = (*istk(id + mn ) - 1) * (*istk(*il + 2 +1) + 1);
  return ;
} 

/*     matrix of boolean case */

static void C2F(bpak)(int *il, int *pack, int *np, int *npMax)
{
  int ix1;
  static int id, mn;
  /* Function Body */
  *np += 2;
  if (*np > *npMax) {
    return ;
  }
  id = *il + 8;
  mn = *istk(*il +1) * *istk(*il + 1 +1);
  ix1 = mn + 3;
  pack[0] = ODD(ix1);
  pack[1] = 0;
  return ;
} 

/*     sparse matrix of numbers  case */

static void C2F(sppak)(int *il, int *pack, int *np, int *npMax)
{
  int ix1;
  static int m, n, it;
  static int nel;
  *np += 2;
  if (*np > *npMax) {
    return ;
  }
  nel = *istk(*il + 3 +1);
  m = *istk(*il +1);
  n = *istk(*il + 1 +1);
  it = *istk(*il + 2 +1);
  ix1 = m + 5 + nel;
  pack[0] = ODD(ix1);
  pack[1] = nel * (it + 1);
  return ;
} 


/*     sparse matrix of numbers  case */

static void C2F(bsppak)(int *il, int *pack, int *np, int *npMax)
{
  int ix1;
  static int m, n, it;
  static int nel;

  *np += 2;
    if (*np > *npMax) {
	return ;
    }
    nel = *istk(*il + 3 +1);
    m = *istk(*il +1);
    n = *istk(*il + 1 +1);
    it = *istk(*il + 2 +1);
    ix1 = m + 5 + nel;
    pack[0] = ODD(ix1);
    pack[1] = 0;
    return ;
} 

/*     matrix of strings  case */

static void C2F(cpak)(int *il, int *pack, int *np, int *npMax)
{
    int ix1;
    static int id, mn;
    static int vol;
    /* Function Body */
    *np += 2;
    if (*np > *npMax) {
	return ;
    }
    mn = *istk(*il +1) * *istk(*il + 1 +1);
    id = *il + 4;
    vol = *istk(*il + 4 + mn ) - 1;
    ix1 = mn + 5 + vol;
    pack[0] = ODD(ix1);
    pack[1] = 0;
    return ;
} 


/*     function  case (compiled or not) */

static void C2F(mpak)(int *il, int *pack, int *np, int *npMax)
{
  int ix1;
  static int nout;
  static int nt, il1;
  static int nin;

  *np += 2;
  if (*np > *npMax) {
    return ;
  }
  il1 = *il + 1;
  nin = *istk(il1 );
  il1 = il1 + nin * nsiz + 1;
  nout = *istk(il1 );
  il1 = il1 + nout * nsiz + 1;
  nt = *istk(il1 );
  ix1 = nin * nsiz + 4 + nout * nsiz + nt;
  pack[0] = ODD(ix1);
  pack[1] = 0;
  return ;
} 

/*    library   case (compiled or not) */

static void C2F(libpak)(int *il, int *pack, int *np, int *npMax)
{
  int ix1;
  static int nf, nh, nm;
  static int ilh, iln;

  *np += 2;
  if (*np > *npMax) {
	return ;
  }
  nf = *istk(*il +1);
  ilh = *istk(*il + 2 + nf );
  nh = *istk(ilh );
  iln = ilh + nh + 1;
  nm = *istk(iln );
  /*      write(*,*) nf, nh, nm, istk(iln+1) */
  ix1 = nf + 4 + nh + (nm << 1) + 100;
  pack[0] = ODD(ix1);
  pack[1] = 0;
  return ;
} 


