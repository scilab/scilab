/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "stack-c.h"
#include "stcreate.h"
#include "cvstr.h"
#include "Scierror.h"
#include "localization.h"

#define memused(it,mn) ((((mn)*( it % 10))/sizeof(int))+1)

/* Table of constant values */
static int c17 = 17;
static int c1 = 1;
static int c4 = 4;


int C2F(stcreate)(int *lw, int *nz, int *sz, int *nf, char *fnames[], int *retval)
{
    int next,k,lr,nels;

    static int l;
    static int l0, n1, il;

    *retval = 0;
    l0 = *Lstk(*lw);

    C2F(intersci).ntypes[*lw -Top + Rhs - 1] = '$';
    C2F(intersci).iwhere[*lw -Top + Rhs - 1] = *Lstk(*lw);

    if (*lw > intersiz) 
	{
		Scierror(998,_("Too many arguments in the stack, edit stack.h and enlarge intersiz.\n"));
		return 1;
    }
    sttyp(&n1, "size", nf, fnames);

    *retval = 1;
/*     create the mlist header */
    il = iadr(l0);
    Err = sadr(il+6) + n1 - *Lstk(Bot);
    if (Err > 0)
	{
		Error(c17);
		return 1;
    }
    *istk(il) = 17;
    *istk(il+1) = 2+*nf;
    *istk(il + 2) = 1;
/*     set mlist type ["st","dims","field1",...,"fieldnf"] */
    l = sadr(il+5+*nf);
    sttyp(istk(iadr(l)), "set", nf, fnames);
    *istk(il + 3) = *istk(il + 2) + n1;
    l += n1;
/*     set dims entry (int32 matrix) */
/*  if (!C2F(creimati)("stcreate", &l, &c4, nz, &c1, &lr, &c1,  12L)) return 1;  FD comment*/
    if (!C2F(creimati)("stcreate", &l, &c4, &c1, nz, &lr, &c1,  12L)) return 1;
    C2F(icopy)(nz,sz,&c1,istk(lr),&c1);
    n1 = sadr(iadr(l)+4) + memused(c4,*nz) - l;
    *istk(il + 4) = *istk(il + 3) + n1;
    l += n1;

/*     set struct fields */
    nels=1;
    for (k=0; k< *nz;k++) nels=nels*sz[k];
    if (nels==1) {
    for (k=0; k< *nf;k++)
      {
	creonevoid(&l,&next);
	n1 = next - l;
	l += n1;
	*istk(il + 5 + k) = *istk(il + 4 + k) + n1;
      }
    }
    else {
    for (k=0; k< *nf;k++)
      {
	crelistofvoids(&l,&next,&nels);
	n1 = next - l;
	l += n1;
	*istk(il + 5 + k) = *istk(il + 4 + k) + n1;
      }
    }
    *Lstk(*lw+1)=l;
    return 1;
}

int creonevoid(int *slw,int *lw)
{
  int il;
  il = iadr(*slw);
  *istk(il  )=1;
  *istk(il+1)=0;
  *istk(il+2)=0;
  *istk(il+3)=0;
  il += 4;
  *lw = sadr(il);
  return 0;
}

int crelistofvoids(int *slw,int *lw,int *nels)
{
  int il,k;
  il = iadr(*slw);
  *istk(il) = 15;
  *istk(il + 1) = *nels;
  *istk(il + 2) = 1;
  for (k=0; k<*nels; k++) *istk(il+3+k) = *istk(il+2+k) + 2;
  il += 3+*nels;
  il=iadr(sadr(il));
  for (k=0; k<*nels; k++) {
    *istk(il  )=1;
    *istk(il+1)=0;
    *istk(il+2)=0;
    *istk(il+3)=0;
    il += 4;
  };
  if ( ((il-iadr(*slw)) %2) ) il +=1;
  *lw =  sadr(il);
  return 0;
}

int sttyp(int *ivt, char *job, int *nf, char *fnames[])
{
  static int l; int ls,k,shft,stp,Job;

    if (strcmp(job, "size") == 0) {
/*        size of header = ["st","dims","f_1",...,"f_nf"] */
      ls=0;
      for ( k= 0; k < *nf; k++) ls += (int)strlen(fnames[k]);
      ivt[0] = 4 + (2 +*nf) + 1 + 2 + 4 + ls;  /* 2 + *nf strings */
      /* if (ivt[0] % 2) ivt[0]++;  */
    } else {
/*      header = row vector made of 2 + *nf strings */
	ivt[0] = sci_strings;
	ivt[1] = 1;
	ivt[2] = 2 + *nf;
	ivt[3] = 0;
	ivt[4] = 1;
	stp=4;
	l = 5 + (2 + *nf);
/*       string 1 =  "st" */
	shft=2;
	ivt[l] = 28;      /*  "s"   */
	ivt[l + 1] = 29;  /*  "t"   */
	ivt[stp+1] = ivt[stp] + shft;
	l += shft;stp++;
/*        string 2 =  "dims" */
	shft=4;
	ivt[l] = 13;      /* "d"   */
	ivt[l + 1] = 18;  /* "i"   */
	ivt[l + 2] = 22;  /* "m"   */
	ivt[l + 3] = 28;  /* "s"   */
	ivt[stp+1] = ivt[stp] + shft;
	l += shft;stp++;
/*        strings 3:2+nf = ["field_1","field_2","field_nf"]    */
	for (k=0; k < *nf; k++) {
	shft = (int)strlen(fnames[k]);
	C2F(cvstr)(&shft,&ivt[l],fnames[k],(Job=0,&Job),shft);
	ivt[stp+1] = ivt[stp] + shft;
	l += shft;stp++;
	}
    }
    return 0;
}
